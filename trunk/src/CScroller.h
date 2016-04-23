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
 *  \file CScroller.h
 *  \brief Header file of the scroller
 */

#ifndef __CSCROLLER_H__
#define __CSCROLLER_H__

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CScroller
{
private:

    int         m_SpriteWidth;
    int         m_SpriteHeight;
    float       m_SpeedX;
    float       m_SpeedY;
    float       m_LoopTime;
    float       m_RemainingLoopTime;
    bool        m_Looping;
    float       m_fPositionX;
    float       m_fPositionY;
    int         m_iPositionX;
    int         m_iPositionY;

public:

                CScroller (void);
                ~CScroller (void);
    void        Create (int PositionX, int PositionY, int SpriteWidth, int SpriteHeight, float SpeedX, float SpeedY, float LoopTime);
    void        Destroy (void);
    void        Update (float DeltaTime);
    bool        OutOfBounds (void);
    inline int  GetPositionX (void);
    inline int  GetPositionY (void);
    inline void SetSpeed (float SpeedX, float SpeedY);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CScroller::GetPositionX (void)
{
    return m_iPositionX;
}

inline int CScroller::GetPositionY (void)
{
    return m_iPositionY;
}

inline void CScroller::SetSpeed (float SpeedX, float SpeedY)
{
    m_SpeedX = SpeedX;
    m_SpeedY = SpeedY;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CSCROLLER_H__
