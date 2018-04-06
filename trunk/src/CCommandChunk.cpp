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
 *  \file CCommandChunk.cpp
 *  \brief Command chunk
 */

#include "StdAfx.h"
#include "CCommandChunk.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCommandChunk::Create (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCommandChunk::Destroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCommandChunk::Reset (void)
{
    m_NumberOfSteps = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCommandChunk::Store (EBomberMove BomberMove, EBomberAction BomberAction, float DeltaTime)
{
    // If there is no step yet
    if (m_NumberOfSteps == 0)
    {
        // Create first step
        m_Steps[0].BomberMove = BomberMove;
        m_Steps[0].BomberAction = BomberAction;
        m_Steps[0].Duration = DeltaTime;

        m_NumberOfSteps++;
    }
    // If there is already at least one step
    else
    {
        // If the latest step has a different move or action than the ones we have to add
        if (m_Steps[m_NumberOfSteps - 1].BomberMove != BomberMove ||
            m_Steps[m_NumberOfSteps - 1].BomberAction != BomberAction)
        {
            ASSERT(m_NumberOfSteps < MAX_STEPS_IN_COMMAND_CHUNK);

            if (m_NumberOfSteps < MAX_STEPS_IN_COMMAND_CHUNK)
            {            
                // This is a new step
                m_Steps[m_NumberOfSteps].BomberMove = BomberMove;
                m_Steps[m_NumberOfSteps].BomberAction = BomberAction;
                m_Steps[m_NumberOfSteps].Duration = DeltaTime;

                m_NumberOfSteps++;
            }
        }
        // If the move and action did not change since latest step
        else
        {
            // Use latest step, increase duration
            m_Steps[m_NumberOfSteps - 1].Duration += DeltaTime;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
