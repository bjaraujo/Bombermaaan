/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold
    Copyright (C) 2008 Jerome Bigot

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
 *  \file CBomb.h
 *  \brief Header file of a bomb
 */

#ifndef __CBOMB_H__
#define __CBOMB_H__

#include "CElement.h"

class CArena;
class CDisplay;
class CArenaSnapshot;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a kick direction. This is the movement direction of the moving bomb.

enum EBombKick
{
    BOMBKICK_NONE,       //!< Don't move (not kicked)
    BOMBKICK_UP,         //!< Move up
    BOMBKICK_DOWN,       //!< Move down
    BOMBKICK_LEFT,       //!< Move left
    BOMBKICK_RIGHT       //!< Move right
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a fly direction. This is the movement direction of the flying bomb.

enum EBombFly
{
    BOMBFLY_NONE = 0,   //!< Don't move (not flying)
    BOMBFLY_UP,         //!< Move up
    BOMBFLY_DOWN,       //!< Move down
    BOMBFLY_LEFT,       //!< Move left
    BOMBFLY_RIGHT,      //!< Move right
    NUMBER_OF_BOMBFLY_DIRECTIONS
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes why a bomb is flying.
enum EBombFlightType
{
    BOMBFLIGHTTYPE_NONE,    //!< The bomb is not flying.
    BOMBFLIGHTTYPE_THROW,   //!< The bomb is flying because it was thrown by a bomber.
    BOMBFLIGHTTYPE_PUNCH,   //!< The bomb is flying because it was punched by a bomber.
    BOMBFLIGHTTYPE_BOUNCE   //!< The bomb is flying because it has bounced on a wall or on a bomber.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! An element in the arena which represents a bomb.
class CBomb : public CElement
{
private:

    int                 m_iX;                       //!< Integer position X (in pixels) in the arena
    int                 m_iY;                       //!< Integer position Y (in pixels) in the arena
    float               m_X;                        //!< Float position X (in pixels) in the arena
    float               m_Y;                        //!< Float position Y (in pixels) in the arena
    int                 m_BlockX;                   //!< Position X (in blocks) in the arena grid
    int                 m_BlockY;                   //!< Position Y (in blocks) in the arena grid
    int                 m_Sprite;                   //!< Current sprite to use when displaying the bomb
    float               m_Timer;                    //!< Time counter for animation
    EBombKick           m_BombKick;                 //!< Is the bomb moving and in which direction?   
    bool                m_HasToStopMoving;          //!< Does the bomb have to stop moving?
    int                 m_OwnerPlayer;              //!< Number of the player that owns the bomb
    int                 m_KickerPlayer;             //!< Number of the player that just kicked the bomb (until the bomb stops moving)
    bool                m_Checked;                  //!< Was the dead bomb noticed by the owner bomber?
    bool                m_Dead;                     //!< Is the bomb dead?
    int                 m_FlameSize;                //!< Size of the flames (in blocks) when exploding
    float               m_ElapsedTime;              //!< Total elapsed time since bomb was created
    float               m_TimeLeft;                 //!< Time left (in seconds) before the bomb explodes
    float               m_AnimationTimes[4];        //!< Animation times of the bomb, depending on the time left before the bomb explodes that is specified on bomb's creation
    bool                m_BeingHeld;                //!< Is a bomber holding the bomb?
    bool                m_BeingLifted;              //!< Is a bomber lifting the bomb?
    bool                m_BeingPunched;             //!< Is a bomber punching the bomb?
    EBombFly            m_BombFly;                  //!< Is the bomb flying and in which direction?   
    float               m_FlightTimer;              //!< Movement timer when the bomb is flying, used to determine the current flight frame for this bomb.
    int                 m_FlightFrame;              //!< Index in the corresponding flight position arrays, used to know the bomb's position.
    EBombFlightType     m_FlightType;               //!< Cause of the flight, used to make the flight movement different according to the cause.
    bool                m_Warping;                  //!< Is the bomb currently moving from one side of the arena and coming out from the opposite side? (up/down or left/right)
    bool                m_Remote;                   //!< Is the bomb a remote controled bomb ?

