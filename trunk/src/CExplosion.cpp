// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CExplosion.cpp
 *  \brief Explosions
 */

#include "StdAfx.h"
#include "CExplosion.h"
#include "CArena.h"
#include "CWall.h"
#include "CItem.h"
#include "CBomb.h"
#include "CBomber.h"
#include "CDisplay.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Explosions sprite layer number
#define EXPLOSION_SPRITELAYER       30

// States
#define STATE0      0
#define STATE1      1
#define STATE2      2
#define STATE3      3

// Used to speed up or slow down explosions evolution
#define ANIM_LONGER         1

// State times during the explosion evolution
#define ANIM_STATETIME1     (ANIM_LONGER * 0.060f)
#define ANIM_STATETIME2     (ANIM_LONGER * 0.130f)
#define ANIM_STATETIME3     (ANIM_LONGER * 0.210f)
#define ANIM_STATETIME4     (ANIM_LONGER * 0.280f)
#define ANIM_STATETIME5     (ANIM_LONGER * 0.340f)
#define ANIM_STATETIME6     (ANIM_LONGER * 0.400f)

// Describes a type of flame and contains the appropriate sprite number
#define FLAME_VERT          0
#define FLAME_HORIZ         4
#define FLAME_VERTUP        8
#define FLAME_HORIZLEFT     12
#define FLAME_VERTDOWN      16
#define FLAME_HORIZRIGHT    20
#define FLAME_CENTER        24

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CExplosion::CExplosion (void) : CElement()
{
    
    m_iX = -1; 
    m_iY = -1;
    m_BlockX = -1; 
    m_BlockY = -1;
    m_FlameSize = 0;
    m_State = STATE2;
    m_Timer = 0.0f;
    m_Dead = false;
    
    m_FlameSizeUp = 0;
    m_FlameSizeDown = 0;
    m_FlameSizeLeft = 0;
    m_FlameSizeRight = 0;
    m_StopUp = false;
    m_StopDown = false;
    m_StopLeft = false;
    m_StopRight = false;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CExplosion::~CExplosion (void)
{
    // Nothing to do
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CExplosion::Create (int BlockX, int BlockY, int FlameSize) 
{
    CElement::Create();

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_EXPLOSION, "New explosion [x=%02d, y=%02d, flamesize=%02d].", BlockX, BlockY, FlameSize);
#endif

    m_iX = m_pArena->ToPosition (BlockX); 
    m_iY = m_pArena->ToPosition (BlockY);
    m_BlockX = BlockX; 
    m_BlockY = BlockY;
    m_FlameSize = FlameSize;
    m_State = STATE2;
    m_Timer = 0.0f;
    m_Dead = false;
        
#ifdef EXPLOSIONS_ORIGINALSTYLE

    // In the original style, the flame sizes for each ray
    // must be set once, when the explosion is created.
    // The blocks where the flame rays stop burn once.
        
    int i;

    // Right
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockX+i < ARENA_WIDTH)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX+i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX+i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX+i,m_BlockY) ||
                m_pArena->IsExplosion(m_BlockX+i,m_BlockY)) 
            {
                // Save the maximum flame size
                m_FlameSizeRight = i;
                // Flame ray is stopped
                m_StopRight = true;
                
                // Make the block stopping the flame burn once 
                // (it won't burn on each update)
                Burn (m_BlockX+i, m_BlockY, BURNDIRECTION_RIGHT);
                // Stop making that ray of flames
                break;
            }
        }
        // Not within the arena
        else 
        {
            // Save the maximum flame size
            m_FlameSizeRight = i;
            // Flame ray is stopped
            m_StopRight = true;
            // Stop making that ray of flames
            break;
        }

        // If there was no obstacle to the flame
        if (i == m_FlameSize)
        {
            // Save the maximum flame size
            m_FlameSizeRight = i;
            // Flame ray is not stopped
            m_StopRight = false;
        }
    }

    // Left
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockX-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX-i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX-i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX-i,m_BlockY) ||
                m_pArena->IsExplosion(m_BlockX-i,m_BlockY)) 
            {
                // Save the maximum flame size
                m_FlameSizeLeft = i;
                // Flame ray is stopped
                m_StopLeft = true;
                // Make the block stopping the flame burn once 
                // (it won't burn on each update)
                Burn (m_BlockX-i, m_BlockY, BURNDIRECTION_LEFT);
                // Stop making that ray of flames
                break;
            }
        }
        // Not within the arena
        else
        {
            // Save the maximum flame size
            m_FlameSizeLeft = i;
            // Flame ray is stopped
            m_StopLeft = true;
            // Stop making that ray of flames
            break;
        }

        // If there was no obstacle to the flame
        if (i == m_FlameSize)
        {
            // Save the maximum flame size
            m_FlameSizeLeft = i;
            // Flame ray is not stopped
            m_StopLeft = false;
        }
    }

    // Up
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockY-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY-i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY-i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY-i) ||
                m_pArena->IsExplosion(m_BlockX,m_BlockY-i)) 
            {
                // Save the maximum flame size
                m_FlameSizeUp = i;
                // Flame ray is stopped
                m_StopUp = true;
                // Make the block stopping the flame burn once 
                // (it won't burn on each update)
                Burn (m_BlockX, m_BlockY-i, BURNDIRECTION_UP);
                // Stop making that ray of flames
                break;
            }
        }
        else
        {
            // Save the maximum flame size
            m_FlameSizeUp = i;
            // Flame ray is stopped
            m_StopUp = true;
            // Stop making that ray of flames
            break;
        }

        // If there was no obstacle to the flame
        if (i == m_FlameSize)
        {
            // Save the maximum flame size
            m_FlameSizeUp = i;
            // Flame ray is not stopped
            m_StopUp = false;
        }
    }
                
    // Down
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockY+i < ARENA_HEIGHT)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY+i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY+i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY+i) ||
                m_pArena->IsExplosion(m_BlockX,m_BlockY+i)) 
            {
                // Save the maximum flame size
                m_FlameSizeDown = i;
                // Flame ray is stopped
                m_StopDown = true;
                // Make the block stopping the flame burn once 
                // (it won't burn on each update)
                Burn (m_BlockX, m_BlockY+i, BURNDIRECTION_DOWN);
                // Stop making that ray of flames
                break;
            }
        }
        else
        {
            // Save the maximum flame size
            m_FlameSizeDown = i;
            // Flame ray is stopped
            m_StopDown = true;
            // Stop making that ray of flames
            break;
        }

        // If there was no obstacle to the flame
        if (i == m_FlameSize)
        {
            // Save the maximum flame size
            m_FlameSizeDown = i;
            // Flame ray is not stopped
            m_StopDown = false;
        }
    }

