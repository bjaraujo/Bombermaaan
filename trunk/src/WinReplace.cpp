// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2008-2010 Markus Drescher

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
 *  \file winreplace.cpp
 *  \brief Windows function calls rewritten for Linux
 */

#include "WinReplace.h"

static abs_time_t server_start_time = { 0, 0 };  /* time of server startup */
static unsigned char *currentResource = NULL;
static unsigned long currentResourceLen = 0;

// many functions from this file have been taken from the WINE project
// see http://www.winehq.org

// WINAPI functions

// taken from the wine api

#define USE_SCANDIR

#ifdef USE_SCANDIR
static struct dirent **files;
static int filesCount = 0;
static int currentFile = 0;
static char *fileSelectSuffix = NULL;

static int file_select(const struct dirent *entry)
{
    size_t l;
    
    // do not take "." or ".." as valid file
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return false;
    else
    {
        // pattern matching
        if (fileSelectSuffix != NULL)
        {
            l = strlen(entry->d_name);
    
            if (strcasecmp(entry->d_name + l - strlen(fileSelectSuffix),
                fileSelectSuffix) != 0)
                return false;
        }
        return true;
    }
}
#else
static DIR* finddir = NULL; // finddir for _findfirst/_findnext/_findclose
#endif

long _findfirst(const char *pattern, _finddata_t *findData) {
    const char *backslash = NULL;        // backslash position; #3078839
    const char *searchPattern = NULL;    // actual search pattern
    char *directory = NULL;                // directory
    char *pos;
    char *suffix;
    
    struct dirent *entry;

#ifndef USE_SCANDIR    
    size_t fileNameLength;

    if (finddir != NULL)
    {
        // earlier directory open, close this one.
        closedir(finddir);
    }
#endif
    
    // parse directory/searchpattern.
    backslash = strrchr(pattern, '\\');
    
    // if there is no backslash present look for a slash
    if (backslash == NULL) backslash = strrchr(pattern, '/');
    
    if (backslash == NULL) { // no backslash present
        searchPattern = pattern;
        directory = (char *)malloc(2);
        if (directory == NULL) return -1;
        strcpy(directory, ".");
    }
    else { // backslash present, divide between directory and search pattern
        searchPattern = backslash + 1;
        directory = (char *)malloc(backslash - pattern + 1);
        if (directory == NULL) return -1;
            
        strncpy(directory, pattern, backslash - pattern);
        directory[backslash - pattern] = '\0';
    }
    
    // replace each occurence of \ with in / in directory
    pos = strchr(directory, '\\');
    
    while (pos) {
        pos[0] = '/';
        pos = strchr(pos + 1, '\\');
    }

    // we use a workaround for wildcard matching, since we only need *.txt
    suffix = (char *)searchPattern;
    
    while (suffix[0] == '*' || suffix[0] == '?') {
        suffix++;
    }

#ifdef USE_SCANDIR
    if (files != NULL)
    {
        for (int i = 0; i < filesCount; i++)
            free(files[i]);
        free(files);
    }
    
    if (fileSelectSuffix != NULL) 
        free(fileSelectSuffix);
    
    fileSelectSuffix = (char *)malloc(strlen(suffix) + 1);
    
    if (fileSelectSuffix == NULL) 
        return -1;
    
    strcpy(fileSelectSuffix, suffix);
    
    // sort files alphabetically
    filesCount = scandir(directory, &files, file_select, alphasort);
    currentFile = 0;
#else
    finddir = opendir(directory);
    
    if (finddir == NULL) {
        return -1;
    }
#endif
    
    free(directory);
    
    // save suffix
    if (findData->suffix != NULL) {
        free(findData->suffix);
        findData->suffix = NULL;
    }
    
    findData->suffix = (char *)malloc(strlen(suffix) + 1);
    if (findData->suffix == NULL) return -1;

    strcpy(findData->suffix, suffix);
    
    // read directory entry
#ifdef USE_SCANDIR
    if (filesCount > 0)
    {
        entry = files[0];
        
        if (findData->name != NULL) {
            // clear old one
            free(findData->name);
            findData->name = NULL;
        }
        
        // create new one
        findData->name = (char *)malloc(strlen(entry->d_name) + 1);
        if (findData->name == NULL) return -1;
            
        strcpy(findData->name, entry->d_name);
    }
    else
        entry = NULL;
#else
    entry = readdir(finddir);
    
    while (entry != NULL) {
        // so now check the suffix
        fileNameLength = strlen(entry->d_name);
    
        if (strcasecmp(entry->d_name + fileNameLength - strlen(suffix), suffix) == 0) {
            // found
            if (findData->name != NULL) {
                // clear old one
                free(findData->name);
                findData->name = NULL;
            }
            
            // create new one
            findData->name = (char *)malloc(strlen(entry->d_name) + 1);
            if (findData->name == NULL) return -1;
                
            strcpy(findData->name, entry->d_name);
            break;
        }
        
        entry = readdir(finddir);
    }
#endif
    
    if (entry == NULL) {
        // clean file structure
        if (findData->name != NULL) {
            free(findData->name);
            findData->name = NULL;
        }
        
        if (findData->suffix != NULL) {
            free(findData->suffix);
            findData->suffix = NULL;
        }
        
        return -1;
    }
    else {
        return 1234; // we don't care for an handler
    }        
}

