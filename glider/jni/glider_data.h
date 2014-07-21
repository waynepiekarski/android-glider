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

/* aircraft 9x3 pixmap */

char *crash_messages [] = {
"",
"collision",
"fire",
"water",
"floor" };


char *_aircraft [] = {
"|\\  __   \
========-\
   ~~~   " };

/* helicopter 15x4 pixmap */
char *_helicopter [] = {
"____________   \
    _|_      | \
   (o__)-----O-\
    '  '       " };

char *_pilot [] = {
" O \
/|\\\
/ \\" };


/* 51x6 */
char *_gameover [] = {
"  ____                         ___                 \
 / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ \
| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|\
| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |   \
 \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   \
                                                   " };


/* 67x6 */
char *_winlevel [] = {
" _                   _    ____                      _      _       \
| |    _____   _____| |  / ___|___  _ __ ___  _ __ | | ___| |_ ___ \
| |   / _ \\ \\ / / _ \\ | | |   / _ \\| '_ ` _ \\| '_ \\| |/ _ \\ __/ _ \\\
| |__|  __/\\ V /  __/ | | |__| (_) | | | | | | |_) | |  __/ ||  __/\
|_____\\___| \\_/ \\___|_|  \\____\\___/|_| |_| |_| .__/|_|\\___|\\__\\___|\
                                             |_|                   " };


/* 27x6 */
char *_crash [] = {
"  ____               _     \
 / ___|_ __ __ _ ___| |__  \
| |   | '__/ _` / __| '_ \\ \
| |___| | | (_| \\__ \\ | | |\
 \\____|_|  \\__,_|___/_| |_|\
                           " };

/*
char *_intro_pixmap [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@                                                                              @\
@  Welcome to the latest release of Glider/UX....                              @\
@         ___            __                               __         ____      @\
@        /\\_ \\    __    /\\ \\                             /  \\       / __ \\     @\
@    ____\\//\\ \\  /\\_\\   \\_\\ \\    ___   _ __      __  __ /\\__ \\     /\\ \\/\\ \\    @\
@   / __ \\ \\ \\ \\ \\/\\ \\  / __ \\  / __ \\/\\`'__\\   /\\ \\/\\ \\\\/_/\\ \\    \\ \\ \\ \\ \\   @\
@  /\\ \\_\\ \\ \\_\\ \\_\\ \\ \\/\\ \\_\\ \\/\\  __/\\ \\ \\/    \\ \\ \\_/ |  \\ \\ \\  __\\ \\ \\_\\ \\  @\
@  \\ \\____ \\/\\____\\\\ \\_\\ \\___,_\\ \\____\\\\ \\_\\     \\ \\___/    \\ \\_\\/\\_\\\\ \\____/  @\
@   \\/____\\ \\/____/ \\/_/\\/__,__/\\/____/ \\/_/      \\/__/      \\/_/\\/_/ \\/___/   @\
@     /\\____/                                                                  @\
@     \\/___/                                                                   @\
@              Copyright (C) 1996 - The Dark Scythe Corporation                @\
@                                                                              @\
@         Coded by Wayne Piekarski (9506012x@lux.levels.unisa.edu.au)          @\
@                                                                              @\
@                   |         |    Based  on  the  game  for  the  Macintosh,  @\
@                   |    _    |    Glider 3.1, written by John Calhoun - 1991  @\
@                  __|__(o)__|__                                               @\
@  ----------======/+-+/ . \\+-+\\======----------                               @\
@            _____( |_|\\___/|_| )_____                                         @\
@                  \\__/ * * \\__/            [ Press any key to continue.... ]  @\
@                   /         \\                                                @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };
*/


