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
 *  \file CCommandChunk.h
 *  \brief Header file of the command chunk
 */

#ifndef __CCOMMANDCHUNK_H__
#define __CCOMMANDCHUNK_H__

#include "CBomber.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_STEPS_IN_COMMAND_CHUNK      8

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SCommandStep
{
    EBomberMove BomberMove;
    EBomberAction BomberAction;
    float Duration;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// POD CLASS 

class CCommandChunk
{
private:

    SCommandStep            m_Steps [MAX_STEPS_IN_COMMAND_CHUNK];
    int                     m_NumberOfSteps;
                                                        
public:                                                 

    void                    Create (void);                      //!< 
    void                    Destroy (void);                     //!< 
    void                    Reset (void);                       //!< 
    void                    Store (EBomberMove BomberMove, EBomberAction BomberAction, float DeltaTime);
    inline EBomberMove      GetStepMove (int Step);
    inline EBomberAction    GetStepAction (int Step);
    inline float            GetStepDuration (int Step);
    inline int              GetNumberOfSteps (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline EBomberMove CCommandChunk::GetStepMove (int Step)
{
    ASSERT(Step >= 0);
    ASSERT(Step < m_NumberOfSteps);
    return m_Steps[Step].BomberMove;
}

inline EBomberAction CCommandChunk::GetStepAction (int Step)
{
    ASSERT(Step >= 0);
    ASSERT(Step < m_NumberOfSteps);
    return m_Steps[Step].BomberAction;
}

inline float CCommandChunk::GetStepDuration (int Step)
{
    ASSERT(Step >= 0);
    ASSERT(Step < m_NumberOfSteps);
    return m_Steps[Step].Duration;
}

inline int CCommandChunk::GetNumberOfSteps (void)
{
    return m_NumberOfSteps;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
