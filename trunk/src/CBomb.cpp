// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CBomb.cpp
 *  \brief The bomb
 */

#include "StdAfx.h"
#include "CBomb.h"
#include "CArena.h"
#include "CItem.h"
#include "CDisplay.h"
#include "CSound.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

int CBomb::m_ThrowMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][6] =
{
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_NONE
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_UP
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_DOWN
    { -10, -11, -9, -7, -7, -4 }, // BOMBFLY_LEFT
    { 10, 11, 9, 7, 7, 4 }, // BOMBFLY_RIGHT
};

int CBomb::m_ThrowMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][6] =
{
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_NONE
    { -5, -7, -8, -8, -5, -5 }, // BOMBFLY_UP
    { 7, 9, 14, 15, 11, 9 }, // BOMBFLY_DOWN
    { -4, -1, 2, 4, 6, 10 }, // BOMBFLY_LEFT
    { -4, -1, 2, 4, 6, 10 }, // BOMBFLY_RIGHT
};

int CBomb::m_PunchMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][6] =
{
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_NONE
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_UP
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_DOWN
    { -8, -10, -9, -9, -6, -6 }, // BOMBFLY_LEFT
    { 8, 10, 9, 9, 6, 6 }, // BOMBFLY_RIGHT
};

int CBomb::m_PunchMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][6] =
{
    { 0, 0, 0, 0, 0, 0 }, // BOMBFLY_NONE
    { -6, -8, -10, -10, -8, -6 }, // BOMBFLY_UP
    { 6, 8, 10, 10, 8, 6 }, // BOMBFLY_DOWN
    { -9, -5, 0, 3, 4, 7 }, // BOMBFLY_LEFT
    { -9, -5, 0, 3, 4, 7 }, // BOMBFLY_RIGHT
};

int CBomb::m_BounceMoveX[NUMBER_OF_BOMBFLY_DIRECTIONS][3] =
{
    { 0, 0, 0 }, // BOMBFLY_NONE
    { 0, 0, 0 }, // BOMBFLY_UP
    { 0, 0, 0 }, // BOMBFLY_DOWN
    { -5, -6, -5 }, // BOMBFLY_LEFT
    { 5, 6, 5 }, // BOMBFLY_RIGHT
};

