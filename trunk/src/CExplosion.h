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
 *  \file CExplosion.h
 *  \brief Header file of an explosion
 */

#ifndef __CEXPLOSION_H__
#define __CEXPLOSION_H__

#include "CElement.h"

class CArena;
class CDisplay;
class CArenaSnapshot;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describe a flame created by an explosion.
/*!
This is used by the AI to scan all the flames of an explosion
and by the explosion itself when displaying the flames.
*/

struct SFlame
{
    int BlockX;     //!< Block position X of the flame
    int BlockY;     //!< Block position Y of the flame
    int FlameType;  //!< Flame type used to know what sprite to draw
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describe in which direction a flame hits something
/*!
This is used for the skull item. The skull item, when
hit by a flame, needs to know in which direction it has
to fly.
*/

enum EBurnDirection
{
    BURNDIRECTION_NONE,     //!< A center of explosion hits elements in this direction
    BURNDIRECTION_UP,       //!< The element is hit from below (so go up)
    BURNDIRECTION_DOWN,     //!< The element is hit from above (so go down)
    BURNDIRECTION_LEFT,     //!< The element is hit from the right (so go left)
    BURNDIRECTION_RIGHT     //!< The element is hit from the left (so go right)
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Mode of the explosion : original style or new style
/*!
<B>New style</B> (not so good) : makes the explosions as up-to-date as
possible : the flame rays always go further until they are
forced to stop. On each update the blocks where there are
some flames and where the flame rays stop BURN. So if the
explosion duration is too long, the explosion can burn a
wall, and the item under it afterwards.

<B>Original style</B> (Super Bomberman) : the flame sizes are
computed once when the explosion is created. These flames
can be shorter, but not longer than the computed flame sizes.
The blocks where the flame rays stop BURN once. On each update
the blocks where there are flames BURN. So unlike the new
style, the result does not depend on the explosion duration.
*/

#define EXPLOSIONS_ORIGINALSTYLE

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! An element in the arena which represents an explosion.

class CExplosion : public CElement
{
private:

    int             m_iX;               //!< Position X (in pixels) of the center of the explosion in the arena
    int             m_iY;               //!< Position Y (in pixels) of the center of the explosion in the arena
    int             m_BlockX;           //!< Position X (in blocks) of the center of the explosion in the arena grid
    int             m_BlockY;           //!< Position Y (in blocks) of the center of the explosion in the arena grid
    int             m_State;            //!< State of the explosion (min = STATE0, max = STATE3)
    float           m_Timer;            //!< Time counter for states evolution
    int             m_FlameSize;        //!< Maximum size of the rays of flames (in blocks)
    std::vector<SFlame>  m_Flames;           //!< Flames created by the explosion
    bool            m_Dead;             //!< Is the explosion dead? (can it be deleted by the arena?)

#ifdef EXPLOSIONS_ORIGINALSTYLE

    int             m_FlameSizeUp;      //!< Maximum flame size (in blocks) of the ray of flames which goes up
    int             m_FlameSizeDown;    //!< Maximum flame size (in blocks) of the ray of flames which goes down
    int             m_FlameSizeLeft;    //!< Maximum flame size (in blocks) of the ray of flames which goes left
    int             m_FlameSizeRight;   //!< Maximum flame size (in blocks) of the ray of flames which goes right
    bool            m_StopUp;           //!< Is the flame ray stopped by something is this direction?
    bool            m_StopDown;         //!< Is the flame ray stopped by something is this direction?
    bool            m_StopLeft;         //!< Is the flame ray stopped by something is this direction?
    bool            m_StopRight;        //!< Is the flame ray stopped by something is this direction?

#endif // EXPLOSIONS_ORIGINALSTYLE

    void            Burn (int X, int Y, EBurnDirection BurnDirection);  //!< Globally burn the specified block (walls, unprotected items, ...)
    void            PutFlame (int BlockX, int BlockY, int FlameType);   //!< Record a flame whose description is specified

public:

                    CExplosion (void);          //!< Constructor (initialize the base class)
    virtual         ~CExplosion (void);         //!< Destructor (uninitialize the base class)
    void            Create (int BlockX, int BlockY, int FlameSize); //!< Initialize the explosion
    void            Destroy (void);             //!< Uninitialize the explosion
    bool            Update (float DeltaTime);   //!< Update the explosion. Return whether the element should be deleted by the arena.
    void            Display (void);             //!< Display the explosion
    void            OnWriteSnapshot (CArenaSnapshot& Snapshot);
    void            OnReadSnapshot (CArenaSnapshot& Snapshot);
    inline int      GetBlockX (void);           //!< Return the block position X of the center of the explosion
    inline int      GetBlockY (void);           //!< Return the block position Y of the center of the explosion
    inline const std::vector<SFlame>& GetFlames (void); //!< Return the (read-only) flames of the explosions
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CExplosion::GetBlockX (void)
{
    return m_BlockX;
}

inline int CExplosion::GetBlockY (void)
{
    return m_BlockY;
}

inline const std::vector<SFlame>& CExplosion::GetFlames (void)
{
    return m_Flames;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CEXPLOSION_H__
