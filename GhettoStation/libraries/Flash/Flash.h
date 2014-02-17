/*
  Flash.h - An Arduino library for flash-based (ROM) data collections.
  Copyright (C) 2009-2012 Mikal Hart
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <avr/pgmspace.h>
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FLASH_LIBRARY_VERSION 4

// Use these macros to define your flash-based data structures
// Example: FLASH_STRING(str, "Four score and seven years ago");
#define FLASH_STRING(name, value) \
  static const char name##_flash[] PROGMEM = value; \
  _FLASH_STRING name(name##_flash);

// Example: FLASH_ARRAY(float, temperatures, 98.1, 98.5, 99.1, 102.1);
#define FLASH_ARRAY(type, name, values...) \
  static const type name##_flash[] PROGMEM = { values }; \
  _FLASH_ARRAY<type> name(name##_flash, sizeof(name##_flash) / sizeof(type));

// Example: FLASH_TABLE(uint8_t, fonts, 7, {ON, OFF, ON, ON, OFF, ON, OFF}, {OFF, ON, OFF, ON, OFF, ON, OFF});
#define FLASH_TABLE(type, name, cols, values...) \
  static const type name##_flash[][cols] PROGMEM = { values }; \
  _FLASH_TABLE<type> name((const PROGMEM type *)name##_flash, sizeof(name##_flash) / sizeof(name##_flash[0]), cols);

// Example: FLASH_STRING_ARRAY(nums, PSTR("One"), PSTR("Two"), PSTR("Three"), PSTR("Four"), PSTR("Five"));
#define FLASH_STRING_ARRAY(name, values...) \
  const PROGMEM char *name##_arr[] = { values }; \
  _FLASH_STRING_ARRAY name(name##_arr, sizeof(name##_arr) / sizeof(name##_arr[0]));

#ifndef ARDUINO_CORE_PRINTABLE_SUPPORT
class _Printable
{
public:
  virtual void print(Print &stream) const = 0;
};
#endif

#if ARDUINO < 100
// Inline ROM strings.  Example: Serial << F("Hello, big world!");
#define F(str) (_FLASH_STRING(PSTR(str)).Printable())
#endif

/* _FLASH_STRING class.  Use the FLASH_STRING() macro to create these, or use inline F() macro. */
class _FLASH_STRING : public _Printable
{
public:
  _FLASH_STRING(const prog_char *arr);

  size_t length() const 
  { return strlen_P(_arr); }

  char *copy(char *to, size_t size = -1, size_t offset = 0) const 
  { 
    return size == -1 ?
      strcpy_P(to, _arr + offset) : strncpy_P(to, _arr + offset, size);
  }

  const prog_char *access() const 
  { return _arr; }

  const _Printable &Printable() const
  { return *this; }

  char operator[](int index) const
  { return static_cast<char>(pgm_read_byte(_arr + index)); }

  void print(Print &stream) const;

private:
  const prog_char *_arr;
};

/* _FLASH_ARRAY template class.  Use the FLASH_ARRAY() macro to create these. */
template<class T>
class _FLASH_ARRAY : public _Printable
{
  typedef T PROGMEM _DataType;

public:
  _FLASH_ARRAY(const _DataType *arr, size_t count) : _arr(arr), _size(count)
  { }

  size_t count() const 
  { return _size; }

  const _DataType *access() const 
  { return _arr; }

  T operator[](int index) const
  {
    uint32_t val = 0;
    if (sizeof(T) == 1)
      val = pgm_read_byte(_arr + index);
    else if (sizeof(T) == 2)
      val = pgm_read_word(_arr + index);
    else if (sizeof(T) == 4)
      val = pgm_read_dword(_arr + index);
    return *reinterpret_cast<T *>(&val);
  }

  void print(Print &stream) const
  {
    for (size_t i=0; i<_size; ++i)
    {
      stream.print((*this)[i]);
      if (i < _size - 1) 
        stream.print(",");
    }
  }

private:
  const _DataType *_arr;
  size_t _size;
};

/* _FLASH_TABLE template class.  Use the FLASH_TABLE() macro to create these. */
template<class T>
class _FLASH_TABLE : public _Printable
{
  typedef T PROGMEM _DataType;

public:
  _FLASH_TABLE(const _DataType *arr, size_t rows, size_t cols) : _arr(arr), _rows(rows), _cols(cols)
  { }

  size_t rows() const 
  { return _rows; }

  size_t cols() const 
  { return _cols; }

  const _DataType *access() 
  { return _arr; }

  _FLASH_ARRAY<T> operator[](int index) const
  {
    _FLASH_ARRAY<T> row(_arr + index * _cols, _cols);
    return row;
  }

  void print(Print &stream) const
  {
    for (int i=0; i<_rows; ++i)
    {
      _FLASH_ARRAY<T> row(_arr + i * _cols, _cols);
      row.print(stream);
      stream.println();
    }
  }

private:
  const _DataType *_arr;
  size_t _rows, _cols;
};

/* _FLASH_STRING_ARRAY class.  Use the FLASH_STRING_ARRAY() macro to create these. */
class _FLASH_STRING_ARRAY : public _Printable
{
public:
  _FLASH_STRING_ARRAY(const prog_char **arr, size_t count) : _arr(arr), _size(count)
  { }

  size_t count() const 
  { return _size; }

  _FLASH_STRING operator[](int index) const
  { return _FLASH_STRING(_arr[index]); }

  void print(Print &stream) const
  {
    for (size_t i=0; i<_size; ++i)
    {
      _FLASH_STRING str(_arr[i]);
      str.print(stream);
      stream.println();
    }
  }

private:
  const prog_char **_arr;
  size_t _size;
};

#ifndef ARDUINO_STREAMING
#define ARDUINO_STREAMING

template<class T> 
inline Print &operator <<(Print &stream, T arg) 
{ stream.print(arg); return stream; }

#endif

inline Print &operator <<(Print &stream, const _Printable &printable) 
{ printable.print(stream); return stream; }

inline Print &operator <<(Print &stream, const _FLASH_STRING &printable) 
{ printable.print(stream); return stream; }

template<class T> 
inline Print &operator <<(Print &stream, const _FLASH_ARRAY<T> &printable) 
{ printable.print(stream); return stream; }

template<class T> 
inline Print &operator <<(Print &stream, const _FLASH_TABLE<T> &printable) 
{ printable.print(stream); return stream; }

inline Print &operator <<(Print &stream, const _FLASH_STRING_ARRAY &printable) 
{ printable.print(stream); return stream; }

