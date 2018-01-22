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

package net.waynepiekarski.glider

// Generate ANSI escape sequences to test out the AnsiTerminalView
object AnsiString {
    val escape = "\u001b["
    val BLACK = 0
    val RED = 1
    val GREEN = 2
    val YELLOW = 3
    val BLUE = 4
    val MAGENTA = 5
    val CYAN = 6
    val WHITE = 7

    val NORMAL = 0
    val REVERSE = 7

    fun position(r: Int, c: Int): String {
        // Negative RC values are illegal, but values that are larger than the terminal size are
        // ok and should generate wraparound automatically
        if (r < 1 || c < 1) Logging.fatal("Negative row or column value not allowed")
        return escape + r + ";" + c + "H"
    }

    fun putChar(r: Int, c: Int, ch: Char): String {
        return position(r, c) + ch
    }

    fun putString(r: Int, c: Int, str: String): String {
        return position(r, c) + str
    }

    fun clearScreen(): String {
        return escape + "2J"
    }

    fun defaultAttr(): String {
        return setAttr(NORMAL)
    }

    fun setAttr(mode: Int): String {
        return escape + mode + "m"
    }

    fun setForeground(color: Int): String {
        return escape + "3" + color + "m"
    }

    fun setBackground(color: Int): String {
        return escape + "4" + color + "m"
    }

    fun setColor(attr: Int, fg: Int, bg: Int): String {
        return setAttr(attr) + setForeground(fg) + setBackground(bg)
    }

    fun putFramedString(row: Int, col: Int, str: String, border: Int): String {
        val result = StringBuilder()
        val clen = border * 2 + str.length
        val rlen = border * 2 + 1
        for (c in 0 until clen)
            for (r in 0 until rlen)
            // Make sure we never generate a negative coordinate, but overflow is good
                if (col + c - border >= 1 && row + r - border >= 1)
                    result.append(putChar(row + r - border, col + c - border, ' '))
        result.append(putString(row, col, str))
        return result.toString()
    }

    fun putDebugGrid(width: Int, height: Int): String {
        val result = StringBuilder()
        for (c in 1..width) {
            for (r in 1..height) {
                if (c == 1 || c == width || r == 1 || r == height)
                    result.append(putChar(r, c, '#'))
                else {
                    result.append(putChar(r, c, Character.forDigit(c % 10, 10)))
                }
            }
        }
        return result.toString()
    }
}
