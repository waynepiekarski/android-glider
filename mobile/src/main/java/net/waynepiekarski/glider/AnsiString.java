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

package net.waynepiekarski.glider;

// Generate ANSI escape sequences to test out the AnsiTerminalView
public class AnsiString {
    public final static String escape = "\033[";
    public final static int BLACK   = 0;
    public final static int RED     = 1;
    public final static int GREEN   = 2;
    public final static int YELLOW  = 3;
    public final static int BLUE    = 4;
    public final static int MAGENTA = 5;
    public final static int CYAN    = 6;
    public final static int WHITE   = 7;

    public final static int NORMAL  = 0;
    public final static int REVERSE = 7;

    public static String position(int r, int c) {
        // Negative RC values are illegal, but values that are larger than the terminal size are
        // ok and should generate wraparound automatically
        if ((r < 1) || (c < 1)) Logging.fatal("Negative row or column value not allowed");
        return escape + r + ";" + c + "H";
    }

    public static String putChar(int r, int c, char ch) {
        return position(r, c) + ch;
    }

    public static String putString(int r, int c, String str) {
        return position(r, c) + str;
    }

    public static String clearScreen() {
        return escape + "2J";
    }

    public static String defaultAttr() {
        return setAttr(NORMAL);
    }

    public static String setAttr(int mode) {
        return escape + mode + "m";
    }

    public static String setForeground(int color) {
        return escape + "3" + color + "m";
    }

    public static String setBackground(int color) {
        return escape + "4" + color + "m";
    }

    public static String setColor(int attr, int fg, int bg) {
        return setAttr(attr) + setForeground(fg) + setBackground(bg);
    }

    public static String putFramedString(int row, int col, String str, int border) {
        StringBuilder result = new StringBuilder();
        int clen = border*2 + str.length();
        int rlen = border*2 + 1;
        for (int c = 0; c < clen; c++)
            for (int r = 0; r < rlen; r++)
                // Make sure we never generate a negative coordinate, but overflow is good
                if ((col+c-border >= 1) && (row+r-border >= 1))
                    result.append(putChar(row+r-border, col+c-border, ' '));
        result.append(putString(row, col, str));
        return result.toString();
    }

    public static String putDebugGrid(int width, int height) {
        StringBuilder result = new StringBuilder();
        for (int c = 1; c <= width; c++) {
            for (int r = 1; r <= height; r++) {
                if ((c == 1) || (c == width) || (r == 1) || (r == height))
                    result.append(putChar(r, c, '#'));
                else {
                    result.append(putChar(r, c, Character.forDigit(c % 10, 10)));
                }
            }
        }
        return result.toString();
    }
}
