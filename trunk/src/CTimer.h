/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold

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
 *  \file CTimer.h
 *  \brief Header file of the timer
 *
 *  Based on the code of Dhonn Lushine
 *  http://members.aol.com/dhonn
 *  dhonn@usa.net
 */

#ifndef __CTIMER_H__
#define __CTIMER_H__

#include "StdAfx.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief The CTimer class provides an accurate timer using the Windows API.
 *
 *  It can return the current
 *  time since the construction of the timer, and
 *  the deltatime (time between last call to get
 *  deltatime and the next call). It also handles
 *  pauses during the execution so that the deltatime
 *  when resuming is not huge but normal.
 */

class CTimer
{
private:

    __int64 m_StartClock;       // Clock value on start
    double m_InvRate;           // Inversed rate of the clock
    double m_Time;              // Latest saved time value
    float m_DeltaTime;          // Latest saved delta time value
    bool m_Pause;               // Is the timer paused?
    float m_DeltaTimeAtPause;   // Delta time saved on pause and restored on resume
    float m_Speed;              // Coefficient to apply to the deltatime before returning it

private:

    // This method returns the time value at this moment
    double GetCurrentTime ()
    {
        __int64 EndClock;

        // Get the number of counts
        QueryPerformanceCounter((LARGE_INTEGER*)&EndClock);

        // Return the number of seconds
        return (double)(EndClock - m_StartClock) * m_InvRate;
    }

public:

    // The CTimer constructor initializes the windows
    // timer and throws an exception if there is an error.
    CTimer (void)
    {
        __int64 Rate;

        // Get the number of counts per second
        // If it failed

        if(!QueryPerformanceFrequency((LARGE_INTEGER*)&Rate))
        {
            // Log failure
            theLog.WriteLine ("Timer           => !!! QueryPerformanceFrequency failed.");
        }

        // Usually the rate will be 1193180
        // If the rate is zero
        if (Rate == 0)
        {
            // Log failure
            theLog.WriteLine ("Timer           => !!! Rate is zero.");
        }

        m_InvRate = 1.0 / (double)Rate;

        // Get the number of counts
        // If it failed
        if(!QueryPerformanceCounter((LARGE_INTEGER*)&m_StartClock))
        {
            // Log failure
            theLog.WriteLine ("Timer           => !!! QueryPerformanceCounter failed.");
        }

        // Initialize some members
        m_Time = 0.0;
        m_DeltaTime = 0.0f;
        m_DeltaTimeAtPause = 0.0f;
        m_Pause = false;
        m_Speed = 1.0f;
    }

    // This method updates the time value and deltatime
    // value of the timer.
    void Update ()
    {
        // Without the sleep, the clouds hopped and the mouse pointer froze (tracker item #1870410)
        Sleep( 1 );

        // The timer must not be paused
        ASSERT (!m_Pause);

        // Get the current time value
        double Time = GetCurrentTime ();

        // If timer has already been updated
        if (m_Time > 0.0)
        {
            m_DeltaTime = (float)(Time - m_Time);
            m_Time = Time;
        }
        // If timer has never been updated
        else
        {
            m_Time = Time;
            m_DeltaTime = 0.0f;
        }
    }

    // Pauses the timer, so that the delta time value is 
    // still right when resuming.
    void Pause (void)
    {
        if (!m_Pause)                                       // Timer must not be already paused
        {
            m_Pause = true;                                 // Set pause
            m_DeltaTimeAtPause = m_DeltaTime;               // Save deltatime
        }
    }

    // Resume after a pause
    void Resume (void)
    {                                                       
        if (m_Pause)                                        // Timer must not be already unpaused
        {
            m_Pause = false;                                // Set unpaused
            m_Time = GetCurrentTime() - m_DeltaTimeAtPause; // Update time
            m_DeltaTime = m_DeltaTimeAtPause;               // Update deltatime
        }
    }

    // Set coefficient to apply to the deltatime before returning it
    void SetSpeed (float Speed)
    {
        m_Speed = Speed;
    }

    // These methods are used to get the time and deltatime values
    float GetDeltaTime (void) { ASSERT(!m_Pause); return m_DeltaTime * m_Speed; }
    double GetTime (void) { ASSERT(!m_Pause); return m_Time; }
};





#endif
