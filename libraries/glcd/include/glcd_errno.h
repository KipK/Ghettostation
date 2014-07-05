/*
  glcd_errno.h
  Copyright (c) 2011 Bill Perry
  
  vi:ts=4

  This file is part of the Arduino GLCD library.

  GLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  GLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with GLCD.  If not, see <http://www.gnu.org/licenses/>.

  This file defines the error code return values
 
 */

#ifndef	GLCD_ERRNO_H
#define GLCD_ERRNO_H


/*
 * Define the error code return values for the glcd library
 */

#define GLCD_ENOERR		0	// no error This must be 0. Do not change!

#define GLCD_EBUSY		1	// glcd BUSY wait timeout
#define GLCD_ERESET		2	// glcd RESET wait timeout
#define GLCD_EINVAL		3	// invalid argument



#endif // GLCD_ERRNO_H
