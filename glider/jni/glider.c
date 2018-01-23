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

#include <ctype.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include "terminal.h"
#include "glider_constants.h"
#include "glider_data.h"
#include "glider_levels.h"
#include "glider_init.h"
#include "glider_gfx.h"
#include "glider_stats.h"
#include "glider_anim.h"
#include "ansi.h"


struct termios default_terminal;
struct termios current_terminal;

int data_bu [num_bu+1] = { 502, 503, 504, 505, 506 };


void wait_for_select (void)
{
  int KeyPressed;

  do
    {
      KeyPressed = wait_for_key ();
    }
  while ((KeyPressed != MENU_SELECT_1) && (KeyPressed != MENU_SELECT_2) && (KeyPressed != MENU_SELECT_3));
}


int check_aircraft (int x, int y, int level, char *plane_map)
{
  int cx, cy, counter;
  int return_value = OBJ_NOTHING;

  /* Lets check for collisions with other objects */

  for (cy = 0; cy <= min (MAX_SCR_PLANE_Y-1, MAX_SCR_ROWS-y-1); cy++)
    for (cx = 0; cx <= min (MAX_SCR_PLANE_X-1, MAX_SCR_COLS-x-1); cx++)

/*  for (cx = 0; cx <= MAX_SCR_PLANE_X-1; cx++)
    for (cy = 0; cy <= MAX_SCR_PLANE_Y-1; cy++) */

      {
	if (plane_map [cy*MAX_SCR_PLANE_X+cx] != CHAR_space)
	  switch (level_data [level].map [(cy+y-1)*MAX_SCR_COLS+cx+x-1])
	    {
	    case CHAR_COLLISION: return_value = return_value | OBJ_COLLISION; break;
	    case CHAR_FIRE: return_value = return_value | OBJ_FIRE; break;
	    case CHAR_WATER: return_value = return_value | OBJ_WATER; break;
	    case CHAR_WINGAME: return_value = return_value | OBJ_WINGAME; break;
	    }
      }
  
  /* Check for collisions with any of the walls, floor, and roof */
  
  if ((y+MAX_SCR_PLANE_Y) >= FLOOR_Y)
    return_value |= OBJ_FLOOR;
  
  if (x >= (MAX_SCR_COLS))
    return_value = return_value | OBJ_RIGHTWALL;
  
  if (y <= 2)
    return_value = return_value | OBJ_ROOF;
  
  if (x <= 2)
    return_value = return_value | OBJ_LEFTWALL;
  
  /* The vents are active for VENT_WIDTH squares on each side of the specified point. */
  
  for (counter = 0; counter <= MAX_VENTS; counter++)
    {
      int vent_pos = level_data [level].vents [counter];

      if (((x <= vent_pos-VENT_WIDTH) && (vent_pos-VENT_WIDTH <= x+MAX_SCR_PLANE_X)) || ((x <= vent_pos+VENT_WIDTH) && (vent_pos+VENT_WIDTH <= x+MAX_SCR_PLANE_X)))
	return_value = return_value | OBJ_VENT;
    }
  return (return_value);
}


void draw_message (int width, char *message)
{
  int height = MAX_SCR_MESSAGE_Y;
  int x1 = (MAX_SCR_COLS - width) / 2 - MSG_BORDER_X;
  int y1 = (MAX_SCR_ROWS - height) / 2 - MSG_BORDER_Y;

  ansi_SetAttr (ATTR_reverse);
  fill_area_WH (x1+1, y1+1, width + 2 * MSG_BORDER_X, height + 2 * MSG_BORDER_Y, CHAR_space);
  draw_box_WH (x1+1, y1+1, width + 2 * MSG_BORDER_X, height + 2 * MSG_BORDER_Y, CHAR_dash, CHAR_pipe, CHAR_plus);
  put_picture (x1 + MSG_BORDER_X + 1, y1 + MSG_BORDER_Y + 1, width, height, message, IMAGE_SOLID);
  ansi_SetAttr (ATTR_normal);
}