#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CExplosion::Destroy (void)
{
    m_Flames.clear();
    CElement::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Record a flame and display it if possible

void CExplosion::PutFlame (int BlockX, int BlockY, int FlameType)
{
    // Prepare a new flame
    SFlame Flame;
    Flame.BlockX = BlockX;
    Flame.BlockY = BlockY;
    Flame.FlameType = FlameType;

    // Store it
    m_Flames.push_back(Flame);
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Make the block at (X,Y) burn : burn walls,
// items, bombers, make bombs explode.

void CExplosion::Burn (int BlockX, int BlockY, EBurnDirection BurnDirection)
{
    int Index;

    // Check coordinates
    ASSERT (BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT (BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // If there is a wall at BlockX,BlockY
    if (m_pArena->IsWall(BlockX,BlockY))
    {
        // Seek this wall
        for (Index = 0 ; Index < m_pArena->MaxWalls(); Index++)
        {
            // Test existence and position
            if (m_pArena->GetWall(Index).Exist() &&
                m_pArena->GetWall(Index).GetBlockX() == BlockX && 
                m_pArena->GetWall(Index).GetBlockY() == BlockY)
            {
                // Make the wall burn
                m_pArena->GetWall(Index).Burn ();
                break;
            }
        }
    }
    // If there is no wall then maybe there is an unprotected item to burn
    else if (m_pArena->IsItem(BlockX,BlockY))
    {
        // Seek this item
        for (Index = 0 ; Index < m_pArena->MaxItems(); Index++)
        {
            // Test existence and position
            if (m_pArena->GetItem(Index).Exist() &&
                m_pArena->GetItem(Index).GetBlockX() == BlockX && 
                m_pArena->GetItem(Index).GetBlockY() == BlockY)
            {
                // Make the item burn
                m_pArena->GetItem(Index).Burn (BurnDirection);
                break;
            }
        }
    }
    
    // If there is a bomb at BlockX,BlockY
    if (m_pArena->IsBomb(BlockX,BlockY))
    {
        // Seek this bomb
        for (Index = 0 ; Index < m_pArena->MaxBombs(); Index++)
        {
            // Test existence and position
            if (m_pArena->GetBomb(Index).Exist() &&
                m_pArena->GetBomb(Index).GetBlockX() == BlockX && 
                m_pArena->GetBomb(Index).GetBlockY() == BlockY)
            {
                // Make the bomb explode soon
                m_pArena->GetBomb(Index).Burn();
                break;
            }
        }
    }

    // If there is an alive bomber at BlockX,BlockY
    if (m_pArena->IsAliveBomber(BlockX,BlockY))
    {
        // Make the bombers die
        for (Index = 0 ; Index < m_pArena->MaxBombers() ; Index++)
        {
            // Test existence and position
            if (m_pArena->GetBomber(Index).Exist() &&
                m_pArena->GetBomber(Index).GetBlockX() == BlockX && 
                m_pArena->GetBomber(Index).GetBlockY() == BlockY)
            {
                // Make the bomber burn
                m_pArena->GetBomber(Index).Burn ();
                // No break in order to burn every bomber on this block
            }
        }
    }
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// The update of an explosion burns every block where
// flames will appear in the arena. Each ray of flame
// is made separately. It also updates the state of
// the explosion (flame animation).

bool CExplosion::Update (float DeltaTime)
{
    int i;

    // Remove all flames
    m_Flames.clear();
            
    // If there is no wall or a falling wall at the center of the explosion
    if (!m_pArena->IsWall(m_BlockX,m_BlockY) ||
        m_pArena->IsFallingWall(m_BlockX,m_BlockY))
    {
        // Burn at the center of the explosion
        Burn (m_BlockX, m_BlockY, BURNDIRECTION_NONE);
    
        // Put a center flame
        PutFlame (m_BlockX, m_BlockY, FLAME_CENTER);
    }
            
    // Rays of flames : Right Left Up Down

#ifdef EXPLOSIONS_ORIGINALSTYLE

    // Right
    for (i = 1 ; i <= (m_StopRight ? m_FlameSizeRight - 1 : m_FlameSizeRight) ; i++) // Don't burn where the flame ray stopped
    {
        // If it is within the arena
        if (m_BlockX+i < ARENA_WIDTH)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX+i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX+i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX+i,m_BlockY))
            {
                // Then burn this block
                Burn (m_BlockX+i, m_BlockY, BURNDIRECTION_RIGHT);
                // Reset flame size
                m_FlameSizeRight = i;
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX+i,m_BlockY)) 
            {
                // Reset flame size
                m_FlameSizeRight = i;
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX+i, m_BlockY, BURNDIRECTION_RIGHT);

                // If this is not the last flame to manage
                // Or if it is the last flame to manage, if the explosion was not stopped in this direction
                if (i != m_FlameSizeRight || !m_StopRight)
                {
                    // Put a flame with a tip or not
                    PutFlame (m_BlockX+i, m_BlockY, (i != m_FlameSize ? FLAME_HORIZ : FLAME_HORIZRIGHT));
                }
            }
        }
        // Not within the arena, so stop
        else break;
    }

    // Left
    for (i = 1 ; i <= (m_StopLeft ? m_FlameSizeLeft - 1 : m_FlameSizeLeft) ; i++) // Don't burn where the flame ray stopped
    {
        // If it is within the arena
        if (m_BlockX-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX-i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX-i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX-i,m_BlockY))
            {
                // Then burn this block
                Burn (m_BlockX-i, m_BlockY, BURNDIRECTION_LEFT);
                // Reset flame size
                m_FlameSizeLeft = i;
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX-i,m_BlockY)) 
            {
                // Reset flame size
                m_FlameSizeLeft = i;
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX-i, m_BlockY, BURNDIRECTION_LEFT);

                // If this is not the last flame to manage
                // Or if it is the last flame to manage, if the explosion was not stopped in this direction
                if (i != m_FlameSizeLeft || !m_StopLeft)
                {
                    // Put a flame with a tip or not
                    PutFlame (m_BlockX-i, m_BlockY, (i != m_FlameSize ? FLAME_HORIZ : FLAME_HORIZLEFT));
                }
            }
        }
        else break;
    }

    // Up
    for (i = 1 ; i <= (m_StopUp ? m_FlameSizeUp - 1 : m_FlameSizeUp) ; i++) // Don't burn where the flame ray stopped
    {
        // If it is within the arena
        if (m_BlockY-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY-i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY-i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY-i))
            {
                // Then burn this block
                Burn (m_BlockX, m_BlockY-i, BURNDIRECTION_UP);
                // Reset flame size
                m_FlameSizeUp = i;
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX,m_BlockY-i)) 
            {
                // Reset flame size
                m_FlameSizeUp = i;
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX, m_BlockY-i, BURNDIRECTION_UP);
                
                // If this is not the last flame to manage
                // Or if it is the last flame to manage, if the explosion was not stopped in this direction
                if (i != m_FlameSizeUp || !m_StopUp)
                {
                    // Put a flame with a tip or not
                    PutFlame (m_BlockX, m_BlockY-i, (i != m_FlameSize ? FLAME_VERT : FLAME_VERTUP));
                }
            }
        }
        else break;
    }

    // Down
    for (i = 1 ; i <= (m_StopDown ? m_FlameSizeDown - 1 : m_FlameSizeDown) ; i++) // Don't burn where the flame ray stopped
    {
        // If it is within the arena
        if (m_BlockY+i < ARENA_HEIGHT)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY+i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY+i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY+i))
            {
                // Then burn this block
                Burn (m_BlockX, m_BlockY+i, BURNDIRECTION_DOWN);
                // Reset flame size
                m_FlameSizeDown = i;
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX,m_BlockY+i)) 
            {
                // Reset flame size
                m_FlameSizeDown = i;
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX, m_BlockY+i, BURNDIRECTION_DOWN);

                // If this is not the last flame to manage
                // Or if it is the last flame to manage, if the explosion was not stopped in this direction
                if (i != m_FlameSizeDown || !m_StopDown)
                {
                    // Put a flame with a tip or not
                    PutFlame (m_BlockX, m_BlockY+i, (i != m_FlameSize ? FLAME_VERT : FLAME_VERTDOWN));
                }
            }
        }
        else break;
    }