long _findnext(long handle, _finddata_t *findData) {
    struct dirent *d;
        
#ifndef USE_SCANDIR    
    size_t fileNameLength;
    if (finddir == NULL) return -1;
#endif
    
    if (findData == NULL || findData->suffix == NULL) {
        // no open directory
        return -1;
    }
    
#ifdef USE_SCANDIR
    currentFile++;
    if (currentFile < filesCount)
    {
        d = files[currentFile];
        
        if (findData->name != NULL) {
            // clear old one
            free(findData->name);
            findData->name = NULL;
        }

        // create new one
        findData->name = (char *)malloc(strlen(d->d_name) + 1);
        if (findData->name == NULL) return -1;
    
        strcpy(findData->name, d->d_name);
    }
    else
        d = NULL;
#else
    // read directory entry
    d = readdir(finddir);
    
    while (d != NULL) {
        // so now check the suffix
        fileNameLength = strlen(d->d_name);
    
        if (strcasecmp(d->d_name + fileNameLength - strlen(findData->suffix),
            findData->suffix) == 0) {
            // found
            if (findData->name != NULL) {
                // clear old one
                free(findData->name);
                findData->name = NULL;
            }
            
            // create new one
            findData->name = (char *)malloc(strlen(d->d_name) + 1);
            if (findData->name == NULL) return -1;
                
            strcpy(findData->name, d->d_name);
            break;
        }
        
        d = readdir(finddir);
    }
#endif

    if (d == NULL) {
        // clean file structure
        if (findData->name != NULL) {
            free(findData->name);
            findData->name = NULL;
        }
        
        if (findData->suffix != NULL) {
            free(findData->suffix);
            findData->suffix = NULL;
        }
        
        return -1;
    }
    else {
        return 1234; // we don't care for an handler
    }        
}

int _findclose(long handle) {
    // close: clear suffix, filename, directory, ...?
    
#ifdef USE_SCANDIR
    if (files != NULL)
    {
        for (int i = 0; i < filesCount; i++)
            free(files[i]);
        free(files);
    }
    files = NULL;
    filesCount = 0;
    
    free(fileSelectSuffix);
    fileSelectSuffix = NULL;
#else
    closedir(finddir);
    finddir = NULL;
#endif
    
    return -1;
}

bool NtQueryPerformanceCounter( PLARGE_INTEGER Counter, PLARGE_INTEGER Frequency )
{
    struct timeval now;

    if (!Counter) return false;
    gettimeofday( &now, 0 );
    /* convert a counter that increments at a rate of 1 MHz
     * to one of 1.193182 MHz, with some care for arithmetic
     * overflow ( will not overflow for 5000 years ) and
     * good accuracy ( 105/88 = 1.19318182) */
    Counter->QuadPart = (((now.tv_sec - server_start_time.sec) * (ULONGLONG)1000000 +
                          (now.tv_usec - server_start_time.usec)) * 105) / 88;
    if (Frequency) Frequency->QuadPart = 1193182;
    return true;
}

bool QueryPerformanceFrequency(PLARGE_INTEGER frequency)
{
    LARGE_INTEGER counter;
    NtQueryPerformanceCounter( &counter, frequency );
    return true;
}

bool QueryPerformanceCounter(PLARGE_INTEGER counter)
{
    NtQueryPerformanceCounter( counter, NULL );
    return true;
}

/**
 * @brief   adapted version of GetObject for our purposes. replaced 2nd param
 * @param   hBitmap     the resource id to the desired bitmap
 * @param   ignored
 * @param   lpvObject   pointer to data
 * @return  returns size of bitmap
 */
int GetObject(HBITMAP hBitmap, int ignored, LPVOID *lpvObject)
{
    // type == BITMAP
    //currentResource = getResource(0, hBitmap, &currentResourceLen);
    
    if (currentResource == NULL)
        return 0;
    else
    {
        *lpvObject = currentResource;
        return currentResourceLen;
    }
}

/**
 * @brief   clone of FindResource function in Windows API - inspired by Wine
 *          note: the syntax is taken from FindResourceA which calls
 *          FindResourceExA (except: name is an int)
 * @param   hModule hModule (ignored)
 * @param   name    name of the resource (as int)
 * @param   type    type of the resource
 * @return  returns a handle
 */
HRSRC FindResource( HMODULE hModule, int name, LPCSTR type )
{
    // currently we have two types of resources: BITMAP and SOUND    
    if (strncmp(type, "BITMAP", 6) != 0 || strncmp(type, "SOUND", 5) == 0)
        return NULL;
    
    //currentResource = getResource(t, name, &currentResourceLen);
    
    if (currentResource != NULL)
        return (HRSRC)1234; // the handler is being ignored
    else
        return NULL;
}

/**
 * @brief   clone of LoadResource
 * @param   hModule     hModule (ignored)
 * @param   hResource   resource handler returned by FindResource
 * @return  global      handle to the resource
 */
HGLOBAL LoadResource (HMODULE hModule, HRSRC hResource)
{
    if (currentResource != NULL)
        return (HGLOBAL)1234;
    else
        return NULL;
}

/**
 * @brief   clone of LockResource
 * @param   hGlobal handle obtained by LoadResource
 * @return  pointer to the demanded resource
 */
LPVOID LockResource (HGLOBAL hGlobal)
{
    return currentResource;
}

/**
 * @brief   clone of SizeofResource (determine data length of a certain res)
 * @param   hModule     hModule (ignored)
 * @param   hResource   resource handle
 * @return  size of data buffer
 */
DWORD SizeofResource (HMODULE hModule, HRSRC hResource)
{
    return currentResourceLen;    
}