void display_message (int message, int lives, int level, int score)
{
  char text [80];
  int height = MAX_SCR_MESSAGE_Y;
  int y1 = (MAX_SCR_ROWS - height) / 2 - MSG_BORDER_Y;

  if ((message == MSG_FLOOR) || (message == MSG_COLLISION) || (message == MSG_FIRE) || (message == MSG_WATER))
    {
      if (lives == 0)
	draw_message (MAX_SCR_GAMEOVER_X, gameover);
      else
	draw_message (MAX_SCR_CRASH_X, crash);
      sprintf (text, "Caused by %s", crash_messages [message]);
      ansi_SetAttr (ATTR_reverse);
      centre_text (text, y1 + MAX_SCR_MESSAGE_Y + 2 * MSG_BORDER_Y, MAX_SCR_COLS);
      centre_text ("Press <5> <Enter> or <Space>", y1 + MAX_SCR_MESSAGE_Y + 2 * MSG_BORDER_Y + 1, MAX_SCR_COLS);
      centre_text ("to continue....", y1 + MAX_SCR_MESSAGE_Y + 2 * MSG_BORDER_Y + 2, MAX_SCR_COLS); 
      ansi_Position (1, MAX_SCR_ROWS);
      ansi_printf (" Game Info  -  Score: [%6d]  Lives Remaining: [%2d]  Next Level: [%2d]    ", score, lives, level);
      ansi_SetAttr (ATTR_normal);
    }
  else if (message == MSG_WINLEVEL)
    draw_message (MAX_SCR_WINLEVEL_X, winlevel);
  wait_for_select ();
}


long int calculate_score (long int time, long int bonus, int level)
{
  long int temp_time;
  long int temp_score = 10000;
  long int c;

  temp_time = time - level_data [level].par_time;

  for (c = 1000; c <= temp_time; c+=1000)
    temp_score /= 2;

  if (temp_time < 0)
    temp_score = 0;

  return (1000 + temp_score + bonus * BONUS_SCORE);
}


/* Macros used to determine time spent playing each level */
#define start_time() gettimeofday (&start_time, &start_timezone)
#define end_time() gettimeofday (&end_time, &end_timezone)
#define diff_time() (((end_time.tv_sec - start_time.tv_sec) * 1000000 + end_time.tv_usec - start_time.tv_usec) / 1000)


int play_level (int level, long int *score, int lives, int total_score)
{
  int aircraft_x = 2, prev_aircraft_x = aircraft_x;
  float aircraft_y_float = 8.0;
  int aircraft_y = aircraft_y_float, prev_aircraft_y = aircraft_y;
  int direction = DIR_FORWARD;
  int aircraft_stats;
  int x_offset = 0;
  float y_offset = 0.0;
  char KeyPressed = 0;
  struct timeval start_time, end_time;
  struct timezone start_timezone, end_timezone;
  long int bonus = 0;
  struct termios stored_terminal;

  struct get_arrow_keys num_keypad = { '8', '2', '4', '6' };
  struct get_arrow_keys alpha_keypad = { 0, 0, ',', '.' };

  get_term_settings (stored_terminal);
  set_term (0, 0);
  draw_frame ();
  put_screen (level_data [level].map);
  ansi_Position (2, MAX_SCR_ROWS);
  ansi_SetAttr (ATTR_reverse);
  ansi_printf (" Game Info  -  Score: [%6d]  Lives Remaining: [%2d]  Current Level: [%2d]", total_score, lives, level);
  ansi_SetAttr (ATTR_normal);
  park_cursor ();
  ansi_fflush ();
  sleep (WAIT_FOR_UPDATE); /* Really slow terminals need a few seconds to catch up */
  start_time ();
  
  put_plane (aircraft_x, aircraft_y, aircraft);

  do
    {
      aircraft_x += x_offset;
      aircraft_y_float += y_offset;
      aircraft_y = aircraft_y_float;
      
      move_picture (prev_aircraft_x, prev_aircraft_y, aircraft_x, aircraft_y, MAX_SCR_PLANE_X, MAX_SCR_PLANE_Y, aircraft, MAX_SCR_COLS, MAX_SCR_ROWS, level_data [level].map);

      prev_aircraft_x = aircraft_x;
      prev_aircraft_y = aircraft_y;
      
      if (delay_for_key (WAIT_TIME_Sec, WAIT_TIME_uSec))
	switch (get_arrow (TRUE, &num_keypad, &alpha_keypad))
	  {
	  case DIR_left: direction = DIR_BACKWARD; break;
	  case DIR_right: direction = DIR_FORWARD; break;
	  }
      
      aircraft_stats = check_aircraft (aircraft_x, aircraft_y, level, aircraft);
      
/* Check to see if any 'events' have happened */
      
      if (aircraft_stats != OBJ_NOTHING)
	{
	  if ((aircraft_stats & OBJ_COLLISION) == OBJ_COLLISION)
	    {
	      set_term_settings (stored_terminal);
	      return (MSG_COLLISION);
	    }

	  if ((aircraft_stats & OBJ_FIRE) == OBJ_FIRE)
	    {
	      set_term_settings (stored_terminal);
	      return (MSG_FIRE);
	    }
	  
	  if ((aircraft_stats & OBJ_WATER) == OBJ_WATER)
	    {
	      set_term_settings (stored_terminal);
	      return (MSG_WATER);
	    }
	  
	  if ((aircraft_stats & OBJ_FLOOR) == OBJ_FLOOR)
	    {
	      set_term_settings (stored_terminal);	  
	      return (MSG_FLOOR);
	    }
	  
	  if ((aircraft_stats & OBJ_RIGHTWALL) == OBJ_RIGHTWALL)
	    {
	      end_time ();
	      *score = calculate_score (diff_time (), bonus, level);
	      set_term_settings (stored_terminal);
	      return (MSG_WINLEVEL);
	    }
	  
	  if ((aircraft_stats & OBJ_WINGAME) == OBJ_WINGAME)
	    {
	      end_time ();
	      *score = calculate_score (diff_time (), bonus, level);
	      set_term_settings (stored_terminal);
	      return (MSG_WINGAME);
	    }
	}
      
      if ((aircraft_stats & OBJ_BONUS) == OBJ_BONUS)
	bonus++;
      
      /* Ok, the Glider has not crashed and the level has not ended, we must now move it appropriately */
      
      x_offset = direction;
      y_offset = STEP_DOWN;
      
      if ((aircraft_stats & OBJ_VENT) == OBJ_VENT)
	y_offset = STEP_VENT;
      if ((aircraft_stats & OBJ_ROOF) == OBJ_ROOF)
	if ((aircraft_stats & OBJ_VENT) == OBJ_VENT)
	  y_offset = 0.0;
      if ((aircraft_stats & OBJ_LEFTWALL) == OBJ_LEFTWALL)
	if (direction == DIR_BACKWARD)
	  x_offset = 0;
    } while (toupper (KeyPressed) != 'Q');
  set_term_settings (stored_terminal); 
  return (MSG_QUIT);
}