    static int          m_ThrowMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][6]; //!< Offset to apply to the bomb's X position for each "frame" of the flight, according to the direction of the flight. When bomb was thrown.
    static int          m_ThrowMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][6]; //!< Offset to apply to the bomb's Y position for each "frame" of the flight, according to the direction of the flight. When bomb was thrown.
    static int          m_PunchMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][6]; //!< Offset to apply to the bomb's X position for each "frame" of the flight, according to the direction of the flight. When bomb was punched.
    static int          m_PunchMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][6]; //!< Offset to apply to the bomb's Y position for each "frame" of the flight, according to the direction of the flight. When bomb was punched.
    static int          m_BounceMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][3]; //!< Offset to apply to the bomb's X position for each "frame" of the flight, according to the direction of the flight. When bomb has bounced.
    static int          m_BounceMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][3]; //!< Offset to apply to the bomb's Y position for each "frame" of the flight, according to the direction of the flight. When bomb has bounced.

    void                ManageMove(float DeltaTime);           //!< Manage the movement of the bomb if needed. Uses TryMove().
    bool                TryMove(float fPixels);                //!< Try to move the bomb in the current EBombKick direction by fPixels pixels.
    bool                IsObstacle(int BlockX, int BlockY);    //!< Return whether there is an obstacle (bomb, wall, bomber) on the specified block.
    void                Explode(void);                         //!< Trigger the explosion of the bomb : create an explosion, make the bomb die.
    void                ManageFlight(float DeltaTime);         //!< Make the bomb move, bounce, land, etc if it is currently flying.
    void                CrushItem(int BlockX, int BlockY);     //!< Find an item that is on the same block as this bomb, and kill it.
    void                Bounce(float DeltaTime);               //!< Make the bomb bounce, play a bounce sound, make a bomber stunt if bouncing on a bomber.
    void                CenterOnBlock(void);                   //!< Position the bomb so that it is exactly on the current block
    inline void         AbortKick(void);                       //!< Brutally abort any bomb kicking. This method is to be used when a bomber is going to lift or punch a bomb that can be currently moving because a bomber kicked it

public:

    CBomb(void);                           //!< Constructor (initialize the base class)
    virtual             ~CBomb(void);                          //!< Destructor (uninitialize the base class)
    void                Create(int BlockX, int BlockY, int FlameSize, float TimeLeft, int OwnerPlayer); //!< Initialize the bomb
    void                Destroy(void);                         //!< Uninitialize the bomb
    bool                Update(float DeltaTime);               //!< Update the bomb (make it tick, move if needed...). Return whether the element should be deleted by the arena. 
    void                Display(void);                         //!< Display the bomb.
    void                OnWriteSnapshot(CArenaSnapshot& Snapshot);
    void                OnReadSnapshot(CArenaSnapshot& Snapshot);
    void                Crush(void);                           //!< Make the bomb react when it is crushed by a falling wall
    void                Burn(void);                            //!< Make the bomb react when it is burnt by an explosion
    void                StartMoving(EBombKick BombKick, int KickerPlayer); //!< Trigger a bomb kick in the specified direction, by the specified player
    void                StopMoving(void);                      //!< Tell the bomb to stop moving as soon as possible.
    void                StartFlying(EBombFly BombFly, EBombFlightType FlightType); //!< Tell the bomb to be pushed in a direction and to fly in that direction. Current pixel/block position is the start.
    inline void         SetBeingLifted(void);                  //!< Tell the bomb that a bomber is lifting it
    inline void         SetBeingHeld(void);                    //!< Tell the bomb that a bomber is holding it
    inline void         SetBeingPunched(void);                 //!< Tell the bomb that a bomber is punching it
    inline void         SetPosition(int X, int Y);             //!< Set the pixel position of the bomb
    inline void         SetBlock(int BlockX, int BlockY);      //!< Set the block position of the bomb
    inline void         SetChecked(void);                      //!< Called by the bomber. Tell the bomb the owner bomber noticed it is dead.
    inline int          GetOwnerPlayer(void);                  //!< Return the number of the player that dropped this bomb.
    inline int          GetKickerPlayer(void);                 //!< Return the number of the player that just kicked this bomb. The bomb must be still moving.
    inline bool         IsDead(void);                          //!< Return whether the bomb is dead.
    inline int          GetBlockX(void);                       //!< Return the block position X of the bomb
    inline int          GetBlockY(void);                       //!< Return the block position Y of the bomb
    inline float        GetTimeLeft(void);                     //!< Return the time left before the bomb will explode (unless the explosion is triggered earlier)
    inline float        GetElapsedTime(void);                  //!< Return the time since the bomb was created
    inline int          GetFlameSize(void);                    //!< Return the size of the flames (in blocks) when the bomb will explode
    inline bool         IsOnFloor(void);                       //!< Return whether the bomb is on the floor (not in the air)
    inline bool         IsRemote(void);                        //!< Return whether the bomb is a remote bomb
    inline bool         IsBeingLifted(void);                   //!< Return whether the bomb is lifted
    inline bool         IsBeingHeld(void);                     //!< Return whether the bomb is held
    inline bool         IsBeingPunched(void);                  //!< Return whether the bomb is punched

