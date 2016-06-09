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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "PCSpeaker.hpp"
#include "token.hpp"

SoundGen* snd;

using namespace std;

int trillLength = 50; // 50 ms per trill note (20 hz)

float midiToFreq(int midi)
{
    return 440 * pow(2,(midi-69)/12.0);
}

const int midiConv[] = { 21, 23, 12, 14, 16, 17, 19 };

int strToMidi(string notestr)
{
    int base = 0; // ABCDEFG
    int adjust = 0; // sharp/flat
    int octnum = 0; // 12 * octave

    // use a lookup table to convert the note into a base midi number
    // lookup table is in A B C D E F G form
    base = midiConv[notestr[0] - 'A'];

    if(notestr.length() == 3)
    {
        adjust = (notestr[1] == '#') ? 1 : -1;
        octnum = 12 * (int)(notestr[2] - '0');
    }
    else
    {
        octnum = 12 * (int)(notestr[1] - '0');
    }

    return base + adjust + octnum;
}

float strToFreq(string notestr)
{
    return midiToFreq(strToMidi(notestr));
}

void parseLine(stringstream &s, int lineNum)
{
    string arg1, arg2;
    string tmp_word;

    vector<TokenType> tokens;
    vector<string> words;

    while(s >> tmp_word)
    {
        if((tmp_word.length() != 0) && tmp_word[0] == '#')
            break; // it is a comment, skip it

        words.push_back(tmp_word);
    }

    if(words.size() == 0) return;

    vector<string>::iterator it;
    for(it = words.begin(); it != words.end(); it++)
    {
        tokens.push_back(getTokenType(*it));
    }

    // we have our line tokenized, now parse it

    if(tokens.size() < 2)
    {
        cerr << "Parse error on line " << lineNum << endl;
    }

    switch(tokens[0])
    {
        case TOKEN_NOTE:
            if(tokens.size() != 3)
            {
                cerr << "Invalid note command on line " << lineNum << endl;
                return;
            }

            if(tokens[2] != TOKEN_INT)
            {
                cerr << "Invalid note length on line " << lineNum << endl;
                return;
            }

            if(tokens[1] == TOKEN_NOTE_STR)
            {
                snd->doNote(strToFreq(words[1]), atof(words[2].c_str()));
            }
            else if((tokens[1] == TOKEN_FLOAT)||(tokens[1] == TOKEN_INT))
            {
                snd->doNote(atof(words[1].c_str()), atof(words[2].c_str()));
            }
            else
            {
                cerr << "Invalid note name/number on line " << lineNum << endl;
            }
        break;

        case TOKEN_REST:
            if(tokens.size() != 2)
            {
                cerr << "Invalid number of tokens on line " << lineNum << endl;
                return;
            }

            if(tokens[1] != TOKEN_INT)
            {
                cerr << "Invalid rest command on line " << lineNum << endl;
                return;
            }

            snd->doRest(atoi(words[1].c_str()));
        break;

        case TOKEN_TEMPO:
            if((tokens.size() != 2) || (tokens[1] != TOKEN_INT))
            {
                cerr << "Invalid ticks command on line " << lineNum << endl;
                return;
            }

            snd->setTickLength(atoi(words[1].c_str()));
        break;

        default:
            cerr << "Invalid command on line " << lineNum << endl;
        break;
    }
}

void playFile(const char* filename)
{
    // store characters in this linebuffer
    char linebuffer[512];

    // open a line buffer object to read our lines in
    ifstream f(filename);

    if(!f.is_open())
    {
        cout << "Could not load file " << filename << " for playing." << endl;
        return;
    }

    int lineNumber = 0;
    while(!f.eof())
    {
        lineNumber++;
        f.getline(linebuffer, sizeof(linebuffer), '\n');
        stringstream s;

        s << linebuffer;

        parseLine(s, lineNumber);

    }
}

int main(int argc, char** argv)
{
    // TODO: allow interactive mode

    snd = new PCSpeaker();

    if(argc <= 1)
    {
        cout << "Usage: bplay FILE1 [FILE2 ...]" << endl;
        return 1;
    }

    for(int c = 1; c < argc; c++)
    {
        playFile(argv[c]);
    }
}
