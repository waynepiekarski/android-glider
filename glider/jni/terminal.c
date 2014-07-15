#include "terminal.h"


#ifndef __ANDROID__

  
void flush_stdin (void)
{
  struct termios stored_terminal;
  char vortex;
  
  get_term_settings (stored_terminal);
  set_term (0, 0);

  while ((vortex = getchar ()) != -1);

  set_term_settings (stored_terminal);
}


int wait_for_key (void)
{
  char KeyPressed;
  struct termios stored_terminal;

  get_term_settings (stored_terminal);
  set_term_raw ();
  KeyPressed = getchar ();
  set_term_settings (stored_terminal);
  return (KeyPressed);
}


void wait_for_keypress (void)
{
  char KeyPressed;
  struct termios stored_terminal;
	
  get_term_settings (stored_terminal);
  set_term_raw ();
  KeyPressed = getchar ();
  ungetc (KeyPressed, stdin);
  set_term_settings (stored_terminal);
}


int flush_stdin_keep3 (void)
/* Returns FALSE if no chars are in the buffer */
{
  char c1 = -1, c2 = -1, c3 = -1, temp;
  struct termios stored_terminal;

  get_term_settings (stored_terminal);
  set_term (0, 0);

  while ((temp = getchar ()) != -1)
    {
      c3 = c2;
      c2 = c1;
      c1 = temp;
    }

  set_term_settings (stored_terminal);
  if ((c1 == -1) && (c2 == -1) && (c3 == -1))
    return (FALSE);
      
  if (c1 != -1)
    ungetc (c1, stdin);
  if (c2 != -1)
    ungetc (c2, stdin);
  if (c3 != -1)
    ungetc (c3, stdin);
  
  return (TRUE);
}

int flush_stdin_keep1 (void)
/* Returns FALSE if there are no chars remaining in the buffer */
{
  char current = -1, previous;
  struct termios stored_terminal;

  get_term_settings (stored_terminal);
  set_term (0, 0);
  
  do
    previous = current;
  while ((current = getchar ()) != -1);

  set_term_settings (stored_terminal);

  if (previous != -1)
    {
      ungetc (previous, stdin);
      return (TRUE);
    }
  else
    return (FALSE);
}

#ifdef LINUX
void terminal_sleep (int seconds, int microseconds)
{
  ansi_fflush (); /* Make sure all output gets sent to the screen before we sleep */
  usleep (seconds * 1000000 + microseconds);
}
#endif

#ifdef SUNOS
void terminal_sleep (int seconds, int microseconds)
{
  struct timespec Delay;
  
  Delay.tv_sec = seconds;
  Delay.tv_nsec = microseconds * 1000;
  
  ansi_fflush (); /* Make sure all output gets sent to the screen before we sleep */
  nanosleep (&Delay, &Delay);
}
#endif



/* Returns DIR_unknown (-1) if the current char in the stdin stream is
   unrecognized and leaves the stdin stream untouched.

   Otherwise, it returns an int DIR_up, DIR_down, DIR_left, or DIR_right
   depending on what key was pressed */

int get_arrow (int enable_arrows, struct get_arrow_keys *set1, struct get_arrow_keys *set2)
{
  char KeyPressed, KeyPressed2, KeyPressed3;
  int Exit_Code = DIR_unknown;
  struct termios stored_terminal;

  get_term_settings (stored_terminal);
  set_term (0, 0);

  if ((KeyPressed = getchar ()) != -1)
    {
      if (set1 != NULL)
	{
	  if (KeyPressed == set1->left_key)
	    Exit_Code = DIR_left;
	  else if (KeyPressed == set1->right_key)
	    Exit_Code = DIR_right;
	  else if (KeyPressed == set1->up_key)
	    Exit_Code = DIR_up;
	  else if (KeyPressed == set1->down_key)
	    Exit_Code = DIR_down;
	}
      if (set2 != NULL)
	{
	  if (KeyPressed == set2->left_key)
	    Exit_Code = DIR_left;
	  else if (KeyPressed == set2->right_key)
	    Exit_Code = DIR_right;
	  else if (KeyPressed == set2->up_key)
	    Exit_Code = DIR_up;
	  else if (KeyPressed == set2->down_key)
	    Exit_Code = DIR_down;
	}
      if ((KeyPressed == 27) && (enable_arrows))
	if ((KeyPressed2 = getchar ()) == 91)
	  {
	    switch (KeyPressed3 = getchar ())
	      {
	      case DIR_left: Exit_Code = DIR_left; break;
	      case DIR_right: Exit_Code = DIR_right; break;
	      case DIR_up: Exit_Code = DIR_up; break;
	      case DIR_down: Exit_Code = DIR_down; break;
	      default:
		ungetc (KeyPressed3, stdin);
		ungetc (KeyPressed2, stdin);
		ungetc (KeyPressed, stdin);
		break;
	      }
	  }
	else
	  {
	    ungetc (KeyPressed2, stdin);
	    ungetc (KeyPressed, stdin);
	  }
      else if (Exit_Code == DIR_unknown)
	ungetc (KeyPressed, stdin);
    }

  set_term_settings (stored_terminal);
  return (Exit_Code);
}

int delay_for_key (int seconds, int microseconds)
/* Flushes the standard input, waits the specified time, and then returns FALSE
   if no keys were pressed during this interval. Otherwise, it leaves the file pointer
   for stdin poiting to either the last char pressed, or the last 3 charachters if an
   extended key has been pressed, like one of the arrow keys */
{
  char KeyPressed, KeyPressed2;
  struct termios stored_terminal;
  
  get_term_settings (stored_terminal);
  set_term (0, 0);
  flush_stdin ();
  terminal_sleep (seconds, microseconds);

  if (flush_stdin_keep3 () == FALSE)
      return (FALSE); /* No keys were pressed at all */

  /* If the 1st and 2nd keys match those of an extended keypress, then put
     these keys back onto the stdin stream. Otherwise, just ignore them */
  
  if (((KeyPressed = getchar ()) == 27) && ((KeyPressed2 = getchar ()) == 91))
    {
      ungetc (KeyPressed2, stdin);
      ungetc (KeyPressed, stdin);
    }
  else
    ungetc (KeyPressed, stdin);

  set_term_settings (stored_terminal);
  return (TRUE);
}

#endif // __ANDROID__
