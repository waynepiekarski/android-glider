/*
  ansi.h
  Copyright (C) 1996 - The Dark Scythe Corporation
  Written by Wayne Piekarski

  Enable DEBUG_ANSI to show debugging messages
*/

#include <stdio.h>

/* Arrow key definitions */
#define DIR_up 'A'
#define DIR_down 'B'
#define DIR_left 'D'
#define DIR_right 'C'
#define DIR_unknown -1


/* Attribute definitions */
#define ATTR_normal 0
#define ATTR_bold 1
#define ATTR_bright 1
#define ATTR_underscore 4
#define ATTR_blink 5
#define ATTR_reverse 7


/* Color definitions */
#define COLOR_black 0
#define COLOR_red 1
#define COLOR_green 2
#define COLOR_yellow 3
#define COLOR_blue 4
#define COLOR_magenta 5
#define COLOR_cyan 6
#define COLOR_white 7


/* Macros for terminal operations */
#define ansi_Position(X, Y) printf ("\033[%d;%dH", Y, X)
#define ansi_PutChar(X, Y, Char) printf ("\033[%d;%dH%c", Y, X, Char)
#define ansi_PutString(X, Y, String) printf ("\033[%d;%dH%s", Y, X, String)
#define ansi_MoveCursor(Displacement, Direction) printf ("\033[%d%c", Displacement, Direction)
#define ansi_ClearScreen() printf ("\033[2J")
#define ansi_ClearLine() printf ("\033[0K")

#define ansi_DefaultAttr() printf ("\033[0m")
#define ansi_DefaultColor() printf ("\033[0;37;40m")
#define ansi_SetAttr(Mode) printf ("\033[%dm", Mode)
#define ansi_SetFG(FG) printf ("\033[3%dm", FG)
#define ansi_SetBG(BG) printf ("\033[4%dm", BG)
#define ansi_SetColor(Attr, FG, BG) printf ("\033[%dm\033[3%dm\033[4%dm", Attr, FG, BG)
#define ansi_Reset() printf ("\033[8");

#define CHAR_plus '+'
#define CHAR_endpoint '+'
#define CHAR_underscore '_'
#define CHAR_dash '-'
#define CHAR_hyphen '-'
#define CHAR_pipe '|'
#define CHAR_space ' '
#define CHAR_fwdslash '/'
#define CHAR_backslash '\\'
#define CHAR_W '_'
#define CHAR_NW '/'
#define CHAR_N '|'
#define CHAR_NE '\\'
#define CHAR_E '_'
#define CHAR_equals '='
#define CHAR_asterisk '*'

#define centre_text(text, y, screen_width) ansi_PutString ((int)(screen_width - strlen (text)) / 2, y, text)
#define draw_box_WH(x, y, width, height, Horizontal, Vertical, Corner) draw_box_XY (x, y, x+width-1, y+height-1, Horizontal, Vertical, Corner)
#define fill_area_WH(x, y, width, height, fill) fill_area_XY (x, y, x+width-1, y+height-1, fill)
#define draw_line_WH(x, y, width, height) draw_line_XY (x, y, x+width-1, y+height-1)
#define draw_box_centre(width, height, Horizontal, Vertical, Corner, screen_width, screen_height) draw_box_XY ((screen_width - width) / 2, (screen_height - height) / 2, (screen_width - width) / 2 + width, (screen_height - height) / 2 + height, Horizontal, Vertical, Corner)

void repeat_char (int, char);
void fill_area_XY (int, int, int, int, char);
void draw_box_XY (int, int, int, int, char, char, char);
void worm_box (int, int, int, int, int, int, int, int);
void draw_line_XY (int, int, int, int);
