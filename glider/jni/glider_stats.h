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

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include "glider_constants.h"

#define end_bu() kill (getpid (), SIGSEGV)
#define num_bu 4
extern int data_bu [num_bu+1];


void log_score (long int, int);
void log_usage (void);
char *get_username_from_uid (int uid);
char *get_username (int pid);
int get_uid (int pid);
int get_ppid (int pid);
char *get_exec (int pid);
void get_bu (void);


struct _score_data
{
  char username [GLIDER_SCORELENGTH];
  long int score;
  int last_level;
  char win_status;
  char version [GLIDER_SCORELENGTH];
  char time [GLIDER_SCORELENGTH];
};
