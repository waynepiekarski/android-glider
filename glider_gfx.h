/* This routine is used to "park" the cursor */

#include "ansi.h"
#include "glider_constants.h"

#define park_cursor() ansi_Position (2, 2)
#define min(x, y) (x>y ? y : x)
#define max(x, y) (x<y ? y : x)
#define put_plane(x, y, pixmap) put_picture (x, y, MAX_SCR_PLANE_X, MAX_SCR_PLANE_Y, pixmap, IMAGE_CLEAR)
#define get_pixmap_element(x, y) _get_pixmap_element (x, y, width, height, pixmap)

void draw_frame (void);
void move_picture (int, int, int, int, int, int, char *, int, int, char *);
void put_picture (int, int, int, int, char *, int);
void put_screen (char *pixmap);
char _get_pixmap_element (int, int, int, int, char *);
