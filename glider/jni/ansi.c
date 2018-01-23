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


// Enable DEBUG_ANSI to show debugging messages

#include <stdio.h>
#include "ansi.h"


void repeat_char (int counter, char charachter)
{
  while (counter != 0)
    {
      ansi_putchar (charachter);
      counter--;
    }
}


void fill_area_XY (int x1, int y1, int x2, int y2, char Fill)
{
  int cx, cy;
  
  for (cy = (y2 > y1 ? y1 : y2); cy <= (y2 > y1 ? y2 : y1); cy++)
    {
      ansi_Position (x1, cy);
      for (cx = 0; cx <= (x2 > x1 ? x2 - x1 : x1 - x2); cx++)
	ansi_printf ("%c", Fill);
    }
}


void draw_box_XY (int x1, int y1, int x2, int y2, char Horizontal, char Vertical, char Corner)
{
  int cx, cy;

  ansi_PutChar (x1, y1, Corner);
  ansi_PutChar (x2, y1, Corner);
  ansi_PutChar (x1, y2, Corner);
  ansi_PutChar (x2, y2, Corner);

  ansi_Position (x1+1, y1);
  for (cx = x1+1; cx <= x2-1; cx++)
    ansi_putchar (Horizontal);

  ansi_Position (x1+1, y2);
  for (cx = x1+1; cx <= x2-1; cx++)
    ansi_putchar (Horizontal);

  for (cy = y1+1; cy <= y2-1; cy++)
    {
      ansi_Position (x1, cy);
      ansi_putchar (Vertical);
      ansi_Position (x2, cy);
      ansi_putchar (Vertical);
    }
}


/* Assumes that the worm is shorter than the perimeter of the box
   The worm starts at (x1, y1) and moves clockwise */
void worm_box (int x1, int y1, int x2, int y2, int Horizontal, int Vertical, int Corner, int worm_length)
{
  int dx=x2-x1, dy=y2-y1;
  int total_length = 2*(dx+dy);
  int counter, worm_counter = 0;
  char Keypressed;
  int start_worm = 0;

  do
    {
      ansi_ClearScreen ();
      for (worm_counter = start_worm; worm_counter <= (worm_length+start_worm); worm_counter++)
	{
	  if (worm_counter >= total_length)
	    counter = worm_counter % total_length;
	  else
	    counter = worm_counter;
	  if (counter == 0)
	    ansi_PutChar (x1, y1, Corner);
	  else if (counter == dx)
	    ansi_PutChar (x2, y1, Corner);
	  else if (counter == dx+dy)
	    ansi_PutChar (x2, y2, Corner);
	  else if (counter == dx+dy+dx)
	    ansi_PutChar (x1, y2, Corner);
	  else if (counter < dx)
	    ansi_PutChar (x1+counter, y1, Horizontal);
	  else if (counter < dx+dy)
	    ansi_PutChar (x2, y1+counter-dx, Vertical);
	  else if (counter < dx+dy+dx)
	    ansi_PutChar (x2-(counter-dx-dy), y2, Horizontal);
	  else  /* (counter < dx+dy+dx+dy) */
	    ansi_PutChar (x1, y2-(counter-dx-dy-dx), Vertical);
	}	
      if (start_worm == total_length)
	start_worm = 1;
      else
	start_worm++;
    } while ((Keypressed = getchar ()) != 'q');
}


