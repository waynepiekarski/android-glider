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

#include "terminal.h"
#include "glider_types.h"
#include "glider_constants.h"
#include "glider_gfx.h"

extern char *pilot;
extern char *helicopter;
extern struct _level_data level_data [MAX_ROOMS];

void game_win (void)
{
  int x, y, prev_x, prev_y;
  int count;
  
  x = 53;
  y = 6;

/* Pilot appears out of aircraft */

  put_picture (x, y, MAX_SCR_PILOT_X, MAX_SCR_PILOT_Y, pilot, IMAGE_CLEAR);

  prev_y = y;
  
/* Pilot moves down ladder */
  
  for (y = 7; y <= 19; y++)
    {
      move_picture (x, prev_y, x, y, MAX_SCR_PILOT_X, MAX_SCR_PILOT_Y, pilot, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_y = y;
      terminal_sleep (0, WAIT_PILOT);
    }

/* Move the pilot a bit to the left now */

  prev_x = x;
  y--;

  for (x = 52; x >= 44; x--)
    {
      move_picture (prev_x, y, x, y, MAX_SCR_PILOT_X, MAX_SCR_PILOT_Y, pilot, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_x = x;
      terminal_sleep (0, WAIT_PILOT);
    }

  /* Pilot is in position - move in the helicopter from the right*/

  y = 13;
  prev_x = 79;

  for (x = 79; x >= 34; x--)
    {
      move_picture (prev_x, y, x, y, MAX_SCR_HELICOPTER_X, MAX_SCR_HELICOPTER_Y, helicopter, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_x = x;
      terminal_sleep (0, WAIT_HELICOPTER);
    }


  /* Pilot waves */

  for (count = 0; count <= 3; count++)
    {
      terminal_sleep (0, WAIT_PILOT);
           
      ansi_Position (44, 19);
      ansi_printf ("\\O/");
      ansi_Position (44, 20);
      ansi_printf (" | ");

      terminal_sleep (0, WAIT_PILOT);
      ansi_Position (44, 19);
      ansi_printf (" O ");
      ansi_Position (44, 20);
      ansi_printf ("/|\\");
    }

  /* Helicopter lowers rope */

  for (y = 16; y <= 19; y++)
    {
      terminal_sleep (0, WAIT_PILOT);
      ansi_Position (40, y);
      ansi_putchar ('|');
    }

/* Pilot now moves under the rope */

  y = 19;
  prev_x = 44;
  for (x = 43; x >= 39; x--)
    {
      move_picture (prev_x, y, x, y, MAX_SCR_PILOT_X, MAX_SCR_PILOT_Y, pilot, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_x = x;
      terminal_sleep (0, WAIT_PILOT);
    }

  prev_y = y;
  x = 39;

  for (y = 18; y >= 15; y--)
    {
      move_picture (x, prev_y, x, y, MAX_SCR_PILOT_X, MAX_SCR_PILOT_Y, pilot, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_y = y;
      terminal_sleep (0, WAIT_PILOT);
    }

/* Erase pilot */

  y = 15;
  for (count = y; count < y + 3; count++)
    ansi_PutString (39, count, "   ");
  y = 13;
  x = 34;
  put_picture (x, y, MAX_SCR_HELICOPTER_X, MAX_SCR_HELICOPTER_Y, helicopter, IMAGE_SOLID);

/* Move the helicopter off the screen */

  prev_x = x;

  for (x = 33; x >= -MAX_SCR_HELICOPTER_X; x--)
    {
      move_picture (prev_x, y, x, y, MAX_SCR_HELICOPTER_X, MAX_SCR_HELICOPTER_Y, helicopter, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [LAST_LEVEL].map);
      prev_x = x;
      terminal_sleep (0, WAIT_HELICOPTER);
    }

  for (count = LAST_LEVEL - 1; count >= 0; count--)
    {
      put_screen (level_data [count].map);
      sleep (WAIT_FOR_UPDATE);
      
      prev_x = 80;
      for (x = 79; x >= -MAX_SCR_HELICOPTER_X; x--)
	{
	  move_picture (prev_x, y, x, y, MAX_SCR_HELICOPTER_X, MAX_SCR_HELICOPTER_Y, helicopter, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [count].map);
	  prev_x = x;
	  terminal_sleep (0, WAIT_HELICOPTER);
	}
    }
}
