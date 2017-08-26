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
 *  \file CAiArena.cpp
 *  \brief The AI arena
 */

#include <array>

#include "StdAfx.h"
#include "CAiArena.h"

#define AIARENADEBUG_SPRITELAYER        70

// if you want to visualise softwall blocks, uncomment:
//#define DEBUG_DRAW_SOFTWALL_BLOCKS

// if you want to visualise danger and "wall burn" blocks, uncomment:
//#define DEBUG_DRAW_BURNWALLDANGER_BLOCKS

// if you want to visualise the owner of a bomb, uncomment
//#define DEBUG_DRAW_BOMB_OWNERS

#if defined(DEBUG_DRAW_SOFTWALL_BLOCKS) || defined (DEBUG_DRAW_BURNWALLDANGER_BLOCKS) || defined (DEBUG_DRAW_BOMB_OWNERS)
#include "CFont.h"

static CFont m_Font;
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CAiArena::CAiArena (void)
{
    m_pArena = NULL;
    m_pDisplay = NULL;
        
    for (int i = 0; i < MAX_DEAD_END; i++)
    {
        m_DeadEndExit[i].BlockX = -1;
        m_DeadEndExit[i].BlockY = -1;
    }
    
    for (int BlockX = 0; BlockX < ARENA_WIDTH; BlockX++)
    {
        for (int BlockY = 0; BlockY < ARENA_HEIGHT; BlockY++)
        {
            m_Danger[BlockX][BlockY] = DANGER_NONE;
            m_DangerTimeLeft[BlockX][BlockY] = 0.0f;
            m_DeadEnd[BlockX][BlockY] = -1;
            m_SoftWallNear[BlockX][BlockY] = 0;
            m_WallBurn[BlockX][BlockY] = false;
        }
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CAiArena::~CAiArena (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAiArena::Create (void)
{
    ASSERT (m_pArena != NULL);

#if defined(DEBUG_DRAW_SOFTWALL_BLOCKS) || defined (DEBUG_DRAW_BURNWALLDANGER_BLOCKS) || defined (DEBUG_DRAW_BOMB_OWNERS)
    m_Font.SetDisplay(m_pDisplay);
                    
    m_Font.Create ();
    m_Font.SetShadow (true);
    m_Font.SetShadowColor (FONTCOLOR_BLACK);
    m_Font.SetShadowDirection (SHADOWDIRECTION_DOWNRIGHT);
    m_Font.SetSpriteLayer (800);
    m_Font.SetTextColor(FONTCOLOR_WHITE);
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAiArena::Destroy (void)
{
#if defined(DEBUG_DRAW_SOFTWALL_BLOCKS) || defined (DEBUG_DRAW_BURNWALLDANGER_BLOCKS) || defined (DEBUG_DRAW_BOMB_OWNERS)
    if (m_pDisplay != NULL)
        m_pDisplay->RemoveAllDebugRectangles();
#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAiArena::Update (float DeltaTime)
{
    // Block coordinates used in many places
    int BlockX;
    int BlockY;

    // Auxiliary variables for code readability : is there a wall around the current block?
    bool IsWallHere;
    bool IsWallUp;
    bool IsWallDown;
    bool IsWallLeft;
    bool IsWallRight;
    
    // Current dead end number. Incremented each time there is a new dead end.
    int CurrentDeadEnd = 0;

    //*************
    // DEAD END
    //*************

    // Scan each block of the dead end array
    for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
    {
        for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
        {
            // Set undefined dead end (is there one? we don't know yet)
            m_DeadEnd[BlockX][BlockY] = -2;
        }
    }

    // Scan each block of the arena
    for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
    {
        for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
        {
            // If the dead end on this block is currently undefined
            if (m_DeadEnd[BlockX][BlockY] == -2)
            {
                // If the block is on the edges of the arena grid
                if (BlockX == 0 || BlockX == ARENA_WIDTH - 1 ||
                    BlockY == 0 || BlockY == ARENA_HEIGHT - 1)
                {
                    // There is definitely no dead end here
                    m_DeadEnd[BlockX][BlockY] = -1;
                }
                // If the block is not on the edges of the arena grid
                else
                {
                    // Is there a wall on this block?
                    IsWallHere = m_pArena->IsWall (BlockX, BlockY);
                    
                    // If there is no wall on this block
                    if (!IsWallHere)
                    {
                        // Is there a wall around this block?
                        IsWallUp    = m_pArena->IsWall (BlockX, BlockY - 1);
                        IsWallDown  = m_pArena->IsWall (BlockX, BlockY + 1);
                        IsWallLeft  = m_pArena->IsWall (BlockX - 1, BlockY);
                        IsWallRight = m_pArena->IsWall (BlockX + 1, BlockY);

                        // If this block is the back of a dead end ("[")
                        if (IsWallLeft && IsWallUp && IsWallDown)
                        {
                            // Start scanning the dead end on this block
                            int DeadEndBlockX = BlockX;
                            int DeadEndBlockY = BlockY;

                            // While we are still in this dead end and there is no wall blocking the way
                            while (IsWallUp && IsWallDown && !IsWallHere)
                            {
                                // Set the dead end number of the current block : it's the current dead end number
                                m_DeadEnd[DeadEndBlockX][DeadEndBlockY] = CurrentDeadEnd;

                                // Continue scanning (go right)
                                DeadEndBlockX++;

                                // Update the auxiliary variables value.
                                IsWallHere = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY);
                                IsWallUp   = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY - 1);
                                IsWallDown = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY + 1);
                            }

                            // If there is a no wall blocking the way and however the dead end was entirely scanned
                            if (!IsWallHere)
                            {
                                // Then this dead end has an exit. Record the block coordinates of this exit.
                                m_DeadEndExit[CurrentDeadEnd].BlockX = DeadEndBlockX;
                                m_DeadEndExit[CurrentDeadEnd].BlockY = DeadEndBlockY;
                            }

                            // Next dead end number
                            CurrentDeadEnd++;
                        }
                        // If this block is the back of a dead end ("Ú¿")
                        else if (IsWallUp && IsWallLeft && IsWallRight)
                        {
                            // Start scanning the dead end on this block
                            int DeadEndBlockX = BlockX;
                            int DeadEndBlockY = BlockY;

                            // While we are still in this dead end and there is no wall blocking the way
                            while (IsWallLeft && IsWallRight && !IsWallHere)
                            {
                                // Set the dead end number of the current block : it's the current dead end number
                                m_DeadEnd[DeadEndBlockX][DeadEndBlockY] = CurrentDeadEnd;

                                // Continue scanning (go down)
                                DeadEndBlockY++;

                                // Update the auxiliary variables value.
                                IsWallHere  = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY);
                                IsWallLeft  = m_pArena->IsWall (DeadEndBlockX - 1, DeadEndBlockY);
                                IsWallRight = m_pArena->IsWall (DeadEndBlockX + 1, DeadEndBlockY);
                            }

                            // If there is a no wall blocking the way and however the dead end was entirely scanned
                            if (!IsWallHere)
                            {
                                // Then this dead end has an exit. Record the block coordinates of this exit.
                                m_DeadEndExit[CurrentDeadEnd].BlockX = DeadEndBlockX;
                                m_DeadEndExit[CurrentDeadEnd].BlockY = DeadEndBlockY;
                            }

                            // Next dead end number
                            CurrentDeadEnd++;
                        }
                        // If this block is the back of a dead end ("]")
                        else if (IsWallRight && IsWallUp && IsWallDown)
                        {
                            // Start scanning the dead end on this block
                            int DeadEndBlockX = BlockX;
                            int DeadEndBlockY = BlockY;

                            // While we are still in this dead end and there is no wall blocking the way
                            while (IsWallUp && IsWallDown && !IsWallHere)
                            {   
                                // Set the dead end number of the current block : it's the current dead end number
                                m_DeadEnd[DeadEndBlockX][DeadEndBlockY] = CurrentDeadEnd;

                                // Continue scanning (go left)
                                DeadEndBlockX--;

                                // Update the auxiliary variables value.
                                IsWallHere = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY);
                                IsWallUp   = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY - 1);
                                IsWallDown = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY + 1);
                            }

                            // If there is a no wall blocking the way and however the dead end was entirely scanned
                            if (!IsWallHere)
                            {
                                // Then this dead end has an exit. Record the block coordinates of this exit.
                                m_DeadEndExit[CurrentDeadEnd].BlockX = DeadEndBlockX;
                                m_DeadEndExit[CurrentDeadEnd].BlockY = DeadEndBlockY;
                            }

                            // Next dead end number
                            CurrentDeadEnd++;
                        }
                        // If this block is the back of a dead end ("ÀÙ)
                        else if (IsWallDown && IsWallLeft && IsWallRight)
                        {
                            // Start scanning the dead end on this block
                            int DeadEndBlockX = BlockX;
                            int DeadEndBlockY = BlockY;

                            // While we are still in this dead end and there is no wall blocking the way
                            while (IsWallLeft && IsWallRight && !IsWallHere)
                            {
                                // Set the dead end number of the current block : it's the current dead end number
                                m_DeadEnd[DeadEndBlockX][DeadEndBlockY] = CurrentDeadEnd;

                                // Continue scanning (go up)
                                DeadEndBlockY--;

                                // Update the auxiliary variables value.
                                IsWallHere  = m_pArena->IsWall (DeadEndBlockX, DeadEndBlockY);
                                IsWallLeft  = m_pArena->IsWall (DeadEndBlockX - 1, DeadEndBlockY);
                                IsWallRight = m_pArena->IsWall (DeadEndBlockX + 1, DeadEndBlockY);
                            }

                            // If there is a no wall blocking the way and however the dead end was entirely scanned
                            if (!IsWallHere)
                            {
                                // Then this dead end has an exit. Record the block coordinates of this exit.
                                m_DeadEndExit[CurrentDeadEnd].BlockX = DeadEndBlockX;
                                m_DeadEndExit[CurrentDeadEnd].BlockY = DeadEndBlockY;
                            }

                            // Next dead end number
                            CurrentDeadEnd++;
                        }
                        // If this block is the back of NO dead end
                        else
                        {
                            // Set there is no dead end on this block. There may be a dead end
                            // containing this block but we record dead ends by detecting their back.
                            m_DeadEnd[BlockX][BlockY] = -1;
                        }
                    }
                    // If there is a wall on this block
                    else
                    {
                        // There is definitely no dead end here
                        m_DeadEnd[BlockX][BlockY] = -1;
                    }
                } // if
            } // if
        } // for
    } // for

    //*****************
    // SOFT WALL NEAR
    //*****************

