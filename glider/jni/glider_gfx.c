// ---------------------------------------------------------------------
//
// Glider
//
// Copyright (C) 1996-2014 Wayne Piekarski
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

#include "glider_gfx.h"

void draw_frame (void)
{
  int cx, cy;
  
  ansi_ClearScreen ();
  
  ansi_SetAttr (ATTR_reverse);
  ansi_Position (1, 1);
  ansi_printf ("       Glider/Android Version %s - Coded by Wayne Piekarski - %2d/%d/%2d      \n", GLIDER_VERSION, GLIDER_DAY, GLIDER_MONTH, GLIDER_YEAR);
  
  for (cy = 1; cy <= MAX_SCR_ROWS; cy++)
    {
      ansi_Position (1, cy);
      ansi_putchar (CHAR_space);
      ansi_Position (MAX_SCR_COLS, cy);
      ansi_putchar (CHAR_space);
    }
  
  ansi_Position (1, MAX_SCR_ROWS);
  for (cx = 1; cx <= MAX_SCR_COLS; cx++)
    ansi_putchar (CHAR_space);
  
  ansi_DefaultAttr ();
}


char _get_pixmap_element (int x, int y, int width, int height, char *pixmap)
{
  if ((x < 0) || (y < 0) || (x > width-1) || (y > height-1))
    return (CHAR_space);
  else
    return (pixmap [y*width+x]);
}


void move_picture (int prev_x, int prev_y, int x, int y, int width, int height, char *pixmap, int bg_width, int bg_height, char *background)
{
  int cx, cy;
  char current, previous;  
  char new_cell, old_cell;
  int dx = prev_x - x;
  int dy = prev_y - y;
  
  for (cy = max (min(dy, 0), -y+2); cy <= min (height+max(dy, 0), bg_height-y-1); cy++)
    {
      previous = 0;
      for (cx = max (min(dx, 0), -x+2); cx <= min (width+max(dx, 0), bg_width-x-1); cx++)
	{
	  current = 0;
	  
	  new_cell = get_pixmap_element (cx, cy);
	  old_cell = get_pixmap_element (cx-dx, cy-dy);
	  
	  if (new_cell != old_cell)
	    {
	      if (old_cell != CHAR_space)
		current = background [(cy+y-1)*bg_width+cx+x-1];
	      if (new_cell != CHAR_space)
		current = new_cell;
	    }
	  
	  if (current != 0)
	    {
	      if (previous == 0)
		ansi_Position (cx+x, cy+y);
	      ansi_putchar (current);
	    } 
	  previous = current;
	}
    }
}


void put_picture (int x_offset, int y_offset, int width, int height, char *pixmap, int image_attributes)
{
  int cx, cy;
  
  for (cy = 0; cy <= height-1; cy++)
    for (cx = 0; cx <= width-1; cx++)
      {
	if (image_attributes == IMAGE_CLEAR)
	  {
	    if ((cx == 0) || (pixmap [cy*width+cx-1] == CHAR_space))
	      ansi_Position (x_offset+cx, y_offset+cy);
	    if (pixmap [cy*width+cx] != CHAR_space)
	      ansi_putchar (pixmap [cy*width+cx]);
	  }
	else if (image_attributes == IMAGE_SOLID)
	  {
	    if (cx == 0)
	      ansi_Position (x_offset+cx, y_offset+cy);
	    ansi_putchar (pixmap [cy*width+cx]);
	  }
      }
}


void put_screen (char *pixmap)
{
  int cx, cy;

  for (cy = 1; cy <= MAX_SCR_ROWS-2; cy++)
    {
      ansi_Position (2, cy+1);
      for (cx = 1; cx <= MAX_SCR_COLS-2; cx++)
	ansi_putchar (pixmap [cy*MAX_SCR_COLS+cx]);
    }
}