int CBomb::m_BounceMoveY[NUMBER_OF_BOMBFLY_DIRECTIONS][3] =
{
    { 0, 0, 0 }, // BOMBFLY_NONE
    { -5, -7, -4 }, // BOMBFLY_UP
    { 4, 7, 5 }, // BOMBFLY_DOWN
    { -4, 0, 4 }, // BOMBFLY_LEFT
    { -4, 0, 4 }, // BOMBFLY_RIGHT
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Time (in seconds) before the bomb explodes when touched by a flame
#define EXPLODE_SOON            0.080f
#define MAX_EXPLOSION_TIME      8.0f

// Speed of a moving bomb (in pixels per second)
#define SPEED_BOMBMOVE          100

// Bomb animation times (in seconds)
// Normal speed
#define ANIMBOMB_NORMAL_TIME1       0.200f
#define ANIMBOMB_NORMAL_TIME2       (ANIMBOMB_NORMAL_TIME1 * 2)
#define ANIMBOMB_NORMAL_TIME3       (ANIMBOMB_NORMAL_TIME1 * 3)
#define ANIMBOMB_NORMAL_TIME4       (ANIMBOMB_NORMAL_TIME1 * 4)
// Slow
#define ANIMBOMB_SLOW_TIME1         0.300f
#define ANIMBOMB_SLOW_TIME2         (ANIMBOMB_SLOW_TIME1 * 2)
#define ANIMBOMB_SLOW_TIME3         (ANIMBOMB_SLOW_TIME1 * 3)
#define ANIMBOMB_SLOW_TIME4         (ANIMBOMB_SLOW_TIME1 * 4)
// FastSLOW_
#define ANIMBOMB_FAST_TIME1         0.100f
#define ANIMBOMB_FAST_TIME2         (ANIMBOMB_FAST_TIME1 * 2)
#define ANIMBOMB_FAST_TIME3         (ANIMBOMB_FAST_TIME1 * 3)
#define ANIMBOMB_FAST_TIME4         (ANIMBOMB_FAST_TIME1 * 4)

// Bomb animation sprites
#define ANIMBOMB_SPRITE0        0
#define ANIMBOMB_SPRITE1        1
#define ANIMBOMB_SPRITE2        2

// Bomb sprite layer
#define BOMB_SPRITELAYER_BELOW_BOMBERS        40
#define BOMB_SPRITELAYER_ABOVE_BOMBERS        55

// Movement times when the bomb is flying
#define THROW_BASE_FRAME_TIME       0.030f // When bomb was thrown
#define BOUNCE_BASE_FRAME_TIME      0.030f // When bomb is bouncing on a bomber or on a wall
#define PUNCH_BASE_FRAME_TIME       0.030f // When bomb was punched

// Moving of bombs by special blocks
#define TIME_BEFORE_MOVING_BOMB     0.3f // This time must elapse before the bomb starts moving

// Bombs can change direction (can be re-kicked) when passing special blocks
#define BOMB_CAN_CHANGE_DIRECTION_WHEN_KICKED   true


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBomb::CBomb(void) : CElement()
{

    m_iX = 0;
    m_iY = 0;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_BlockX = 0;
    m_BlockY = 0;
    m_Sprite = 0;
    m_Timer = 0.0f;
    m_HasToStopMoving = false;
    m_OwnerPlayer = 0;
    m_KickerPlayer = 0;
    m_Checked = false;
    m_Dead = false;
    m_FlameSize = 0;
    m_ElapsedTime = 0.0f;
    m_TimeLeft = 0.0f;
    m_BeingHeld = false;
    m_BeingLifted = false;
    m_BeingPunched = false;
    m_FlightTimer = 0.0f;
    m_FlightFrame = 0;
    m_Warping = false;
    m_Remote = false;

    m_BombKick = BOMBKICK_NONE;
    m_BombFly = BOMBFLY_NONE;
    m_FlightType = BOMBFLIGHTTYPE_NONE;

    for (int i = 0; i < 4; i++)
        m_AnimationTimes[i] = 0.0f;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


CBomb::~CBomb(void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CBomb::Create(int BlockX, int BlockY, int FlameSize, float TimeLeft, int OwnerPlayer)
{
    CElement::Create();

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "New bomb [x=%02d, y=%02d, flamesize=%02d, owner=%d].", BlockX, BlockY, FlameSize, OwnerPlayer);
#endif

    m_iX = m_pArena->ToPosition(BlockX);
    m_iY = m_pArena->ToPosition(BlockY);
    m_X = float(m_iX);
    m_Y = float(m_iY);
    m_BlockX = BlockX;
    m_BlockY = BlockY;
    m_TimeLeft = TimeLeft;
    m_FlameSize = FlameSize;
    m_Sprite = ANIMBOMB_SPRITE2;
    m_BombKick = BOMBKICK_NONE;
    m_BombFly = BOMBFLY_NONE;
    m_ElapsedTime = 0.0f;
    m_Timer = 0.0f;
    m_Dead = false;
    m_HasToStopMoving = false;
    m_Checked = false;
    m_OwnerPlayer = OwnerPlayer;
    m_KickerPlayer = -1;   // No kicker yet
    m_BeingLifted = false;
    m_BeingHeld = false;
    m_BeingPunched = false;
    m_FlightTimer = 0.0f;
    m_FlightFrame = -1;
    m_FlightType = BOMBFLIGHTTYPE_NONE;
    m_Warping = false;
    m_Remote = m_pArena->GetBomber(OwnerPlayer).CanRemoteFuseBombs();

    if (TimeLeft <= 1.0f)
    {
        m_AnimationTimes[0] = ANIMBOMB_FAST_TIME1;
        m_AnimationTimes[1] = ANIMBOMB_FAST_TIME2;
        m_AnimationTimes[2] = ANIMBOMB_FAST_TIME3;
        m_AnimationTimes[3] = ANIMBOMB_FAST_TIME4;
    }
    else if (TimeLeft >= 4.0f)
    {
        m_AnimationTimes[0] = ANIMBOMB_SLOW_TIME1;
        m_AnimationTimes[1] = ANIMBOMB_SLOW_TIME2;
        m_AnimationTimes[2] = ANIMBOMB_SLOW_TIME3;
        m_AnimationTimes[3] = ANIMBOMB_SLOW_TIME4;
    }
    else
    {
        m_AnimationTimes[0] = ANIMBOMB_NORMAL_TIME1;
        m_AnimationTimes[1] = ANIMBOMB_NORMAL_TIME2;
        m_AnimationTimes[2] = ANIMBOMB_NORMAL_TIME3;
        m_AnimationTimes[3] = ANIMBOMB_NORMAL_TIME4;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CBomb::Destroy(void)
{
    CElement::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Makes the bomb explode, that is to say create an
// explosion in the arena, Then the bomb is dead and can
// be deleted by the arena.

void CBomb::Explode(void)
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb exploding [x=%02d, y=%02d, flamesize=%02d, owner=%d].", m_BlockX, m_BlockY, m_FlameSize, m_OwnerPlayer);
#endif

    // Create the explosion
    m_pArena->NewExplosion(m_BlockX, m_BlockY, m_FlameSize);

    // Play a random explosion sound according to its flame size
    switch (m_FlameSize)
    {
    case 1: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_01_1 : SAMPLE_EXPLOSION_01_2)); break;
    case 2: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_02_1 : SAMPLE_EXPLOSION_02_2)); break;
    case 3: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_03_1 : SAMPLE_EXPLOSION_03_2)); break;
    case 4: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_04_1 : SAMPLE_EXPLOSION_04_2)); break;
    case 5: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_05_1 : SAMPLE_EXPLOSION_05_2)); break;
    case 6: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_06_1 : SAMPLE_EXPLOSION_06_2)); break;
    case 7: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_07_1 : SAMPLE_EXPLOSION_07_2)); break;
    case 8: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_08_1 : SAMPLE_EXPLOSION_08_2)); break;
    case 9: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_09_1 : SAMPLE_EXPLOSION_09_2)); break;
    default: m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_EXPLOSION_10_1 : SAMPLE_EXPLOSION_10_2)); break;
    }

    // The bomb is dead now
    m_Dead = true;

    // The bomb isn't kicked any longer now
    m_BombKick = BOMBKICK_NONE;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// This is called by walls crushing the bomb.