// Used when determining the number of soft walls near each block.
// This is the maximum distance (in blocks) of the soft wall from the block.
#define SOFT_WALL_NEAR_MAX_DEPTH    2

    // Auxiliary variables for code readability : is there a soft wall around the current block?
    bool IsSoftWallUp;
    bool IsSoftWallDown;
    bool IsSoftWallLeft;
    bool IsSoftWallRight;
    
    // Current distance of the scanned block from the start block
    int Depth;

    // Scan the blocks of the arena
    for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
    {
        for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
        {
            // If the block is on the edges of the arena grid
            if (BlockX == 0 || BlockX == ARENA_WIDTH - 1 ||
                BlockY == 0 || BlockY == ARENA_HEIGHT - 1)
            {
                // There is definitely no soft wall near here
                m_SoftWallNear[BlockX][BlockY] = -1;
            }
            // If the block is not on the edges of the arena grid
            else
            {
                // Is there a wall on this block?
                IsWallHere = m_pArena->IsWall (BlockX, BlockY);

                // If there is no wall on this block
                if (!IsWallHere)
                {
                    //----------------------
                    // Scan above the block
                    //----------------------
                    
                    // Start scanning (distance 1 from the block)
                    Depth = 1;

                    // Scan until there is a wall or until maximum depth has been reached
                    do
                    {
                        // Is there a wall on the scanned block?
                        IsWallUp = m_pArena->IsWall (BlockX, BlockY - Depth);

                        // Is there a soft wall on the scanned block?
                        IsSoftWallUp = m_pArena->IsSoftWall (BlockX, BlockY - Depth);

                        // Continue scanning
                        Depth++;
                    }
                    while (Depth <= SOFT_WALL_NEAR_MAX_DEPTH && !IsWallUp && !m_pArena->IsItem(BlockX, BlockY - Depth));

                    //----------------------
                    // Scan below the block
                    //----------------------
                    
                    // Start scanning (distance 1 from the block)
                    Depth = 1;

                    // Scan until there is a wall or until maximum depth has been reached
                    do
                    {
                        // Is there a wall on the scanned block?
                        IsWallDown = m_pArena->IsWall (BlockX, BlockY + Depth);

                        // Is there a soft wall on the scanned block?
                        IsSoftWallDown = m_pArena->IsSoftWall (BlockX, BlockY + Depth);

                        // Continue scanning
                        Depth++;
                    }
                    while (Depth <= SOFT_WALL_NEAR_MAX_DEPTH && !IsWallDown && !m_pArena->IsItem(BlockX, BlockY + Depth));

                    //-------------------------------
                    // Scan to the left of the block
                    //-------------------------------
                    
                    // Start scanning (distance 1 from the block)
                    Depth = 1;

                    // Scan until there is a wall or until maximum depth has been reached
                    do
                    {
                        // Is there a wall on the scanned block?
                        IsWallLeft = m_pArena->IsWall (BlockX - Depth, BlockY);
                        
                        // Is there a soft wall on the scanned block?
                        IsSoftWallLeft = m_pArena->IsSoftWall (BlockX - Depth, BlockY);
                        
                        // Continue scanning
                        Depth++;
                    }
                    while (Depth <= SOFT_WALL_NEAR_MAX_DEPTH && !IsWallLeft && !m_pArena->IsItem(BlockX - Depth, BlockY));

                    //--------------------------------
                    // Scan to the right of the block
                    //--------------------------------

                    // Start scanning (distance 1 from the block)                    
                    Depth = 1;

                    // Scan until there is a wall or until maximum depth has been reached
                    do
                    {
                        // Is there a wall on the scanned block?
                        IsWallRight = m_pArena->IsWall (BlockX + Depth, BlockY);
                        
                        // Is there a soft wall on the scanned block?
                        IsSoftWallRight = m_pArena->IsSoftWall (BlockX + Depth, BlockY);
                        
                        // Continue scanning
                        Depth++;
                    }
                    while (Depth <= SOFT_WALL_NEAR_MAX_DEPTH && !IsWallRight && !m_pArena->IsItem(BlockX + Depth, BlockY));

                    //--------------------------------------------------
                    // Count total number of soft walls near this block
                    //--------------------------------------------------

                    // No soft walls near this block yet
                    int NumSoftWallsNear = 0;

                    // Increase this number for each direction if there is a soft wall in this direction
                    if (IsSoftWallUp)         NumSoftWallsNear++;
                    if (IsSoftWallDown)       NumSoftWallsNear++;
                    if (IsSoftWallLeft)       NumSoftWallsNear++;
                    if (IsSoftWallRight)      NumSoftWallsNear++;

                    // Set the number of soft walls near this block
                    m_SoftWallNear[BlockX][BlockY] = NumSoftWallsNear;
                }
                // If there is a wall on this block
                else
                {   
                    // There is definitely no soft wall near here         
                    m_SoftWallNear[BlockX][BlockY] = -1;
                }
            } // if
        } // for
    } // for
    
    // if debug function enabled: draw some squares showing the accesibility
