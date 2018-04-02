// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
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
 *  \file CItem.cpp
 *  \brief Item a bomber may pick
 */

#include "StdAfx.h"
#include "CItem.h"
#include "CArena.h"
#include "CBomber.h"
#include "CDisplay.h"
#include "CExplosion.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Item sprite layer
#define ITEM_SPRITELAYER        10

// Fire sprite layer (when item is on fire)
#define FIRE_SPRITELAYER        50

// Falling wall sprite layer (this is the sprite layer for flying objects, bombers and item fires)
#define FLY_SPRITELAYER         50

// Times of item animation (in seconds)
#define ANIMITEM_TIME1      0.080f
#define ANIMITEM_TIME2      (ANIMITEM_TIME1 * 2)

// Times of item animation (in seconds)
#define ANIMFUMES_TIME1      0.100f
#define ANIMFUMES_TIME2      (ANIMFUMES_TIME1 * 2)
#define ANIMFUMES_TIME3      (ANIMFUMES_TIME1 * 3)

// Duration value for the anim fire. Used to speed up or slow down animation
#define ANIMFIRE_DURATION   0.9f

// Times of item fire animation (in seconds)
#define ANIMFIRE_TIME1      (0.070f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME2      (0.170f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME3      (0.310f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME4      (0.460f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME5      (0.560f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME6      (0.630f * ANIMFIRE_DURATION)
#define ANIMFIRE_TIME7      (0.720f * ANIMFIRE_DURATION)

// Item sprites according to item type
#define SPRITE_BOMB0        0
#define SPRITE_BOMB1        1
#define SPRITE_FLAME0       2
#define SPRITE_FLAME1       3
#define SPRITE_KICK0        4
#define SPRITE_KICK1        5
#define SPRITE_ROLLER0      6
#define SPRITE_ROLLER1      7
#define SPRITE_SKULL0       8
#define SPRITE_SKULL1       9
#define SPRITE_THROW0       10
#define SPRITE_THROW1       11
#define SPRITE_PUNCH0       12
#define SPRITE_PUNCH1       13
#define SPRITE_REMOTE0      14
#define SPRITE_REMOTE1      15
#define SPRITE_SHIELD0      16
#define SPRITE_SHIELD1      17
#define SPRITE_STRONGWEAK0  18
#define SPRITE_STRONGWEAK1  19

// Fume animation sprites
#define ANIM_FUMES_1    0
#define ANIM_FUMES_2    1
#define ANIM_FUMES_3    2

// Fire animation sprites
#define ANIM_FIRE1      0
#define ANIM_FIRE2      1
#define ANIM_FIRE3      2
#define ANIM_FIRE4      3
#define ANIM_FIRE5      4
#define ANIM_FIRE6      5
#define ANIM_FIRE7      6

// Offset when drawing fire sprites
#define FIRE_OFFSETX        (-10)
#define FIRE_OFFSETY        (-(54-32))

// Flying item animation sprites
#define ANIM_FLYING1            0
#define ANIM_FLYING2            1
#define ANIM_FLYING3            2
#define ANIM_FLYING4            1

// Flying item animation times  
#define ANIMFLYING_TIME1        0.050f
#define ANIMFLYING_TIME2        (ANIMFLYING_TIME1 * 2)
#define ANIMFLYING_TIME3        (ANIMFLYING_TIME1 * 3)
#define ANIMFLYING_TIME4        (ANIMFLYING_TIME1 * 4)

// Flying speed in pixels per second
#define FLYING_SPEED            200

