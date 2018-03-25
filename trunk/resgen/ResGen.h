/************************************************************************************

    Copyright (C) 2008 Markus Drescher

    This file is part of Bombermaaan.

    Bombermaaan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombermaaan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombermaaan.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************************/

/// ResGen.h
#ifndef __RESGEN_H__
#define __RESGEN_H__

#include <fstream>

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "StdAfx.h" // this is the StdAfx.H in the Bombermaaan directory

#define RCFILE      "res.rc"
#define HFILE       "res.h"

#define RESDATAC    "ResData.c"

#define BUFSIZE 1024

#ifndef MIN
#define MIN(_x,_y)  ((_x) < (_y) ? (_x) : (_y))
#endif

// data structures
typedef struct _SRES
{
    int r_number;
    char *r_name;
    char *r_path;
} SRES;

enum {
    TYPE_BITMAP = 0,
    TYPE_SOUND,
    NUM_TYPES
};

using namespace std;

// function prototypes
void determineResourceCount(std::ifstream& rcfile, std::ifstream& hfile, int& rcBitmapCount, int& rcSoundCount, int& hBitmapCount, int& hSoundCount);
bool buildResourceArrayWithHeader(int hBitmapCount, int hSoundCount, std::ifstream& hfile, filebuf *hbuf, int& bitmapNo, int& soundNo);
int findResourceElement(int type, const char *name, int count);
bool completeResourceArrayWithRC(int rcBitmapCount, int rcSoundCount, std::ifstream& rcfile, filebuf *rcbuf, int& bitmapNo, int& soundNo);
bool generateSourceCode(int hBitmapCount, int hSoundCount, int realBitmapCount, int realSoundCount);

string RES_DIR;

#endif
