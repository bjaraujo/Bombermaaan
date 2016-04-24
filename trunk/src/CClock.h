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
 *  \file CClock.h
 *  \brief Header file of the clock
 */

#ifndef __CCLOCK_H__
#define __CCLOCK_H__

#include "StdAfx.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Clock type : describes how to make the date evolve

enum EClockType
{
    CLOCKTYPE_CHRONOMETER,   //!< Chronometer from a given date A:B:C 
    CLOCKTYPE_COUNTDOWN      //!< Count down from a given date A:B:C
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Clock mode : describes what time units to count.
/*! 
For instance, if the user of CClock wants the
seconds count and the minutes count of the time,
he will choose the 'MS' mode.
*/

enum EClockMode
{
    CLOCKMODE_HMSC,  //!< Compute Hours and Minutes and Seconds and Seconds100 for each update
    CLOCKMODE_HMS,   //!< Compute Hours and Minutes and Seconds for each update
    CLOCKMODE_HM,    //!< Compute Hours and Minutes for each update
    CLOCKMODE_MSC,   //!< Compute Minutes = total minutes, and Seconds and Seconds100 for each update
    CLOCKMODE_MS,    //!< Compute Minutes = total minutes, and Seconds for each update
    CLOCKMODE_SC,    //!< Compute Seconds = total seconds, and Seconds100 for each update
    CLOCKMODE_S      //!< Compute Seconds = total seconds, for each update
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! CClock is a reusable class which manages a clock with several types and modes. 
/*! 
You have to periodically update the clock after creating it. 
You then just have to get the "date parts" (hours/minutes/seconds/seconds100)
according to the clock mode you chose.<BR>
<BR>
<B>Basic use of the CClock class</B>
\code
// Make a CClock object
CClock Clock;
// Initialize the clock. 
Clock.Create (CLOCKTYPE_COUNTDOWN, // We want a count down clock
              CLOCKMODE_MSC, // We want to be able to extract these date 
                             // components : minutes, seconds, seconds100.
              3,  // Initialize the hours component of the clock
              54, // Initialize the minutes component of the clock
              32, // Initialize the seconds component of the clock
              0); // Initialize the seconds100 component of the clock

// Main loop
while (...)
{
    //...

    // Tell the clock how many seconds elapsed since last update
    Clock.Update (FloatDeltaTime);

    // You can now extract the components of the clock's date
    int Minutes = Clock.GetMinutes();
    int Seconds = Clock.GetSeconds();
    int Seconds100 = Clock.GetSeconds100();

    // Here if the update didn't change the clock's date,
    // we would have : Date = 3 * 3600 + 54 * 60 + 32 + 0 * 0.010f = 14072 seconds
    // So :
    // Minutes = 234
    // Seconds = 32
    // Seconds100 = 0
    
    //...
}

// Uninitialize the clock since we don't need it anymore
Clock.Destroy ();

\endcode
*/

class CClock
{
private:

    float           m_Date;                 //!< Most accurate available date
    int             m_Hours;                //!< Hours count in the date, according to clock mode
    int             m_Minutes;              //!< Minutes count in the date, according to clock mode
    int             m_Seconds;              //!< Seconds count in the date, according to clock mode
    int             m_Seconds100;           //!< Seconds100 count in the date, according to clock mode
    int             m_StartHours;           //!< Hours count to start with and to use when resetting (Reset method)
    int             m_StartMinutes;         //!< Minutes count to start with and to use when resetting (Reset method)
    int             m_StartSeconds;         //!< Seconds count to start with and to use when resetting (Reset method)
    int             m_StartSeconds100;      //!< Seconds100 count to start with and to use when resetting (Reset method)
    EClockType      m_ClockType;            //!< Type of the clock
    EClockMode      m_ClockMode;            //!< Mode of the clock
    bool            m_Pause;                //!< Update the date (or not) when calling Update method
                    
    void            CountHours (float &RemainingDate); //!< Count the hours in the remaining date by decreasing the given date and incrementing the hours count progressively
    void            CountMinutes (float &RemainingDate); //!< Count the minutes in the remaining date by decreasing the given date and incrementing the minutes count progressively
    void            CountSeconds (float &RemainingDate); //!< Count the seconds in the remaining date by decreasing the given date and incrementing the seconds count progressively
    void            CountSeconds100 (float &RemainingDate); //!< Count the seconds100 in the remaining date by decreasing the given date and incrementing the seconds100 count progressively
    
public:

                    CClock (void);          //!< Constructor (does nothing)
                    ~CClock (void);         //!< Destructor (does nothing)
    void            Create (EClockType ClockType, EClockMode ClockMode, int Hours, int Minutes, int Seconds, int Seconds100); //!< Initialize the clock
    void            Destroy (void);         //!< Uninitialize the clock
    void            Update (float DeltaTime); //!< Update the clock's date
    void            Reset (void);           //!< Reset the date to the starting date (which was set on last call to Create())
    inline void     Pause (void);           //!< Pause the clock
    inline void     Resume (void);          //!< Resume the clock
    inline int      GetHours (void);        //!< Get the Hour component of the current date
    inline int      GetMinutes (void);      //!< Get the Minute component of the current date
    inline int      GetSeconds (void);      //!< Get the Second component of the current date
    inline int      GetSeconds100 (void);   //!< Get the Second100 component of the current date
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CClock::Pause (void) 
{ 
    m_Pause = true; 
}

inline void CClock::Resume (void) 
{ 
    m_Pause = false; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CClock::GetHours (void) 
{
    // Assert the clock mode is suitable
    ASSERT (m_ClockMode == CLOCKMODE_HMSC ||
            m_ClockMode == CLOCKMODE_HMS ||
            m_ClockMode == CLOCKMODE_HM);

    return m_Hours; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CClock::GetMinutes (void) 
{ 
    // Assert the clock mode is suitable
    ASSERT (m_ClockMode == CLOCKMODE_HMSC ||
            m_ClockMode == CLOCKMODE_HMS ||
            m_ClockMode == CLOCKMODE_HM ||
            m_ClockMode == CLOCKMODE_MSC ||
            m_ClockMode == CLOCKMODE_MS);

    return m_Minutes; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


inline int CClock::GetSeconds (void) 
{ 
    // Assert the clock mode is suitable
    ASSERT (m_ClockMode == CLOCKMODE_HMSC ||
            m_ClockMode == CLOCKMODE_HMS ||
            m_ClockMode == CLOCKMODE_MSC ||
            m_ClockMode == CLOCKMODE_MS ||
            m_ClockMode == CLOCKMODE_SC ||
            m_ClockMode == CLOCKMODE_S);

    return m_Seconds; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CClock::GetSeconds100 (void) 
{ 
    // Assert the clock mode is suitable
    ASSERT (m_ClockMode == CLOCKMODE_HMSC ||
            m_ClockMode == CLOCKMODE_MSC ||
            m_ClockMode == CLOCKMODE_SC);

    return m_Seconds100; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