#if defined(DEBUG_DRAW_SOFTWALL_BLOCKS) || defined(DEBUG_DRAW_BURNWALLDANGER_BLOCKS) || defined(DEBUG_DRAW_BOMB_OWNERS)
    // debug display
    BYTE r, g, b;
    BYTE rbase, gbase, bbase;
    int w, h;

    m_pDisplay->RemoveAllDebugRectangles();
#endif
#ifdef DEBUG_DRAW_SOFTWALL_BLOCKS
    rbase = 128; gbase = 128; bbase = 128; 
    
    w = m_pArena->ToPosition(1);
    h = m_pArena->ToPosition(1);
    
    if (m_pDisplay != NULL)
    {
        m_pDisplay->RemoveAllDebugRectangles();
        for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
        {
            for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
            {
                if (m_SoftWallNear[BlockX][BlockY] > 0)
                {
                    r = rbase + m_SoftWallNear[BlockX][BlockY] * 8;
                    g = gbase + m_SoftWallNear[BlockX][BlockY] * 8;
                    b = bbase + m_SoftWallNear[BlockX][BlockY] * 8;
                    
                    m_pDisplay->DrawDebugRectangle (
                        m_pArena->ToPosition(BlockX), 
                        m_pArena->ToPosition(BlockY), 
                        w, h, r, g, b, AIARENADEBUG_SPRITELAYER, PRIORITY_UNUSED);
                }
            }
        }
    }
