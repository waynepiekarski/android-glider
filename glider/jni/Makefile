CFLAGS = -DLINUX -Wall -O2 -g
LFLAGS =
#CFLAGS = -DSUNOS -DLUX -Wall -O2
#CFLAGS = -DSUNOS -DTOASTER -Wall -O2
#LFLAGS = -lposix4

glider:		terminal.o ansi.o glider_anim.o glider_gfx.o glider_stats.o glider.o
		gcc -o glider glider.o ansi.o terminal.o glider_anim.o glider_gfx.o glider_stats.o $(CFLAGS) $(LFLAGS)

glider.o:	glider.c glider_constants.h glider_data.h glider_levels.h glider_init.h glider_gfx.h glider_stats.h glider_anim.h
		gcc -c glider.c $(CFLAGS)

terminal.o:	terminal.c terminal.h
		gcc -c terminal.c $(CFLAGS)

ansi.o:		ansi.c ansi.h
		gcc -c ansi.c $(CFLAGS)

glider_anim.o:	glider_anim.c glider_anim.h glider_constants.h glider_gfx.h glider_types.h
		gcc -c glider_anim.c $(CFLAGS)

glider_gfx.o:	glider_gfx.c glider_gfx.h glider_constants.h ansi.h
		gcc -c glider_gfx.c $(CFLAGS)

glider_stats.o:	glider_stats.c glider_constants.h glider_stats.h
		gcc -c glider_stats.c $(CFLAGS)

clean:
		rm -f *.o glider

pure:		clean
		rm -f *~
