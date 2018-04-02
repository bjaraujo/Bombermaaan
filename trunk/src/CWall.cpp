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
 *  \file CWall.cpp
 *  \brief Wall element of the arena
 */

#include "StdAfx.h"
#include "CWall.h"
#include "CBomber.h"
#include "CArena.h"
#include "CBomb.h"
#include "CDisplay.h"
#include "CItem.h"
#include "CSound.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Wall sprites
#define SPRITE_WALLHARD       0
#define SPRITE_WALLSOFT       1

// Burning wall animation sprites
#define ANIM_BURNING1       2
#define ANIM_BURNING2       3
#define ANIM_BURNING3       4
#define ANIM_BURNING4       5
#define ANIM_BURNING5       6

// Burning wall animation times
#define ANIMBURNING_TIME1       0.120f
#define ANIMBURNING_TIME2       0.230f
#define ANIMBURNING_TIME3       0.330f
#define ANIMBURNING_TIME4       0.420f
#define ANIMBURNING_TIME5       0.500f

// Wall sprite layer
#define WALL_SPRITELAYER        20

// Falling wall sprite layer (this is the sprite layer for flying objects, bombers and item fires)
#define FLY_SPRITELAYER         50

// Falling wall animation sprites
#define ANIM_FALLING1           0
#define ANIM_FALLING2           1
#define ANIM_FALLING3           2
#define ANIM_FALLING4           1

// Falling wall animation times
#define ANIMFALLING_TIME1       0.050f
#define ANIMFALLING_TIME2       (ANIMFALLING_TIME1 * 2)
#define ANIMFALLING_TIME3       (ANIMFALLING_TIME1 * 3)
#define ANIMFALLING_TIME4       (ANIMFALLING_TIME1 * 4)

// Shadow sprite for falling wall
#define FALLING_SHADOW          3

// Falling speed in pixels per second
#define FALLING_SPEED           500

// Priority in layer to use when drawing wall sprites
#define WALL_PRIORITY           0

