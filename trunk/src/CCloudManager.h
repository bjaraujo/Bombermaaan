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
 *  \file CCloudManager.h
 *  \brief Header file of the cloud manager
 */

#ifndef __CCLOUDMANAGER_H__
#define __CCLOUDMANAGER_H__

#include "CScroller.h"

class CDisplay;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define NUMBER_OF_CLOUDS        3

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! This class manages the clouds
class CCloudManager
{
private:

    CDisplay*       m_pDisplay;
    CScroller       m_Clouds[NUMBER_OF_CLOUDS];
    int             m_CloudSprites[NUMBER_OF_CLOUDS];
   
public:

                    CCloudManager (void);
                    ~CCloudManager (void);
    inline void     SetDisplay (CDisplay* pDisplay);
    void            Create (void);
    void            Destroy (void);
    void            Update (float DeltaTime);
    void            Display (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CCloudManager::SetDisplay (CDisplay* pDisplay)
{
    m_pDisplay = pDisplay;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CCLOUDMANAGER_H__
