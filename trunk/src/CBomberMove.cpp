// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CBomberMove.cpp
 *  \brief Bomber moves
 */

#include "StdAfx.h"
#include "CBomberMove.h"
#include "CBomber.h"
#include "CArena.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Add these values (in pixels) to the bomber position in order 
// to get the position of the bomb the bomber is holding
#define BOMBER_TO_HELD_BOMB_POSITION_X  0
#define BOMBER_TO_HELD_BOMB_POSITION_Y  -17

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBomberMove::CBomberMove(void)
{
    m_pArena = NULL;
    m_Player = -1;
    
    m_X = -1;
    m_Y = -1;
    
    m_iX = -1;
    m_iY = -1;
    
    m_BlockX = -1;
    m_BlockY = -1;
    
    m_BomberMove = BOMBERMOVE_NONE;
    m_LastRealBomberMove = BOMBERMOVE_NONE;
    m_Turning = TURNING_NOTTURNING;
    m_CouldMove = false;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBomberMove::~CBomberMove(void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomberMove::Create(int BlockX, int BlockY, int Player)
{
    ASSERT(m_pArena != NULL);

    ASSERT(BlockX >= 0);
    ASSERT(BlockX < ARENA_WIDTH);

    ASSERT(BlockY >= 0);
    ASSERT(BlockY < ARENA_HEIGHT);

    ASSERT(Player >= 0);
    ASSERT(Player < MAX_PLAYERS);

    m_Player = Player;
    m_BlockX = BlockX;
    m_BlockY = BlockY;
    m_iX = m_pArena->ToPosition(BlockX);
    m_iY = m_pArena->ToPosition(BlockY);
    m_X = float(m_iX);
    m_Y = float(m_iY);
    m_BomberMove = BOMBERMOVE_NONE;
    m_LastRealBomberMove = BOMBERMOVE_DOWN;
    m_Turning = TURNING_NOTTURNING;
    m_CouldMove = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomberMove::Destroy(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomberMove::Command(EBomberMove BomberMove)
{
    // If the bomber is currently lifting or throwing or punching a bomb
    if (m_pArena->GetBomber(m_Player).GetState() == BOMBERSTATE_LIFT ||
        m_pArena->GetBomber(m_Player).GetState() == BOMBERSTATE_THROW ||
        m_pArena->GetBomber(m_Player).GetState() == BOMBERSTATE_PUNCH)
    {
        // Then he cannot move.
        m_BomberMove = BOMBERMOVE_NONE;
    }
    else
    {
        // The given BomberMove will be modified 
        // according to the bomber's sickness. When updating,
        // these modified BomberMove will be used.
        switch (m_pArena->GetBomber(m_Player).GetSickness())
        {
            // Sicknesses that don't affect the move
        case SICK_NOTSICK:
        case SICK_SLOW:
        case SICK_FAST:
        case SICK_SMALLFLAME:
        case SICK_LONGBOMB:
        case SICK_SHORTBOMB:
        case SICK_CONSTIPATED:
        case SICK_COLIC:
        case SICK_INVISIBILITY:
        case SICK_FLAMEPROOF:
        {
            m_BomberMove = BomberMove;
            break;
        }

        case SICK_INVERTION:
        {
            // Invert move
            switch (BomberMove)
            {
            case BOMBERMOVE_NONE:      m_BomberMove = BOMBERMOVE_NONE;         break;
            case BOMBERMOVE_UP:        m_BomberMove = BOMBERMOVE_DOWN;         break;
            case BOMBERMOVE_DOWN:      m_BomberMove = BOMBERMOVE_UP;           break;
            case BOMBERMOVE_LEFT:      m_BomberMove = BOMBERMOVE_RIGHT;        break;
            case BOMBERMOVE_RIGHT:     m_BomberMove = BOMBERMOVE_LEFT;         break;
            case BOMBERMOVE_UPLEFT:    m_BomberMove = BOMBERMOVE_DOWNRIGHT;    break;
            case BOMBERMOVE_UPRIGHT:   m_BomberMove = BOMBERMOVE_DOWNLEFT;     break;
            case BOMBERMOVE_DOWNLEFT:  m_BomberMove = BOMBERMOVE_UPRIGHT;      break;
            case BOMBERMOVE_DOWNRIGHT: m_BomberMove = BOMBERMOVE_UPLEFT;       break;
            }

            break;
        }

        case SICK_INERTIA:
        {
            // If bomber is asked to go somewhere
            if (BomberMove != BOMBERMOVE_NONE)
            {
                bool Can = false;

                switch (BomberMove)
                {
                    // BomberMove with two directions
                case BOMBERMOVE_UPLEFT:
                case BOMBERMOVE_UPRIGHT:
                case BOMBERMOVE_DOWNLEFT:
                case BOMBERMOVE_DOWNRIGHT:
                    // Always OK
                    Can = true;
                    break;

                    // BomberMove with single direction
                case BOMBERMOVE_UP:
                case BOMBERMOVE_DOWN:
                case BOMBERMOVE_LEFT:
                case BOMBERMOVE_RIGHT:
                    // OK if you won't be blocked (Otherwise the player could be able to stop his bomber)
                    Can = (CanMove(BomberMove) != CANMOVE_CANNOT);
                    break;
                default:
                    break;
                }

                // If he can, go in that direction
                if (Can)
                    m_BomberMove = BomberMove;
            }
            // Else if he could not move when he last tried
            else if (!m_CouldMove)
            {
                // Stop trying
                m_BomberMove = BOMBERMOVE_NONE;
            }

            break;
        }
	case NUMBER_SICKNESSES:
	    assert(false);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomberMove::Update(float DeltaTime)
{
    // If the bomber is not alive (dead or dying) then he cannot move
    if (!m_pArena->GetBomber(m_Player).IsAlive())
        return;

    // If the bomber is stunt then he cannot move
    if (m_pArena->GetBomber(m_Player).GetState() == BOMBERSTATE_STUNT)
        return;

    // If bomber has to move
    if (m_BomberMove != BOMBERMOVE_NONE)
    {
        // Number of pixels the bomber has to move
        float fPixels = m_pArena->GetBomber(m_Player).GetPixelsPerSecond() * DeltaTime;

        // Convert the move with multiple directions to a move with a single direction
        // For the player's comfort, if the bomber is asked to go up and left, one
        // of the two directions will be choosen using tests. The direction making
        // the bomber turn (not any turning type!) will be choosen, and if no
        // choice could be made, the direction where the way is free will be
        // choosen, else the direction making the move possible will be choosen.
        // Priorities :
        // 1- BomberMove in a direction that will make you turn (not any turning type)
        // 2- BomberMove in a direction where the way is free
        // 3- BomberMove in a direction that will make a move possible
        // 4- Else... go in a direction, even if the move is impossible

        switch (m_BomberMove)
        {
        case BOMBERMOVE_UPLEFT:
            if (TurnTest(BOMBERMOVE_UP) == TURNING_UPLEFT_UP)                  m_BomberMove = BOMBERMOVE_UP;
            else if (TurnTest(BOMBERMOVE_LEFT) == TURNING_UPLEFT_LEFT)         m_BomberMove = BOMBERMOVE_LEFT;
            else if (CanMove(BOMBERMOVE_UP) == CANMOVE_FREEWAY)                m_BomberMove = BOMBERMOVE_UP;
            else if (CanMove(BOMBERMOVE_LEFT) == CANMOVE_FREEWAY)              m_BomberMove = BOMBERMOVE_LEFT;
            else if (CanMove(BOMBERMOVE_UP) != CANMOVE_CANNOT)                 m_BomberMove = BOMBERMOVE_UP;
            else                                                               m_BomberMove = BOMBERMOVE_LEFT;
            break;

        case BOMBERMOVE_UPRIGHT:
            if (TurnTest(BOMBERMOVE_UP) == TURNING_UPRIGHT_UP)                 m_BomberMove = BOMBERMOVE_UP;
            else if (TurnTest(BOMBERMOVE_RIGHT) == TURNING_UPRIGHT_RIGHT)      m_BomberMove = BOMBERMOVE_RIGHT;
            else if (CanMove(BOMBERMOVE_UP) == CANMOVE_FREEWAY)                m_BomberMove = BOMBERMOVE_UP;
            else if (CanMove(BOMBERMOVE_RIGHT) == CANMOVE_FREEWAY)             m_BomberMove = BOMBERMOVE_RIGHT;
            else if (CanMove(BOMBERMOVE_UP) != CANMOVE_CANNOT)                 m_BomberMove = BOMBERMOVE_UP;
            else                                                               m_BomberMove = BOMBERMOVE_RIGHT;
            break;

        case BOMBERMOVE_DOWNLEFT:
            if (TurnTest(BOMBERMOVE_DOWN) == TURNING_DOWNLEFT_DOWN)            m_BomberMove = BOMBERMOVE_DOWN;
            else if (TurnTest(BOMBERMOVE_LEFT) == TURNING_DOWNLEFT_LEFT)       m_BomberMove = BOMBERMOVE_LEFT;
            else if (CanMove(BOMBERMOVE_DOWN) == CANMOVE_FREEWAY)              m_BomberMove = BOMBERMOVE_DOWN;
            else if (CanMove(BOMBERMOVE_LEFT) == CANMOVE_FREEWAY)              m_BomberMove = BOMBERMOVE_LEFT;
            else if (CanMove(BOMBERMOVE_DOWN) != CANMOVE_CANNOT)               m_BomberMove = BOMBERMOVE_DOWN;
            else                                                               m_BomberMove = BOMBERMOVE_LEFT;
            break;

        case BOMBERMOVE_DOWNRIGHT:
            if (TurnTest(BOMBERMOVE_DOWN) == TURNING_DOWNRIGHT_DOWN)           m_BomberMove = BOMBERMOVE_DOWN;
            else if (TurnTest(BOMBERMOVE_RIGHT) == TURNING_DOWNRIGHT_RIGHT)    m_BomberMove = BOMBERMOVE_RIGHT;
            else if (CanMove(BOMBERMOVE_DOWN) == CANMOVE_FREEWAY)              m_BomberMove = BOMBERMOVE_DOWN;
            else if (CanMove(BOMBERMOVE_RIGHT) == CANMOVE_FREEWAY)             m_BomberMove = BOMBERMOVE_RIGHT;
            else if (CanMove(BOMBERMOVE_DOWN) != CANMOVE_CANNOT)               m_BomberMove = BOMBERMOVE_DOWN;
            else                                                               m_BomberMove = BOMBERMOVE_RIGHT;
            break;

        default:
            break;
        }

        // If the move is no more than one pixel then there is no problem. 
        // Otherwise move pixel by pixel in order not to avoid any collision.
        float fPixelsLeft = fPixels;    // How many pixels left. Used to set m_CouldMove

        while (true)
        {
            if (fPixelsLeft >= 1.0f)
            {
                // If you can't move by one pixel 
                // then you can't move at all
                if (!TryMove(1.0f))
                    break;

                // You moved
                fPixelsLeft -= 1.0f;
            }
            else
            {
                // If you can move by one pixel then 
                // you can move by less than one pixel
                if (TryMove(fPixelsLeft))
                    fPixelsLeft = 0.0f;     // You moved

                // Finished moving
                break;
            }
        }

        // If the bomber could move by any number of pixels 
        // or part of pixels then he could move
        m_CouldMove = (fPixelsLeft < fPixels);

        // Update integer position
        m_iX = int(m_X);
        m_iY = int(m_Y);

        // Update Block coordinates
        m_BlockX = m_pArena->ToBlock(m_iX + BLOCK_SIZE / 2);
        m_BlockY = m_pArena->ToBlock(m_iY + BLOCK_SIZE / 2);

        ASSERT(m_BlockX >= 0);
        ASSERT(m_BlockX < ARENA_WIDTH);

        ASSERT(m_BlockY >= 0);
        ASSERT(m_BlockY < ARENA_HEIGHT);

    }

    // If the bomber is holding a bomb
    if (m_pArena->GetBomber(m_Player).GetState() == BOMBERSTATE_WALK_HOLD)
    {
        // Get the bomb the bomber is holding
        CBomb& Bomb = m_pArena->GetBomb(m_pArena->GetBomber(m_Player).GetBombIndex());

        // Make this bomb follow the bomber
        Bomb.SetBlock(m_BlockX, m_BlockY);
        Bomb.SetPosition(m_iX + BOMBER_TO_HELD_BOMB_POSITION_X, m_iY + BOMBER_TO_HELD_BOMB_POSITION_Y);
    }

    // If the bomber had to move
    if (m_BomberMove != BOMBERMOVE_NONE)
    {
        // Remember this move, which is a move in a valid direction.
        m_LastRealBomberMove = m_BomberMove;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomberMove::WriteSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.WriteInteger(m_Player);
    Snapshot.WriteFloat(m_X);
    Snapshot.WriteFloat(m_Y);
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteInteger(m_BomberMove);
    Snapshot.WriteInteger(m_LastRealBomberMove);
    Snapshot.WriteInteger(m_Turning);
    Snapshot.WriteBoolean(m_CouldMove);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CBomberMove::ReadSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.ReadInteger(&m_Player);
    Snapshot.ReadFloat(&m_X);
    Snapshot.ReadFloat(&m_Y);
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadInteger((int*)&m_BomberMove);
    Snapshot.ReadInteger((int*)&m_LastRealBomberMove);
    Snapshot.ReadInteger((int*)&m_Turning);
    Snapshot.ReadBoolean(&m_CouldMove);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// These defines are here to simplify the code
// of the 3 following methods TryMove, CanMove, TurnTest.
// They're not all uppercase for readability.

#define ToBlock(a)          (m_pArena->ToBlock (a))
#define HalfBlock           (BLOCK_SIZE / 2)
#define TurnLimit           (HalfBlock / 4)
#define BlockedLimit        (-HalfBlock + (HalfBlock / 4))

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// TryMove tries to go by fPixels pixels in the current direction and turning state.
// Bombs which are in the bomber's way will be kicked if the bomber can kick them.
// Important : the tests are made for a move of one single pixel or less and do not
// depend on fPixels. If the move was judged possible, position is modified by fPixels.
// The move is tested in the current BomberMove or turning state :
// - This can make direction or turning state change : then try again
// - This can modify the coordinates because the way is free : then return true
// - Or maybe it is impossible to go further : then return false

bool CBomberMove::TryMove(float fPixels)
{
    // The m_BomberMove must describe a SINGLE DIRECTION. If not, infinite loop.
    ASSERT(m_BomberMove != BOMBERMOVE_UPLEFT   &&
        m_BomberMove != BOMBERMOVE_UPRIGHT  &&
        m_BomberMove != BOMBERMOVE_DOWNLEFT &&
        m_BomberMove != BOMBERMOVE_DOWNRIGHT);

    CBomber* pBomber = &m_pArena->GetBomber(m_Player);

    // Compute coordinates
    int X = int(m_X) + HalfBlock;   // Modified integer current coordinates (x+hb and y+hb to point to center)
    int Y = int(m_Y) + HalfBlock;   // Note : we Have to compute int(m_X) and int(m_Y) because they change

    int iter = 0;

    // Try until bomber coordinates are modified or bomber is blocked
    while (iter++ < MAX_ITER)
    {

        // Not turning
        if (m_Turning == TURNING_NOTTURNING)
        {
            switch (m_BomberMove)
            {
            case BOMBERMOVE_UP:
                // If obstacle above
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y - HalfBlock - 1)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y - HalfBlock - 1))
                    {
                        // Can move
                        m_Y -= fPixels;
                        return true;
                    }

                    // Else try to kick a bomb here
                    pBomber->TryKickBomb(m_BlockX, ToBlock(Y - HalfBlock - 1), BOMBKICK_UP);

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up left, avoid
                        m_BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up right, avoid
                        m_BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return false;
                }
                // No wall or bomb above, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        m_Turning = TURNING_UPLEFT_UP;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }
                    // If wall up left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        m_Turning = TURNING_UPRIGHT_UP;
                        continue;
                    }

                    // Else the way is free, modify the coordinates
                    m_Y -= fPixels;
                    return true;
                }
                break;

            case BOMBERMOVE_DOWN:
                // If obstacle below
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y + HalfBlock)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y + HalfBlock))
                    {
                        // Can move
                        m_Y += fPixels;
                        return true;
                    }

                    // Else try to kick a bomb here
                    pBomber->TryKickBomb(m_BlockX, ToBlock(Y + HalfBlock), BOMBKICK_DOWN);

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down left, avoid
                        m_BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down right, avoid
                        m_BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return false;
                }
                // No wall or bomb below, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        m_Turning = TURNING_DOWNLEFT_DOWN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        m_Turning = TURNING_DOWNRIGHT_DOWN;
                        continue;
                    }

                    // Else the way is free, modify the coordinates
                    m_Y += fPixels;
                    return true;
                }
                break;

            case BOMBERMOVE_LEFT:
                // If obstacle to the left
                if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X - HalfBlock - 1))
                    {
                        // Can move
                        m_X -= fPixels;
                        return true;
                    }

                    // Else try to kick a bomb here
                    pBomber->TryKickBomb(ToBlock(X - HalfBlock - 1), m_BlockY, BOMBKICK_LEFT);

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up left, avoid
                        m_BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down left, avoid
                        m_BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return false;
                }
                // No wall or bomb to the left, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        m_Turning = TURNING_UPLEFT_LEFT;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        m_Turning = TURNING_DOWNLEFT_LEFT;
                        continue;
                    }

                    // Else the way is free, modify the coordinates
                    m_X -= fPixels;
                    return true;
                }
                break;

            case BOMBERMOVE_RIGHT:
                // If obstacle to the right
                if (pBomber->IsObstacle(ToBlock(X + HalfBlock), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X + HalfBlock))
                    {
                        // Can move
                        m_X += fPixels;
                        return true;
                    }

                    // Else try to kick a bomb here
                    pBomber->TryKickBomb(ToBlock(X + HalfBlock), m_BlockY, BOMBKICK_RIGHT);

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up right, avoid
                        m_BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down right, avoid
                        m_BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return false;
                }
                // No wall or bomb to the right, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        m_Turning = TURNING_UPRIGHT_RIGHT;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        m_BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        m_Turning = TURNING_DOWNRIGHT_RIGHT;
                        continue;
                    }

                    // Else the way is free, modify the coordinates
                    m_X += fPixels;
                    return true;
                }
                break;
            default:
                break;
            }
        }
        // Turning
        else
        {
            switch (m_Turning)
            {
            case TURNING_UPLEFT_UP:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X -= fPixels;
                        m_Y -= fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_DOWNRIGHT_RIGHT;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPLEFT_LEFT:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X -= fPixels;
                        m_Y -= fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_DOWNRIGHT_DOWN;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_UP:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X += fPixels;
                        m_Y -= fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_DOWNLEFT_LEFT;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_RIGHT:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X += fPixels;
                        m_Y -= fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_DOWNLEFT_DOWN;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_DOWN:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X -= fPixels;
                        m_Y += fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_UPRIGHT_RIGHT;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_LEFT:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X -= fPixels;
                        m_Y += fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_UPRIGHT_UP;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_DOWN:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X += fPixels;
                        m_Y += fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_UPLEFT_LEFT;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_RIGHT:
                switch (m_BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        m_Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok, modify the coordinates
                        m_X += fPixels;
                        m_Y += fPixels;
                        m_Turning = TURNING_NOTTURNING;
                        return true;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    m_Turning = TURNING_UPLEFT_UP;
                    continue;

                default:
                    // Stop turning to try this direction
                    m_Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;
            default:
                break;
            }
        }
    }

    // Can't happen
    return false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// CanMove is very similar to TryMove. CanMove tries to go in the desired direction, but
