#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "ansi.h"


#define TRUE 1
#define FALSE 0

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
