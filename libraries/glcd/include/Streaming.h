/*
Streaming.h - Arduino library for supporting the << streaming operator
Copyright (c) 2009 Mikal Hart.  All rights reserved.

 modifed to support floating point precision - mem 20 Feb 2010

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLCD.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ARDUINO_STREAMING
#define ARDUINO_STREAMING

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define STREAMING_LIBRARY_VERSION 3

// Generic template
template<class T> 
inline Print &operator <<(Print &stream, const T arg) 
{ stream.print(arg); return stream; }


/// @cond hide_from_doxygen
struct _BASED 
{ 
  long val; 
  int base;
  _BASED(long v, int b): val(v), base(b) 
  {}
};

struct _FLOAT 
{ 
  float val; 
  int digits;
  _FLOAT(double v, int d): val(v), digits(d) 
  {}
};
/// @endcond

#define _HEX(a)     _BASED(a, HEX)
#define _DEC(a)     _BASED(a, DEC)
#define _OCT(a)     _BASED(a, OCT)
#define _BIN(a)     _BASED(a, BIN)
#define _BYTE(a)    _BASED(a, BYTE)

// Specialization for class _BASED
// Thanks to Arduino forum user Ben Combee who suggested this 
// clever technique to allow for expressions like
//   Serial << _HEX(a);

inline Print &operator <<(Print &obj, const _BASED &arg)
{ obj.print(arg.val, arg.base); return obj; } 

// Specialization for class _FLOAT
inline Print &operator <<(Print &obj, const _FLOAT &arg)
{ obj.print(arg.val, arg.digits); return obj; } 


// Specialization for enum _EndLineCode
// Thanks to Arduino forum user Paul V. who suggested this
// clever technique to allow for expressions like
//   Serial << "Hello!" << endl;

enum _EndLineCode { endl };

inline Print &operator <<(Print &obj, _EndLineCode arg) 
{ obj.println(); return obj; }

#endif
