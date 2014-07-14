package net.waynepiekarski.ansiterminal;

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
        if ((r < 0) || (c < 0)) Logging.fatal("Negative row or column value not allowed");
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
}