// The shadow must be on the top of any soft/hard wall, so the 
// priority in the wall sprite layer must be higher than WALL_PRIORITY.
#define FLYSHADOW_PRIORITY      1

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CWall::CWall (void) : CElement()
{
    
    m_iX = 0;
    m_iY = 0;

    m_fX = 0.0f;
    m_fY = 0.0f;

    m_BlockX = 0;
    m_BlockY = 0;

    m_Timer = 0.0f;

    m_Burning = false;
    m_Dead = false;

    m_Type = WALL_HARD;

    m_Sprite = 0;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CWall::~CWall (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWall::Create (int BlockX, int BlockY, EWallType Type)
{
    CElement::Create();

    switch (Type)
    {
        case WALL_HARD :   
        {
            // Never changing sprite
            m_Sprite = SPRITE_WALLHARD; 
            break;
        }

        case WALL_SOFT :  
        {
            // Sprite that won't change until the wall burns
            m_Sprite = SPRITE_WALLSOFT; 
            break;
        }

        case WALL_FALLING :
        {
            // First sprite of the animation
            m_Sprite = ANIM_FALLING1;

            // The floating position in arena is used for falling walls.
            // It will follow the move of the falling wall from top to bottom.
            // The integer position will be used to save the position of the shadow sprite.
            m_fX = float (m_pArena->ToPosition (BlockX));
            m_fY = float (m_pArena->ToPosition (BlockY) - BLOCK_SIZE * ARENA_HEIGHT);
            break;
        }
    }

    m_iX = m_pArena->ToPosition (BlockX); 
    m_iY = m_pArena->ToPosition (BlockY);
    m_BlockX = BlockX; 
    m_BlockY = BlockY;
    m_Type = Type;
    m_Burning = false;
    m_Timer = 0.0f;
    m_Dead = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWall::Destroy (void)
{
    CElement::Destroy();
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called by explosions touching this wall

void CWall::Burn ()
{
    // Flames can only burn a soft wall
    if (m_Type == WALL_SOFT)
        m_Burning = true;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called by falling walls crushing this wall

void CWall::Crush ()
{
    // Die at next update
    m_Dead = true;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CWall::Update (float DeltaTime)
{
    // For hard and falling walls, crush the bombers and
    // make the bombs explode if they touch the wall
    if (m_Type == WALL_HARD || m_Type == WALL_FALLING)
    {       
        // Crush the bombers
        // If there is at least one alive bomber here
        if (m_pArena->IsAliveBomber(m_BlockX,m_BlockY))
        {
            // Seek the alive bombers
            for (int Index = 0 ; Index < m_pArena->MaxBombers() ; Index++)
            {
                // Test existence and position and dead state
                if (m_pArena->GetBomber(Index).Exist() &&
                    m_pArena->GetBomber(Index).GetBlockX() == m_BlockX && 
                    m_pArena->GetBomber(Index).GetBlockY() == m_BlockY &&
                    m_pArena->GetBomber(Index).IsAlive())
                {
                    // Crush the bomber
                    m_pArena->GetBomber(Index).Crush ();
                }
            }
        }

        // Make the bombs explode
        // If there is a bomb here
        if (m_pArena->IsBomb(m_BlockX,m_BlockY))
        {
            // Seek this bomb
            for (int Index = 0 ; Index < m_pArena->MaxBombs() ; Index++)
            {
                // Test existence, state and position
                if (m_pArena->GetBomb(Index).Exist() &&
                    m_pArena->GetBomb(Index).IsOnFloor() &&
                    m_pArena->GetBomb(Index).GetBlockX() == m_BlockX && 
                    m_pArena->GetBomb(Index).GetBlockY() == m_BlockY)
                {
                    // Crush the bomb
                    m_pArena->GetBomb(Index).Crush ();
                    break;
                }
            }
        }
    }

    // If it's a hard wall
    if (m_Type == WALL_HARD)
    {
        // Crush the floor under the wall
        // If there is a floor
        if (m_pArena->IsFloor(m_BlockX,m_BlockY))
        {
            // Seek this floor
            for (int Index = 0 ; Index < m_pArena->MaxFloors() ; Index++)
            {
                // Test existence and position
                if (m_pArena->GetFloor(Index).Exist() &&
                    m_pArena->GetFloor(Index).GetBlockX() == m_BlockX && 
                    m_pArena->GetFloor(Index).GetBlockY() == m_BlockY)
                {
                    // Crush the floor
                    m_pArena->GetFloor(Index).Crush ();
                    break;
                }
            }
        }
    }
    // If it's a soft wall
    else if (m_Type == WALL_SOFT)
    {
        // If burning, play the burning animation (non constant frametime)
        if (m_Burning)
        {
                 if (m_Timer < ANIMBURNING_TIME1)   m_Sprite = ANIM_BURNING1;
            else if (m_Timer < ANIMBURNING_TIME2)   m_Sprite = ANIM_BURNING2;
            else if (m_Timer < ANIMBURNING_TIME3)   m_Sprite = ANIM_BURNING3;
            else if (m_Timer < ANIMBURNING_TIME4)   m_Sprite = ANIM_BURNING4;
            else if (m_Timer < ANIMBURNING_TIME5)   m_Sprite = ANIM_BURNING5;
            else 
            { 
                // Dead
                m_Dead = true;
            }

            // Play animation
            m_Timer += DeltaTime;
        }
    }
    // If it's a falling wall
    else if (m_Type == WALL_FALLING)    
    {
        // Animate
             if (m_Timer < ANIMFALLING_TIME1)   m_Sprite = ANIM_FALLING1;
        else if (m_Timer < ANIMFALLING_TIME2)   m_Sprite = ANIM_FALLING2;
        else if (m_Timer < ANIMFALLING_TIME3)   m_Sprite = ANIM_FALLING3;
        else if (m_Timer < ANIMFALLING_TIME4)   m_Sprite = ANIM_FALLING4;
        else
        {
            // Loop animation (hoping setting timer to 0
            // won't make the animation look weird)
            m_Timer = 0.0f;
        }

        // Play animation
        m_Timer += DeltaTime;
                
        // Make the falling wall move
        m_fY += DeltaTime * FALLING_SPEED;

        // If the wall has landed
        if (m_fY >= float(m_iY))    // Flying object Y position >= Shadow Y position ?
        {
            int Index;

            // Play a random sound (clap!)
            m_pSound->PlaySample ((RANDOM(100) >= 50 ? SAMPLE_WALL_CLAP_1 : SAMPLE_WALL_CLAP_2));

            // If there is a non falling wall at (BlockX,BlockY), crush it
            if (m_pArena->IsWall(m_BlockX,m_BlockY))
            {
                // Seek this wall
                for (Index = 0 ; Index < m_pArena->MaxWalls() ; Index++)
                {
                    // Test position
                    if (m_pArena->GetWall(Index).Exist() &&
                        &m_pArena->GetWall(Index) != this &&    // Not this one! **** perhaps the type != falling should be
                        m_pArena->GetWall(Index).GetBlockX() == m_BlockX &&   // **** tested instead of this obscure ptr thing
                        m_pArena->GetWall(Index).GetBlockY() == m_BlockY)
                    {
                        // Crush the wall
                        m_pArena->GetWall(Index).Crush ();
                        break;
                    }
                }
            }

            // Now the same for items. Crush any item at (BlockX, BlockY)
            // If there is an item here
            if (m_pArena->IsItem(m_BlockX,m_BlockY))
            {
                // Seek this item
                for (Index = 0 ; Index < m_pArena->MaxItems() ; Index++)
                {
                    // Test existence and position
                    if (m_pArena->GetItem(Index).Exist() &&
                        m_pArena->GetItem(Index).GetBlockX() == m_BlockX &&
                        m_pArena->GetItem(Index).GetBlockY() == m_BlockY)
                    {
                        // Crush the item
                        m_pArena->GetItem(Index).Crush ();
                        break;
                    }
                }
            }

            // Type transformation. The falling wall becomes a hard wall.
            // Prepare for a brand new life.
            m_Type = WALL_HARD;
            m_Timer = 0.0f;
            m_Sprite = SPRITE_WALLHARD; 
        }
    }

    return m_Dead;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWall::Display (void)
{
    // If it's a hard wall
    if (m_Type == WALL_HARD)
    {
        // Add the sprite in the right layer.
        m_pDisplay->DrawSprite (m_iX,                            // Top left corner of the wall's block
                                m_iY, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_ARENA_WALL,
                                m_Sprite, 
                                WALL_SPRITELAYER, 
                                WALL_PRIORITY);
    }
    // If it's a soft wall
    else if (m_Type == WALL_SOFT)
    {
        // Add the sprite in the right layer.
        m_pDisplay->DrawSprite (m_iX, 
                                m_iY, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_ARENA_WALL,
                                m_Sprite, 
                                WALL_SPRITELAYER, 
                                WALL_PRIORITY);
    }
    // If it's a falling wall
    else if (m_Type == WALL_FALLING)    
    {
        // Prepare a clipping rect to pass to the draw sprite function,
        // since the sprite can be out of the arena view.
        RECT Clip;
        Clip.left = 0;              // Left of the arena view
        Clip.top = 0;               // Top of the arena view
        Clip.right = VIEW_WIDTH;    // Right of the arena view
        Clip.bottom = VIEW_HEIGHT - 26;

        // Put the falling wall sprite. It's a flying object, put it in the appropriate layer.
        // The priority in layer depends on the Y position of the block where the wall lands.
        m_pDisplay->DrawSprite (int(m_fX), 
                                int(m_fY), 
                                NULL,                            // Draw entire sprite
                                &Clip,                           // Clip sprite to arena view!!!
                                BMP_ARENA_FLY,
                                m_Sprite, 
                                FLY_SPRITELAYER, 
                                m_iY);

        // Put the shadow sprite.
        m_pDisplay->DrawSprite (m_iX, 
                                m_iY, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_ARENA_FLY,
                                FALLING_SHADOW, 
                                WALL_SPRITELAYER, 
                                FLYSHADOW_PRIORITY);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWall::OnWriteSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteFloat(m_fX);
    Snapshot.WriteFloat(m_fY);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteInteger(m_Sprite);
    Snapshot.WriteFloat(m_Timer);
    Snapshot.WriteBoolean(m_Dead);
    Snapshot.WriteBoolean(m_Burning);
    Snapshot.WriteInteger(m_Type);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CWall::OnReadSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadFloat(&m_fX);
    Snapshot.ReadFloat(&m_fY);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadInteger(&m_Sprite);
    Snapshot.ReadFloat(&m_Timer);
    Snapshot.ReadBoolean(&m_Dead);
    Snapshot.ReadBoolean(&m_Burning);
    Snapshot.ReadInteger((int*)&m_Type);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