char *_intro_pixmap [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@                                                                              @\
@  Welcome to the latest release of Glider/UX....                              @\
@         ___            __                               __         ____      @\
@        /\\_ \\    __    /\\ \\                             /  \\       / __ \\     @\
@    ____\\//\\ \\  /\\_\\   \\_\\ \\    ___   _ __      __  __ /\\__ \\     /\\ \\/\\ \\    @\
@   / __ \\ \\ \\ \\ \\/\\ \\  / __ \\  / __ \\/\\`'__\\   /\\ \\/\\ \\\\/_/\\ \\    \\ \\ \\ \\ \\   @\
@  /\\ \\_\\ \\ \\_\\ \\_\\ \\ \\/\\ \\_\\ \\/\\  __/\\ \\ \\/    \\ \\ \\_/ |  \\ \\ \\  __\\ \\ \\_\\ \\  @\
@  \\ \\____ \\/\\____\\\\ \\_\\ \\___,_\\ \\____\\\\ \\_\\     \\ \\___/    \\ \\_\\/\\_\\\\ \\____/  @\
@   \\/____\\ \\/____/ \\/_/\\/__,__/\\/____/ \\/_/      \\/__/      \\/_/\\/_/ \\/___/   @\
@     /\\____/                                                                  @\
@     \\/___/                                                                   @\
@              Copyright (C) 1996 - The Dark Scythe Corporation                @\
@                                                                              @\
@         Coded by Wayne Piekarski (9506012x@lux.levels.unisa.edu.au)          @\
@                                                                              @\
@                                  Based  on  the  game  for  the  Macintosh,  @\
@     |           |                Glider 3.1, written by John Calhoun - 1991  @\
@     |---. ,---. |---  ,---.                                                  @\
@     |   | |---' |     ,---|      Please report bugs, problems & suggestions  @\
@     `---' `---' `---' `---`                                                  @\
@                                           [ Press any key to continue.... ]  @\
@                                                                              @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };


char *_menu_picture [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@             |\\                                                               @\
@             | \\                   ___ _    ___ ___  ___ ___    ___   ___  __ @\
@             |  |                 / __| |  |_ _|   \\| __| _ \\  / / | | \\ \\/ / @\
@             |  |)               | (_ | |__ | || |) | _||   / / /| |_| |>  <  @\
@             |  | )               \\___|____|___|___/|___|_|_\\/_/  \\___//_/\\_\\ @\
@             |  |_)                                                           @\
@            /|  / /                                                           @\
@           /_| /_/|                                                           @\
@           | | | ||                                                           @\
@           | | | ||\\                                                          @\
@          /| | | || \\                                                         @\
@         / | | | ||  \\                                                        @\
@        /  | | | ||   \\                                                       @\
@       /   | | | ||    \\                                                      @\
@      /    | | | ||     \\                                                     @\
@     /     | | | ||      \\                                                    @\
@     \\  ___| | | ||___   /                                                    @\
@      --   | | | ||\\  ---                                                     @\
@          /| | | |\\  \\                                                        @\
@         / \\_/ \\_/ \\___|                                                      @\
@        /  |     |  \\                                                         @\
@        \\__|     |__/                                                         @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };


char *menu_options [] = {
"   PLAY GAME   ",
"  HIGH SCORES  ",
" CONFIGURATION ",
"    CREDITS    ",
"     HELP      ",
"     QUIT      " };


char *_notes_1 [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@         |\\                                                                   @\
@         | \\     Greetings Glider fans! Well, its 1996 and as I promised, I   @\
@         |  |    have produced a version of Glider which incorporates lots of @\
@         |  |)   new features, speed-ups in the screen code, and bug fixes.   @\
@         |  | )  I've tested the code out quite a bit and it seems to work    @\
@         |  |_)  very well. However, there are allways problems, so if you    @\
@        /|  / /  find any, I'd like you to tell me about them and they'll be  @\
@       /_| /_/|  fixed as soon as I can.                                      @\
@       | | | ||                                                               @\
@       | | | ||\\   One of the most popular features asked for was to have     @\
@      /| | | || \\   more than one life. Well, now you have 4 lives, which     @\
@     / | | | ||  \\   should make it a tiny bit easier to beat the game. The   @\
@    /  | | | ||   \\   number of lives may be changed later on, that depends   @\
@   /   | | | ||    \\   on how easy you find the game.                         @\
@  /    | | | ||     \\                                                         @\
@ /     | | | ||      \\                                                        @\
@ \\  ___| | | ||___   /                                        [ Continued...] @\
@  --   | | | ||\\  ---                                                         @\
@      /| | | |\\  \\       If you have and problems or questions about Glider,  @\
@     / \\_/ \\_/ \\___|     please email me at 9506012x@lux.levels.unisa.edu.au  @\
@    /  |     |  \\                                                             @\
@    \\__|     |__/                      [ Press any key to continue.... ]      @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };


char *_notes_2 [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@         |\\                                                                   @\
@         | \\     I've also added move levels to the game - the standard 3     @\
@         |  |    levels which made up the original prehistoric Glider are     @\
@         |  |)   still with us, but now I've added new and more challending   @\
@         |  | )  levels. If you think you have an idea for a level, please    @\
@         |  |_)  tell me - I'm running out of ideas for new levels. Oh, and   @\
@        /|  / /  one other thing I was told is that the ending sucked. Well,  @\
@       /_| /_/|  now I've added a real cool animation to the ending, which I  @\
@       | | | ||   hope is some reward for completing my game :)               @\
@       | | | ||\\                                                              @\
@      /| | | || \\   For instructions on how to play, select HELP from the     @\
@     / | | | ||  \\   main menu and it will explain how to play and what keys  @\
@    /  | | | ||   \\   are used to control the Glider.                         @\
@   /   | | | ||    \\                                                          @\
@  /    | | | ||     \\   Well, I hope you enjoy playing Glider as much as I    @\
@ /     | | | ||      \\   have enjoyed making it....                           @\
@ \\  ___| | | ||___   /                                                        @\
@  --   | | | ||\\  ---                                                         @\
@      /| | | |\\  \\       If you have and problems or questions about Glider,  @\
@     / \\_/ \\_/ \\___|     please email me at 9506012x@lux.levels.unisa.edu.au  @\
@    /  |     |  \\                                                             @\
@    \\__|     |__/                      [ Press any key to continue.... ]      @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };
                               
                                   
char *_help_1 [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@         |\\                              _   _      _             Page 1 of 2 @\
@         | \\                            | | | | ___| |_ __                    @\
@         |  |                           | |_| |/ _ \\ | \'_ \\                   @\
@         |  |)                          |  _  |  __/ | |_) |                  @\
@         |  | )                         |_| |_|\\___|_| .__/                   @\
@         |  |_)                                      |_|                      @\
@        /|  / /                                                               @\
@       /_| /_/|  To play Glider, you can use the numeric keypad, the arrow    @\
@       | | | ||  keys, or the J M < > keys which are present on all keyboards.@\
@       | | | ||\\  When Glider asks you to press <Select>, you can press Enter,@\
@      /| | | || \\  number 5, or the Space Bar. You can swap between any of the@\
@     / | | | ||  \\  supported keys at any time, the program will not care.    @\
@    /  | | | ||   \\                                                           @\
@   /   | | | ||    \\  Note that you can only steer the Glider left and right, @\
@  /    | | | ||     \\  and the only way to move up is to use vents. The       @\
@ /     | | | ||      \\  Glider will fall all the way to the floor if you are  @\
@ \\  ___| | | ||___   /   not careful.                                         @\
@  --   | | | ||\\  ---                                                         @\
@      /| | | |\\  \\       If you have and problems or questions about Glider,  @\
@     / \\_/ \\_/ \\___|     please email me at 9506012x@lux.levels.unisa.edu.au  @\
@    /  |     |  \\                                                             @\
@    \\__|     |__/                      [ Press any key to continue.... ]      @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };


char *_help_2 [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@         |\\                              _   _      _             Page 2 of 2 @\
@         | \\                            | | | | ___| |_ __                    @\
@         |  |                           | |_| |/ _ \\ | \'_ \\                   @\
@         |  |)                          |  _  |  __/ | |_) |                  @\
@         |  | )                         |_| |_|\\___|_| .__/                   @\
@         |  |_)                                      |_|                      @\
@        /|  / /                                                               @\
@       /_| /_/|  If you hit an object, your plane will crash and take away    @\
@       | | | ||  one of your lives. Objects that can kill you are anything    @\
@       | | | ||\\  that is made up of ! # * charachters. To beat each level,   @\
@      /| | | || \\  you must make your plane touch the right hand side of the  @\
@     / | | | ||  \\  screen, then you can move on to the next level. The line  @\
@    /  | | | ||   \\  at the bottom shows your game stats like score, lives... @\
@   /   | | | ||    \\                                                          @\
@  /    | | | ||     \\  To beat the game, you must land the Glider on the      @\
@ /     | | | ||      \\  special region. (You will know it when you see it)    @\
@ \\  ___| | | ||___   /   Once your plane touches a % charachter, you win!     @\
@  --   | | | ||\\  ---                                                         @\
@      /| | | |\\  \\       If you have and problems or questions about Glider,  @\
@     / \\_/ \\_/ \\___|     please email me at 9506012x@lux.levels.unisa.edu.au  @\
@    /  |     |  \\                                                             @\
@    \\__|     |__/            [ Press any key to return to the menu.... ]      @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };


char *_credits [] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@         |\\                         ____              _ _ _                   @\
@         | \\                       / ___|_ __ ___  __| (_) |_ ___             @\
@         |  |                     | |   | \'__/ _ \\/ _` | | __/ __|            @\
@         |  |)                    | |___| | |  __/ (_| | | |_\\__ \\            @\
@         |  | )                    \\____|_|  \\___|\\__,_|_|\\__|___/            @\
@         |  |_)                                                               @\
@        /|  / /  I\'d like to thank the following people who had something to  @\
@       /_| /_/|  do with the making of this game -                            @\
@       | | | ||                                                               @\
@       | | | ||\\   Game Concept - John Calhoun (Author of Mac version)        @\
@      /| | | || \\                                                             @\
@     / | | | ||  \\  ANSI Image - Aapool Biman, Curtis Smith, Thomas Hong, and @\
@    /  | | | ||   \\  Wim Brinks (Taken from the \"ASCII Airplane Collection\"   @\
@   /   | | | ||    \\  compiled by Paul Tomblin)                               @\
@  /    | | | ||     \\                                                         @\
@ /     | | | ||      \\  Testing: Arron, Chiro, Saeed, Dave, Kat, Justin, Mark @\
@ \\  ___| | | ||___   /                                                        @\
@  --   | | | ||\\  ---    I\'d also like to thank the many people who have made @\
@      /| | | |\\  \\       Linux the best OS in the world, and the people who   @\
@     / \\_/ \\_/ \\___|     wrote Emacs.                                         @\
@    /  |     |  \\                                                             @\
@    \\__|     |__/            [ Press any key to return to the menu.... ]      @\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" };