#else   // New style

    // Right
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockX+i < ARENA_WIDTH)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX+i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX+i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX+i,m_BlockY))
            {
                // Then burn this block
                Burn (m_BlockX+i, m_BlockY, BURNDIRECTION_RIGHT);
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX+i,m_BlockY)) 
            {
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX+i, m_BlockY, BURNDIRECTION_RIGHT);
                
                // Put a flame with a tip or not
                PutFlame (m_BlockX+i, m_BlockY, (i != m_FlameSize ? FLAME_HORIZ : FLAME_HORIZRIGHT));
            }
        }
        // Not within the arena, so stop
        else break;
    }

    // Left
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockX-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX-i,m_BlockY) || 
                m_pArena->IsBomb(m_BlockX-i,m_BlockY) || 
                m_pArena->IsItem(m_BlockX-i,m_BlockY))
            {
                // Then burn this block
                Burn (m_BlockX-i, m_BlockY, BURNDIRECTION_LEFT);
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX-i,m_BlockY)) 
            {
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX-i, m_BlockY, BURNDIRECTION_LEFT);
                
                // Put a flame with a tip or not
                PutFlame (m_BlockX-i, m_BlockY, (i != m_FlameSize ? FLAME_HORIZ : FLAME_HORIZLEFT));
            }
        }
        else break;
    }

    // Up
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockY-i >= 0)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY-i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY-i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY-i))
            {
                // Then burn this block
                Burn (m_BlockX, m_BlockY-i, BURNDIRECTION_UP);
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX,m_BlockY-i)) 
            {
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX, m_BlockY-i, BURNDIRECTION_UP);
                
                // Put a flame with a tip or not
                PutFlame (m_BlockX, m_BlockY-i, (i != m_FlameSize ? FLAME_VERT : FLAME_VERTUP));
            }
        }
        else break;
    }

    // Down
    for (i = 1 ; i <= m_FlameSize ; i++)
    {
        // If it is within the arena
        if (m_BlockY+i < ARENA_HEIGHT)
        {
            // If there is a wall or a bomb or an item
            if (m_pArena->IsWall(m_BlockX,m_BlockY+i) || 
                m_pArena->IsBomb(m_BlockX,m_BlockY+i) || 
                m_pArena->IsItem(m_BlockX,m_BlockY+i))
            {
                // Then burn this block
                Burn (m_BlockX, m_BlockY+i, BURNDIRECTION_DOWN);
                // And stop making that ray of flames
                break;
            }
            // If there is a center of explosion here
            else if (m_pArena->IsExplosion(m_BlockX,m_BlockY+i)) 
            {
                // Stop making that ray of flames
                break;
            }
            // If there is nothing
            else 
            {
                // Burn this block
                Burn (m_BlockX, m_BlockY+i, BURNDIRECTION_DOWN);
                
                // Put a flame with a tip or not
                PutFlame (m_BlockX, m_BlockY+i, (i != m_FlameSize ? FLAME_VERT : FLAME_VERTDOWN));
            }
        }
        else break;
    }

