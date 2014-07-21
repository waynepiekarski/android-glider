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

#include "glider_types.h"

/* Declare thle variables to store the levels and their vents */

  char *aircraft;
  char *helicopter;
  char *pilot;
  char *gameover;
  char *winlevel;
  char *crash;
  char *intro_pixmap;
  char *menu_picture;
  char *notes_1;
  char *notes_2;
  char *help_1;
  char *help_2;
  char *credits;


void init_level_maps (void)
{
  aircraft = _aircraft [0];
  helicopter = _helicopter [0];
  pilot = _pilot [0];
  gameover = _gameover [0];
  winlevel = _winlevel [0];
  crash = _crash [0];
  intro_pixmap = _intro_pixmap [0];
  menu_picture = _menu_picture [0];
  notes_1 = _notes_1 [0];
  notes_2 = _notes_2 [0];
  help_1 = _help_1 [0];
  help_2 = _help_2 [0];
  credits = _credits [0];

  level_data [0].vents = room_escape_vents;
  level_data [0].map = room_escape_map [0];
  level_data [0].par_time = room_escape_par_time;

  level_data [1].vents = room_1_vents;
  level_data [1].map = room_1_map [0];
  level_data [1].par_time = room_1_par_time;

  level_data [2].vents = room_2_vents;
  level_data [2].map = room_2_map [0];
  level_data [2].par_time = room_2_par_time;

  level_data [3].vents = room_3_vents;
  level_data [3].map = room_3_map [0];
  level_data [3].par_time = room_3_par_time;

  level_data [4].vents = room_4_vents;
  level_data [4].map = room_4_map [0];
  level_data [4].par_time = room_4_par_time;

  level_data [5].vents = room_5_vents;
  level_data [5].map = room_5_map [0];
  level_data [5].par_time = room_5_par_time;

  level_data [6].vents = room_6_vents;
  level_data [6].map = room_6_map [0];
  level_data [6].par_time = room_6_par_time;

  level_data [7].vents = room_7_vents;
  level_data [7].map = room_7_map [0];
  level_data [7].par_time = room_7_par_time;

  level_data [8].vents = room_8_vents;
  level_data [8].map = room_8_map [0];
  level_data [8].par_time = room_8_par_time;

  level_data [9].vents = room_9_vents;
  level_data [9].map = room_9_map [0];
  level_data [9].par_time = room_9_par_time;

  level_data [10].vents = room_end_vents;
  level_data [10].map = room_end_map [0];
  level_data [10].par_time = room_end_par_time;
}