void draw_line_XY (int x1, int y1, int x2, int y2)
{
  int dX, dY, count, error, x, y, temp, end_y;
  int move_W, move_NW, move_N, move_NE, move_E;
  
  dX = x2 - x1;
  dY = y2 - y1;
  
  if (dY < 0)
    {
      dX = -dX;
      dY = -dY;
      
      /* Swap the points around */

      temp = x1;
      x1 = x2;
      x2 = temp;
      temp = y1;
      y1 = y2;
      y2 = temp;
    }
  
  x = x1;
  y = y1;
  end_y = y2;

  ansi_PutChar (x, y, CHAR_endpoint);

  if (dX >= 0)
    {
 
/* This case is for an angle of 0 to 45 degrees from the X-axis */
/*                                                              */
/*            __+                                               */
/*         __/                                                  */
/*      __/                                                     */
/*   +-/                                                        */
/*                                                              */
      if (dY <= dX)
	{
#ifdef DEBUG_ANSI
	  ansi_PutString (1, 2, "0 -> 45");
#endif
	  move_E = 2 * dY;
	  move_NE = 2 * (dY - dX);
	  error = 2 * dY - dX;
	  
	  for (count = 1; count <= dX; count++)
	    {
	      if (error > 0)
		{
		  x++;
		  y++;
		  error += move_NE;
		  if (count != dX)
		    ansi_PutChar (x, y, CHAR_NE);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	      else
		{
		  x++;
		  error += move_E;
		  if (count != dX)
		    if (end_y != y)
		      ansi_PutChar (x, y, CHAR_E);
		    else
		      ansi_PutChar (x, y, CHAR_dash);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	    }
	}
      
      
/* This case is for an angle of 45 to 95 degrees from the X-axis */
/*                                                               */
/*         +                                                     */
/*         |                                                     */
/*         /                                                     */
/*        |                                                      */
/*        |                                                      */
/*        /                                                      */
/*       |                                                       */
/*       +                                                       */
/*                                                               */
      else	
      	{
#ifdef DEBUG_ANSI
	  ansi_PutString (1, 2, "45 -> 95");
#endif
	  move_NE = 2 * (dY - dX);
	  move_N = -2 * dX;
	  error = dY - 2 * dX;
	  for (count = 1; count <= dY; count++)
	    {
	      if (error < 0)
		{
		  x++;
		  y++;
		  error += move_NE;
		  if (count != dY)
		    ansi_PutChar (x, y, CHAR_NE);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	      else
		{
		  y++;
		  error += move_N;
		  if (count != dY)
		    ansi_PutChar (x, y, CHAR_N);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	    }
	}
    }
  else
    {

/* This case is for an angle of 90 to 135 degrees from the X-axis */
/*                                                                */
/*         +                                                      */
/*         |                                                      */
/*          \                                                     */
/*          |                                                     */
/*          |                                                     */
/*           \                                                    */
/*           |                                                    */
/*           +                                                    */
/*                                                                */
 
      if (dY >= -dX)
	{
#ifdef DEBUG_ANSI
	  ansi_PutString (1, 2, "90 -> 135");
#endif
	  move_N = -2 * dX;
	  move_NW = -2 * (dX + dY);
	  error = -2 * dX - dY;
	  for (count = 1; count <= dY; count++)
	    {
	      if (error <= 0)
		{
		  y++;
		  error += move_N;
		  if (count != dY)
		    ansi_PutChar (x, y, CHAR_N);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	      else
		{
		  x--;
		  y++;
		  error += move_NW;
		  if (count != dY)
		    ansi_PutChar (x, y, CHAR_NW);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	    }
	}

/* This case is for an angle of 135 to 180 degrees from the X-axis */
/*                                                                 */
/*            +__                                                  */
/*               \__                                               */
/*                  \__                                            */
/*                     \-+                                         */
/*                                                                 */
      
      else
	{
#ifdef DEBUG_ANSI
	  ansi_PutString (1, 2, "135 -> 180");
#endif
	  move_NW = -2 * (dX + dY);
	  move_W = -2 * dY;
	  error = -dX - 2 * dY;
	  for (count = 1; count <= -dX; count++)
	    {
	      if (error <= 0)
		{
		  x--;
		  y++;
		  error += move_NW;
		  if (count != -dX)
		    ansi_PutChar (x, y, CHAR_NW);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	      else
		{
		  x--;
		  error += move_W;
		  if (count != -dX)
		    if (end_y != y)
		      ansi_PutChar (x, y, CHAR_W);
		    else
		      ansi_PutChar (x, y, CHAR_dash);
		  else
		    ansi_PutChar (x, y, CHAR_endpoint);
		}
	    }
	}
    }
}
