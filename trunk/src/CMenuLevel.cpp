// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008 Markus Drescher
    Copyright (C) 2008 Bernd Arnold

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
 *  \file CMenuLevel.cpp
 *  \brief The menu where you can choose a level
 */

#include "StdAfx.h"
#include "CMenuLevel.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CFont.h"
#include "CMenu.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUMATCH_SPRITELAYER       1               //!< Sprite layer where to draw sprites

#define TITLE_TEXT_POSITION_Y       90              //!< Position Y of the title text that is centered on the X axis
#define WARNING_TEXT_POSITION_Y    350              //!< Position Y of a warning text that is centered on the X axis (will be displayed if starting points are missing for some players)

#define MINI_ARENA_POSITION_X   120
#define MINI_ARENA_POSITION_Y   (73+60)
#define TILE_POSITION_TO_BOMBER_POSITION    (-4)
#define MINI_ARENA_TILE_SIZE    16

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuLevel::CMenuLevel(void) : CMenuBase()
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuLevel::~CMenuLevel(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnCreate(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnDestroy(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnUp(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnDown(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnLeft(void)
{
    // If the first level is selected
    if (m_pOptions->GetLevel() == 0)
    {
        // Select the last level
        m_pOptions->SetLevel(m_pOptions->GetNumberOfLevels() - 1);
    }
    // If the first level is not selected
    else
    {
        // Select the previous level
        m_pOptions->SetLevel(m_pOptions->GetLevel() - 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnRight(void)
{
    // If the last level is selected
    if (m_pOptions->GetLevel() == m_pOptions->GetNumberOfLevels() - 1)
    {
        // Select the first level
        m_pOptions->SetLevel(0);
    }
    // If the last level is not selected
    else
    {
        // Select the next level
        m_pOptions->SetLevel(m_pOptions->GetLevel() + 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnPrevious(void)
{
    // Go to the previous screen
    Exit(MENUACTION_PREVIOUS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnNext(void)
{
    // Play the menu next sound
    m_pSound->PlaySample(SAMPLE_MENU_NEXT);

    // Go to the next screen
    Exit(MENUACTION_NEXT);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnUpdate(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuLevel::OnDisplay(void)
{
    int Player;
    // Set the right font text color and write the menu title string
    m_pFont->SetTextColor(FONTCOLOR_WHITE);
    m_pFont->DrawCenteredX(0, VIEW_WIDTH - 1, TITLE_TEXT_POSITION_Y, m_pOptions->GetLevelName());

    bool StartPointAvailable[MAX_PLAYERS];
    for (Player = 0; Player < MAX_PLAYERS; Player++)
        StartPointAvailable[Player] = false;

    // Scan all the blocks of the arena
    for (int X = 0; X < ARENA_WIDTH; X++)
    {
        for (int Y = 0; Y < ARENA_HEIGHT; Y++)
        {
            EBlockType BlockType = m_pOptions->GetBlockType(X, Y);

            if (BlockType == BLOCKTYPE_HARDWALL)
            {
                m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE,
                    MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE,
                    NULL,
                    NULL,
                    BMP_LEVEL_MINI_TILES,
                    0,
                    1,
                    1);
            }
            else if (BlockType == BLOCKTYPE_SOFTWALL || BlockType == BLOCKTYPE_RANDOM)
            {
                m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE,
                    MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE,
                    NULL,
                    NULL,
                    BMP_LEVEL_MINI_TILES,
                    1,
                    1,
                    1);
            }
            else
            {
                bool Shadow = (Y - 1 >= 0 &&
                    (m_pOptions->GetBlockType(X, Y - 1) == BLOCKTYPE_HARDWALL ||
                    m_pOptions->GetBlockType(X, Y - 1) == BLOCKTYPE_SOFTWALL ||
                    m_pOptions->GetBlockType(X, Y - 1) == BLOCKTYPE_RANDOM));

                m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE,
                    MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE,
                    NULL,
                    NULL,
                    BMP_LEVEL_MINI_TILES,
                    (Shadow ? 3 : 2),
                    1,
                    0);

                int spriteNumberAction = -1;

                switch (m_pOptions->GetBlockType(X, Y))
                {
                case BLOCKTYPE_MOVEBOMB_RIGHT:    spriteNumberAction = 4;   break;
                case BLOCKTYPE_MOVEBOMB_DOWN:     spriteNumberAction = 5;   break;
                case BLOCKTYPE_MOVEBOMB_LEFT:     spriteNumberAction = 6;   break;
                case BLOCKTYPE_MOVEBOMB_UP:       spriteNumberAction = 7;   break;
                case BLOCKTYPE_ITEM_BOMB:         spriteNumberAction = 8;   break;
                case BLOCKTYPE_ITEM_FLAME:        spriteNumberAction = 9;   break;
                case BLOCKTYPE_ITEM_KICK:         spriteNumberAction = 10;  break;
                case BLOCKTYPE_ITEM_ROLLER:       spriteNumberAction = 11;  break;
                case BLOCKTYPE_ITEM_SKULL:        spriteNumberAction = 12;  break;
                case BLOCKTYPE_ITEM_THROW:        spriteNumberAction = 13;  break;
                case BLOCKTYPE_ITEM_PUNCH:        spriteNumberAction = 14;  break;
                case BLOCKTYPE_ITEM_REMOTES:      spriteNumberAction = 15;  break;
                case BLOCKTYPE_ITEM_SHIELD:       spriteNumberAction = 16;  break;
                case BLOCKTYPE_ITEM_STRONGWEAK:   spriteNumberAction = 17;  break;
                default: break;
                }

                if (spriteNumberAction != -1)
                {
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE,
                        NULL,
                        NULL,
                        BMP_LEVEL_MINI_TILES,
                        spriteNumberAction,
                        2,
                        0);
                }

                switch (BlockType)
                {
                case BLOCKTYPE_WHITEBOMBER:
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        NULL, NULL, BMP_LEVEL_MINI_BOMBERS, 0, 1, 2);
                    StartPointAvailable[0] = true;
                    break;
                case BLOCKTYPE_BLACKBOMBER:
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        NULL, NULL, BMP_LEVEL_MINI_BOMBERS, 1, 1, 2);
                    StartPointAvailable[1] = true;
                    break;
                case BLOCKTYPE_REDBOMBER:
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        NULL, NULL, BMP_LEVEL_MINI_BOMBERS, 2, 1, 2);
                    StartPointAvailable[2] = true;
                    break;
                case BLOCKTYPE_BLUEBOMBER:
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        NULL, NULL, BMP_LEVEL_MINI_BOMBERS, 3, 1, 2);
                    StartPointAvailable[3] = true;
                    break;
                case BLOCKTYPE_GREENBOMBER:
                    m_pDisplay->DrawSprite(MINI_ARENA_POSITION_X + X * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        MINI_ARENA_POSITION_Y + Y * MINI_ARENA_TILE_SIZE + TILE_POSITION_TO_BOMBER_POSITION,
                        NULL, NULL, BMP_LEVEL_MINI_BOMBERS, 4, 1, 2);
                    StartPointAvailable[4] = true;
                    break;
                default:
                    break;
                }
            }
        }
    }

    // show warning if starting points are missing
    bool warningShown = false;
    for (Player = 0; Player < MAX_PLAYERS; Player++)
    {
        if (!StartPointAvailable[Player] && m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF)
        {
            m_pFont->SetTextColor(FONTCOLOR_RED);
            if (!warningShown)
            {
                m_pFont->Draw(MINI_ARENA_POSITION_X / 2, WARNING_TEXT_POSITION_Y, "NO START POS:");
                warningShown = true;
            }
            m_pDisplay->DrawSprite(VIEW_WIDTH - MINI_ARENA_POSITION_X - (MAX_PLAYERS - Player) * (MINI_ARENA_TILE_SIZE - TILE_POSITION_TO_BOMBER_POSITION * 2) + TILE_POSITION_TO_BOMBER_POSITION,
                WARNING_TEXT_POSITION_Y + TILE_POSITION_TO_BOMBER_POSITION,
                NULL, NULL, BMP_LEVEL_MINI_BOMBERS, Player, 1, 2);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