#endif // EXPLOSIONS_ORIGINALSTYLE

    // Animate (make the state evolve)
    if (m_Timer < ANIM_STATETIME1)      m_State = STATE2;
    else if (m_Timer < ANIM_STATETIME2) m_State = STATE1;
    else if (m_Timer < ANIM_STATETIME3) m_State = STATE0;
    else if (m_Timer < ANIM_STATETIME4) m_State = STATE1;
    else if (m_Timer < ANIM_STATETIME5) m_State = STATE2;
    else if (m_Timer < ANIM_STATETIME6) m_State = STATE3;
    else 
    {
        m_Dead = true;
    }

    // Play animation
    m_Timer += DeltaTime;

    return m_Dead;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CExplosion::Display (void)
{
    // Draw the flames
    for (unsigned int i = 0 ; i < m_Flames.size() ; i++)
    {
        // To find the right sprite for a flame, the int value
        // of the flame type is added to the current state of the
        // explosion : FlameSprite = FlameType + ExplosionState 
        m_pDisplay->DrawSprite (m_pArena->ToPosition (m_Flames[i].BlockX),   // Position X 
                                m_pArena->ToPosition (m_Flames[i].BlockY),   // Position Y
                                NULL,                                        // Draw entire sprite
                                NULL,                                        // No need to clip
                                BMP_ARENA_FLAME,                             // Sprite table number
                                m_Flames[i].FlameType + m_State,             // Sprite number
                                EXPLOSION_SPRITELAYER,                       // Sprite layer number
                                PRIORITY_UNUSED);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CExplosion::OnWriteSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteInteger(m_State);
    Snapshot.WriteFloat(m_Timer);
    Snapshot.WriteInteger(m_FlameSize);
    
    Snapshot.WriteInteger(m_Flames.size());

    for (unsigned int i = 0 ; i < m_Flames.size() ; i++)
    {
        Snapshot.WriteInteger(m_Flames[i].BlockX);
        Snapshot.WriteInteger(m_Flames[i].BlockY);
        Snapshot.WriteInteger(m_Flames[i].FlameType);
    }

    Snapshot.WriteBoolean(m_Dead);

#ifdef EXPLOSIONS_ORIGINALSTYLE         

    Snapshot.WriteInteger(m_FlameSizeUp);
    Snapshot.WriteInteger(m_FlameSizeDown);
    Snapshot.WriteInteger(m_FlameSizeLeft);
    Snapshot.WriteInteger(m_FlameSizeRight);
    Snapshot.WriteBoolean(m_StopUp);
    Snapshot.WriteBoolean(m_StopDown);
    Snapshot.WriteBoolean(m_StopLeft);
    Snapshot.WriteBoolean(m_StopRight);
                                        
#endif // EXPLOSIONS_ORIGINALSTYLE
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CExplosion::OnReadSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadInteger(&m_State);
    Snapshot.ReadFloat(&m_Timer);
    Snapshot.ReadInteger(&m_FlameSize);
    
    SFlame Flame;
    int NumberOfFlames;

    Snapshot.ReadInteger(&NumberOfFlames);
    m_Flames.clear();

    for (int i = 0 ; i < NumberOfFlames ; i++)
    {
        Snapshot.ReadInteger(&Flame.BlockX);
        Snapshot.ReadInteger(&Flame.BlockY);
        Snapshot.ReadInteger(&Flame.FlameType);

        m_Flames.push_back(Flame);
    }

    Snapshot.ReadBoolean(&m_Dead);

#ifdef EXPLOSIONS_ORIGINALSTYLE         

    Snapshot.ReadInteger(&m_FlameSizeUp);
    Snapshot.ReadInteger(&m_FlameSizeDown);
    Snapshot.ReadInteger(&m_FlameSizeLeft);
    Snapshot.ReadInteger(&m_FlameSizeRight);
    Snapshot.ReadBoolean(&m_StopUp);
    Snapshot.ReadBoolean(&m_StopDown);
    Snapshot.ReadBoolean(&m_StopLeft);
    Snapshot.ReadBoolean(&m_StopRight);
                                        
#endif // EXPLOSIONS_ORIGINALSTYLE
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
