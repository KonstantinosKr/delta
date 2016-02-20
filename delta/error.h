/*
Copyright (c) Durham University, 2013

This file is part of PARMES.

PARMES is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License
version 3 as published by the Free Software Foundation.

PARMES is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PARMES. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __error__
#define __error__

/* textual assertion */
#define ASSERT(__test__, ...)\
  do {\
  if (!(__test__)) { fprintf (stderr, "%s: %d => ", __FILE__, __LINE__);\
    fprintf (stderr, __VA_ARGS__);\
    fprintf (stderr, "\n"); exit (1); } } while (0)

/* memory validity assertion */
#define ERRMEM(__pointer__) ASSERT (__pointer__, "Out of memory!");

#endif
