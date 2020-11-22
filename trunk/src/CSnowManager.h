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
 *  \file CSnowManager.h
 *  \brief Header file of the snow manager
 */

#ifndef __CSNOWMANAGER_H__
#define __CSNOWMANAGER_H__

#include "CScroller.h"

class CDisplay;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define NUMBER_OF_SNOWFLAKES 30

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! This class manages the snow
class CSnowManager
{
private:
    CDisplay* m_pDisplay;
    CScroller m_Snows[NUMBER_OF_SNOWFLAKES];
    int m_SnowSprites[NUMBER_OF_SNOWFLAKES];

public:
    CSnowManager();
    ~CSnowManager();
    inline void SetDisplay(CDisplay* pDisplay);
    void Create();
    void Destroy();
    void Update(float DeltaTime);
    void Display();
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CSnowManager::SetDisplay(CDisplay* pDisplay) { m_pDisplay = pDisplay; }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CSNOWMANAGER_H__
