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
// #define ATTR_bold 1
// #define ATTR_bright 1
// #define ATTR_underscore 4
// #define ATTR_blink 5
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
#ifdef __ANDROID__
#include <android/log.h>
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "AnsiTerminal-Glider", "%s:%d " fmt, __FILE__, __LINE__, ## __VA_ARGS__)
void ansi_fflush(void);
#define error_printf(...) ansi_printf(__VA_ARGS__), ansi_fflush()
void ansi_printf(const char* format, ...);
void ansi_putchar(char ch);
#else // __ANDROID__
#define LOGD(fmt, ...)
#define ansi_fflush() fflush(stdout)
#define error_printf(...) fprintf(stderr, __VA_ARGS__)
#define ansi_printf(...) printf(__VA_ARGS__)
#define ansi_putchar(ch) putchar(ch)
#endif // __ANDROID__

#define ansi_Position(X, Y) ansi_printf ("\033[%d;%dH", Y, X)
#define ansi_PutChar(X, Y, Char) ansi_printf ("\033[%d;%dH%c", Y, X, Char)
#define ansi_PutString(X, Y, String) ansi_printf ("\033[%d;%dH%s", Y, X, String)
#define ansi_ClearScreen() ansi_printf ("\033[2J")

#define ansi_DefaultAttr() ansi_printf ("\033[0m")
#define ansi_SetAttr(Mode) ansi_printf ("\033[%dm", Mode)
#define ansi_SetFG(FG) ansi_printf ("\033[3%dm", FG)
#define ansi_SetBG(BG) ansi_printf ("\033[4%dm", BG)
#define ansi_SetColor(Attr, FG, BG) ansi_printf ("\033[%dm\033[3%dm\033[4%dm", Attr, FG, BG)

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