#endif

    //*************
    // DANGER AND BURNING WALL AND BURNING SOON WALL
    //*************
    
    // Scan each block of the danger array and wallburn array
    for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
    {
        for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
        {
            // Set no danger for the moment
            m_Danger[BlockX][BlockY] = DANGER_NONE;

            m_DangerTimeLeft[BlockX][BlockY] = 999.0f;

            // Set true if there is a burning wall
            m_WallBurn[BlockX][BlockY] = m_pArena->IsBurningWall(BlockX, BlockY);
        }
    }
    
    // create an index array for the bombs
    // each element represents a bomb. its value is the index to the bomb which
    // will ignite this bomb.
    std::vector<int> BombIndex(m_pArena->MaxBombs());
    
    for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
    {
        BombIndex[Index] = Index; // start with the bomb itself
    }

    // Was there an update?
    bool Updated;
    
    // Scan each block of the arena
    do {
        Updated = false;
        
        for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
        {
            for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
            {
                // If there is a flame or a wall on this block
                if (m_pArena->IsFlame (BlockX, BlockY) || m_pArena->IsWall (BlockX, BlockY))
                {
                    // This block is mortal
                    m_Danger[BlockX][BlockY] = DANGER_MORTAL;
                    m_DangerTimeLeft[BlockX][BlockY] = 0.0f;
                }
                // If there is a bomb on this block
                else if (m_pArena->IsBomb (BlockX, BlockY))
                {
                    // This block will at least soon be mortal (but it can already be mortal)
                    if (m_Danger[BlockX][BlockY] == DANGER_NONE)
                        m_Danger[BlockX][BlockY] = DANGER_SOON;

                    float TimeLeft = -1.0f;
                    int FlameSize = -1;
                    bool IgnoreBomb = false;
                    int ThisBombIndex = -1;
                    
                    for (int Index = 0 ; Index < m_pArena->MaxBombs() ; Index++)
                    {
                        if (m_pArena->GetBomb(Index).GetBlockX() == BlockX &&
                            m_pArena->GetBomb(Index).GetBlockY() == BlockY)
                        {
                            FlameSize = m_pArena->GetBomb(Index).GetFlameSize();
                            // Time left will be propagated to all bombs being ignited by this bomb
                            TimeLeft = m_pArena->GetBomb(BombIndex[Index]).GetTimeLeft();
                            // ignore bomb: when we don't know when a bomb explodes
                            // we can't know when the soft walls around will disappear
                            ThisBombIndex = BombIndex[Index];
                            IgnoreBomb = m_pArena->GetBomb(Index).IsRemote() &&
                                !m_pArena->GetBomber(m_pArena->GetBomb(Index).GetOwnerPlayer()).IsAlive();
                            break;
                        }
                    }

                    ASSERT (TimeLeft != -1.0f);
                    ASSERT (FlameSize != -1);
                    ASSERT (ThisBombIndex != -1);

                    if (FlameSize >= 4)
                    {
                        switch (FlameSize)
                        {
                            case 4  : FlameSize =  5; break;
                            case 5  : FlameSize =  7; break;
                            case 6  : FlameSize =  8; break;
                            default : FlameSize = 99; break;
                        }
                    }

                    int Depth;
                    
                    // Block coordinates used to scan the blocks where the bomb creates danger
                    int DangerBlockX;
                    int DangerBlockY;
                    
                    // Auxiliary variables for updating the timeleft when another bomb
                    // was found which may ignite the bomb we're currently dealing with
                    int UpdateX;
                    int UpdateY;
                    
                    // Auxilary variable for the time left on the current block
                    float TimeLeftHere;
                    
                    // Auxiliary variables for code readability : is there a wall/bomb on the current block?
                    bool IsWall;
                    bool IsBomb;
                    
                    // Start scanning on this block
                    DangerBlockX = BlockX;
                    DangerBlockY = BlockY;
                    Depth = 0;

                    // No wall and no bomb on the current block
                    IsWall = false;
                    IsBomb = false;
                    
                    // While there is no wall or bomb that could stop the explosion flames
                    while (true)
                    {
                        // If there is a bomb where we are scanning or if we scanned deep enough
                        if (IsBomb || Depth > FlameSize)
                        {
                            // Update time left if this bomb might ignite the other
                            for (int Index = 0 ; IsBomb && Index < m_pArena->MaxBombs() ; Index++)
                            {
                                if (m_pArena->GetBomb(Index).GetBlockX() == DangerBlockX &&
                                    m_pArena->GetBomb(Index).GetBlockY() == DangerBlockY)
                                {
                                    TimeLeftHere = m_pArena->GetBomb(BombIndex[Index]).GetTimeLeft();
                                    if (TimeLeft > TimeLeftHere)
                                    {
                                        TimeLeft = TimeLeftHere;
                                        Updated = true;
                                        
                                        // update timeleft on the blocks which have been passed
                                        for (UpdateX = BlockX; UpdateX <= DangerBlockX; UpdateX++)
                                            m_DangerTimeLeft[UpdateX][DangerBlockY] = TimeLeft;
                                        
                                        // update BombIndex array elements
                                        for (int i = 0; i < m_pArena->MaxBombs(); i++)
                                            if (BombIndex[i] == ThisBombIndex)
                                                BombIndex[i] = BombIndex[Index];
                                    }
                                    
                                    break;
                                }
                            }
                            
                            // Stop scanning.
                            break;
                        }
                        // If there is a wall where we are scanning
                        else if (IsWall)
                        {
                            // If this is a soft wall
                            if (m_pArena->IsSoftWall(DangerBlockX,DangerBlockY))
                            {
                                // Then this wall will soon burn (unless we think we can ignore the bomb)
                                // in this case it is uncertain when the wall will be burned
                                if (!IgnoreBomb)
                                    m_WallBurn[DangerBlockX][DangerBlockY] = true;
                            }

                            // Stop scanning.
                            break;
                        }
                        // If there is no bomb and no wall on the block where we are scanning
                        else if (!IgnoreBomb)
                        {
                            // If no danger was recorded on this block
                            if (m_Danger[DangerBlockX][DangerBlockY] == DANGER_NONE)
                            {
                                // This block will soon be mortal
                                m_Danger[DangerBlockX][DangerBlockY] = DANGER_SOON;
                            }

                            if (m_DangerTimeLeft[DangerBlockX][DangerBlockY] > TimeLeft)
                            {
                                m_DangerTimeLeft[DangerBlockX][DangerBlockY] = TimeLeft;
                            }
                        }

                        // Continue scanning (go right)
                        DangerBlockX++;
                        Depth++;            // Go deeper

                        // Update auxiliary variables
                        IsWall = m_pArena->IsWall(DangerBlockX,DangerBlockY);
                        IsBomb = m_pArena->IsBomb(DangerBlockX,DangerBlockY);
                    } // while

                    // Start scanning on this block
                    DangerBlockX = BlockX;
                    DangerBlockY = BlockY;
                    Depth = 0;

                    // No wall and no bomb on the current block
                    IsWall = false;
                    IsBomb = false;

                    // While there is no wall or bomb that could stop the explosion flames
                    while (true)
                    {
                        // If there is a bomb where we are scanning or if we scanned deep enough
                        if (IsBomb || Depth > FlameSize)
                        {
                            // Update time left if this bomb might ignite the other
                            for (int Index = 0 ; IsBomb && Index < m_pArena->MaxBombs() ; Index++)
                            {
                                if (m_pArena->GetBomb(Index).GetBlockX() == DangerBlockX &&
                                    m_pArena->GetBomb(Index).GetBlockY() == DangerBlockY)
                                {
                                    TimeLeftHere = m_pArena->GetBomb(BombIndex[Index]).GetTimeLeft();
                                    if (TimeLeft > TimeLeftHere)
                                    {
                                        TimeLeft = TimeLeftHere;
                                        Updated = true;
                                        
                                        // update timeleft on the blocks which have been passed
                                        for (UpdateX = DangerBlockX; UpdateX <= BlockX; UpdateX++)
                                            m_DangerTimeLeft[UpdateX][DangerBlockY] = TimeLeft;

                                        // update BombIndex array elements
                                        for (int i = 0; i < m_pArena->MaxBombs(); i++)
                                            if (BombIndex[i] == ThisBombIndex)
                                                BombIndex[i] = BombIndex[Index];
                                    }
                                    
                                    break;
                                }
                            }
                            
                            // Stop scanning.
                            break;
                        }
                        // If there is a wall where we are scanning
                        else if (IsWall)
                        {
                            // If this is a soft wall
                            if (m_pArena->IsSoftWall(DangerBlockX,DangerBlockY))
                            {
                                // Then this wall will soon burn (unless we think we can ignore the bomb)
                                if (!IgnoreBomb)
                                    m_WallBurn[DangerBlockX][DangerBlockY] = true;
                            }

                            // Stop scanning.
                            break;
                        }
                        // If there is no bomb and no wall on the block where we are scanning
                        else if (!IgnoreBomb)
                        {
                            // If no danger was recorded on this block
                            if (m_Danger[DangerBlockX][DangerBlockY] == DANGER_NONE)
                            {
                                // This block will soon be mortal
                                m_Danger[DangerBlockX][DangerBlockY] = DANGER_SOON;
                            }

                            if (m_DangerTimeLeft[DangerBlockX][DangerBlockY] > TimeLeft)
                            {
                                m_DangerTimeLeft[DangerBlockX][DangerBlockY] = TimeLeft;
                            }
                        }
                        
                        // Continue scanning (go left)
                        DangerBlockX--;
                        Depth++;            // Go deeper

                        // Update auxiliary variables
                        IsWall = m_pArena->IsWall(DangerBlockX,DangerBlockY);
                        IsBomb = m_pArena->IsBomb(DangerBlockX,DangerBlockY);
                    } // while

                    // Start scanning on this block
                    DangerBlockX = BlockX;
                    DangerBlockY = BlockY;
                    Depth = 0;

                    // No wall and no bomb on the current block
                    IsWall = false;
                    IsBomb = false;

                    // While there is no wall or bomb that could stop the explosion flames
                    while (true)
                    {
                        // If there is a bomb where we are scanning or if we scanned deep enough
                        if (IsBomb || Depth > FlameSize)
                        {
                            // Update time left if this bomb might ignite the other
                            for (int Index = 0 ; IsBomb && Index < m_pArena->MaxBombs() ; Index++)
                            {
                                if (m_pArena->GetBomb(Index).GetBlockX() == DangerBlockX &&
                                    m_pArena->GetBomb(Index).GetBlockY() == DangerBlockY)
                                {
                                    TimeLeftHere = m_pArena->GetBomb(BombIndex[Index]).GetTimeLeft();
                                    if (TimeLeft > TimeLeftHere)
                                    {
                                        TimeLeft = TimeLeftHere;
                                        Updated = true;
                                        
                                        // update timeleft on the blocks which have been passed
                                        for (UpdateY = DangerBlockY; UpdateY <= BlockY; UpdateY++)
                                            m_DangerTimeLeft[DangerBlockX][UpdateY] = TimeLeft;

                                        // update BombIndex array elements
                                        for (int i = 0; i < m_pArena->MaxBombs(); i++)
                                            if (BombIndex[i] == ThisBombIndex)
                                                BombIndex[i] = BombIndex[Index];
                                    }
                                    
                                    break;
                                }
                            }
                            
                            // Stop scanning.
                            break;
                        }
                        // If there is a wall where we are scanning
                        else if (IsWall)
                        {
                            // If this is a soft wall
                            if (m_pArena->IsSoftWall(DangerBlockX,DangerBlockY))
                            {
                                // Then this wall will soon burn (unless we think we can ignore the bomb)
                                // in this case it is uncertain when the wall will be burned
                                if (!IgnoreBomb)
                                    m_WallBurn[DangerBlockX][DangerBlockY] = true;
                            }

                            // Stop scanning.
                            break;
                        }
                        // If there is no bomb and no wall on the block where we are scanning
                        else if (!IgnoreBomb)
                        {
                            // If no danger was recorded on this block
                            if (m_Danger[DangerBlockX][DangerBlockY] == DANGER_NONE)
                            {
                                // This block will soon be mortal
                                m_Danger[DangerBlockX][DangerBlockY] = DANGER_SOON;
                            }

                            if (m_DangerTimeLeft[DangerBlockX][DangerBlockY] > TimeLeft)
                            {
                                m_DangerTimeLeft[DangerBlockX][DangerBlockY] = TimeLeft;
                            }
                        }
                        
                        // Continue scanning (go up)
                        DangerBlockY--;
                        Depth++;            // Go deeper

                        // Update auxiliary variables
                        IsWall = m_pArena->IsWall(DangerBlockX,DangerBlockY);
                        IsBomb = m_pArena->IsBomb(DangerBlockX,DangerBlockY);
                    } // while

                    // Start scanning on this block
                    DangerBlockX = BlockX;
                    DangerBlockY = BlockY;
                    Depth = 0;

                    // No wall and no bomb on the current block
                    IsWall = false;
                    IsBomb = false;

                    // While there is no wall or bomb that could stop the explosion flames
                    while (true)
                    {
                        // If there is a bomb where we are scanning or if we scanned deep enough
                        if (IsBomb || Depth > FlameSize)
                        {
                            // Update time left if this bomb might ignite the other
                            for (int Index = 0 ; IsBomb && Index < m_pArena->MaxBombs() ; Index++)
                            {
                                if (m_pArena->GetBomb(Index).GetBlockX() == DangerBlockX &&
                                    m_pArena->GetBomb(Index).GetBlockY() == DangerBlockY)
                                {
                                    TimeLeftHere = m_pArena->GetBomb(BombIndex[Index]).GetTimeLeft();
                                    if (TimeLeft > TimeLeftHere)
                                    {
                                        TimeLeft = TimeLeftHere;
                                        Updated = true;
                                        
                                        // update timeleft on the blocks which have been passed
                                        for (UpdateY = BlockY; UpdateY <= DangerBlockY; UpdateY++)
                                            m_DangerTimeLeft[DangerBlockX][UpdateY] = TimeLeft;
                                        
                                        // update BombIndex array elements
                                        for (int i = 0; i < m_pArena->MaxBombs(); i++)
                                            if (BombIndex[i] == ThisBombIndex)
                                                BombIndex[i] = BombIndex[Index];
                                    }
                                    
                                    break;
                                }
                            }
                            
                            // Stop scanning.
                            break;
                        }
                        // If there is a wall where we are scanning
                        else if (IsWall)
                        {
                            // If this is a soft wall
                            if (m_pArena->IsSoftWall(DangerBlockX,DangerBlockY))
                            {
                                // Then this wall will soon burn (unless we think we can ignore the bomb)
                                // in this case it is uncertain when the wall will be burned
                                if (!IgnoreBomb)
                                    m_WallBurn[DangerBlockX][DangerBlockY] = true;
                            }

                            // Stop scanning.
                            break;
                        }
                        // If there is no bomb and no wall on the block where we are scanning
                        else if (!IgnoreBomb)
                        {
                            // If no danger was recorded on this block
                            if (m_Danger[DangerBlockX][DangerBlockY] == DANGER_NONE)
                            {
                                // This block will soon be mortal
                                m_Danger[DangerBlockX][DangerBlockY] = DANGER_SOON;
                            }

                            if (m_DangerTimeLeft[DangerBlockX][DangerBlockY] > TimeLeft)
                            {
                                m_DangerTimeLeft[DangerBlockX][DangerBlockY] = TimeLeft;
                            }
                        }
                        
                        // Continue scanning (go down)
                        DangerBlockY++;
                        Depth++;            // Go deeper

                        // Update auxiliary variables
                        IsWall = m_pArena->IsWall(DangerBlockX,DangerBlockY);
                        IsBomb = m_pArena->IsBomb(DangerBlockX,DangerBlockY);
                    } // while
                } // if
            } // for
        } // for
    } // do
    while (Updated);
        
    // If the arena is closing right now
    if (m_pArena->GetArenaCloser().IsClosing ())
    {
        // Save in how many seconds the next falling wall will start falling
        float DangerTimeLeft = m_pArena->GetArenaCloser().GetTimeLeftBeforeClosingNextBlock();

        // Scan the next X blocks that will be closed
        for (int Index = 0 ; Index < m_pArena->GetArenaCloser().GetNumberOfBlocksLeft() && Index < 10 ; Index++)
        {
            // Save the block position of the block that will soon be closed
            BlockX = m_pArena->GetArenaCloser().GetNextBlockPositionX (Index);
            BlockY = m_pArena->GetArenaCloser().GetNextBlockPositionY (Index);

            // If there is no danger on this block
            if (m_Danger[BlockX][BlockY] == DANGER_NONE)
            {
                // Now this block will soon be dangerous
                m_Danger[BlockX][BlockY] = DANGER_SOON;
            }

            // If the danger of this falling wall will happen earlier
            // than the current recorded danger on this block
            if (m_DangerTimeLeft[BlockX][BlockY] > DangerTimeLeft)
            {
                // The danger that will happen earlier has a higher priority
                m_DangerTimeLeft[BlockX][BlockY] = DangerTimeLeft;
            }

            // Get the time left in seconds before the next falling wall will start falling
            DangerTimeLeft += m_pArena->GetArenaCloser().GetTimeBetweenTwoBlockClosures();
        }
    }

