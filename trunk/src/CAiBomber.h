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
 *  \file CAiBomber.h
 *  \brief Header file of the bot bomber
 */

#ifndef __CAIBOMBER_H__
#define __CAIBOMBER_H__

#include "CBomber.h"

class CAiArena;
enum EDanger;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the mode of the computer player
enum EComputerMode
{
    COMPUTERMODE_THINK,   //!< Deciding what to do and set the new computer mode
    COMPUTERMODE_ITEM,    //!< Picking up items or dropping bombs in order to burn walls
    COMPUTERMODE_ATTACK,  //!< Attacking a bomber
    COMPUTERMODE_THROW,   //!< Attacking a bomber (throw the bomb afterwards with COMPUTERMODE_ATTACK)
    COMPUTERMODE_2NDACTION, //!< Use punch/stop bomb while being kicked/remote detonate
    COMPUTERMODE_DEFENCE, //!< Trying to be in a safe place
    COMPUTERMODE_WALK     //!< Walking in random directions until there is some activity around the bomber
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the direction to the next enemy
enum EEnemyDirection
{
    ENEMYDIRECTION_UNKNOWN, //!< No enemy near us (at least we think so)
    ENEMYDIRECTION_HERE,    //!< Enemy is at the same position as we are
    ENEMYDIRECTION_ABOVE,   //!< Enemy is above us
    ENEMYDIRECTION_BELOW,   //!< Enemy is below us
    ENEMYDIRECTION_LEFT,
    ENEMYDIRECTION_RIGHT
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CAiBomber
{
private:

    CAiArena*       m_pArena;
    CDisplay*       m_pDisplay;
    int             m_Player;
    int             m_Accessible [ARENA_WIDTH][ARENA_HEIGHT];           // If this block is not accessible to the bomber (walls and bombs are obstacles), this contains -1. Otherwise this contains the distance (in blocks) between the bomber and the square.
    int             m_PseudoAccessible [ARENA_WIDTH][ARENA_HEIGHT];
    int             m_NumAccessible;
    float           m_StopTimeLeft;                                     // Number of seconds left before sending commands to the bomber. Stopping the computer bomber from time to time makes the player more human.
    int             m_ItemGoalBlockX;                                   // Used for item mode only. Coordinates of the block where to go.
    int             m_ItemGoalBlockY;
    bool            m_ItemDropBomb;                                     // Used for item mode only. True if the bomber has to drop a bomb when he gets to the item goal block.
    int             m_BlockWalk;
    float           m_WalkTime;
    EComputerMode   m_ComputerMode;                                     // Current mode of the computer player
    EBomberMove     m_BomberMove;                                       // Current bomber commands to send to the bomber
    EBomberAction   m_BomberAction;
    float           m_BomberMoveTimeLeft;                               // Number of seconds left before thinking and therefore updating the bomber commands to send.
    CBomber*        m_pBomber;                                          // Pointer to the bomber for easy access
    int             m_BlockHereX;                                       // Auxiliary variables for code readability : block coordinates around the bomber
    int             m_BlockHereY;
    int             m_BlockUpX;
    int             m_BlockUpY;
    int             m_BlockDownX;
    int             m_BlockDownY;
    int             m_BlockLeftX;
    int             m_BlockLeftY;
    int             m_BlockRightX;
    int             m_BlockRightY;
    static int      m_BurnMark[4][6];

    void            SetComputerMode (EComputerMode ComputerMode);       // Set the mode of the computer player
    void            ModeThink (void);
    void            ModeItem (float DeltaTime);
    void            ModeAttack ();
    void            ModeThrow ();
    void            ModeSecondAction ();
    void            ModeDefence (float DeltaTime);
    void            ModeWalk (float DeltaTime);
    void            UpdateAccessibility (void);
    bool            GoTo (int GoalBlockX, int GoalBlockY);              // Modify the commands to send to the bomber so that it moves to the specified goal
    bool            EnemyNearAndFront (EEnemyDirection *direction = NULL, bool BeyondArenaFrontiers = false); // Returns true if a bomber enemy is near the bomber and in front of him
    bool            EnemyNearRemoteFuseBomb (CBomb& bomb);
    bool            TeamMateNearRemoteFuseBomb(CBomb& bomb);
    bool            DropBombOK(int BlockX, int BlockY);
    bool            EnemyNear (int BlockX, int BlockY);
    int             ItemMark (int BlockX, int BlockY);
                                                            
public:                                                 
                                                        
                    CAiBomber (void);
    virtual         ~CAiBomber (void);
    inline void     SetArena (CAiArena* pArena);
    inline void     SetDisplay (CDisplay* pDisplay);
    void            Create (int Player);
    void            Destroy (void);
    void            Update (float DeltaTime);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CAiBomber::SetArena (CAiArena* pArena)
{
    ASSERT (pArena != NULL);
    m_pArena = pArena;
}

inline void CAiBomber::SetDisplay (CDisplay *pDisplay)
{
    ASSERT (pDisplay != NULL);
    
    // Save the display object pointer to pass to elements
    m_pDisplay = pDisplay;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
