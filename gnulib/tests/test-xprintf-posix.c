/* Test of error-checking xprintf() function with POSIX compatible formatting.
   Copyright (C) 2007-2021 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007.  */

#include <config.h>

#include "xprintf.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "macros.h"

#include "test-printf-posix.h"

int
main (_GL_UNUSED int argc, char *argv[])
{
  test_function (xprintf);
  return 0;
}