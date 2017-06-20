// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer

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
 *  \file CClock.cpp
 *  \brief The clock
 */

#include "StdAfx.h"
#include "CClock.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CClock::CClock (void)
{
    
    m_Date = 0.0f;
    m_Hours = 0;
    m_Minutes = 0;
    m_Seconds = 0;
    m_Seconds100 = 0;
    m_StartHours = 0;
    m_StartMinutes = 0;
    m_StartSeconds = 0;
    m_StartSeconds100 = 0;
    m_Pause = false;

    m_ClockType = CLOCKTYPE_COUNTDOWN;
    m_ClockMode = CLOCKMODE_MS;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CClock::~CClock (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CClock, it must be created.
// This initializes the clock with a type (evolution of the time), 
// with a mode (how to count the time), and with a date (hours, 
// minutes, seconds, seconds100)

void CClock::Create (EClockType ClockType, EClockMode ClockMode, int Hours, int Minutes, int Seconds, int Seconds100)
{
    // Set type and mode
    m_ClockType = ClockType;
    m_ClockMode = ClockMode;

    // Assert the clock numbers are valid
    ASSERT (Hours >= 0 && Hours <= 23);
    ASSERT (Minutes >= 0 && Minutes <= 59);
    ASSERT (Seconds >= 0 && Seconds <= 59);
    ASSERT (Seconds100 >= 0 && Seconds100 <= 99);

    // Set current hours/minutes/seconds/seconds100
    m_Hours = Hours;
    m_Minutes = Minutes;
    m_Seconds = Seconds;
    m_Seconds100 = Seconds100;
    
    // Count the total time in seconds
    m_Date = float(Hours * 3600 + Minutes * 60 + Seconds) + float(Seconds100) * 0.010f;

    // Remember starting hours/minutes/seconds/seconds100 to enable resets (see Reset method)
    m_StartHours = Hours;
    m_StartMinutes = Minutes;
    m_StartSeconds = Seconds;
    m_StartSeconds100 = Seconds100;

    // Clock is unpaused
    m_Pause = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CClock is not needed anymore, it should be destroyed

void CClock::Destroy (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CClock::CountHours (float &RemainingDate)
{
    while (RemainingDate >= 3600.0f)
    {
        RemainingDate -= 3600.0f;
        m_Hours++;
        if (m_Hours == 24)
            m_Hours = 0;
    }
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Count the minutes in the remaining date by decreasing
// the date and incrementing the minutes count progressively

void CClock::CountMinutes (float &RemainingDate)
{
    while (RemainingDate >= 60.0f)
    {
        RemainingDate -= 60.0f;
        m_Minutes++;
        if (m_Minutes == 60)
            m_Minutes = 0;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Count the seconds in the remaining date by decreasing
// the date and incrementing the seconds count progressively

void CClock::CountSeconds (float &RemainingDate)
{
    while (RemainingDate >= 1.0f)
    {
        RemainingDate -= 1.0f;
        m_Seconds++;
        if (m_Seconds == 60)
            m_Seconds = 0;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Count the seconds100 in the remaining date by decreasing
// the date and incrementing the seconds100 count progressively

void CClock::CountSeconds100 (float &RemainingDate)
{
    while (RemainingDate >= 0.010f)
    {
        RemainingDate -= 0.010f;
        m_Seconds100++;
        if (m_Seconds100 == 100)
            m_Seconds100 = 0;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// This updates the numbers of the clock (hours, minutes, seconds and seconds100)
// using the given deltatime.

void CClock::Update (float DeltaTime)
{
    // If the clock is not paused then update it!
    if (!m_Pause)
    {
        // Update the date (total time in seconds) according
        // to the type of the clock.
        switch (m_ClockType)
        {
            // If it's a countdown clock
            case CLOCKTYPE_COUNTDOWN :
            {
                // Decrease the date 
                m_Date -= DeltaTime;
                
                // Saturate to zero
                if (m_Date < 0.0f)
                    m_Date = 0.0f;
             
                break;
            }

            // If it's a chronometer clock
            case CLOCKTYPE_CHRONOMETER :
            {
                // Increase the date
                m_Date += DeltaTime;

                break;
            }
        }

        // Make a duplicate of the date. It will be used to
        // count progressively the numbers of the clock.
        float RemainingDate = m_Date;

        // All clock numbers to zero, because the update
        // of each clock number will be done by incrementing
        // them repeatedly.
        m_Hours = 0;
        m_Minutes = 0;
        m_Seconds = 0;
        m_Seconds100 = 0;

        // Count the numbers differently according to the
        // clock mode, that allows to know how to count.
        // For a clock mode like HM, the seconds and the
        // seconds100 won't be updated : they will be zero.
        switch (m_ClockMode)
        {
            case CLOCKMODE_HMSC :
                CountHours (RemainingDate);
                CountMinutes (RemainingDate);
                CountSeconds (RemainingDate);
                CountSeconds100 (RemainingDate);
                break;

            case CLOCKMODE_HMS :
                CountHours (RemainingDate);
                CountMinutes (RemainingDate);
                CountSeconds (RemainingDate);
                break;

            case CLOCKMODE_HM :
                CountHours (RemainingDate);
                CountMinutes (RemainingDate);
                break;

            case CLOCKMODE_MSC :
                CountMinutes (RemainingDate);
                CountSeconds (RemainingDate);
                CountSeconds100 (RemainingDate);
                break;

            case CLOCKMODE_MS :
                CountMinutes (RemainingDate);
                CountSeconds (RemainingDate);
                break;

            case CLOCKMODE_SC :
                CountSeconds (RemainingDate);
                CountSeconds100 (RemainingDate);
                break;

            case CLOCKMODE_S :
                CountSeconds (RemainingDate);
                break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// This resets the clock hours/minutes/seconds/seconds100
// to the ones that were given on clock creation using Create() method.

void CClock::Reset (void)
{
    // Reset the hours/minutes/seconds/seconds100
    m_Hours = m_StartHours;
    m_Minutes = m_StartMinutes;
    m_Seconds = m_StartSeconds;
    m_Seconds100 = m_StartSeconds100;

    // Count the total time in seconds
    m_Date = float(m_Hours * 3600 + m_Minutes * 60 + m_Seconds) + float(m_Seconds100) * 0.010f;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
