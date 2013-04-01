/*
  Flash.cpp - An Arduino library for flash-based (ROM) data collections.
  Copyright (C) 2009 Mikal Hart
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

#include "Flash.h"

_FLASH_STRING::_FLASH_STRING(const prog_char *arr) : _arr(arr) 
{ }

void _FLASH_STRING::print(Print &stream) const
{
  for (size_t i=0; ; ++i)
  {
    char c = (*this)[i];
    if (!c) break;
    stream.print(c); // print in char form
  }
}
