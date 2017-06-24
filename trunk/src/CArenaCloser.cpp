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
 *  \file CArenaCloser.cpp
 *  \brief Arena closer (walls falling down)
 */

#include "StdAfx.h"
#include "CArenaCloser.h"
#include "CArena.h"
#include "COptions.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CArenaCloser::CArenaCloser (void)
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pArena = NULL;
    m_pOptions = NULL;

    m_TimeBetweenTwoBlockClosures = 0.0f;
    m_TimeLeftBeforeClosingNextBlock = 0.0f;
    m_ClosureData.clear();
    m_IsClosing = false;
}   

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CArenaCloser::~CArenaCloser (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::Create (void)
{
    switch (RANDOM(3))
    {
        case 0 : CreateSpiralClosing();     break;
        case 1 : CreateHorizontalClosing(); break;
        case 2 : CreateVerticalClosing();   break;
    }

    m_IsClosing = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::Destroy (void)
{
    Stop();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::Start (void)
{   
    // Check if the pointers were all set
    ASSERT (m_pArena != NULL);
    ASSERT (m_pOptions != NULL);
          
    // Make some dummy variables
    int TotalSeconds = m_pOptions->GetTimeUpMinutes() * 60 + m_pOptions->GetTimeUpSeconds();
    int TotalBlocksToClose = m_ClosureData.size();
    
    // Set close interval to (Total closing time / Number of blocks to close)
    if (TotalBlocksToClose > 0)
        m_TimeBetweenTwoBlockClosures = (float)TotalSeconds / TotalBlocksToClose;
    else
        m_TimeBetweenTwoBlockClosures = (float)TotalSeconds;

    // Set time left before closing the first block
    m_TimeLeftBeforeClosingNextBlock = 0.0f;

    // The arena closer is now ready to close the arena
    m_IsClosing = true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::CreateSpiralClosing (void)
{
    SClosePosition ClosePosition;
    int I = 0;
    int X = 1;
    int Y = 1;
    int K = 1;

    while (K < 6 && I < 143)
    {
        while (X < ARENA_WIDTH - K)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            X++; 
            I++;
        }

        X = ARENA_WIDTH - 1 - K;
        Y++;

        while (Y < ARENA_HEIGHT - K)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            Y++;
            I++;
        }

        Y = ARENA_HEIGHT - 1 - K;
        X--;

        while (X >= 0 + K)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            X--;
            I++;
        }

        X = 0 + K;
        Y--;

        while (Y >= 2 + K)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            Y--;
            I++;
        }

        Y = 2 + K;
        Y--;

        K++;
    }

    while (X <= 8)
    {
        ClosePosition.X = X;
        ClosePosition.Y = Y;
        m_ClosureData.push_back (ClosePosition);
        X++;
        I++;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::CreateHorizontalClosing (void)
{
    SClosePosition ClosePosition;
    int I = 0;
    int X = -1;
    int Y = -1;

    while (I <= 5)
    {
        X = ARENA_WIDTH - 2;
        Y = ARENA_HEIGHT - 2 - I;
    
        while (X >= 1)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            X--; 
        }

        X = 1;
        Y = 1 + I;

        while (X <= ARENA_WIDTH - 2)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            X++; 
        }
    
        I++;    
    }

    Y = ARENA_HEIGHT - 2 - I;
    
    while (X >= 1)
    {
        ClosePosition.X = X;
        ClosePosition.Y = Y;
        m_ClosureData.push_back (ClosePosition);
        X--; 
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::CreateVerticalClosing (void)
{
    SClosePosition ClosePosition;
    int I = 0;
    int X;
    int Y;

    while (I <= 5)
    {
        X = 1 + I;
        Y = 1;
    
        while (Y <= ARENA_HEIGHT - 2)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            Y++; 
        }

        X = ARENA_WIDTH - 2 - I;
        Y = ARENA_HEIGHT - 2;

        while (Y >= 1)
        {
            ClosePosition.X = X;
            ClosePosition.Y = Y;
            m_ClosureData.push_back (ClosePosition);
            Y--; 
        }
    
        I++; 
    }

    X = 1 + I;
    Y = 1;

    while (Y <= ARENA_HEIGHT - 2)
    {
        ClosePosition.X = X;
        ClosePosition.Y = Y;
        m_ClosureData.push_back (ClosePosition);
        Y++; 
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::Stop (void)
{
    // Empty the closure data
    m_ClosureData.clear();
    
    m_IsClosing = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaCloser::Update (float DeltaTime)
{
    // If the arena must be closing
    if (m_IsClosing)
    {
        // If there remains some blocks to close then continue closing
        if (!m_ClosureData.empty())
        {
            // Reduce time left before closing the next block
            m_TimeLeftBeforeClosingNextBlock -= DeltaTime;
    
            // If we have to start closing the next block
            if (m_TimeLeftBeforeClosingNextBlock <= 0.0f)
            {
                // Set time left before closing the block after the one we are closing
                m_TimeLeftBeforeClosingNextBlock += m_TimeBetweenTwoBlockClosures;

                // Save the position of the block that must be closed
                int BlockX = m_ClosureData.front().X;
                int BlockY = m_ClosureData.front().Y;

                // Assume it is necessary to create a falling wall
                bool CloseIt = true;

                // Scan the walls to see if there is not a hard wall at the block
                // that must be closed. If there is one then no need to close this block.
                for (int Index = 0 ; Index < m_pArena->MaxWalls() ; Index++)
                {
                    if (m_pArena->GetWall(Index).Exist() &&
                        m_pArena->GetWall(Index).GetType() == WALL_HARD &&
                        m_pArena->GetWall(Index).GetBlockX() == BlockX &&
                        m_pArena->GetWall(Index).GetBlockY() == BlockY)
                    {
                        // No need to close this block.
                        CloseIt = false;
                        break;
                    }
                }

                // If the block has to be closed
                if (CloseIt)
                {
                    // Create a falling wall on the block to close
                    m_pArena->NewWall (BlockX, BlockY, WALL_FALLING);
                }

                // Remove the front element of the queue : this one is done
                m_ClosureData.pop_front ();
            }
        }
        // If all blocks were closed
        else
        {
            // Stop closing the arena
            Stop();
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArenaCloser::WriteSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.WriteBoolean(m_IsClosing);
    Snapshot.WriteBoolean(m_ClosureData.empty());
    
    if (!m_ClosureData.empty())
    {
        Snapshot.WriteFloat(m_TimeLeftBeforeClosingNextBlock);
        Snapshot.WriteInteger(m_ClosureData.front().X);
        Snapshot.WriteInteger(m_ClosureData.front().Y);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArenaCloser::ReadSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.ReadBoolean(&m_IsClosing);
    
    bool ClosureDateEmpty;
    Snapshot.ReadBoolean(&ClosureDateEmpty);
    
    if (!ClosureDateEmpty)
    {
        int BlockX;
        int BlockY;

        Snapshot.ReadFloat(&m_TimeLeftBeforeClosingNextBlock);
        Snapshot.ReadInteger(&BlockX);
        Snapshot.ReadInteger(&BlockY);

        // Calibrer le closure data avec le snapshot
        // Il ne faut pas que notre closure data soit plus avancé que celui du serveur ou c'est fichu !
        // Donc chez le client il faut interdire le pop de closure data pendant l'update.
        //while (BlockX != m_ClosureData.front().X || BlockY != m_ClosureData.front().Y)
        //    m_ClosureData.pop_front ();
    }
    else
    {
        m_ClosureData.clear();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************
