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
 *  \file CArenaSnapshot.h
 *  \brief Header file of the arena snapshot
 */

#ifndef __CARENASNAPSHOT_H__
#define __CARENASNAPSHOT_H__

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define ARENA_SNAPSHOT_SIZE     32768

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// POD CLASS 

class CArenaSnapshot
{
private:

    char        m_Buffer [ARENA_SNAPSHOT_SIZE];
    int         m_Position;
                                                            
public:                                                 
                                                        
    void        Create (void);                      //!< 
    void        Destroy (void);                     //!< 
    void        Begin (void);
    void        ReadInteger (int* pValue);
    void        ReadFloat (float* pValue);
    void        ReadBoolean (bool* pValue);
    void        WriteInteger (int Value);
    void        WriteFloat (float Value);
    void        WriteBoolean (bool Value);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif

