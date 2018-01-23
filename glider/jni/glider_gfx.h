// ---------------------------------------------------------------------
//
// Glider
//
// Copyright (C) 1996-2018 Wayne Piekarski
// wayne@tinmith.net http://tinmith.net/wayne
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------

/* This routine is used to "park" the cursor */

#include "ansi.h"
#include "glider_constants.h"

#define park_cursor() ansi_Position (2, 2)
#define min(x, y) (x>y ? y : x)
#define max(x, y) (x<y ? y : x)
#define put_plane(x, y, pixmap) put_picture (x, y, MAX_SCR_PLANE_X, MAX_SCR_PLANE_Y, pixmap, IMAGE_CLEAR)
#define get_pixmap_element(x, y) _get_pixmap_element (x, y, width, height, pixmap)

void draw_frame (void);
void move_picture (int, int, int, int, int, int, char *, int, int, char *);
void put_picture (int, int, int, int, char *, int);
void put_screen (char *pixmap);
char _get_pixmap_element (int, int, int, int, char *);