void game (void)
{
  int lives = MAX_LIVES;
  int score = 0;
  long int level_score;
  int level = START_LEVEL;
  int result;

  do {
    result = play_level (level, &level_score, lives, score);
    /* The level has been played, now we must check to see what happened, display an appropriate
       message to the terminal if necessary, add up points, etc */
    
    switch (result)
      {	
      case MSG_WINLEVEL:
	score+= level_score;
	level++;
	display_message (result, lives, level, score);
	break;
      case MSG_WINGAME:
	score+= level_score;
	level++; /* This is for the log_score function */
	game_win ();
	
	lives = 0; /* This lets us get out of this function */
	
	/* Display ending message, then go back to menu */
	ansi_PutString (1, 25, "Done.");
	wait_for_select ();
	break;
      case MSG_QUIT: return;
      default: /* We know the users aircraft has crashed in some way, so deduct one life and display a message */
	lives--;
	display_message (result, lives, level, score);
	break;
      }
  } while (lives > 0);

#ifndef __ANDROID__
  log_score (score, level); /* This records the score of anyone who finishes or runs out of lives */
#endif // __ANDROID__
}


int menu_get_input (void)
{
  int cx, cy;
  int option = 0;
  int prev_option = 1;
  int x, y;
  int KeyPressed;
  struct termios stored_terminal;

  struct get_arrow_keys num_keypad = { '8', '2', '4', '6' };
  struct get_arrow_keys alpha_keypad = { 'j', 'm', ',', '.' };
 
  get_term_settings (stored_terminal);
  set_term_raw ();

/* Paint the screen with the bitmap */
  
  put_screen (menu_picture);
  
/* Put the buttons on the screen */      

  for (cx = 0; cx <= MAX_MENU_COLS-1; cx++)
    for (cy = 0; cy <= MAX_MENU_ROWS-1; cy++)
      {
	x = MENU_COL+MENU_COL_GAP*cx;
	y = MENU_ROW+MENU_ROW_GAP*cy;
	ansi_PutString (x+1, y+1, menu_options [cy*MAX_MENU_COLS+cx]);
	draw_box_XY (x, y, x+MENU_ITEM_WIDTH, y+MENU_ITEM_HEIGHT, CHAR_hyphen, CHAR_pipe, CHAR_plus);
      }

/* Keep looping until 'select' or 'Q' has been pressed and then return the option selected to the parent function */
  
  while (1)
    {
      if (prev_option != option)
	{
	  cx = prev_option % 2;
	  cy = prev_option / 2;
	  ansi_DefaultAttr ();
	  x = MENU_COL+MENU_COL_GAP*cx;
	  y = MENU_ROW+MENU_ROW_GAP*cy;
	  ansi_PutString (x+1, y+1, menu_options [cy*MAX_MENU_COLS+cx]);
	  draw_box_XY (x, y, x+MENU_ITEM_WIDTH, y+MENU_ITEM_HEIGHT, CHAR_hyphen, CHAR_pipe, CHAR_plus);
	  
	  cx = option % 2;
	  cy = option / 2;
	  ansi_SetAttr (ATTR_reverse);
	  x = MENU_COL+MENU_COL_GAP*cx;
	  y = MENU_ROW+MENU_ROW_GAP*cy;
	  ansi_PutString (x+1, y+1, menu_options [cy*MAX_MENU_COLS+cx]);
	  draw_box_XY (x, y, x+MENU_ITEM_WIDTH, y+MENU_ITEM_HEIGHT, CHAR_hyphen, CHAR_pipe, CHAR_plus);
          ansi_SetAttr (ATTR_normal);
	}
      prev_option = option;
      
      wait_for_keypress ();
      
      switch (get_arrow (TRUE, &alpha_keypad, &num_keypad))
	{
	case DIR_up:
	  if ((option != 0) && (option != 1))
	    option-=2;
	  break;
	case DIR_down:
	  if ((option != 4) && (option != 5))
	    option+=2;
	  break;
	case DIR_left:
	  if (option % 2 == 1)
	    option--;
	  break;
	case DIR_right:
	  if (option % 2 == 0)
	    option++;
	  break;
	case DIR_unknown:
	  KeyPressed = terminal_getchar ();

	  if ((KeyPressed == MENU_SELECT_1) || (KeyPressed == MENU_SELECT_2) || (KeyPressed == MENU_SELECT_3))
	    {
	      set_term_settings (stored_terminal);
	      return (option);
	    }
	  if (toupper (KeyPressed) == 'Q')
	    {
	      set_term_settings (stored_terminal);
	      return (5);
	    }
	  break;
	}
    }
}


