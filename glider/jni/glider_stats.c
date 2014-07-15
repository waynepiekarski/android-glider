#ifndef __ANDROID__

#include "glider_stats.h"
#include <time.h>

void log_score (long int score, int level) /* wingame is TRUE if the whole game was beaten */
{
  char time_string [50];
  char *temp_time_string;
  struct timeval time_val;
  struct timezone time_zone;
  int counter = 0;
  char game_status;
  struct passwd user_info;
  FILE *fp;
  
  /* Get the current time and store it */

  gettimeofday (&time_val, &time_zone);
  temp_time_string = ctime (&time_val.tv_sec);
  strcpy (time_string, temp_time_string);
  
  /* Strip off the carriage-return that ctime put on the end */

  while (time_string [counter] != '\n')
    counter++;
  time_string [counter] = '\0';
  
  /* Get the users ID number and their username */
  user_info = *getpwuid (getuid ());

  if (level == LAST_LEVEL+1)
    {
      fp = fopen (GLIDER_WINFILE, "a");
      if (fp == NULL)
	{
	  fprintf (stderr, "Could not open win file for appending!\n");
	  exit (1);
	}
      fprintf (fp, "User %8s finished Glider v%5s with score %8ld on %s\n", user_info.pw_name, GLIDER_VERSION, score, time_string);
      fclose (fp);
    }

  if (level == LAST_LEVEL+1)
    game_status = 'W';
  else
    game_status = 'D'; 

  fp = fopen (GLIDER_SCOREFILE, "a");
  if (fp == NULL)
    {
      fprintf (stderr, "Could not open score file for appending!\n");
      exit (1);
    }
  fprintf (fp, "Version = %5s User = %8s   Score = %8ld   Level = %2d %c   Time = %s\n", GLIDER_VERSION, user_info.pw_name, score, level, game_status, time_string);
  fclose (fp);
}


void log_usage (void)
{
  char time_string [50];
  char *temp_time_string;
  struct timeval tv;
  struct timezone tz;
  int counter = 0;
  struct passwd user_info;
  FILE *fp;
  
  /* Get the current time and store it */

  gettimeofday (&tv, &tz);
  temp_time_string = ctime (&tv.tv_sec);
  strcpy (time_string, temp_time_string);

  /* Strip off the carriage-return that ctime put on the end */
  while (time_string [counter] != '\n')
    counter++;
  time_string [counter] = '\0';

  /* Get the users ID number and their username */
  user_info = *getpwuid (getuid ());
  
  fp = fopen (GLIDER_LOGFILE, "a");
  if (fp == NULL)
    {
      fprintf (stderr, "Cannot open log file for appending\n");
      exit (1);
    }
  
  fprintf (fp, "User %8s started Glider v%5s on %s\n", user_info.pw_name, GLIDER_VERSION, time_string); /***** CORE DUMP *****/
  fclose (fp);
}


char *get_username_from_uid (int uid)
{
  struct passwd user_info;
  static char username [80];
  
  user_info = *getpwuid (uid);
  strcpy (username, user_info.pw_name);
  return (username);
}


char *get_username (int pid)
{
  struct stat user_info;
  char filename [GLIDER_PATHLENGTH];
  
  sprintf (filename, "/proc/%d", pid);
  stat (filename, &user_info);
  return (get_username_from_uid (user_info.st_uid));
}


int get_uid (int pid)
{
  struct stat user_info;
  char filename [GLIDER_PATHLENGTH];

  sprintf (filename, "/proc/%d", pid);
  stat (filename, &user_info);
  return (user_info.st_uid);
}


int get_ppid (int pid)
{
  char filename [GLIDER_PATHLENGTH];
  char data [512];
  char *ptr;
  FILE *fp;
  int c = 0;
  int ppid;

  sprintf (filename, "/proc/%d/stat", pid);
  fp = fopen (filename, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Could not open /proc to get the stat information\n");
      exit (1);
    }
  fscanf (fp, "%s", data);

  ptr = data;

  while ((*(ptr++) != ' ') && (c < 3))
    c++;

  scanf (data, "%d", &ppid);

  return (ppid);
}


char *get_exec (int pid)
{
  char filename [GLIDER_PATHLENGTH];
  char data [512];
  static char exec_name [80];
  char *ptr;
  int c = 0;
  FILE *fp;

  sprintf (filename, "/proc/%d/stat", pid);
  fp = fopen (filename, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Could not open /proc to get the stat information\n");
      exit (1);
    }
  fscanf (fp, "%s", data);
  ptr = data;

  while (*(ptr++) != '(');

  while (*(ptr++) != ')')
    {
      exec_name [c] = *ptr;
      c++;
    }
  
  exec_name [c] = '\0';

  return (exec_name);
}


void get_bu (void)
{
  int c;

  /* Check that this program or its parent is not owned by a target */
  for (c = 0; c <= num_bu; c++)
    if ((get_uid (getpid ()) == data_bu [c]) || (get_uid (getppid ()) == data_bu [c]))
      end_bu ();

  /* Now we must descend through all the ancestors to check for a target owning a shell */

}



void sort_scores (void)
{
  FILE *fp;
  struct _score_data current_data;
  char bit_bucket [100];
  
  fp = fopen (GLIDER_SCOREFILE, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Could not open score file for reading\n");
      exit (1);
    }
  
  while (!feof (fp))
    {
      fscanf (fp, "Version = %s", current_data.version);

      if (current_data.version [4] != 'D')
	{
	  fscanf (fp, "User = %s Score = %ld Level = %d %c Time = %s %*c", current_data.username, &current_data.score, &current_data.last_level, &current_data.win_status, current_data.time);
	  fgets (bit_bucket, 100, fp);
	}
      else
	fgets (bit_bucket, 100, fp);
    }
  /* All data has been read in */
}
  
#endif // __ANDROID__