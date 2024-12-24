// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
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
 *  \file CConsole.cpp
 *  \brief The console (helpful during development)
 */

#include "CConsole.h"
#include "StdAfx.h"

//! Display a dot on the console every X repeated messages
#define REPEATED_MESSAGES_LIMIT 300

CConsole::CConsole()
    : m_StdOut(nullptr)
    , m_Open(false)
    , m_Color(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE)
    , m_NumberOfRepeatedMessages(0)
    , m_FilterRepeatedMessage(true)
{
    // No message
    m_Message[0] = '\0';
}

CConsole::~CConsole()
{
    // If the console is opened
    if (m_Open)
    {
        // Close the console
        Close();
    }
}

CConsole& CConsole::GetConsole()
{
    static CConsole rConsole;

    // Return the console singleton
    return rConsole;
}

void CConsole::Open()
{
    // If the console is not opened yet
    if (!m_Open)
    {
        // Create a console window
#ifdef WIN32
        AllocConsole();

        // Get the console output (needed to send data to the console)
        m_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
#else
        m_StdOut = stdout;
#endif

        // The console window is now opened
        m_Open = true;
    }
}

void CConsole::Close()
{
    // If the console is opened
    if (m_Open)
    {
        // Destroy the console window
#ifdef WIN32
        FreeConsole();
#endif

        // The console window is not opened anymore
        m_Open = false;
    }
}

void CConsole::Write(const char* pMessage, ...)
{
    // If the console is opened
    if (m_Open)
    {
        // Format the given string using the given arguments ("..." parameter)
        char Message[2048];
        va_list arg;
        va_start(arg, pMessage);
        vsprintf(Message, pMessage, arg);
        va_end(arg);

        // If we have to filter repeated messages
        if (m_FilterRepeatedMessage)
        {
            // If the last message written to the console is not the same
            if (strcmp(Message, m_Message) != 0)
            {
                // Send the formatted string to the console output
#ifdef WIN32
                DWORD Count;
                WriteConsole(m_StdOut, Message, strlen(Message), &Count, nullptr);
#else
                fprintf(m_StdOut, "%s", Message);
#endif

                // Save the message
                strcpy(m_Message, Message);

                // Stop the chain of repeated messages (if there is one)
                m_NumberOfRepeatedMessages = 0;
            }
            // If the last message written to the console is the same
            else
            {
                // It's a repeated message
                m_NumberOfRepeatedMessages++;

                // Show that messages are being repeated, by writing a dot
                // every REPEATED_MESSAGES_LIMIT repeated messages.
                if ((m_NumberOfRepeatedMessages % REPEATED_MESSAGES_LIMIT) == 0)
                {
#ifdef WIN32
                    DWORD Count;
                    WriteConsole(m_StdOut, ".", 1, &Count, nullptr);
#else
                    fprintf(m_StdOut, ",");
#endif
                }
            }
        }
        // If we don't have to filter repeated messages
        else
        {
            // Send the formatted string to the console output
#ifdef WIN32
            DWORD Count;
            WriteConsole(m_StdOut, Message, strlen(Message), &Count, nullptr);
#else
            fprintf(m_StdOut, "%s", Message);
#endif
        }
    }
}
