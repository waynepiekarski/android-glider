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
char *get_exec_name (int pid);
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
