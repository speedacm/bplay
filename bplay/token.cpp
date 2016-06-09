/*
    Copyright (c) 2011 UofL Speed ACM

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "token.hpp"

#include <cstring>

using namespace std;

void toUpper(string& s)
{
    for(int c = 0; c < s.length(); c++)
    {
        if(!isalpha(s[c]))
            continue;
        s[c] = toupper(s[c]);
    }
}

int countOccurrences(string str, char c)
{
    int sum = 0;

    for(int n = 0; n < str.length(); n++)
    {
        if(str[n] == c) sum++;
    }

    return sum;
}

TokenType isNumberType(string word)
{
    // check to make sure it contains only numbers and a period
    // we don't need negative numbers or sci. notation
    if(strcspn(word.c_str(), "0123456789.") != 0)
        return TOKEN_NONE;

    if(strchr(word.c_str(), '.') != NULL)
    {
        if(countOccurrences(word, '.') != 1)
            return TOKEN_NONE;
        return TOKEN_FLOAT;
    }

    return TOKEN_INT;
}

TokenType isNoteString(string word)
{
    if(word.length() < 2) // we have to have at least a note and an octave
        return TOKEN_NONE;
    if(word.length() > 3) // too many characters (an octave of 10 is too high)
        return TOKEN_NONE;

    if(isalpha(word[0]))
        word[0] = toupper(word[0]); // capitalize the note name

    // make sure the first letter is a note name
    if(!(word[0] >= 'A' && word[0] <= 'G'))
        return TOKEN_NONE;

    if(word.length() == 3) // we have a sharp or a flat
    {
        if((word[1] != '#')&&(word[1] != 'b')) // make sure it's a sharp/flat
            return TOKEN_NONE;
        if(!(word[2] >= '0' && word[2] <= '9')) // make sure the third char is a number
            return TOKEN_NONE;
    }
    else
    {
        if(!(word[1] >= '0' && word[1] <= '9')) // if not a digit...
            return TOKEN_NONE;
    }

    return TOKEN_NOTE_STR;
}

TokenType getTokenType(string word)
{
    if(word == "TEMPO")
    {
        return TOKEN_TEMPO;
    }
    else if(word == "NOTE")
    {
        return TOKEN_NOTE;
    }
    else if(word == "REST")
    {
        return TOKEN_REST;
    }
    else if(word == "TRILL")
    {
        return TOKEN_TRILL;
    }

    // check to see if it's a number

    TokenType tmp = isNumberType(word);

    if(tmp != TOKEN_NONE)
        return tmp; // it will be a number

    // now we need to see if it's a note string
    return isNoteString(word);
}
