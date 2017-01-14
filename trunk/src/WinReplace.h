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

/**
 *  \file winreplace.h
 *  \brief Replaces a windows.h include (for Linux)
 */

#ifndef __WINREPLACE_H__
#define __WINREPLACE_H__

#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>

// macros
#define MAX_PATH 260

#define SW_SHOW 5

#define WM_CREATE 1
#define WM_DESTROY 2
#define WM_MOVE 3
#define WM_SIZE 5
#define WM_PAINT 15
#define WM_CLOSE 16
#define WM_ACTIVATEAPP 28
#define WM_KEYDOWN 256
#define WM_KEYUP 257
#define WM_INITDIALOG 272
#define WM_COMMAND 273
#define WM_SYSCOMMAND 274

#define WA_ACTIVE 1
#define WA_CLICKACTIVE 2

#define VK_F1		SDLK_F1
#define VK_F2		SDLK_F2
#define VK_F3		SDLK_F3
#define VK_F4		SDLK_F4
#define VK_F5		SDLK_F5
#define VK_F12		SDLK_F12
#define VK_RETURN	SDLK_RETURN
#define VK_MULTIPLY	SDLK_KP_MULTIPLY
#define VK_DIVIDE	SDLK_KP_DIVIDE
#define VK_ADD		SDLK_KP_PLUS
#define VK_SUBTRACT	SDLK_KP_MINUS
#define VK_NUMPAD0	SDLK_KP0
#define VK_NUMPAD1	SDLK_KP1
#define VK_NUMPAD2	SDLK_KP2
#define VK_NUMPAD3	SDLK_KP3
#define VK_NUMPAD4	SDLK_KP4

#define FOREGROUND_BLUE 1
#define FOREGROUND_GREEN        2
#define FOREGROUND_RED  4
#define FOREGROUND_INTENSITY    8
#define BACKGROUND_BLUE 16
#define BACKGROUND_GREEN        32
#define BACKGROUND_RED  64
#define BACKGROUND_INTENSITY    128

// some types
#ifndef HANDLE
typedef FILE TMP_HANDLE, *HANDLE;
#define DECLARE_HANDLE(n) typedef struct n##__{int i;}*n

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HGLOBAL);
typedef void TMP_HMODULE, *HMODULE;
#endif

#ifndef __int64
typedef long long __int64;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef CHAR
typedef char CHAR;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif
	
#ifndef LONG
typedef long LONG;
#endif

#ifndef HRESULT
typedef LONG HRESULT;
#endif

#ifndef LONGLONG
typedef long long LONGLONG;
#endif

#ifndef ULONGLONG
typedef unsigned long long ULONGLONG;
#endif

#ifndef UINT_PTR
typedef  unsigned int UINT_PTR, *PUINT_PTR;
#endif

#ifndef LONG_PTR
typedef  long LONG_PTR, *PLONG_PTR;
#endif

#ifndef LPVOID
typedef void *PVOID,*LPVOID;
#endif

#ifndef WPARAM
typedef LONG_PTR WPARAM;
#endif

#ifndef LPARAM
typedef LONG_PTR LPARAM;
#endif

#ifndef SOCKET
typedef u_int   SOCKET;
#endif

#ifndef LPCSTR
typedef const CHAR *PCSTR,*LPCSTR;
#endif

// this is actually wrong but fits our purposes
#ifndef HBITMAP
typedef int HBITMAP;
#endif

#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR    (-1)

#ifndef MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) (i)
#endif

typedef struct sockaddr_in SOCKADDR_IN;
#define ADDR_ANY        INADDR_ANY
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr *LPSOCKADDR;

#ifndef _LARGE_INTEGER
typedef union _LARGE_INTEGER {
#if ! defined(NONAMELESSUNION) || defined(__cplusplus)
	struct { //_ANONYMOUS_STRUCT
	  DWORD LowPart;
	  LONG  HighPart;
	} a;
#endif /* NONAMELESSUNION */
	struct {
		DWORD LowPart;
		LONG  HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
#endif

#ifndef RECT
typedef struct tagRECT 
{
   LONG left;
   LONG top;
   LONG right;
   LONG bottom;
} RECT; // , *PRECT, NEAR *NPRECT, FAR *LPRECT
#endif

#ifndef Sleep
#  define Sleep(X) usleep(X*1000);
#endif

// _finddata_t data structure
typedef struct
{
	char	*name;		// current filename
	char	*suffix;	// current suffix looking for
} _finddata_t;

// taken from the wine api

typedef struct
{
    int            sec;
    int            usec;
} abs_time_t;

// function prototypes

long _findfirst(const char *pattern, _finddata_t *findData);
long _findnext(long handle, _finddata_t *findData);
int _findclose(long handle);

bool NtQueryPerformanceCounter( PLARGE_INTEGER Counter, PLARGE_INTEGER Frequency );
bool QueryPerformanceFrequency(PLARGE_INTEGER frequency);
bool QueryPerformanceCounter(PLARGE_INTEGER counter);

int GetObject(HBITMAP hBitmap, int ignored, LPVOID *lpvObject);
HRSRC FindResource(HMODULE hModule,int name,LPCSTR type);
HGLOBAL LoadResource (HMODULE hModule, HRSRC hResource);
LPVOID LockResource (HGLOBAL hGlobal);
DWORD SizeofResource (HMODULE hModule, HRSRC hResource);

// this function is in the shared library
extern "C"
{
    unsigned char *getResource(int type, int id, unsigned long *len);
}

#endif
