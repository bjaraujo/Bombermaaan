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
 *  \file CDebug.h
 *  \brief Header file of the debug utilities
 */

#ifndef __CDEBUG_H__
#define __CDEBUG_H__

class CTimer;
class CGame;
class CMatch;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define theDebug CDebug::GetInstance()

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! This class is for debugging purposes
class CDebug
{
public:
    
    CDebug();
    ~CDebug();
    static CDebug&  GetInstance();
    inline void SetTimer (CTimer* pTimer);
    inline void SetGame (CGame* pGame);
    inline void SetMatch(CMatch* pMatch);
    void Create();
    void Destroy();
    void HandleKey (DWORD VirtualKeyCode, DWORD Modifier);
    inline bool CanBombersDie();
    inline bool CanBombersBeSick();
    inline bool CanBombersKick();
    inline bool IsComputerConsoleActive (int Player);

private:
    
    CTimer* m_pTimer;
    CGame* m_pGame;
    CMatch* m_pMatch;
    float m_GameSpeed;
    bool m_CanBombersDie;
    bool m_CanBombersBeSick;
    bool m_CanBombersKick;
    bool m_IsComputerConsoleActive [5];
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CDebug::SetTimer (CTimer* pTimer)
{
    m_pTimer = pTimer;
}

inline void CDebug::SetGame (CGame* pGame)
{
    m_pGame = pGame;
}

inline void CDebug::SetMatch(CMatch* pMatch)
{
    m_pMatch = pMatch;
}

inline bool CDebug::CanBombersDie()
{
    return m_CanBombersDie;
}

inline bool CDebug::CanBombersBeSick()
{
    return m_CanBombersBeSick;
}

inline bool CDebug::CanBombersKick()
{
    return m_CanBombersKick;
}

inline bool CDebug::IsComputerConsoleActive (int Player)
{
    ASSERT(Player >= 0);
    ASSERT(Player < 5);

    return m_IsComputerConsoleActive[Player];
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CDEBUG_H__
