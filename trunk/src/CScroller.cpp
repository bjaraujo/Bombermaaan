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
 *  \file CScroller.cpp
 *  \brief The scroller used by pause and hurry message
 */

#include "StdAfx.h"
#include "CScroller.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CScroller::CScroller (void)
{
 
    m_SpriteWidth = 0;
    m_SpriteHeight = 0;
    m_SpeedX = 0.0f;
    m_SpeedY = 0.0f;
    m_LoopTime = 0;
    m_RemainingLoopTime = 0.0f;
    m_iPositionX = -1;
    m_iPositionY = -1;
    m_fPositionX = (float) m_iPositionX;
    m_fPositionY = (float) m_iPositionY;
    m_Looping = false;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CScroller::~CScroller (void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CScroller::Create (int PositionX, int PositionY, int SpriteWidth, int SpriteHeight, float SpeedX, float SpeedY, float LoopTime)
{
    ASSERT (LoopTime >= 0.0f || LoopTime == -1.0f);

    m_SpriteWidth = SpriteWidth;
    m_SpriteHeight = SpriteHeight;
    m_SpeedX = SpeedX;
    m_SpeedY = SpeedY;
    m_LoopTime = LoopTime;
    m_RemainingLoopTime = 0.0f;
    m_iPositionX = PositionX;
    m_iPositionY = PositionY;
    m_fPositionX = (float) m_iPositionX;
    m_fPositionY = (float) m_iPositionY;
    m_Looping = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CScroller::Destroy (void)
{
  
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CScroller::Update (float DeltaTime)
{
    if (!m_Looping)
    {
        // Update the scroller position
        m_fPositionX += m_SpeedX * DeltaTime;
        m_fPositionY += m_SpeedY * DeltaTime;

        // Compute the new integer position of the scroller
        m_iPositionX = int (m_fPositionX);
        m_iPositionY = int (m_fPositionY);

        if (OutOfBounds() && m_LoopTime != -1.0f)
        {
            m_RemainingLoopTime = m_LoopTime;
            m_Looping = true;
        }
    }
    else
    {
        m_RemainingLoopTime -= DeltaTime;
        
        if (m_RemainingLoopTime <= 0.0f)
        {
            m_RemainingLoopTime = 0.0f;
            m_Looping = false;
            
            if (m_iPositionX + m_SpriteWidth < 0)
            {
                m_iPositionX = VIEW_WIDTH;
                m_fPositionX = (float) m_iPositionX;
            }
            else if (m_iPositionX >= VIEW_WIDTH)
            {
                m_iPositionX = -m_SpriteWidth;
                m_fPositionX = (float) m_iPositionX;
            }
            else if (m_iPositionY + m_SpriteHeight < 0)
            {
                m_iPositionY = VIEW_HEIGHT;
                m_fPositionY = (float) m_iPositionY;
            }
            else if (m_iPositionY >= VIEW_HEIGHT)
            {
                m_iPositionY = -m_SpriteHeight;
                m_fPositionY = (float) m_iPositionY;
            }
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CScroller::OutOfBounds (void)
{
    return (m_iPositionX > VIEW_WIDTH  || m_iPositionX + m_SpriteWidth  < 0 ||
            m_iPositionY > VIEW_HEIGHT || m_iPositionY + m_SpriteHeight < 0);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
