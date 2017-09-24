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
 *  \file CArena.cpp
 *  \brief Arena during a match
 *
 *  - Ca sert a rien d'enregistrer les centres d'explosions et les bombers vivants ou entrain de mourir dans la vue.
 */

#include "StdAfx.h"
#include "CArena.h"
#include "CDisplay.h"
#include "COptions.h"
#include "CClock.h"
#include "CBomber.h"
#include "CExplosion.h"
#include "CBomb.h"
#include "CItem.h"
#include "CWall.h"
#include "CFloor.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// How many chances to have a soft wall instead of a floor? (in %)
#define CHANCE_SOFTWALL       50

// Position of the up left corner of the arena from the game's view up left corner.
#define ARENA_POSITION_X     0
#define ARENA_POSITION_Y     26

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CArena::CArena(void)
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pDisplay = NULL;
    m_pSound = NULL;
    m_pOptions = NULL;

    m_BombsInUse = 0;

    m_ArenaCloser.SetArena(this);

    int Index;

    for (Index = 0; Index < MaxExplosions(); Index++)
        GetExplosion(Index).SetArena(this);

    for (Index = 0; Index < MaxBombs(); Index++)
        GetBomb(Index).SetArena(this);

    for (Index = 0; Index < MaxBombers(); Index++)
        GetBomber(Index).SetArena(this);

    for (Index = 0; Index < MaxWalls(); Index++)
        GetWall(Index).SetArena(this);

    for (Index = 0; Index < MaxFloors(); Index++)
        GetFloor(Index).SetArena(this);

    for (Index = 0; Index < MaxItems(); Index++)
        GetItem(Index).SetArena(this);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CArena::~CArena()
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::Create(void)
{
    m_Prediction = false;
    m_ArenaCloser.Create();

    //------------------------------------------------------------
    // Create the static elements of the arena (walls and floors)
    //------------------------------------------------------------

    int X, Y;

    // Scan all the blocks of the arena
    for (X = 0; X < ARENA_WIDTH; X++)
    {
        for (Y = 0; Y < ARENA_HEIGHT; Y++)
        {
            // Anyway, there is a floor
            NewFloor(X, Y, m_pOptions->GetBlockType(X, Y));

            // According to the type of this block
            switch (m_pOptions->GetBlockType(X, Y))
            {
            case BLOCKTYPE_HARDWALL:
            {
                // Create a hard wall
                NewWall(X, Y, WALL_HARD);
                break;
            }

            case BLOCKTYPE_SOFTWALL:
            {
                // Create a soft wall
                NewWall(X, Y, WALL_SOFT);
                break;
            }

            case BLOCKTYPE_RANDOM:
            {
                // With a probability of X%
                if (RANDOM(100) < CHANCE_SOFTWALL)
                {
                    // Create a soft wall
                    NewWall(X, Y, WALL_SOFT);
                }

                break;
            }

            case BLOCKTYPE_WHITEBOMBER:
            {
                // If the white bomber is playing, create it
                if (m_pOptions->GetBomberType(PLAYER_WHITE) != BOMBERTYPE_OFF)
                    NewBomber(X, Y, PLAYER_WHITE);

                break;
            }

            case BLOCKTYPE_BLACKBOMBER:
            {
                // If the black bomber is playing, create it
                if (m_pOptions->GetBomberType(PLAYER_BLACK) != BOMBERTYPE_OFF)
                    NewBomber(X, Y, PLAYER_BLACK);

                break;
            }

            case BLOCKTYPE_REDBOMBER:
            {
                // If the red bomber is playing, create it
                if (m_pOptions->GetBomberType(PLAYER_RED) != BOMBERTYPE_OFF)
                    NewBomber(X, Y, PLAYER_RED);

                break;
            }

            case BLOCKTYPE_BLUEBOMBER:
            {
                // If the blue bomber is playing, create it
                if (m_pOptions->GetBomberType(PLAYER_BLUE) != BOMBERTYPE_OFF)
                    NewBomber(X, Y, PLAYER_BLUE);

                break;
            }

            case BLOCKTYPE_GREENBOMBER:
            {
                // If the green bomber is playing, create it
                if (m_pOptions->GetBomberType(PLAYER_GREEN) != BOMBERTYPE_OFF)
                    NewBomber(X, Y, PLAYER_GREEN);

                break;
            }

            case BLOCKTYPE_ITEM_BOMB:
            {

                NewItem(X, Y, ITEM_BOMB, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_FLAME:
            {

                NewItem(X, Y, ITEM_FLAME, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_KICK:
            {

                NewItem(X, Y, ITEM_KICK, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_PUNCH:
            {

                NewItem(X, Y, ITEM_PUNCH, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_ROLLER:
            {

                NewItem(X, Y, ITEM_ROLLER, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_THROW:
            {

                NewItem(X, Y, ITEM_THROW, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_REMOTES:
            {

                NewItem(X, Y, ITEM_REMOTE, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_SKULL:
            {

                NewItem(X, Y, ITEM_SKULL, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_SHIELD:
            {

                NewItem(X, Y, ITEM_SHIELD, false, false);
                break;

            }

            case BLOCKTYPE_ITEM_STRONGWEAK:
            {

                NewItem(X, Y, ITEM_STRONGWEAK, false, false);
                break;

            }

            default:
                break;
            }
        }
    }

    //------------------
    // Create the items
    //------------------

    // Now the walls are created. We need to create the items using
    // CItem::CreateItems(), which uses the simplified view of the
    // arena. This means this view has to be valid, but it's not
    // since we just recreated the arena. So update the view.
    UpdateView();

    // Create some items under soft walls
    //! @see CItem::CreateItems()
    CItem::CreateItems(this,
        ITEMPLACE_SOFTWALLS,
        m_pOptions->GetNumberOfItemsInWalls(ITEM_BOMB),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_FLAME),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_ROLLER),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_KICK),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_SKULL),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_THROW),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_PUNCH),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_REMOTE),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_SHIELD),
        m_pOptions->GetNumberOfItemsInWalls(ITEM_STRONGWEAK));

    //-----------------
    // Finish creation
    //-----------------

    // Update the view of the final arena at least once.
    UpdateView();
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


CArena& CArena::operator = (const CArena &Arena)
{
    m_pDisplay = Arena.m_pDisplay;
    m_pSound = Arena.m_pSound;
    m_pOptions = Arena.m_pOptions;

    int i, j;

    for (i = 0; i < MaxFloors(); i++)
    {
        m_Floors[i] = Arena.m_Floors[i];
        m_Floors[i].SetArena(this);
    }

    for (i = 0; i < MaxWalls(); i++)
    {
        m_Walls[i] = Arena.m_Walls[i];
        m_Walls[i].SetArena(this);
    }

    for (i = 0; i < MaxBombs(); i++)
    {
        m_Bombs[i] = Arena.m_Bombs[i];
        m_Bombs[i].SetArena(this);
    }

    for (i = 0; i < MaxItems(); i++)
    {
        m_Items[i] = Arena.m_Items[i];
        m_Items[i].SetArena(this);
    }

    for (i = 0; i < MaxExplosions(); i++)
    {
        m_Explosions[i] = Arena.m_Explosions[i];
        m_Explosions[i].SetArena(this);
    }

    for (i = 0; i < MaxBombers(); i++)
    {
        m_Bombers[i] = Arena.m_Bombers[i];
        m_Bombers[i].SetArena(this);
    }

    m_ArenaCloser = Arena.m_ArenaCloser;
    m_ArenaCloser.SetArena(this);

    for (i = 0; i < ARENA_WIDTH; i++)
        for (j = 0; j < ARENA_HEIGHT; j++)
            m_BlockHas[i][j] = Arena.m_BlockHas[i][j];

    return (*this);
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Deletes every existing element

void CArena::Destroy(void)
{
    m_ArenaCloser.Destroy();

    //-------------------------------
    // Delete every existing element
    //-------------------------------

    int i;

    for (i = 0; i < MaxFloors(); i++)
        m_Floors[i].Destroy();

    for (i = 0; i < MaxWalls(); i++)
        m_Walls[i].Destroy();

    for (i = 0; i < MaxBombs(); i++)
        m_Bombs[i].Destroy();

    for (i = 0; i < MaxItems(); i++)
        m_Items[i].Destroy();

    for (i = 0; i < MaxExplosions(); i++)
        m_Explosions[i].Destroy();

    for (i = 0; i < MaxBombers(); i++)
    {
        m_Bombers[i].Destroy();
        m_Bombers[i].ResetHasExisted(); // reset has existed status (needed for CBoard.cpp)
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::ClearBlock(int X, int Y)
{
    // Check coordinates
    ASSERT(X >= 0 && X < ARENA_WIDTH);
    ASSERT(Y >= 0 && Y < ARENA_HEIGHT);

    int Index;

    //--------------------------------------
    // If there is a wall at X,Y, delete it
    //--------------------------------------

    // Scan the walls
    for (Index = 0; Index < MaxWalls(); Index++)
    {
        // Test existence then position
        if (GetWall(Index).GetBlockX() == X && GetWall(Index).GetBlockY() == Y)
        {
            // Delete it
            DeleteWall(Index);
            break;
        }
    }

    //---------------------------------------
    // If there is an item at X,Y, delete it
    //---------------------------------------

    // Scan the items
    for (Index = 0; Index < MaxItems(); Index++)
    {
        // Test existence then position
        if (GetItem(Index).GetBlockX() == X && GetItem(Index).GetBlockY() == Y)
        {
            // Delete it
            DeleteItem(Index);
            break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::UpdateElements(float DeltaTime)
{
    int Index;

    // Updating elements.
    // Every existing element of each type will now be updated.
    // If the Update() method of the element being updated returns
    // true, then it has to be deleted and removed from the arena.

    for (Index = 0; Index < MaxExplosions(); Index++)
        if (GetExplosion(Index).Exist())
            if (GetExplosion(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteExplosion(Index);

    for (Index = 0; Index < MaxBombs(); Index++)
        if (GetBomb(Index).Exist())
            if (GetBomb(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteBomb(Index);

    for (Index = 0; Index < MaxBombers(); Index++)
        if (GetBomber(Index).Exist())
            if (GetBomber(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteBomber(Index);

    for (Index = 0; Index < MaxWalls(); Index++)
        if (GetWall(Index).Exist())
            if (GetWall(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteWall(Index);

    for (Index = 0; Index < MaxFloors(); Index++)
        if (GetFloor(Index).Exist())
            if (GetFloor(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteFloor(Index);

    for (Index = 0; Index < MaxItems(); Index++)
        if (GetItem(Index).Exist())
            if (GetItem(Index).Update(DeltaTime))
                if (!m_Prediction)
                    DeleteItem(Index);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::UpdateSingleBomber(int Player, float DeltaTime)
{
    // Update independently the desired bomber.
    if (GetBomber(Player).Exist())
        if (GetBomber(Player).Update(DeltaTime))
            DeleteBomber(Player);

    // Update the simplified elements view. Otherwise the bomber 
    // being updated could drop a bomb and then not be blocked
    // by this bomb.
    UpdateView();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::UpdateView(void)
{
    int Index;
    int BlockX;
    int BlockY;

    //---------------------------
    // Reset the block_has array
    //---------------------------

    // Scan the "block has" array
    for (BlockX = 0; BlockX < ARENA_WIDTH; BlockX++)
    {
        for (BlockY = 0; BlockY < ARENA_HEIGHT; BlockY++)
        {
            // The block currently contains nothing
            m_BlockHas[BlockX][BlockY] = 0;
        }
    }

    //-----------------------------------
    // Record the explosions in the view
    //-----------------------------------

    // Scan the explosions
    for (Index = 0; Index < MaxExplosions(); Index++)
    {
        // If this explosion exists
        if (GetExplosion(Index).Exist())
        {
            // Make a reference to this explosion for easier access
            CExplosion &Explosion = GetExplosion(Index);

            // Record the center of the explosion in the view
            SetBlockHas(Explosion.GetBlockX(), Explosion.GetBlockY(), BLOCKHAS_EXPLOSION);

            // Scan the flames of the explosion
            for (unsigned int Flame = 0; Flame < Explosion.GetFlames().size(); Flame++)
            {
                // Record the flame in the view
                SetBlockHas(Explosion.GetFlames()[Flame].BlockX,
                    Explosion.GetFlames()[Flame].BlockY,
                    BLOCKHAS_FLAME);
            }
        }
    }

    //------------------------------
    // Record the bombs in the view
    //------------------------------

    // Scan the bombs
    for (Index = 0; Index < MaxBombs(); Index++)
    {
        // If it exists
        if (GetBomb(Index).Exist() && GetBomb(Index).IsOnFloor())
        {
            // Register the bomb in the view
            SetBlockHas(GetBomb(Index).GetBlockX(), GetBomb(Index).GetBlockY(), BLOCKHAS_BOMB);
        }
    }

    //--------------------------------
    // Record the bombers in the view
    //--------------------------------

    // Scan the bombers
    for (Index = 0; Index < MaxBombers(); Index++)
    {
        // If the bomber exists and is not dead (don't register the bomber if he's dead)
        if (GetBomber(Index).Exist() && !GetBomber(Index).IsDead())
        {
            // Save the block position of the bomber
            BlockX = GetBomber(Index).GetBlockX();
            BlockY = GetBomber(Index).GetBlockY();

            // Record the bomber in the view
            SetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBER);

            // Record the bomber according to alive or dying
            if (GetBomber(Index).IsAlive()) SetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBERALIVE);
            else if (GetBomber(Index).IsDying()) SetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBERDYING);
        }
    }

    //------------------------------
    // Record the walls in the view
    //------------------------------

    // Scan the walls
    for (Index = 0; Index < MaxWalls(); Index++)
    {
        // If the wall exists
        if (GetWall(Index).Exist())
        {
            // Save the block position of the wall
            BlockX = GetWall(Index).GetBlockX();
            BlockY = GetWall(Index).GetBlockY();

            // Register the wall in the view
            SetBlockHas(BlockX, BlockY, BLOCKHAS_WALL);

            // If the wall is not burning
            if (!GetWall(Index).IsBurning())
            {
                // Record the wall in the view according to its type
                switch (GetWall(Index).GetType())
                {
                case WALL_HARD: SetBlockHas(BlockX, BlockY, BLOCKHAS_WALLHARD);    break;
                case WALL_SOFT: SetBlockHas(BlockX, BlockY, BLOCKHAS_WALLSOFT);    break;
                case WALL_FALLING: SetBlockHas(BlockX, BlockY, BLOCKHAS_WALLFALLING); break;
                }
            }
            // If wall is burning
            else
            {
                // Record a burning wall in the view 
                SetBlockHas(BlockX, BlockY, BLOCKHAS_WALLBURNING);
            }
        }
    }

    //-------------------------------
    // Record the floors in the view
    //-------------------------------

    // Scan the floors
    for (Index = 0; Index < MaxFloors(); Index++)
    {
        // If the floor exists
        if (GetFloor(Index).Exist())
        {
            // Record the floor in the view
            SetBlockHas(GetFloor(Index).GetBlockX(), GetFloor(Index).GetBlockY(), BLOCKHAS_FLOOR);

            if (GetFloor(Index).HasAction()) {
                SetBlockHas(GetFloor(Index).GetBlockX(), GetFloor(Index).GetBlockY(), BLOCKHAS_FLOORWITHMOVEEFFECT);
            }
        }
    }

    //------------------------------
    // Record the items in the view
    //------------------------------

    // Scan the items
    for (Index = 0; Index < MaxItems(); Index++)
    {
        // If the item exists and it is not flying
        if (GetItem(Index).Exist() &&
            !GetItem(Index).IsFlying())
        {
            // Save the block position of the item
            BlockX = GetItem(Index).GetBlockX();
            BlockY = GetItem(Index).GetBlockY();

            // Record the item in the view
            SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEM);

            // If the item is not burning
            if (!GetItem(Index).IsBurning())
            {
                // Record the item in the view according to its type
                switch (GetItem(Index).GetType())
                {
                    /// @todo: Check which functions depend on these BLOCKHAS_xxx values
                case ITEM_BOMB: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMBOMB); break;
                case ITEM_FLAME: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMFLAME); break;
                case ITEM_ROLLER: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMROLLER); break;
                case ITEM_KICK: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMKICK); break;
                case ITEM_SKULL: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSKULL); break;
                case ITEM_THROW: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMTHROW); break;
                case ITEM_PUNCH: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMPUNCH); break;
                case ITEM_REMOTE: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMREMOTE); break;
                case ITEM_SHIELD: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSHIELD); break;
                case ITEM_STRONGWEAK: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSTRONGWEAK); break;
                default: break;
                }
            }
            // If the item is burning
            else
            {
                // Record a burning item in the view
                SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMBURNING);
            }
        }
    }
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::Update(float DeltaTime)
{
    m_ArenaCloser.Update(DeltaTime);
    UpdateElements(DeltaTime);
    UpdateView();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArena::Display(void)
{
    // If a display object was specified
    if (m_pDisplay != NULL)
    {
        // Set the display origin where to draw the arena elements from
        m_pDisplay->SetOrigin(ARENA_POSITION_X, ARENA_POSITION_Y);
    }

    int Index;

    // Displaying the elements.
    // Every existing element of each type will be asked to display itself.

    for (Index = 0; Index < MaxExplosions(); Index++)
        if (GetExplosion(Index).Exist())
            GetExplosion(Index).Display();

    for (Index = 0; Index < MaxBombs(); Index++)
        if (GetBomb(Index).Exist())
            GetBomb(Index).Display();

    for (Index = 0; Index < MaxBombers(); Index++)
        if (GetBomber(Index).Exist())
            GetBomber(Index).Display();

    for (Index = 0; Index < MaxWalls(); Index++)
        if (GetWall(Index).Exist())
            GetWall(Index).Display();

    for (Index = 0; Index < MaxFloors(); Index++)
        if (GetFloor(Index).Exist())
            GetFloor(Index).Display();

    for (Index = 0; Index < MaxItems(); Index++)
        if (GetItem(Index).Exist())
            GetItem(Index).Display();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArena::WriteSnapshot(CArenaSnapshot& Snapshot)
{
    //theConsole.Write("write snapshot\n");

    Snapshot.Begin();

    // for each type of element
    //   write all elements (max number)
    int Index;

    for (Index = 0; Index < MaxFloors(); Index++)
        GetFloor(Index).WriteSnapshot(Snapshot);

    for (Index = 0; Index < MaxWalls(); Index++)
        GetWall(Index).WriteSnapshot(Snapshot);

    for (Index = 0; Index < MaxBombs(); Index++)
        GetBomb(Index).WriteSnapshot(Snapshot);

    for (Index = 0; Index < MaxItems(); Index++)
        GetItem(Index).WriteSnapshot(Snapshot);

    for (Index = 0; Index < MaxExplosions(); Index++)
        GetExplosion(Index).WriteSnapshot(Snapshot);

    for (Index = 0; Index < MaxBombers(); Index++)
        GetBomber(Index).WriteSnapshot(Snapshot);

    // write arenacloser
    m_ArenaCloser.WriteSnapshot(Snapshot);

    // TBlockHas
    for (int BlockX = 0; BlockX < ARENA_WIDTH; BlockX++)
        for (int BlockY = 0; BlockY < ARENA_HEIGHT; BlockY++)
            Snapshot.WriteInteger(m_BlockHas[BlockX][BlockY]);

    // Further attributes
    Snapshot.WriteBoolean(m_Prediction);
    Snapshot.WriteInteger(m_BombsInUse);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArena::ReadSnapshot(CArenaSnapshot& Snapshot)
{
    //theConsole.Write("read snapshot\n");

    Snapshot.Begin();

    // for each type of element
    //   read all elements (max number)
    int Index;

    for (Index = 0; Index < MaxFloors(); Index++)
        GetFloor(Index).ReadSnapshot(Snapshot);

    for (Index = 0; Index < MaxWalls(); Index++)
        GetWall(Index).ReadSnapshot(Snapshot);

    for (Index = 0; Index < MaxBombs(); Index++)
        GetBomb(Index).ReadSnapshot(Snapshot);

    for (Index = 0; Index < MaxItems(); Index++)
        GetItem(Index).ReadSnapshot(Snapshot);

    for (Index = 0; Index < MaxExplosions(); Index++)
        GetExplosion(Index).ReadSnapshot(Snapshot);

    for (Index = 0; Index < MaxBombers(); Index++)
        GetBomber(Index).ReadSnapshot(Snapshot);

    // read arenaclose
    m_ArenaCloser.ReadSnapshot(Snapshot);

    // TBlockHas
    for (int BlockX = 0; BlockX < ARENA_WIDTH; BlockX++)
        for (int BlockY = 0; BlockY < ARENA_HEIGHT; BlockY++)
            Snapshot.ReadInteger(&m_BlockHas[BlockX][BlockY]);

    // Further attributes
    Snapshot.ReadBoolean(&m_Prediction);
    Snapshot.ReadInteger(&m_BombsInUse);

    // update view
    UpdateView();

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArena::NewFloor(int BlockX, int BlockY, EBlockType BlockType)
{
    ASSERT(!m_Prediction);

    // Check coordinates
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    EFloorAction action = FLOORACTION_NONE;

    switch (BlockType) {
    case BLOCKTYPE_MOVEBOMB_RIGHT:  action = FLOORACTION_MOVEBOMB_RIGHT;    break;
    case BLOCKTYPE_MOVEBOMB_DOWN:   action = FLOORACTION_MOVEBOMB_DOWN;     break;
    case BLOCKTYPE_MOVEBOMB_LEFT:   action = FLOORACTION_MOVEBOMB_LEFT;     break;
    case BLOCKTYPE_MOVEBOMB_UP:     action = FLOORACTION_MOVEBOMB_UP;       break;
    default:                                                                break;
    }

    // Scan the floors
    for (int Index = 0; Index < MaxFloors(); Index++)
    {
        // If this floor slot is unused
        if (!m_Floors[Index].Exist())
        {
            // Create the floor on this unused slot and get out
            m_Floors[Index].SetArena(this);
            m_Floors[Index].SetDisplay(m_pDisplay);
            m_Floors[Index].SetSound(m_pSound);
            m_Floors[Index].Create(BlockX, BlockY, action);
            return;
        }
    }

    // No free slot, should never happen.
    ASSERT(0);
}

void CArena::NewWall(int BlockX, int BlockY, EWallType Type)
{
    ASSERT(!m_Prediction);

    // Check coordinates
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // Scan the walls
    for (int Index = 0; Index < MaxWalls(); Index++)
    {
        // If this wall slot is unused
        if (!m_Walls[Index].Exist())
        {
            // Create the wall on this unused slot and get out
            m_Walls[Index].SetArena(this);
            m_Walls[Index].SetDisplay(m_pDisplay);
            m_Walls[Index].SetSound(m_pSound);
            m_Walls[Index].Create(BlockX, BlockY, Type);
            return;
        }
    }

    // No free slot, should never happen.
    ASSERT(0);
}

void CArena::NewBomb(int BlockX, int BlockY, int FlameSize, float TimeLeft, int OwnerPlayer)
{
    ASSERT(!m_Prediction);
    ASSERT(m_BombsInUse < MaxBombs());

    // Check coordinates
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // Scan the bombs
    for (int Index = 0; Index < MaxBombs(); Index++)
    {
        // If this bomb slot is unused
        if (!m_Bombs[Index].Exist())
        {
            // Create the bomb on this unused slot and get out
            m_Bombs[Index].SetArena(this);
            m_Bombs[Index].SetDisplay(m_pDisplay);
            m_Bombs[Index].SetSound(m_pSound);
            m_Bombs[Index].Create(BlockX, BlockY, FlameSize, TimeLeft, OwnerPlayer);
            m_BombsInUse++;
            return;
        }
    }

    // No free slot, should never happen.
    ASSERT(0);
}

void CArena::NewExplosion(int BlockX, int BlockY, int FlameSize)
{
    ASSERT(!m_Prediction);

    // Check coordinates
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // Scan the explosions
    for (int Index = 0; Index < MaxExplosions(); Index++)
    {
        // If this explosion slot is unused
        if (!m_Explosions[Index].Exist())
        {
            // Create the explosion on this unused slot and get out
            m_Explosions[Index].SetArena(this);
            m_Explosions[Index].SetDisplay(m_pDisplay);
            m_Explosions[Index].SetSound(m_pSound);
            m_Explosions[Index].Create(BlockX, BlockY, FlameSize);
            return;
        }
    }

    // No free slot, should never happen.
    ASSERT(0);
}

void CArena::NewItem(int BlockX, int BlockY, EItemType Type, bool Fumes, bool FlyingRandom)
{
    ASSERT(!m_Prediction);

    // Check coordinates
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // Scan the items
    for (int Index = 0; Index < MaxItems(); Index++)
    {
        // If this item slot is unused
        if (!m_Items[Index].Exist())
        {
            // Create the item on this unused slot and get out
            m_Items[Index].SetArena(this);
            m_Items[Index].SetDisplay(m_pDisplay);
            m_Items[Index].SetSound(m_pSound);
            m_Items[Index].Create(BlockX, BlockY, Type, Fumes, FlyingRandom);

            // Update the arena view (fixes the bug where two items can be
            // created on the same block when two or more bombers are killed
            // at the same time).

            // Record the item in the view
            SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEM);

            // Record the item in the view according to its type
            switch (Type)
            {
            case ITEM_BOMB: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMBOMB);   break;
            case ITEM_FLAME: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMFLAME);  break;
            case ITEM_ROLLER: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMROLLER); break;
            case ITEM_KICK: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMKICK);   break;
            case ITEM_SKULL: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSKULL);  break;
            case ITEM_REMOTE: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMREMOTE);  break;
            case ITEM_SHIELD: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSHIELD);  break;
            case ITEM_PUNCH: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMPUNCH);  break;
            case ITEM_THROW: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMTHROW);  break;
            case ITEM_STRONGWEAK: SetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSTRONGWEAK);  break;
            default: break;
            }

            return;
        }
    }

    // No free slot, should never happen.
    ASSERT(0);
}

void CArena::NewBomber(int BlockX, int BlockY, int Player)
{
    ASSERT(!m_Prediction);

    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);
    ASSERT(Player >= 0 && Player < MAX_PLAYERS);
    ASSERT(!m_Bombers[Player].Exist());
    ASSERT(m_pOptions != NULL);

    // Create the bomber and map the player number with it
    m_Bombers[Player].SetArena(this);
    m_Bombers[Player].SetDisplay(m_pDisplay);
    m_Bombers[Player].SetSound(m_pSound);
    m_Bombers[Player].Create(BlockX, BlockY, Player, m_pOptions);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

EFloorAction CArena::GetFloorAction(int BlockX, int BlockY)
{
    ASSERT(BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT(BlockY >= 0 && BlockY < ARENA_HEIGHT);

    CFloor* floor;

    // Search the floor given by x and y
    for (int i = 0; i < MaxFloors(); i++) {

        floor = &(GetFloor(i));

        ASSERT(floor);

        if (floor->GetBlockX() == BlockX && floor->GetBlockY() == BlockY) {
            return floor->GetFloorAction();
        }

    }

    ASSERT(false);

    // Never reached, but prevents compiler message "warning C4715: 'CArena::GetFloorAction' : not all control paths return a value"
    return FLOORACTION_NONE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CArena::DeleteBomber(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxBombers());
    ASSERT(m_Bombers[Index].Exist());

    m_Bombers[Index].Destroy();
}

void CArena::DeleteBomb(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxBombs());
    ASSERT(m_Bombs[Index].Exist());
    ASSERT(m_BombsInUse > 0);

    m_Bombs[Index].Destroy();
    m_BombsInUse--;
}

void CArena::DeleteWall(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxWalls());
    ASSERT(m_Walls[Index].Exist());

    m_Walls[Index].Destroy();
}

void CArena::DeleteItem(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxItems());
    ASSERT(m_Items[Index].Exist());

    m_Items[Index].Destroy();
}

void CArena::DeleteExplosion(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxExplosions());
    ASSERT(m_Explosions[Index].Exist());

    m_Explosions[Index].Destroy();
}

void CArena::DeleteFloor(int Index)
{
    ASSERT(!m_Prediction);
    ASSERT(Index >= 0 && Index < MaxFloors());
    ASSERT(m_Floors[Index].Exist());

    m_Floors[Index].Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************