void menu (void)
{
  int option;
  
  while (1)
    {
      option = menu_get_input ();
      
      switch (option)
	{
	case 0:
	  game ();
	  break;
	case 3:
   	  draw_frame ();
	  put_screen (credits);
	  wait_for_key ();
	  break;
	case 4:
	  draw_frame ();
	  put_screen (help_1);
	  wait_for_key ();
	  put_screen (help_2);
	  wait_for_key ();
	  break;
	case 5:
	  return;
	default:
	  ansi_Position (1, MAX_SCR_ROWS);
	  ansi_SetAttr (ATTR_reverse);
	  ansi_printf ("This option is currently not supported");
	  ansi_SetAttr (ATTR_normal);
	  break;
	}
    }
}


#ifndef __ANDROID__
void signal_handler (int signum)
{
  ansi_printf ("Signal handler %d ... Exiting...", signum);
  exit (1);
}
#endif // __ANDROID__


#ifdef __ANDROID__
int glider_main (void)
#else
int main (void)
#endif // __ANDROID__
{
  char version_string [80];

#ifndef __ANDROID__
  /* We need to install a handler for SIGSEGV and friends so rowan and his
     friends can't drop a core file! */

  signal (SIGSEGV, signal_handler);
  signal (SIGBUS, signal_handler);
  signal (SIGQUIT, signal_handler);
#endif // __ANDROID__

  store_term_default ();
  set_term_raw ();

#ifndef __ANDROID__
  log_usage ();
#endif
  init_level_maps ();

  draw_frame ();
  put_screen (intro_pixmap);
  
  sprintf (version_string, "Version %s - %s %d, %d", GLIDER_VERSION, GLIDER_MONTHSTR, GLIDER_DAY, GLIDER_YEAR);
  centre_text (version_string, 12, MAX_SCR_COLS);
  
  park_cursor ();
  wait_for_key ();
  put_screen (notes_0);
  park_cursor ();
  wait_for_key ();
  put_screen (notes_1);
  park_cursor ();
  wait_for_key ();
  put_screen (notes_2);
  park_cursor ();
  wait_for_key ();

  menu ();

  ansi_Position (1, 1);
  ansi_ClearScreen ();
  set_term_default ();

  return (0); /* Exit cleanly */
}