#ifdef _DEBUG_FLAG_1
    void                _Debug_WriteToLog();
#endif

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CBomb::SetChecked(void)
{
    m_Checked = true;
}

inline int CBomb::GetOwnerPlayer(void)
{
    return m_OwnerPlayer;
}

inline int CBomb::GetKickerPlayer(void)
{
    return m_KickerPlayer;
}

inline bool CBomb::IsDead(void)
{
    return m_Dead;
}

inline int CBomb::GetBlockX(void)
{
    return m_BlockX;
}

inline int CBomb::GetBlockY(void)
{
    return m_BlockY;
}

inline float CBomb::GetTimeLeft(void)
{
    return m_TimeLeft;
}

inline float CBomb::GetElapsedTime(void)
{
    return m_ElapsedTime;
}

inline int CBomb::GetFlameSize(void)
{
    return m_FlameSize;
}

inline bool CBomb::IsRemote(void)
{
    return m_Remote;
}

inline void CBomb::SetPosition(int X, int Y)
{
    ASSERT(m_BombFly == BOMBFLY_NONE);

    m_iX = X;
    m_iY = Y;
    m_X = (float)X;
    m_Y = (float)Y;
}

inline void CBomb::SetBeingLifted(void)
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb set being lifted [x=%02d, y=%02d, owner=%d].", m_BlockX, m_BlockY, m_OwnerPlayer);
#endif

    ASSERT(!m_Dead);
    ASSERT(!m_BeingHeld);
    ASSERT(!m_BeingLifted);
    ASSERT(!m_BeingPunched);

    AbortKick();

    m_BeingPunched = false;
    m_BeingLifted = true;
    m_BeingHeld = false;
}

inline void CBomb::SetBeingHeld(void)
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb set being held [x=%02d, y=%02d, owner=%d].", m_BlockX, m_BlockY, m_OwnerPlayer);
#endif

    ASSERT(m_BeingLifted);
    ASSERT(m_BombKick == BOMBKICK_NONE);
    ASSERT(!m_HasToStopMoving);
    ASSERT(!m_Dead);
    ASSERT(!m_BeingPunched);
    ASSERT(!m_BeingHeld);

    m_BeingPunched = false;
    m_BeingLifted = false;
    m_BeingHeld = true;
}

inline void CBomb::SetBeingPunched(void)
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb set being punched [x=%02d, y=%02d, owner=%d].", m_BlockX, m_BlockY, m_OwnerPlayer);
#endif

    ASSERT(!m_Dead);
    ASSERT(!m_BeingLifted);
    ASSERT(!m_BeingHeld);
    ASSERT(!m_BeingPunched);

    AbortKick();

    m_BeingPunched = true;
    m_BeingLifted = false;
    m_BeingHeld = false;

}

inline bool CBomb::IsOnFloor(void)
{
    return (!m_BeingLifted && !m_BeingHeld && !m_BeingPunched && m_FlightType == BOMBFLIGHTTYPE_NONE);
}

inline void CBomb::SetBlock(int BlockX, int BlockY)
{
    ASSERT(m_BombFly == BOMBFLY_NONE);

    ASSERT(BlockX >= 0);
    ASSERT(BlockX < ARENA_WIDTH);

    ASSERT(BlockY >= 0);
    ASSERT(BlockY < ARENA_HEIGHT);

    m_BlockX = BlockX;
    m_BlockY = BlockY;
}

inline void CBomb::AbortKick(void)
{
    if (m_BombKick != BOMBKICK_NONE)
    {
        m_BombKick = BOMBKICK_NONE;
        m_KickerPlayer = -1;
        m_HasToStopMoving = false;
        CenterOnBlock();
    }
}

inline bool CBomb::IsBeingLifted(void)
{
    return m_BeingLifted;
}

inline bool CBomb::IsBeingHeld(void)
{
    return m_BeingHeld;
}

inline bool CBomb::IsBeingPunched(void)
{
    return m_BeingPunched;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
