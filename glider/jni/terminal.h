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

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "ansi.h"


#define TRUE 1
#define FALSE 0

#ifdef __ANDROID__
#define store_term_default()
#define get_term_settings(arg)
#define set_term_settings(arg)
#define set_term_raw()
#define set_term(buf,delay)
#define set_term_delay(delay)
#define set_term_buffer(buffer)
#define set_term_default()
char terminal_getchar();
#else // __ANDROID__

#define terminal_getchar() getchar()

#define store_term_default() (tcgetattr (0, &default_terminal), \
			   current_terminal = default_terminal)

#define get_term_settings(terminal_settings) tcgetattr (0, &terminal_settings)

#define set_term_settings(terminal_settings) tcsetattr (STDIN_FILENO, TCSANOW, &terminal_settings)
  
#define set_term_raw() (current_terminal.c_lflag &= ~( ICANON | ECHO ), \
			current_terminal.c_cc [VMIN] = 1, \
			current_terminal.c_cc [VTIME] = 0, \
			tcsetattr (STDIN_FILENO, TCSANOW, &current_terminal))

#define set_term(Buffer, Delay) (current_terminal.c_cc [VMIN] = Buffer, \
				 current_terminal.c_cc [VTIME] = Delay, \
				 tcsetattr (STDIN_FILENO, TCSANOW, &current_terminal))

#define set_term_delay(Delay) (current_terminal.c_cc [VTIME] = Delay, \
			       tcsetattr (STDIN_FILENO, TCSANOW, &current_terminal))

#define set_term_buffer(Buffer) (current_terminal.c_cc [VMIN] = Buffer, \
				 tcsetattr (STDIN_FILENO, TCSANOW, &current_terminal))
  
#define set_term_default() tcsetattr (0, TCSANOW, &default_terminal)
#endif // __ANDROID__


extern struct termios default_terminal;
extern struct termios current_terminal;

struct get_arrow_keys
{
  char up_key;
  char down_key;
  char left_key;
  char right_key;
};

  
void flush_stdin (void);
void terminal_sleep (int, int);
void wait_for_keypress (void);
int get_arrow (int, struct get_arrow_keys *, struct get_arrow_keys *);
int flush_stdin_keep3 (void);
int flush_stdin_keep1 (void);
int wait_for_key (void);
int delay_for_key (int, int);