#define MINIMUM_FLY_TIME        ((3.0f * BLOCK_SIZE) / FLYING_SPEED)

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CItem::CItem(void) : CElement()
{

    m_iX = -1;
    m_iY = -1;
    m_fX = float(m_iX);
    m_fY = float(m_iY);
    m_BlockX = -1;
    m_BlockY = -1;
    m_Burning = false;
    m_Timer = 0.0f;
    m_Dead = false;
    m_Type = ITEM_NONE;
    m_Fumes = false;
    m_FumeSprite = ANIM_FUMES_1;

    m_Sprite = 0;
    m_Sprite0 = 0;
    m_Sprite1 = 0;
    
    m_Flying = ITEMFLYING_NONE;
    m_FlyTime = 0.0f;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CItem::~CItem(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * @param BlockX the x coordinate in the arena where the item should be created
 * @param BlockY the y coordinate in the arena where the item should be created
 * @param Type the item type
 * @param Fumes fumes (??) TODO!
 * @param FlyingRandom is true, if the item should fly
 */

void CItem::Create(int BlockX, int BlockY, EItemType Type, bool Fumes, bool FlyingRandom)
{
    // The item type must be valid
    ASSERT(Type != ITEM_NONE);

    CElement::Create();

    m_iX = m_pArena->ToPosition(BlockX);
    m_iY = m_pArena->ToPosition(BlockY);
    m_fX = float(m_iX);
    m_fY = float(m_iY);
    m_BlockX = BlockX;
    m_BlockY = BlockY;
    m_Burning = false;
    m_Timer = 0.0f;
    m_Dead = false;
    m_Type = Type;
    m_Fumes = Fumes;
    m_FumeSprite = ANIM_FUMES_1;

    if (FlyingRandom)
    {
        //! There cannot be fumes if the item must fly.
        ASSERT(!Fumes);

        m_Flying = (EItemFlying)RANDOM(4);
        m_Sprite = ANIM_FLYING1;
        m_FlyTime = 0.0f;
    }
    else
    {
        m_Flying = ITEMFLYING_NONE;
        SetSprites();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CItem::Destroy(void)
{
    CElement::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CItem::SetSprites(void)
{
    // Set the sprite numbers according to the item type
    switch (m_Type)
    {
    case ITEM_BOMB:        m_Sprite0 = SPRITE_BOMB0;       m_Sprite1 = SPRITE_BOMB1;       break;
    case ITEM_FLAME:       m_Sprite0 = SPRITE_FLAME0;      m_Sprite1 = SPRITE_FLAME1;      break;
    case ITEM_KICK:        m_Sprite0 = SPRITE_KICK0;       m_Sprite1 = SPRITE_KICK1;       break;
    case ITEM_ROLLER:      m_Sprite0 = SPRITE_ROLLER0;     m_Sprite1 = SPRITE_ROLLER1;     break;
    case ITEM_SKULL:       m_Sprite0 = SPRITE_SKULL0;      m_Sprite1 = SPRITE_SKULL1;      break;
    case ITEM_THROW:       m_Sprite0 = SPRITE_THROW0;      m_Sprite1 = SPRITE_THROW1;      break;
    case ITEM_PUNCH:       m_Sprite0 = SPRITE_PUNCH0;      m_Sprite1 = SPRITE_PUNCH1;      break;
    case ITEM_REMOTE:      m_Sprite0 = SPRITE_REMOTE0;     m_Sprite1 = SPRITE_REMOTE1;     break;
    case ITEM_SHIELD:      m_Sprite0 = SPRITE_SHIELD0;     m_Sprite1 = SPRITE_SHIELD1;     break;
    case ITEM_STRONGWEAK:  m_Sprite0 = SPRITE_STRONGWEAK0; m_Sprite1 = SPRITE_STRONGWEAK1; break;
    default: break;
    }

    m_Sprite = m_Sprite0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called when contact with flames

void CItem::Burn(EBurnDirection BurnDirection)
{
    // If this item is not a skull item
    if (m_Type != ITEM_SKULL)
    {
        // Set burning only if it was not burning
        if (!m_Burning)
        {
            m_Timer = 0.0f;     // Reset animation timer
            m_Burning = true;   // Burning
        }
    }
    // If this item is a skull item and the burn direction is valid
    else if (BurnDirection != BURNDIRECTION_NONE)
    {
        switch (BurnDirection)
        {
        case BURNDIRECTION_UP: m_Flying = ITEMFLYING_UP;    break;
        case BURNDIRECTION_DOWN: m_Flying = ITEMFLYING_DOWN;  break;
        case BURNDIRECTION_LEFT: m_Flying = ITEMFLYING_LEFT;  break;
        case BURNDIRECTION_RIGHT: m_Flying = ITEMFLYING_RIGHT; break;
        default: break;
        }

        m_Sprite = ANIM_FLYING1;

        m_FlyTime = 0.0f;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Crush the item (immediate death)
// Called by rolling bombs and falling walls

void CItem::Crush()
{
    m_Dead = true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CItem::Update(float DeltaTime)
{
    // If the item is not flying
    if (m_Flying == ITEMFLYING_NONE)
    {
        // Don't update the item at all if it cannot be seen, ie. it is under
        // a soft wall that is not burning or it is under a wall that is not falling.
        // If it can be seen : no wall or burning wall or falling wall
        if (!m_pArena->IsWall(m_BlockX, m_BlockY) ||
            m_pArena->IsBurningWall(m_BlockX, m_BlockY) ||
            m_pArena->IsFallingWall(m_BlockX, m_BlockY))
        {
            // If item is not burning
            if (!m_Burning)
            {
                // Seek a bomber that is on the item block and give him the effects of the item.
                // If there is an alive bomber at the block where the item is
                if (m_pArena->IsAliveBomber(m_BlockX, m_BlockY) && !m_Burning)
                {
                    // Seek this bomber
                    for (int Index = 0; Index < m_pArena->MaxBombers(); Index++)
                    {
                        // Test existence and position and dead state
                        if (m_pArena->GetBomber(Index).Exist() &&
                            m_pArena->GetBomber(Index).GetBlockX() == m_BlockX &&
                            m_pArena->GetBomber(Index).GetBlockY() == m_BlockY &&
                            m_pArena->GetBomber(Index).IsAlive())
                        {
                            // Do item effect
                            m_pArena->GetBomber(Index).ItemEffect(m_Type);
                            // Item must be deleted
                            m_Dead = true;
                            break;
                        }
                    }
                }

                // If the fumes animation is not playing
                if (!m_Fumes)
                {
                    // Animate the item (flash)
                    if (m_Timer < ANIMITEM_TIME1)      m_Sprite = m_Sprite0;
                    else if (m_Timer < ANIMITEM_TIME2)      m_Sprite = m_Sprite1;
                    else
                    {
                        m_Sprite = m_Sprite0;
                        m_Timer = 0.0f;
                    }
                }
                // If the fumes animation is playing
                else
                {
                    // Animate the fumes
                    if (m_Timer < ANIMFUMES_TIME1)      m_FumeSprite = ANIM_FUMES_1;
                    else if (m_Timer < ANIMFUMES_TIME2)      m_FumeSprite = ANIM_FUMES_2;
                    else if (m_Timer < ANIMFUMES_TIME3)      m_FumeSprite = ANIM_FUMES_3;
                    else
                    {
                        // Stop the fumes animation
                        m_Fumes = false;

                        // Reset animation timer
                        m_Timer = 0.0f;
                    }
                }

                // Play animation
                m_Timer += DeltaTime;
            }
            // If the item is burning
            else
            {
                // Animate (item fire)
                if (m_Timer < ANIMFIRE_TIME1)  m_Sprite = ANIM_FIRE1;
                else if (m_Timer < ANIMFIRE_TIME2)  m_Sprite = ANIM_FIRE2;
                else if (m_Timer < ANIMFIRE_TIME3)  m_Sprite = ANIM_FIRE3;
                else if (m_Timer < ANIMFIRE_TIME4)  m_Sprite = ANIM_FIRE4;
                else if (m_Timer < ANIMFIRE_TIME5)  m_Sprite = ANIM_FIRE5;
                else if (m_Timer < ANIMFIRE_TIME6)  m_Sprite = ANIM_FIRE6;
                else if (m_Timer < ANIMFIRE_TIME7)  m_Sprite = ANIM_FIRE7;
                else
                {
                    m_Dead = true;  // The fire has ended, the item is burnt
                }

                // Play animation
                m_Timer += DeltaTime;
            }
        }
    }
    // If the item is flying
    else
    {
        // Animate
        if (m_Timer < ANIMFLYING_TIME1)   m_Sprite = ANIM_FLYING1;
        else if (m_Timer < ANIMFLYING_TIME2)   m_Sprite = ANIM_FLYING2;
        else if (m_Timer < ANIMFLYING_TIME3)   m_Sprite = ANIM_FLYING3;
        else if (m_Timer < ANIMFLYING_TIME4)   m_Sprite = ANIM_FLYING4;
        else
        {
            // Loop animation (hoping setting timer to 0
            // won't make the animation look weird)
            m_Timer = 0.0f;
        }

        // Play animation
        m_Timer += DeltaTime;

        m_FlyTime += DeltaTime;

        if (m_FlyTime >= MINIMUM_FLY_TIME)
        {
            int LandBlockX = m_pArena->ToBlock(m_iX + BLOCK_SIZE / 2);
            int LandBlockY = m_pArena->ToBlock(m_iY + BLOCK_SIZE / 2);

            if (LandBlockX >= 0 && LandBlockX < ARENA_WIDTH &&
                LandBlockY >= 0 && LandBlockY < ARENA_HEIGHT)
            {
                if (!m_pArena->IsWall(LandBlockX, LandBlockY) &&
                    !m_pArena->IsItem(LandBlockX, LandBlockY) &&
                    !m_pArena->IsBomber(LandBlockX, LandBlockY) &&
                    !m_pArena->IsBomb(LandBlockX, LandBlockY) &&
                    !m_pArena->IsFlame(LandBlockX, LandBlockY))
                {
                    m_Flying = ITEMFLYING_NONE;
                    m_FlyTime = 0.0f;
                    m_iX = m_pArena->ToPosition(LandBlockX);
                    m_iY = m_pArena->ToPosition(LandBlockY);
                    m_fX = float(m_iX);
                    m_fY = float(m_iY);
                    m_BlockX = LandBlockX;
                    m_BlockY = LandBlockY;
                    SetSprites();
                }
            }
        }

        // Make the item move according to its direction
        switch (m_Flying)
        {
        case ITEMFLYING_UP:
        {
            m_fY -= DeltaTime * FLYING_SPEED;

            if (m_fY < -20.0f)
            {
                m_fY = ARENA_HEIGHT * BLOCK_SIZE + 20.0f;
            }

            break;
        }

        case ITEMFLYING_DOWN:
        {
            m_fY += DeltaTime * FLYING_SPEED;

            if (m_fY > ARENA_HEIGHT * BLOCK_SIZE + 20.0f)
            {
                m_fY = -20.0f;
            }

            break;
        }

        case ITEMFLYING_LEFT:
        {
            m_fX -= DeltaTime * FLYING_SPEED;

            if (m_fX < -20.0f)
            {
                m_fX = ARENA_WIDTH * BLOCK_SIZE + 20.0f;
            }

            break;
        }

        case ITEMFLYING_RIGHT:
        {
            m_fX += DeltaTime * FLYING_SPEED;

            if (m_fX > ARENA_WIDTH * BLOCK_SIZE + 20.0f)
            {
                m_fX = -20.0f;
            }

            break;
        }

        default:
            break;
        }

        m_iX = int(m_fX);
        m_iY = int(m_fY);
    }

    return m_Dead;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CItem::Display(void)
{
    // If the item is not flying
    if (m_Flying == ITEMFLYING_NONE)
    {
        // Don't display the item at all if it cannot be seen, ie. it is under
        // a soft wall that is not burning or it is under a wall that is not falling.
        // If it can be seen : no wall or burning wall or falling wall
        if (!m_pArena->IsWall(m_BlockX, m_BlockY) ||
            m_pArena->IsBurningWall(m_BlockX, m_BlockY) ||
            m_pArena->IsFallingWall(m_BlockX, m_BlockY))
        {
            // If item is not burning
            if (!m_Burning)
            {
                // Draw the item sprite in the right layer. Priority is not used.
                m_pDisplay->DrawSprite(m_iX,
                    m_iY,
                    NULL,                            // Draw entire sprite
                    NULL,                            // No need to clip
                    BMP_ARENA_ITEM,
                    m_Sprite,
                    ITEM_SPRITELAYER,
                    0);

                // If the fumes animation is playing
                if (m_Fumes)
                {
                    int fumesOffset = 16;
                    // Draw the upper left part of the fumes
                    m_pDisplay->DrawSprite(m_iX - fumesOffset,
                        m_iY - fumesOffset,
                        NULL,
                        NULL,
                        BMP_ARENA_FUMES,
                        m_FumeSprite + 3 * 0,
                        50,
                        m_iY - 4);

                    // Draw the upper right part of the fumes
                    m_pDisplay->DrawSprite(m_iX + fumesOffset,
                        m_iY - fumesOffset,
                        NULL,
                        NULL,
                        BMP_ARENA_FUMES,
                        m_FumeSprite + 3 * 1,
                        50,
                        m_iY - 4);

                    // Draw the lower right part of the fumes
                    m_pDisplay->DrawSprite(m_iX + fumesOffset,
                        m_iY + fumesOffset,
                        NULL,
                        NULL,
                        BMP_ARENA_FUMES,
                        m_FumeSprite + 3 * 2,
                        50,
                        m_iY + 4);

                    // Draw the lower left part of the fumes
                    m_pDisplay->DrawSprite(m_iX - fumesOffset,
                        m_iY + fumesOffset,
                        NULL,
                        NULL,
                        BMP_ARENA_FUMES,
                        m_FumeSprite + 3 * 3,
                        50,
                        m_iY + 4);
                }
            }
            // If item is burning
            else
            {
                // Draw the fire sprite in the arena layer. Priority in layer depends on m_iY.
                m_pDisplay->DrawSprite(m_iX + FIRE_OFFSETX,
                    m_iY + FIRE_OFFSETY,
                    NULL,                            // Draw entire sprite
                    NULL,                            // No need to clip
                    BMP_ARENA_FIRE,
                    m_Sprite,
                    FIRE_SPRITELAYER,
                    m_iY);

                // Draw the item sprite in the right layer. Priority is not used.
                m_pDisplay->DrawSprite(m_iX,
                    m_iY,
                    NULL,                            // Draw entire sprite
                    NULL,                            // No need to clip
                    BMP_ARENA_ITEM,
                    m_Sprite0,
                    ITEM_SPRITELAYER,
                    PRIORITY_UNUSED);
            }
        }
    }
    // If the item is flying
    else
    {
        // Prepare a clipping rect to pass to the draw sprite function,
        // since the sprite can be out of the arena view.
        RECT Clip;
        Clip.left = 0;              // Left of the arena view
        Clip.top = 0;               // Top of the arena view
        Clip.right = VIEW_WIDTH;    // Right of the arena view
        Clip.bottom = VIEW_HEIGHT - 26;

        // Draw the flying item sprite. It's a flying object, put it in the appropriate layer.
        // The priority in layer depends on the Y position of the item.
        m_pDisplay->DrawSprite(m_iX,
            m_iY,
            NULL,                            // Draw entire sprite
            &Clip,                           // Clip sprite to arena view!!!
            BMP_ARENA_FLY,
            m_Sprite,
            FLY_SPRITELAYER,
            m_iY);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CItem::OnWriteSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.WriteFloat(m_fX);
    Snapshot.WriteFloat(m_fY);
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteInteger(m_Sprite);
    Snapshot.WriteInteger(m_Sprite0);
    Snapshot.WriteInteger(m_Sprite1);
    Snapshot.WriteFloat(m_Timer);
    Snapshot.WriteBoolean(m_Dead);
    Snapshot.WriteBoolean(m_Burning);
    Snapshot.WriteInteger(m_Type);
    Snapshot.WriteBoolean(m_Fumes);
    Snapshot.WriteInteger(m_FumeSprite);
    Snapshot.WriteInteger(m_Flying);
    Snapshot.WriteFloat(m_FlyTime);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CItem::OnReadSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.ReadFloat(&m_fX);
    Snapshot.ReadFloat(&m_fY);
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadInteger(&m_Sprite);
    Snapshot.ReadInteger(&m_Sprite0);
    Snapshot.ReadInteger(&m_Sprite1);
    Snapshot.ReadFloat(&m_Timer);
    Snapshot.ReadBoolean(&m_Dead);
    Snapshot.ReadBoolean(&m_Burning);
    Snapshot.ReadInteger((int*)&m_Type);
    Snapshot.ReadBoolean(&m_Fumes);
    Snapshot.ReadInteger(&m_FumeSprite);
    Snapshot.ReadInteger((int*)&m_Flying);
    Snapshot.ReadFloat(&m_FlyTime);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * Creates a bunch of items in the arena, according to the place where items should be (on the floor, under
 * walls...), and how many items of each type to create. This is used by the arena when building a new arena,
 * and by bombers that return the items they pick when they die.
 * Returns whether at least one item was created.
 *
 * @see CArena::Create()
 */

bool CItem::CreateItems(CArena *pArena,
    EItemPlace ItemPlace,
    int NumberOfItemBombs,
    int NumberOfItemFlames,
    int NumberOfItemRollers,
    int NumberOfItemKicks,
    int NumberOfItemSkulls,
    int NumberOfItemThrow,
    int NumberOfItemPunch,
    int NumberOfItemRemote,
    int NumberOfItemShield,
    int NumberOfItemStrongWeak)
{
    int X, Y;
    int Index;
    bool Fumes = false;
    bool Created = false;

    // This array tells if it is allowed to create an item at this place
    // It will first be filled using tests, then an array containing the
    // coordinates of the possible places will be filled.
    bool PossibleGrid[ARENA_WIDTH][ARENA_HEIGHT];

    // Set the possible places for items
    // If items must be created under soft walls
    if (ItemPlace == ITEMPLACE_SOFTWALLS)
    {
        // Set the possible places for items
        for (X = 0; X < ARENA_WIDTH; X++)
        {
            for (Y = 0; Y < ARENA_HEIGHT; Y++)
            {
                PossibleGrid[X][Y] = pArena->IsSoftWall(X, Y);
            }
        }

        // Make no fumes when the item appears
        Fumes = false;
    }
    // If items must be created on the floor
    else if (ItemPlace == ITEMPLACE_FLOOR)
    {
        // Set the possible places for items
        for (X = 0; X < ARENA_WIDTH; X++)
        {
            for (Y = 0; Y < ARENA_HEIGHT; Y++)
            {
                PossibleGrid[X][Y] = !pArena->IsWall(X, Y) &&
                    !pArena->IsItem(X, Y) &&
                    !pArena->IsAliveBomber(X, Y) &&
                    !pArena->IsBomb(X, Y) &&
                    !pArena->IsFlame(X, Y);
            }
        }

        // Make fumes when the item appears
        Fumes = true;
    }

    // Block Position structure used next
    struct SPosition
    {
        int X;
        int Y;
    };

    // This array contains the possible places coordinates.
    // It will be filled using the PossibleGrid array.
    SPosition Possible[ARENA_WIDTH * ARENA_HEIGHT];

    int CountPossible = 0;      // Number of possible places

    // Now count the number of possible places for items and save the possible places
    for (X = 0; X < ARENA_WIDTH; X++)
    {
        for (Y = 0; Y < ARENA_HEIGHT; Y++)
        {
            // If this is possible here, one more
            if (PossibleGrid[X][Y])
            {
                // Save position
                Possible[CountPossible].X = X;
                Possible[CountPossible].Y = Y;
                // One more
                CountPossible++;
            }
        }
    }

    // If there are some possible places
    if (CountPossible > 0)
    {
        // Reduce number of items to create until it's possible to create them all
        while (NumberOfItemBombs +
            NumberOfItemFlames +
            NumberOfItemRollers +
            NumberOfItemKicks +
            NumberOfItemSkulls +
            NumberOfItemThrow +
            NumberOfItemPunch +
            NumberOfItemRemote +
            NumberOfItemShield + 
            NumberOfItemStrongWeak > CountPossible)
        {
            // Choose a type of item and reduce the number
            switch (RANDOM(NUMBER_OF_ITEMS))
            {
            case 0: if (NumberOfItemBombs > 0)      NumberOfItemBombs--;        break;
            case 1: if (NumberOfItemFlames > 0)     NumberOfItemFlames--;       break;
            case 2: if (NumberOfItemRollers > 0)    NumberOfItemRollers--;      break;
            case 3: if (NumberOfItemKicks > 0)      NumberOfItemKicks--;        break;
            case 4: if (NumberOfItemSkulls > 0)     NumberOfItemSkulls--;       break;
            case 5: if (NumberOfItemThrow > 0)      NumberOfItemThrow--;        break;
            case 6: if (NumberOfItemPunch > 0)      NumberOfItemPunch--;        break;
            case 7: if (NumberOfItemRemote > 0)     NumberOfItemRemote--;       break;
            case 8: if (NumberOfItemShield > 0)     NumberOfItemShield--;       break;
            case 9: if (NumberOfItemStrongWeak > 0) NumberOfItemStrongWeak--;   break;
            }
        }

        // While there are still items to create
        while (NumberOfItemBombs > 0 ||
            NumberOfItemFlames > 0 ||
            NumberOfItemRollers > 0 ||
            NumberOfItemKicks > 0 ||
            NumberOfItemSkulls > 0 ||
            NumberOfItemThrow > 0 ||
            NumberOfItemPunch > 0 ||
            NumberOfItemRemote > 0 ||
            NumberOfItemShield > 0 ||
            NumberOfItemStrongWeak > 0
            )
        {
            // Choose a type of item to create
            EItemType Type = ITEM_NONE;

            // If there are still items of this type to create
            if (NumberOfItemBombs > 0)
            {
                // Set this type
                Type = ITEM_BOMB;
                // One less to create
                NumberOfItemBombs--;
            }
            else if (NumberOfItemFlames > 0)
            {
                Type = ITEM_FLAME;
                NumberOfItemFlames--;
            }
            else if (NumberOfItemRollers > 0)
            {
                Type = ITEM_ROLLER;
                NumberOfItemRollers--;
            }
            else if (NumberOfItemKicks > 0)
            {
                Type = ITEM_KICK;
                NumberOfItemKicks--;
            }
            else if (NumberOfItemSkulls > 0)
            {
                Type = ITEM_SKULL;
                NumberOfItemSkulls--;
            }
            else if (NumberOfItemThrow > 0)
            {
                Type = ITEM_THROW;
                NumberOfItemThrow--;
            }
            else if (NumberOfItemPunch > 0)
            {
                Type = ITEM_PUNCH;
                NumberOfItemPunch--;
            }
            else if (NumberOfItemRemote > 0)
            {
                Type = ITEM_REMOTE;
                NumberOfItemRemote--;
            }
            else if (NumberOfItemShield > 0)
            {
                Type = ITEM_SHIELD;
                NumberOfItemShield--;
            }
            else if (NumberOfItemStrongWeak > 0)
            {
                Type = ITEM_STRONGWEAK;
                NumberOfItemStrongWeak--;
            }

            // Try a random index in the possible places array
            Index = RANDOM(CountPossible);

            ASSERT(Type != ITEM_NONE);

            // Create an item with the chosen type, at the possible position
            pArena->NewItem(Possible[Index].X, Possible[Index].Y, Type, Fumes, false);

            // At least one item was created
            Created = true;

            // It's not possible to create an item at this position anymore
            Possible[Index].X = Possible[CountPossible - 1].X;    // The last saved possible position overwrites this position
            Possible[Index].Y = Possible[CountPossible - 1].Y;
            CountPossible--;                                    // And the count is decreased to reduce array size
        }
    }

    return Created;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
