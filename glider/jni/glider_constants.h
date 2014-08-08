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

#define TRUE 1
#define FALSE 0

#define GLIDER_VERSION "1.0h"
#define GLIDER_DAY 18
#define GLIDER_MONTH 8
#define GLIDER_MONTHSTR "August"
#define GLIDER_YEAR 1997

#ifdef LUX
#define GLIDER_SCOREFILE "/home/lux/a/pes/9506012x/.gdata/glider.scores"
#define GLIDER_LOGFILE "/home/lux/a/pes/9506012x/.gdata/glider.log"
#define GLIDER_WINFILE "/home/lux/a/pes/9506012x/.gdata/glider.winners"
#endif

#ifdef ATHENA
#define GLIDER_SCOREFILE "/home/athena/student/9506012x/.gdata/glider.scores"
#define GLIDER_LOGFILE "/home/athena/student/9506012x/.gdata/glider.log"
#define GLIDER_WINFILE "/home/athena/student/9506012x/.gdata/glider.winners"
#endif

#ifdef TOASTER
#define GLIDER_SCOREFILE "/home/admin/9605217d/glider/glider.scores"
#define GLIDER_LOGFILE "/home/admin/9605217d/glider/glider.log"
#define GLIDER_WINFILE "/home/admin/9605217d/glider/glider.winners"
#endif

#ifdef LINUX
#define GLIDER_SCOREFILE "glider.scores"
#define GLIDER_LOGFILE   "glider.log"
#define GLIDER_WINFILE   "glider.winners"
#endif

#define GLIDER_PATHLENGTH 100
#define GLIDER_SCORELENGTH 20

#define MAX_SCR_COLS 80
#define MAX_SCR_ROWS 24
#define MAX_SCR_PLANE_X 9
#define MAX_SCR_PLANE_Y 3
#define MAX_SCR_HELICOPTER_X 15
#define MAX_SCR_HELICOPTER_Y 4
#define MAX_SCR_PILOT_X 3
#define MAX_SCR_PILOT_Y 3

#define MAX_SCR_MESSAGE_Y 6
#define MAX_SCR_GAMEOVER_X 51
#define MAX_SCR_WINLEVEL_X 67
#define MAX_SCR_CRASH_X 27

#define IMAGE_CLEAR 1
#define IMAGE_SOLID 2

#define WAIT_FOR_UPDATE 2

#define START_LEVEL 1
#define DIR_FORWARD 1
#define DIR_BACKWARD -1
#define STEP_DOWN 0.25
#define STEP_VENT -0.5
#define VENT_WIDTH 1
#define MAX_VENTS 3
#define MAX_LIVES 4
#define LAST_LEVEL 10

#define NO_VENT -32
#define MAX_ROOMS 11   /* #define'd to be one more than LAST_LEVEL */

/* Set WAIT_TIME to 0.155 seconds, or 0.155x10^6 microseconds */
#define WAIT_TIME_Sec 0
#define WAIT_TIME_uSec 155000
#define BONUS_SCORE 5000

#define WAIT_PILOT 200000 /* Time to wait for the pilot to move */
#define WAIT_HELICOPTER 50000 /* Time to wait for the helicopter to move */

#define FLOOR_Y 22

#define NO_VENT -32

#define OBJ_NOTHING 0
#define OBJ_COLLISION 1
#define OBJ_FIRE 2
#define OBJ_WATER 4
#define OBJ_VENT 8
#define OBJ_LEFTWALL 16
#define OBJ_RIGHTWALL 32
#define OBJ_ROOF 64
#define OBJ_FLOOR 128
#define OBJ_WINGAME 256
#define OBJ_BONUS 512
#define OBJ_QUIT 1024
#define OBJ_PAUSE 2048

#define MSG_COLLISION 1
#define MSG_FIRE 2
#define MSG_WATER 3
#define MSG_FLOOR 4
#define MSG_WINLEVEL 5
#define MSG_WINGAME 6
#define MSG_PAUSE 7
#define MSG_QUIT 8

#define MENU_ROW 9
#define MENU_COL 39
#define MENU_COL_GAP 19
#define MENU_ROW_GAP 5
#define MENU_ITEM_WIDTH 16
#define MENU_ITEM_HEIGHT 2

#define MENU_SELECT_1 '\n'
#define MENU_SELECT_2 '5'
#define MENU_SELECT_3 ' '

#define MSG_BORDER_X 3
#define MSG_BORDER_Y 2

#define MAX_MENU_COLS 2
#define MAX_MENU_ROWS 3
#define MENU_ROW2 14
#define MENU_ROW3 19
#define MENU_COL1 39
#define MENU_COL2 58

#define CHAR_COLLISION '#'
#define CHAR_FIRE '!'
#define CHAR_WATER '*'
#define CHAR_WINGAME '%'
#define CHAR_BONUS '@'