void CBomb::Crush(void)
{
#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb crushing [x=%02d, y=%02d, owner=%d].", m_BlockX, m_BlockY, m_OwnerPlayer);
#endif

    Explode();
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Shorten the time left before the bomb explodes. This
// is called by flames touching the bomb or by the remote
// trigger. The bomb will then explode very soon.

void CBomb::Burn(void)
{
#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb burning [x=%02d, y=%02d, owner=%d].", m_BlockX, m_BlockY, m_OwnerPlayer);
#endif

    if (m_TimeLeft > EXPLODE_SOON)
        m_TimeLeft = EXPLODE_SOON;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Tell the bomb to start moving in a direction.
// Save the number of the player that kicked the
// bomb for the kicker to be able to find it.

void CBomb::StartMoving(EBombKick BombKick, int KickerPlayer)
{
    // The bomb cannot be moving if a bomber is doing something with it
    if (m_BeingHeld || m_BeingLifted || m_BeingPunched)
        return;

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMB, "Bomb starts moving [x=%02d, y=%02d, owner=%d, new_direction=%d, kicker=%d, old_direction=%d].", m_BlockX, m_BlockY, m_OwnerPlayer, BombKick, KickerPlayer, m_BombKick);
#endif

    switch (m_BombKick)
    {
    case BOMBKICK_LEFT:
    case BOMBKICK_RIGHT:
        if (BombKick == BOMBKICK_UP || BombKick == BOMBKICK_DOWN)
        {
            // 90 degree changement => center bomb on X axis
            CenterOnBlock();
        }
        break;
    case BOMBKICK_UP:
    case BOMBKICK_DOWN:
        if (BombKick == BOMBKICK_LEFT || BombKick == BOMBKICK_RIGHT)
        {
            // 90 degree changement => center bomb on Y axis
            CenterOnBlock();
        }
        break;
    default:
        break;
    }

    m_BombKick = BombKick;
    m_KickerPlayer = KickerPlayer;
    m_HasToStopMoving = false;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Tell the bomb to stop moving as soon
// as possible.

void CBomb::StopMoving()
{
    // The bomb has to stop
    m_HasToStopMoving = true;
    // The bomb has no kicker anymore
    m_KickerPlayer = -1;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Make the bomb move if needed. Move one pixel by one pixel
// only (or less) so as to manage every collision during the
// move. Kill items in the bomb's way.

void CBomb::ManageMove(float DeltaTime)
{
    // If the bomb has to move and it isn't already dead
    if (m_BombKick != BOMBKICK_NONE && !m_Dead)
    {
        // Compute the move length
        float fPixels = SPEED_BOMBMOVE * DeltaTime;

        // If the move is no more than one pixel then there is no problem. 
        // Otherwise move pixel by pixel in order not to avoid any collision.

        while (true)
        {
            // Check if the bomb has to change its direction (initiated by a block) and whether it is centered
            if (m_BombFly == BOMBFLY_NONE)
            {
                ASSERT(m_pArena != NULL);
                if (m_pArena->IsFloorWithMoveEffect(m_BlockX, m_BlockY) && BOMB_CAN_CHANGE_DIRECTION_WHEN_KICKED &&
                    (!(m_iX & (BLOCK_SIZE - 1)) && !(m_iY & (BLOCK_SIZE - 1))) && // Taken from below
                    m_BombKick != BOMBKICK_NONE  // Bomb is still moving (could be reset by TryMove()
                    )
                {
                    EFloorAction action = m_pArena->GetFloorAction(m_BlockX, m_BlockY);

                    EBombKick kickDirection = BOMBKICK_NONE;

                    switch (action) {
                    case FLOORACTION_MOVEBOMB_RIGHT:    kickDirection = BOMBKICK_RIGHT; break;
                    case FLOORACTION_MOVEBOMB_DOWN:     kickDirection = BOMBKICK_DOWN;  break;
                    case FLOORACTION_MOVEBOMB_LEFT:     kickDirection = BOMBKICK_LEFT;  break;
                    case FLOORACTION_MOVEBOMB_UP:       kickDirection = BOMBKICK_UP;    break;
                    default:                                                            break;
                    }

                    ASSERT(kickDirection != BOMBKICK_NONE);

                    // Set the new direction
                    m_BombKick = kickDirection;
                }
            }

            if (fPixels >= 1.0f)
            {
                // If the bomb can't move by one pixel 
                // then it can't move at all
                if (!TryMove(1.0f))
                    break;

                // You moved
                fPixels -= 1.0f;

                // If the bomb has to stop moving
                if (m_HasToStopMoving)
                {
                    // If the bomb is exactly in a block
                    if (!(m_iX & (BLOCK_SIZE - 1)) &&
                        !(m_iY & (BLOCK_SIZE - 1)))
                    {
                        // Then it can stop moving
                        m_BombKick = BOMBKICK_NONE;
                        m_HasToStopMoving = false;
                        break;
                    }
                }
            }
            else
            {
                // If the bomb can move by one pixel then 
                // it can move by less than one pixel
                if (TryMove(fPixels))
                    fPixels = 0.0f;     // It moved

                // If the bomb has to stop moving
                if (m_HasToStopMoving)
                {
                    // If the bomb is exactly in a block
                    if (!(m_iX & (BLOCK_SIZE - 1)) &&
                        !(m_iY & (BLOCK_SIZE - 1)))
                    {
                        // Then it can stop moving
                        m_BombKick = BOMBKICK_NONE;
                        m_HasToStopMoving = false;
                    }
                }

                // Finished moving
                break;
            }
        }

        CrushItem(m_BlockX, m_BlockY);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::CrushItem(int BlockX, int BlockY)
{
    // If there is an item on its way, the item has to be crushed
    if (m_pArena->IsItem(BlockX, BlockY))
    {
        // Seek this item
        for (int Index = 0; Index < m_pArena->MaxItems(); Index++)
        {
            // Test existence and position
            if (m_pArena->GetItem(Index).Exist() &&
                m_pArena->GetItem(Index).GetBlockX() == BlockX &&
                m_pArena->GetItem(Index).GetBlockY() == BlockY)
            {
                // Crush the item
                m_pArena->GetItem(Index).Crush();
                // There can't be two items on the same block
                break;
            }
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Tell if there is an obstacle at (X,Y).
// For bombs, an obstacle is a wall, a bomb or a bomber.

bool CBomb::IsObstacle(int BlockX, int BlockY)
{
    return (m_pArena->IsWall(BlockX, BlockY) ||
        m_pArena->IsBomb(BlockX, BlockY) ||
        m_pArena->IsBomber(BlockX, BlockY));
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// These defines are here to simplify the code
// of the TryMove method. They're not all uppercase for readability.

#define ToBlock(a)          (m_pArena->ToBlock (a))
#define HalfBlock           (BLOCK_SIZE / 2)


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// TryMove tries to move the bomb. If there is no collision, the 
// coordinates will be modified using fPixels (the move length).
// If there is one, the bomb will stop.

bool CBomb::TryMove(float fPixels)
{
    // Don't try to move if not supposed to be moving
    ASSERT(m_BombKick != BOMBKICK_NONE);

    // Compute coordinates
    int X = m_iX + HalfBlock;   // Modified integer current coordinates (x+hb and y+hb to point to center)
    int Y = m_iY + HalfBlock;

    switch (m_BombKick)
    {
    case BOMBKICK_UP:
        // If no obstacle above, or if there is one, if the tested square is the same as bomb's square
        if (!IsObstacle(m_BlockX, ToBlock(Y - HalfBlock - 1)) || ToBlock(Y - HalfBlock - 1) == m_BlockY)
        {
            // Update coordinates
            m_Y -= fPixels;
            m_iY = int(m_Y);
            m_BlockY = ToBlock(m_iY + HalfBlock);
            return true;
        }
        // Else stop moving
        else
        {
            m_BombKick = BOMBKICK_NONE;
            CenterOnBlock();
            return false;
        }
        break;

    case BOMBKICK_DOWN:
        // If no obstacle below, or if there is one, if the tested square is the same as bomb's square
        if (!IsObstacle(m_BlockX, ToBlock(Y + HalfBlock)) || ToBlock(Y + HalfBlock) == m_BlockY)
        {
            // Update coordinates
            m_Y += fPixels;
            m_iY = int(m_Y);
            m_BlockY = ToBlock(m_iY + HalfBlock);
            return true;
        }
        // Else stop moving
        else
        {
            m_BombKick = BOMBKICK_NONE;
            CenterOnBlock();
            return false;
        }
        break;

    case BOMBKICK_LEFT:
        // If no obstacle to the left, or if there is one, if the tested square is the same as bomb's square
        if (!IsObstacle(ToBlock(X - HalfBlock - 1), m_BlockY) || ToBlock(X - HalfBlock - 1) == m_BlockX)
        {
            // Update coordinates
            m_X -= fPixels;
            m_iX = int(m_X);
            m_BlockX = ToBlock(m_iX + HalfBlock);
            return true;
        }
        // Else stop moving
        else
        {
            m_BombKick = BOMBKICK_NONE;
            CenterOnBlock();
            return false;
        }
        break;

    case BOMBKICK_RIGHT:
        // If no obstacle to the right, or if there is one, if the tested square is the same as bomb's square
        if (!IsObstacle(ToBlock(X + HalfBlock), m_BlockY) || ToBlock(X + HalfBlock) == m_BlockX)
        {
            // Update coordinates
            m_X += fPixels;
            m_iX = int(m_X);
            m_BlockX = ToBlock(m_iX + HalfBlock);
            return true;
        }
        // Else stop moving
        else
        {
            m_BombKick = BOMBKICK_NONE;
            CenterOnBlock();
            return false;
        }
        break;
    default:
        break;
    }

    // Can't happen
    return false;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Make the bomb tick, explode, move, animate, display

bool CBomb::Update(float DeltaTime)
{
    // Update the elapsed time
    m_ElapsedTime += DeltaTime;

    // If the bomb is not dead
    if (!m_Dead)
    {
        // If there is no bomber doing something on this bomb and it is not flying,
        // then it can tick and explode.
        // Note: The states set by a bomber (lifted, held, punched) must be reset 
        //       when a bomber dies or the bomb will never explode (see CBomber::Die()).
        if (!m_BeingLifted && !m_BeingHeld && !m_BeingPunched && m_BombFly == BOMBFLY_NONE)
        {
            // dont update timer if it is a not activated remote bomb
            if (!m_Remote || (m_TimeLeft <= EXPLODE_SOON))
            {
                // If the bomb has some ticks left before exploding
                if (m_TimeLeft > 0.0f)
                {
                    // Make the bomb explode if time is up
                    m_TimeLeft -= DeltaTime;
                    if (m_TimeLeft <= 0.0f)
                    {
                        Explode();
                        m_TimeLeft = 0.0f;
                    }
                }
            }
        }

        // Kick this bomb by special blocks
        // Don't start the move if bomb exploded in the meanwhile (m_Dead)
        // Also don't start the move if the bomb is already in action (lifted, held, punched or flying)
        if (m_BombKick == BOMBKICK_NONE && m_ElapsedTime >= TIME_BEFORE_MOVING_BOMB &&
            !m_Dead && !m_BeingLifted && !m_BeingHeld && !m_BeingPunched && m_BombFly == BOMBFLY_NONE)
        {
            ASSERT(m_pArena != NULL);
            if (m_pArena->IsFloorWithMoveEffect(m_BlockX, m_BlockY)) {
                EFloorAction action = m_pArena->GetFloorAction(m_BlockX, m_BlockY);

                EBombKick kickDirection = BOMBKICK_NONE;

                switch (action) {
                case FLOORACTION_MOVEBOMB_RIGHT:    kickDirection = BOMBKICK_RIGHT; break;
                case FLOORACTION_MOVEBOMB_DOWN:     kickDirection = BOMBKICK_DOWN;  break;
                case FLOORACTION_MOVEBOMB_LEFT:     kickDirection = BOMBKICK_LEFT;  break;
                case FLOORACTION_MOVEBOMB_UP:       kickDirection = BOMBKICK_UP;    break;
                default:                                                            break;
                }

                ASSERT(kickDirection != BOMBKICK_NONE);

                StartMoving(kickDirection, -1); // -1 is: not a bomber started the move
            }
        }

        // Make the bomb move or fly if needed
        ManageMove(DeltaTime);
        ManageFlight(DeltaTime);

        // Animate
        if (m_Timer < m_AnimationTimes[0])  m_Sprite = ANIMBOMB_SPRITE2;
        else if (m_Timer < m_AnimationTimes[1])  m_Sprite = ANIMBOMB_SPRITE1;
        else if (m_Timer < m_AnimationTimes[2])  m_Sprite = ANIMBOMB_SPRITE0;
        else if (m_Timer < m_AnimationTimes[3])  m_Sprite = ANIMBOMB_SPRITE1;
        else
        {
            m_Sprite = ANIMBOMB_SPRITE2;
            m_Timer = 0.0f;
        }

        // Play animation
        m_Timer += DeltaTime;
    }

    // HACK to resolve issue with non-exploding bombs
    // would be better to resolve it in the logic though
    if (m_BeingHeld || m_BeingLifted)
    {
        if (m_pArena->GetBomber(GetOwnerPlayer()).GetBlockX() != m_BlockX ||
            m_pArena->GetBomber(GetOwnerPlayer()).GetBlockY() != m_BlockY)
        {
            m_BeingHeld = false;
            m_BeingLifted = false;
        }
    }

    // HACK to resolve issue with non-exploding bombs
    // would be better to resolve it in the logic though
    if (m_BeingPunched && m_BombFly == BOMBFLY_NONE)
    {
        m_BeingPunched = false;
    }

    if (!m_pArena->GetBomber(GetOwnerPlayer()).IsAlive())
    {

        m_Checked = true;

        m_BeingHeld = false;
        m_BeingLifted = false;
        m_BeingPunched = false;

        if (m_Remote && m_BombFly == BOMBFLY_NONE)
        {
            // Explode remotes when bomber dies
            Explode();
        }

    }

    // The bomb can only be destroyed by the arena if the
    // owner bomber knows one of its bomb is dead.
    return m_Dead && m_Checked;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::Display(void)
{
    int SpriteTable = (m_BeingHeld || m_BombFly != BOMBFLY_NONE) ? BOMB_SPRITELAYER_ABOVE_BOMBERS : BOMB_SPRITELAYER_BELOW_BOMBERS;

    // Prepare a clipping rect to pass to the draw sprite function,
    // since the sprite can be out of the arena view.
    RECT Clip;
    Clip.left = 0;              // Left of the arena view
    Clip.top = 0;               // Top of the arena view
    Clip.right = VIEW_WIDTH;    // Right of the arena view
    Clip.bottom = VIEW_HEIGHT - 26;

    // Draw the bomb sprite. Priority is not used.
    if (!m_Remote)
        m_pDisplay->DrawSprite(m_iX,
        m_iY,
        NULL,                            // Draw entire sprite
        &Clip,                           // Clip sprite to arena view !!!
        BMP_ARENA_BOMB,
        m_Sprite,
        SpriteTable,
        PRIORITY_UNUSED);
    else
        m_pDisplay->DrawSprite(m_iX,
        m_iY,
        NULL,                            // Draw entire sprite
        &Clip,                           // Clip sprite to arena view !!!
        BMP_ARENA_REMOTE_BOMB,
        m_Sprite,
        SpriteTable,
        PRIORITY_UNUSED);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CBomb::OnWriteSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteFloat(m_X);
    Snapshot.WriteFloat(m_Y);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteInteger(m_Sprite);
    Snapshot.WriteFloat(m_Timer);
    Snapshot.WriteInteger(m_BombKick);
    Snapshot.WriteBoolean(m_HasToStopMoving);
    Snapshot.WriteInteger(m_OwnerPlayer);
    Snapshot.WriteInteger(m_KickerPlayer);
    Snapshot.WriteBoolean(m_Checked);
    Snapshot.WriteBoolean(m_Dead);
    Snapshot.WriteInteger(m_FlameSize);
    Snapshot.WriteFloat(m_TimeLeft);

    for (int i = 0; i < 4; i++)
        Snapshot.WriteFloat(m_AnimationTimes[i]);

    Snapshot.WriteBoolean(m_BeingHeld);
    Snapshot.WriteBoolean(m_BeingLifted);
    Snapshot.WriteBoolean(m_BeingPunched);
    Snapshot.WriteInteger(m_BombFly);
    Snapshot.WriteFloat(m_FlightTimer);
    Snapshot.WriteInteger(m_FlightFrame);
    Snapshot.WriteInteger(m_FlightType);
    Snapshot.WriteBoolean(m_Warping);
    Snapshot.WriteBoolean(m_Remote);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CBomb::OnReadSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadFloat(&m_X);
    Snapshot.ReadFloat(&m_Y);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadInteger(&m_Sprite);
    Snapshot.ReadFloat(&m_Timer);
    Snapshot.ReadInteger((int*)&m_BombKick);
    Snapshot.ReadBoolean(&m_HasToStopMoving);
    Snapshot.ReadInteger(&m_OwnerPlayer);
    Snapshot.ReadInteger(&m_KickerPlayer);
    Snapshot.ReadBoolean(&m_Checked);
    Snapshot.ReadBoolean(&m_Dead);
    Snapshot.ReadInteger(&m_FlameSize);
    Snapshot.ReadFloat(&m_TimeLeft);

    for (int i = 0; i < 4; i++)
        Snapshot.ReadFloat(&m_AnimationTimes[i]);

    Snapshot.ReadBoolean(&m_BeingHeld);
    Snapshot.ReadBoolean(&m_BeingLifted);
    Snapshot.ReadBoolean(&m_BeingPunched);
    Snapshot.ReadInteger((int*)&m_BombFly);
    Snapshot.ReadFloat(&m_FlightTimer);
    Snapshot.ReadInteger(&m_FlightFrame);
    Snapshot.ReadInteger((int*)&m_FlightType);
    Snapshot.ReadBoolean(&m_Warping);
    Snapshot.ReadBoolean(&m_Remote);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::StartFlying(EBombFly BombFly, EBombFlightType FlightType)
{
    ASSERT(BombFly != BOMBFLY_NONE);
    ASSERT(FlightType != BOMBFLIGHTTYPE_NONE);

    m_BombFly = BombFly;
    m_FlightType = FlightType;
    m_FlightTimer = 0.0f;
    m_FlightFrame = -1;
    m_BeingPunched = false;
    m_BeingLifted = false;
    m_BeingHeld = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::ManageFlight(float DeltaTime)
{
    // If the bomb is currently flying
    if (m_BombFly != BOMBFLY_NONE)
    {
        // According to the cause of the flight
        switch (m_FlightType)
        {
            // The bomb is flying because it was thrown
        case BOMBFLIGHTTYPE_THROW:
        {
            int CurrentFlightFrame;

            // Continue flight movement
            m_FlightTimer += DeltaTime;

            // Determine flight frame number according to the timer
            if (m_FlightTimer < THROW_BASE_FRAME_TIME * 1) CurrentFlightFrame = 0;
            else if (m_FlightTimer < THROW_BASE_FRAME_TIME * 2) CurrentFlightFrame = 1;
            else if (m_FlightTimer < THROW_BASE_FRAME_TIME * 3) CurrentFlightFrame = 2;
            else if (m_FlightTimer < THROW_BASE_FRAME_TIME * 4) CurrentFlightFrame = 3;
            else if (m_FlightTimer < THROW_BASE_FRAME_TIME * 5) CurrentFlightFrame = 4;
            else if (m_FlightTimer < THROW_BASE_FRAME_TIME * 6) CurrentFlightFrame = 5;
            else
            {
                // Last frame of the flight movement!
                CurrentFlightFrame = 5;
                m_FlightTimer = 0.0f;

                // According to the flight direction, set the final block position
                // of the bomb, and determine if the bomb is warping from one side
                // of the arena to the opposite side.
                switch (m_BombFly)
                {
                case BOMBFLY_UP:
                {
                    m_BlockY -= 3;

                    if (m_BlockY < 0)
                    {
                        m_BlockY = ARENA_HEIGHT;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_DOWN:
                {
                    m_BlockY += 3;

                    if (m_BlockY >= ARENA_HEIGHT)
                    {
                        m_BlockY = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_LEFT:
                {
                    m_BlockX -= 3;

                    if (m_BlockX < 0)
                    {
                        m_BlockX = ARENA_WIDTH;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_RIGHT:
                {
                    m_BlockX += 3;

                    if (m_BlockX >= ARENA_WIDTH)
                    {
                        m_BlockX = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }
                default:
                    break;
                }

                // We've set the block position, now set the correct pixel position.
                CenterOnBlock();

                // If the bomb is not warping and there is no solid obstacle where is lands
                if (!m_Warping && !IsObstacle(m_BlockX, m_BlockY))
                {
                    // The bomb finishes its flight and lands here, crushing any item here.
                    m_BombFly = BOMBFLY_NONE;
                    m_FlightType = BOMBFLIGHTTYPE_NONE;
                    CrushItem(m_BlockX, m_BlockY);
                }
                // If the bomb is warping, it bounces anyway until it is not warping anymore.
                // If the bomb lands on a solid obstacle, it bounces.
                else
                {
                    Bounce(DeltaTime);
                }
            }

            // If the new flight frame is not the same as the last flight frame
            if (CurrentFlightFrame != m_FlightFrame)
            {
                // Then we have to modify the bomb's position by using the current frame movement data!
                // FIX ME : Is it safe? What if we miss a frame because of a big delta time? It should
                // perform all the frames between the last one and the new frame, rather than just
                // performing the current frame.

                m_FlightFrame = CurrentFlightFrame;

                m_iX += m_ThrowMoveX[m_BombFly][m_FlightFrame];
                m_iY += m_ThrowMoveY[m_BombFly][m_FlightFrame];

                m_X = (float)m_iX;
                m_Y = (float)m_iY;
            }

            break;
        }

        // The bomb is flying because it has bounced
        case BOMBFLIGHTTYPE_BOUNCE:
        {
            int CurrentFlightFrame;

            // Determine flight frame number according to the timer
            if (m_FlightTimer < BOUNCE_BASE_FRAME_TIME * 1) CurrentFlightFrame = 0;
            else if (m_FlightTimer < BOUNCE_BASE_FRAME_TIME * 2) CurrentFlightFrame = 1;
            else if (m_FlightTimer < BOUNCE_BASE_FRAME_TIME * 5) CurrentFlightFrame = 2;
            else
            {
                // Last frame of the flight movement!
                CurrentFlightFrame = 2;
                m_FlightTimer = 0.0f;

                // According to the flight direction, set the final block position
                // of the bomb, and determine if the bomb is warping from one side
                // of the arena to the opposite side.
                switch (m_BombFly)
                {
                case BOMBFLY_UP:
                {
                    m_BlockY--;

                    if (m_BlockY < 0)
                    {
                        m_BlockY = ARENA_HEIGHT;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_DOWN:
                {
                    m_BlockY++;

                    if (m_BlockY >= ARENA_HEIGHT)
                    {
                        m_BlockY = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_LEFT:
                {
                    m_BlockX--;

                    if (m_BlockX < 0)
                    {
                        m_BlockX = ARENA_WIDTH;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_RIGHT:
                {
                    m_BlockX++;

                    if (m_BlockX >= ARENA_WIDTH)
                    {
                        m_BlockX = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }
                default:
                    break;
                }

                // We've set the block position, now set the correct pixel position.
                CenterOnBlock();

                // If the bomb is not warping and there is no solid obstacle where is lands
                if (!m_Warping && !IsObstacle(m_BlockX, m_BlockY))
                {
                    // The bomb finishes its flight and lands here, crushing any item here.
                    m_BombFly = BOMBFLY_NONE;
                    m_FlightType = BOMBFLIGHTTYPE_NONE;
                    CrushItem(m_BlockX, m_BlockY);
                }
                // If the bomb is warping, it bounces anyway until it is not warping anymore.
                // If the bomb lands on a solid obstacle, it bounces.
                else
                {
                    Bounce(DeltaTime);
                }
            }

            // Continue flight movement
            m_FlightTimer += DeltaTime;

            // If the new flight frame is not the same as the last flight frame
            if (CurrentFlightFrame != m_FlightFrame)
            {
                // Then we have to modify the bomb's position by using the current frame movement data!
                // FIX ME : Is it safe? What if we miss a frame because of a big delta time? It should
                // perform all the frames between the last one and the new frame, rather than just
                // performing the current frame.

                m_FlightFrame = CurrentFlightFrame;

                m_iX += m_BounceMoveX[m_BombFly][m_FlightFrame];
                m_iY += m_BounceMoveY[m_BombFly][m_FlightFrame];

                m_X = (float)m_iX;
                m_Y = (float)m_iY;
            }

            break;
        }

        // The bomb is flying because it was punched
        case BOMBFLIGHTTYPE_PUNCH:
        {
            int CurrentFlightFrame;

            // Continue flight movement
            m_FlightTimer += DeltaTime;

            // Determine flight frame number according to the timer
            if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 1) CurrentFlightFrame = 0;
            else if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 2) CurrentFlightFrame = 1;
            else if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 3) CurrentFlightFrame = 2;
            else if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 4) CurrentFlightFrame = 3;
            else if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 5) CurrentFlightFrame = 4;
            else if (m_FlightTimer < PUNCH_BASE_FRAME_TIME * 6) CurrentFlightFrame = 5;
            else
            {
                // Last frame of the flight movement!
                CurrentFlightFrame = 5;
                m_FlightTimer = 0.0f;

                // According to the flight direction, set the final block position
                // of the bomb, and determine if the bomb is warping from one side
                // of the arena to the opposite side.
                switch (m_BombFly)
                {
                case BOMBFLY_UP:
                {
                    m_BlockY -= 3;

                    if (m_BlockY < 0)
                    {
                        m_BlockY = ARENA_HEIGHT;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_DOWN:
                {
                    m_BlockY += 3;

                    if (m_BlockY >= ARENA_HEIGHT)
                    {
                        m_BlockY = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_LEFT:
                {
                    m_BlockX -= 3;

                    if (m_BlockX < 0)
                    {
                        m_BlockX = ARENA_WIDTH;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }

                case BOMBFLY_RIGHT:
                {
                    m_BlockX += 3;

                    if (m_BlockX >= ARENA_WIDTH)
                    {
                        m_BlockX = -1;
                        m_Warping = true;
                    }
                    else
                    {
                        m_Warping = false;
                    }

                    break;
                }
                default:
                    break;
                }

                // We've set the block position, now set the correct pixel position.
                CenterOnBlock();

                // If the bomb is not warping and there is no solid obstacle where is lands
                if (!m_Warping && !IsObstacle(m_BlockX, m_BlockY))
                {
                    // The bomb finishes its flight and lands here, crushing any item here.
                    m_BombFly = BOMBFLY_NONE;
                    m_FlightType = BOMBFLIGHTTYPE_NONE;
                    CrushItem(m_BlockX, m_BlockY);
                }
                // If the bomb is warping, it bounces anyway until it is not warping anymore.
                // If the bomb lands on a solid obstacle, it bounces.
                else
                {
                    Bounce(DeltaTime);
                }
            }

            // If the new flight frame is not the same as the last flight frame
            if (CurrentFlightFrame != m_FlightFrame)
            {
                // Then we have to modify the bomb's position by using the current frame movement data!
                // FIX ME : Is it safe? What if we miss a frame because of a big delta time? It should
                // perform all the frames between the last one and the new frame, rather than just
                // performing the current frame.

                m_FlightFrame = CurrentFlightFrame;

                m_iX += m_PunchMoveX[m_BombFly][m_FlightFrame];
                m_iY += m_PunchMoveY[m_BombFly][m_FlightFrame];

                m_X = (float)m_iX;
                m_Y = (float)m_iY;
            }

            break;
        }
        default:
            break;
        }

    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::Bounce(float DeltaTime)
{
    if (!m_Warping)
    {
        if (m_pArena->IsBomber(m_BlockX, m_BlockY))
        {
            // Seek one bomber on this block and make him stunt.
            // Bomber searching is pseudo random here. Considering 
            // that DeltaTime * 100000 is a random integer value, we 
            // start searching from a pseudo random bomber index, then 
            // check the next, and the next, etc. The index warps to 
            // zero if we go beyond the max bomber index. When the index 
            // is back to its initial starting value, we've browsed all 
            // the bombers.

            // Make the pseudo random value
            int iPseudoRandom = (int)(DeltaTime * 100000.0f);

            // Make the pseudo random start index
            int StartIndex = iPseudoRandom % m_pArena->MaxBombers();
            int Index = StartIndex;

            // While we have not browsed all the bombers
            while (true)
            {
                // Just check if our indexes are valid. With these 
                // strange calculus you never know.
                assert(Index >= 0);
                assert(Index < m_pArena->MaxBombers());

                // Test existence and position
                if (m_pArena->GetBomber(Index).Exist() &&
                    m_pArena->GetBomber(Index).GetBlockX() == m_BlockX &&
                    m_pArena->GetBomber(Index).GetBlockY() == m_BlockY)
                {
                    // Make the bomber stunt
                    m_pArena->GetBomber(Index).Stunt();

                    // Only one bomber stunt even if there are several bombers on this block
                    break;
                }

                // Next bomber index.
                Index++;

                // Warp if we go beyond the max index
                if (Index >= m_pArena->MaxBombers())
                    Index = 0;

                // Stop here if we are back to the index we started with.
                if (Index == StartIndex)
                    break;
            }
        }
    }

    m_pSound->PlaySample(SAMPLE_BOMB_BOUNCE);
    m_FlightType = BOMBFLIGHTTYPE_BOUNCE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomb::CenterOnBlock(void)
{
    m_iX = m_pArena->ToPosition(m_BlockX);
    m_iY = m_pArena->ToPosition(m_BlockY);
    m_X = (float)m_iX;
    m_Y = (float)m_iY;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#ifdef _DEBUG_FLAG_1

void CBomb::_Debug_WriteToLog() {

    theLog.WriteLine("Exist=%d", Exist());
    theLog.WriteLine("Dead=%d", m_Dead);
    theLog.WriteLine("OnFloor=%d", IsOnFloor());
    theLog.WriteLine("Checked=%d", m_Checked);
    theLog.WriteLine("BlockX=%d,BlockY=%d", m_BlockX, m_BlockY);
    theLog.WriteLine("OwnerPlayer=%d", m_OwnerPlayer);
    theLog.WriteLine("TimeLeft=%f", m_TimeLeft);
    theLog.WriteLine("BombFly=%d", m_BombFly);
    theLog.WriteLine("BeingHeld=%d,BeingLifted=%d,BeingPunched=%d", m_BeingHeld, m_BeingLifted, m_BeingPunched);

}

#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