#if defined(DEBUG_DRAW_BURNWALLDANGER_BLOCKS) || defined(DEBUG_DRAW_BOMB_OWNERS)
    // red: danger, blue: burnwall
    rbase = 128; gbase = 0; bbase = 128; 
    
    w = m_pArena->ToPosition(1);
    h = m_pArena->ToPosition(1);
    
    if (m_pDisplay != NULL)
    {
        for (BlockX = 0 ; BlockX < ARENA_WIDTH ; BlockX++)
        {
            for (BlockY = 0 ; BlockY < ARENA_HEIGHT ; BlockY++)
            {
                if (m_Danger[BlockX][BlockY] != DANGER_NONE &&
                    !m_pArena->IsWall(BlockX,BlockY) &&
                    !m_pArena->IsBomb(BlockX,BlockY))
                {
                    r = rbase;
                    if (m_Danger[BlockX][BlockY] == DANGER_MORTAL)
                        r += 64;
                    
                    r += MIN(64, (int)floor(m_DangerTimeLeft[BlockX][BlockY] * 30.0));
                    
                    m_pDisplay->DrawDebugRectangle (
                        m_pArena->ToPosition(BlockX), 
                        m_pArena->ToPosition(BlockY), 
                        w, h, r, 0, 0, AIARENADEBUG_SPRITELAYER, PRIORITY_UNUSED);
                    
                    m_Font.Draw (m_pArena->ToPosition(BlockX),
                                 m_pArena->ToPosition(BlockY)+h/4, "%.1f",
                                 floor(m_DangerTimeLeft[BlockX][BlockY]*10)/10);

                }
                else if (m_WallBurn[BlockX][BlockY])
                {
                    b = bbase;
                    
                    m_pDisplay->DrawDebugRectangle (
                        m_pArena->ToPosition(BlockX), 
                        m_pArena->ToPosition(BlockY), 
                        w, h, 0, 0, b, AIARENADEBUG_SPRITELAYER, PRIORITY_UNUSED);
                }
                
#ifdef DEBUG_DRAW_BOMB_OWNERS
                if (m_pArena->IsBomb (BlockX, BlockY))
                {
                    for (int Index = 0 ; Index < m_pArena->MaxBombs() ; Index++)
                    {
                        if (m_pArena->GetBomb(Index).GetBlockX() == BlockX &&
                            m_pArena->GetBomb(Index).GetBlockY() == BlockY)
                        {
                            if (!m_pArena->GetBomber(m_pArena->GetBomb(Index).GetOwnerPlayer()).IsAlive())
                            {
                                // bomber is dead
                                r = 0; g = 255; b = 255;
                            }
                            else
                            {
                                switch (m_pArena->GetBomb(Index).GetOwnerPlayer())
                                {
                                    case 0 : r = 255; g = 255; b = 255; break;
                                    case 1 : r = 0;   g = 0;   b = 0;   break;
                                    case 2 : r = 255; g = 0;   b = 0;   break;
                                    case 3 : r = 0;   g = 0;   b = 255; break;
                                    case 4 : r = 0;   g = 255; b = 0;   break;
                                    default: r = g = b = 0;             break;
                                }
                            }
                            
                            m_pDisplay->DrawDebugRectangle (
                                m_pArena->ToPosition(BlockX), 
                                m_pArena->ToPosition(BlockY), 
                                w, h, r, g, b, AIARENADEBUG_SPRITELAYER, PRIORITY_UNUSED);
                            break;
                        }
                    }
                }  
#endif
            }
        }
    }
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