// contrary to TryMove, the test are made with a virtual bomber, the real states won't
// be modified. CanMove returns an information about the test : could he move, did he
// have to avoid or turn around an obstacle?

ECanMove CBomberMove::CanMove(EBomberMove TestMove)
{
    // The TestMove must describe a SINGLE DIRECTION. If not, infinite loop.
    ASSERT(TestMove != BOMBERMOVE_NONE && TestMove != BOMBERMOVE_UPLEFT && TestMove != BOMBERMOVE_UPRIGHT && TestMove != BOMBERMOVE_DOWNLEFT && TestMove != BOMBERMOVE_DOWNRIGHT);

    CBomber* pBomber = &m_pArena->GetBomber(m_Player);
    EBomberMove BomberMove = TestMove;      // Temporary move variable in order not to modify the real one
    ETurning Turning = m_Turning;           // Temporary turning variable in order not to modify the real one
    ECanMove CanMove = CANMOVE_FREEWAY;     // The variable that will be returned. Assume the way is free.

    // Compute coordinates
    int X = m_iX + HalfBlock;   // Modified integer current coordinates (x+hb and y+hb to point to center)
    int Y = m_iY + HalfBlock;

    int iter = 0;

    // Try until bomber coordinates are modified or bomber is blocked
    while (iter++ < MAX_ITER)
    {

        // Not turning
        if (Turning == TURNING_NOTTURNING)
        {
            switch (BomberMove)
            {
            case BOMBERMOVE_UP:
                // If obstacle above
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y - HalfBlock - 1)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y - HalfBlock - 1))
                    {
                        // Can move
                        return CanMove;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up left, avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up right, avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return CANMOVE_CANNOT;
                }
                // No wall or bomb above, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPLEFT_UP;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall up left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPRIGHT_UP;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // Else the way is free, but perhaps he had to avoid or turn so return the variable
                    return CanMove;
                }
                break;

            case BOMBERMOVE_DOWN:
                // If obstacle below
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y + HalfBlock)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y + HalfBlock))
                    {
                        // Can move
                        return CanMove;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down left, avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down right, avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return CANMOVE_CANNOT;
                }
                // No wall or bomb below, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNLEFT_DOWN;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNRIGHT_DOWN;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // Else the way is free, but perhaps he had to avoid or turn so return the variable
                    return CanMove;
                }
                break;

            case BOMBERMOVE_LEFT:
                // If obstacle to the left
                if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X - HalfBlock - 1))
                    {
                        // Can move
                        return CanMove;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up left, avoid
                        BomberMove = BOMBERMOVE_UP;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down left, avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return CANMOVE_CANNOT;
                }
                // No wall or bomb to the left, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_UP;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPLEFT_LEFT;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNLEFT_LEFT;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // Else the way is free, but perhaps he had to avoid or turn so return the variable
                    return CanMove;
                }
                break;

            case BOMBERMOVE_RIGHT:
                // If obstacle to the right
                if (pBomber->IsObstacle(ToBlock(X + HalfBlock), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X + HalfBlock))
                    {
                        // Can move
                        return CanMove;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up right, avoid
                        BomberMove = BOMBERMOVE_UP;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down right, avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return CANMOVE_CANNOT;
                }
                // No wall or bomb to the right, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_UP;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPRIGHT_RIGHT;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        CanMove = CANMOVE_AVOID;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNRIGHT_RIGHT;
                        CanMove = CANMOVE_TURN;
                        continue;
                    }

                    // Else the way is free, but perhaps he had to avoid or turn so return the variable
                    return CanMove;
                }
                break;
            default:
                break;
            }
        }
        // Turning
        else
        {
            switch (Turning)
            {
            case TURNING_UPLEFT_UP:
                switch (BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNRIGHT_RIGHT;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPLEFT_LEFT:
                switch (BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNRIGHT_DOWN;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_UP:
                switch (BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNLEFT_LEFT;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_RIGHT:
                switch (BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNLEFT_DOWN;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_DOWN:
                switch (BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPRIGHT_RIGHT;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_LEFT:
                switch (BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPRIGHT_UP;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_DOWN:
                switch (BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPLEFT_LEFT;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_RIGHT:
                switch (BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return CanMove;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPLEFT_UP;
                    CanMove = CANMOVE_TURN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;
            default:
                break;
            }
        }
    }

    // Can't happen
    return CANMOVE_CANNOT;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// TurnTest is very similar to CanMove. TurnTest makes a virtual bomber move, then when
// the move is finished (ie the virtual bomber has moved or is blocked), it returns
// the virtual turning state.

ETurning CBomberMove::TurnTest(EBomberMove TestMove)
{
    // The TestMove must describe a SINGLE DIRECTION. If not, infinite loop.
    ASSERT(TestMove != BOMBERMOVE_UPLEFT && TestMove != BOMBERMOVE_UPRIGHT && TestMove != BOMBERMOVE_DOWNLEFT && TestMove != BOMBERMOVE_DOWNRIGHT);

    CBomber* pBomber = &m_pArena->GetBomber(m_Player);
    EBomberMove BomberMove = TestMove;  // Temporary go variable in order not to modify the real one
    ETurning Turning = m_Turning;       // Temporary turning variable in order not to modify the real one

    // Compute coordinates
    int X = m_iX + HalfBlock;       // Modified integer current coordinates (x+hb and y+hb to point to center)
    int Y = m_iY + HalfBlock;

    int iter = 0;

    // Try until bomber coordinates are modified or bomber is blocked
    while (iter++ < MAX_ITER)
    {

        // Not turning
        if (Turning == TURNING_NOTTURNING)
        {
            switch (BomberMove)
            {
            case BOMBERMOVE_UP:
                // If obstacle above
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y - HalfBlock - 1)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y - HalfBlock - 1))
                    {
                        // Can move
                        return Turning;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up left, avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // No obstacle up right, avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return Turning;
                }
                // No wall or bomb above, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPLEFT_UP;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }
                    // If wall up left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPRIGHT_UP;
                        continue;
                    }

                    // Else the way is free
                    return Turning;
                }
                break;

            case BOMBERMOVE_DOWN:
                // If obstacle below
                if (pBomber->IsObstacle(m_BlockX, ToBlock(Y + HalfBlock)))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockY == ToBlock(Y + HalfBlock))
                    {
                        // Can move
                        return Turning;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + BlockedLimit), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down left, avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - BlockedLimit - 1), ToBlock(Y + HalfBlock)))
                    {
                        // No obstacle down right, avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return Turning;
                }
                // No wall or bomb below, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_LEFT;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNLEFT_DOWN;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - TurnLimit), ToBlock(Y + HalfBlock)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_RIGHT;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNRIGHT_DOWN;
                        continue;
                    }

                    // Else the way is free
                    return Turning;
                }
                break;

            case BOMBERMOVE_LEFT:
                // If obstacle to the left
                if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X - HalfBlock - 1))
                    {
                        // Can move
                        return Turning;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up left, avoid
                        BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down left, avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return Turning;
                }
                // No wall or bomb to the left, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    // If wall down left
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPLEFT_LEFT;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNLEFT_LEFT;
                        continue;
                    }

                    // Else the way is free
                    return Turning;
                }
                break;

            case BOMBERMOVE_RIGHT:
                // If obstacle to the right
                if (pBomber->IsObstacle(ToBlock(X + HalfBlock), m_BlockY))
                {
                    // For walls and bombs : if the tested square is the same as the bomber's square
                    if (m_BlockX == ToBlock(X + HalfBlock))
                    {
                        // Can move
                        return Turning;
                    }

                    // Can he avoid the obstacle?
                    if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + BlockedLimit)))
                    {
                        // No obstacle up right, avoid
                        BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    else if (!pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - BlockedLimit - 1)))
                    {
                        // No obstacle down right, avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }

                    // Can't avoid the obstacle
                    return Turning;
                }
                // No wall or bomb to the right, should he turn around a wall (or avoid it) or be blocked?
                else
                {
                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_UP;
                        continue;
                    }
                    // If wall down right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock - 1)))
                    {
                        // Turn
                        Turning = TURNING_UPRIGHT_RIGHT;
                        continue;
                    }

                    // To avoid turning too early
                    if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - TurnLimit)))
                    {
                        // Avoid
                        BomberMove = BOMBERMOVE_DOWN;
                        continue;
                    }
                    // If wall up right
                    else if (m_pArena->IsWall(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock)))
                    {
                        // Turn
                        Turning = TURNING_DOWNRIGHT_RIGHT;
                        continue;
                    }

                    // Else the way is free
                    return Turning;
                }
                break;
            default:
                break;
            }
        }
        // Turning
        else
        {
            switch (Turning)
            {
            case TURNING_UPLEFT_UP:
                switch (BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNRIGHT_RIGHT;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPLEFT_LEFT:
                switch (BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNRIGHT_DOWN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_UP:
                switch (BomberMove)
                {
                case BOMBERMOVE_UP:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNLEFT_LEFT;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_UPRIGHT_RIGHT:
                switch (BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y - HalfBlock - 1)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_DOWN:
                    // Try turning in the opposite direction
                    Turning = TURNING_DOWNLEFT_DOWN;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_DOWN:
                switch (BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_RIGHT:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPRIGHT_RIGHT;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNLEFT_LEFT:
                switch (BomberMove)
                {
                case BOMBERMOVE_LEFT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X - HalfBlock - 1), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPRIGHT_UP;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_DOWN:
                switch (BomberMove)
                {
                case BOMBERMOVE_DOWN:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_LEFT:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPLEFT_LEFT;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;

            case TURNING_DOWNRIGHT_RIGHT:
                switch (BomberMove)
                {
                case BOMBERMOVE_RIGHT:
                    // Stop turning if obstacle
                    if (pBomber->IsObstacle(ToBlock(X + HalfBlock), ToBlock(Y + HalfBlock)))
                    {
                        Turning = TURNING_NOTTURNING;
                        continue;
                    }
                    else
                    {
                        // Else it's ok
                        return Turning;
                    }

                case BOMBERMOVE_UP:
                    // Try turning in the opposite direction
                    Turning = TURNING_UPLEFT_UP;
                    continue;

                default:
                    // Stop turning to try this direction
                    Turning = TURNING_NOTTURNING;
                    continue;
                }
                break;
            default:
                break;
            }
        }
    }

    // Can't happen
    return Turning;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


