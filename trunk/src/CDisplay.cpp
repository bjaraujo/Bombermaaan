// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold
    Copyright (C) 2008 Jerome Bigot
    Copyright (C) 2008 Markus Drescher

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
 *  \file CDisplay.cpp
 *  \brief Load and display sprites, change screen mode
 */

#include "StdAfx.h"
#include "CDisplay.h"

#include "res.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDisplay::CDisplay(void)
{
    // No connection to the resources yet
    m_hModule = NULL;

    // Reset view origin
    m_ViewOriginX = 0;
    m_ViewOriginY = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDisplay::~CDisplay(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::Create(int Width, int Height, bool FullScreen)
{
    // Check if we have a connection with the resources
#ifndef LOAD_RESOURCES_FROM_FILES
    ASSERT(m_hModule != NULL);
#endif

    int Depth = 32;

    // If no display mode has been set yet or the current display mode is not the right one
#ifdef DIRECTX
    if (!m_VideoDX.IsModeSet(Width, Height, Depth, FullScreen))
#else
    if (!m_VideoSDL.IsModeSet(Width, Height, Depth, FullScreen))
#endif
    {
        // Destroy DirectDraw/SDLVideo interface and the sprite tables
        Destroy();

        // If DirectDraw/SDLVideo object creation failed
#ifdef DIRECTX
        if (!m_VideoDX.Create(Width, Height, Depth, FullScreen))
#else
        if (!m_VideoSDL.Create(Width, Height, Depth, FullScreen))
#endif
        {
            // Get out
            return false;
        }

#ifdef DIRECTX
        // Set the RGB color for transparent pixels in sprites
        // If it failed
        if (!m_VideoDX.SetTransparentColor (0, 255, 0))
        {
            // Get out, it failed
            return false;
        }
#endif

        if (!LoadSprites(1, 1, 82, 41, false, BMP_GREEN_BACKGROUND_SOLID, "green_background_solid.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_BLUE_BACKGROUND_SOLID, "blue_background_solid.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_PURPLE_BACKGROUND_SOLID, "purple_background_solid.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_RED_BACKGROUND_SOLID, "red_background_solid.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_GREEN_BACKGROUND_BOMB, "green_background_bomb.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_BLUE_BACKGROUND_BOMB, "blue_background_bomb.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_PURPLE_BACKGROUND_BOMB, "purple_background_bomb.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_RED_BACKGROUND_BOMB, "red_background_bomb.bmp") ||
            !LoadSprites(2, 1, 32, 32, false, BMP_ARENA_FLOOR, "arena_floor.bmp") ||
            !LoadSprites(7, 1, 32, 32, true, BMP_ARENA_WALL, "arena_wall.bmp") ||
            !LoadSprites(28, 1, 32, 32, true, BMP_ARENA_FLAME, "arena_flame.bmp") ||
            !LoadSprites(20, 1, 32, 32, false, BMP_ARENA_ITEM, "arena_item.bmp") ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_BOMB, "arena_bomb.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK, "arena_bomber_walk.bmp") ||
            !LoadSprites(7, 1, 52, 54, true, BMP_ARENA_FIRE, "arena_fire.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK_HOLD, "arena_bomber_walk_hold.bmp") ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_FLY, "arena_fly.bmp") ||
            !LoadSprites(1, 1, 480, 26, false, BMP_BOARD_BACKGROUND, "board_background.bmp") ||
            !LoadSprites(12, 1, 7, 10, true, BMP_BOARD_TIME, "board_time.bmp") ||
            !LoadSprites(2, 1, 15, 7, true, BMP_BOARD_CLOCK_TOP, "board_clock_top.bmp") ||
            !LoadSprites(8, 1, 15, 13, true, BMP_BOARD_CLOCK_BOTTOM, "board_clock_bottom.bmp") ||
            !LoadSprites(6, 1, 6, 8, true, BMP_BOARD_SCORE, "board_score.bmp") ||
            !LoadSprites(5, 2, 14, 14, true, BMP_BOARD_HEADS, "board_heads.bmp") ||
            !LoadSprites(1, 1, 480, 442, false, BMP_DRAWGAME_MAIN, "drawgame_main.bmp") ||
            !LoadSprites(2, 1, 68, 96, false, BMP_DRAWGAME_FLAG, "drawgame_flag.bmp") ||
            !LoadSprites(4, 1, 20, 62, true, BMP_DRAWGAME_FUMES, "drawgame_fumes.bmp") ||
            !LoadSprites(4, 5, 24, 32, true, BMP_WINNER_BOMBER, "winner_bomber.bmp") ||
            !LoadSprites(16, 1, 22, 22, true, BMP_WINNER_COIN, "winner_coin.bmp") ||
            !LoadSprites(4, 1, 6, 6, true, BMP_WINNER_LIGHTS, "winner_lights.bmp") ||
            !LoadSprites(4, 2, 16, 16, true, BMP_WINNER_SPARKS, "winner_sparks.bmp") ||
            !LoadSprites(1, 1, 158, 16, true, BMP_WINNER_TITLE, "winner_title.bmp") ||
            !LoadSprites(1, 1, 32, 405, false, BMP_VICTORY_WALL, "victory_wall.bmp") ||
            !LoadSprites(9, 1, 14, 16, true, BMP_VICTORY_CROWD, "victory_crowd.bmp") ||
            !LoadSprites(14, 5, 36, 61, true, BMP_VICTORY_BOMBER, "victory_bomber.bmp") ||
            !LoadSprites(1, 1, 192, 60, true, BMP_VICTORY_TITLE, "victory_title.bmp") ||
            !LoadSprites(46, 6, 10, 10, true, BMP_GLOBAL_FONT, "global_font.bmp") ||
            !LoadSprites(5, 2, 21, 19, true, BMP_MENU_BOMBER, "menu_bomber.bmp") ||
            !LoadSprites(1, 1, 420, 362, true, BMP_MENU_FRAME_1, "menu_frame_1.bmp") ||
            !LoadSprites(2, 1, 15, 16, true, BMP_MENU_HAND, "menu_hand.bmp") ||
            !LoadSprites(5, 1, 23, 23, true, BMP_WINNER_CROSS, "winner_cross.bmp") ||
            !LoadSprites(5, 5, 14, 15, true, BMP_VICTORY_CONFETTIS_LARGE, "victory_confettis_large.bmp") ||
            !LoadSprites(5, 5, 13, 14, true, BMP_VICTORY_CONFETTIS_MEDIUM, "victory_confettis_medium.bmp") ||
            !LoadSprites(5, 5, 10, 10, true, BMP_VICTORY_CONFETTIS_SMALL, "victory_confettis_small.bmp") ||
            !LoadSprites(1, 1, 200, 36, true, BMP_PAUSE, "arena_pause.bmp") ||
            !LoadSprites(1, 1, 200, 36, true, BMP_HURRY, "arena_hurry.bmp") ||
            !LoadSprites(1, 1, 154, 93, true, BMP_MENU_FRAME_2, "menu_frame_2.bmp") ||
            !LoadSprites(3, 4, 32, 32, true, BMP_ARENA_FUMES, "arena_fumes.bmp") ||
            !LoadSprites(1, 1, 14, 14, true, BMP_BOARD_DRAWGAME, "board_drawgame.bmp") ||
            !LoadSprites(1, 1, 480, 442, false, BMP_TITLE_BACKGROUND, "title_background.bmp") ||
            !LoadSprites(1, 1, 480, 126, true, BMP_TITLE_BOMBERS, "title_bombers.bmp") ||
            !LoadSprites(1, 1, 298, 139, true, BMP_TITLE_TITLE, "title_title.bmp") ||
            !LoadSprites(2, 6, 128, 26, true, BMP_TITLE_MENU_ITEMS, "title_menu_items.bmp") ||
            !LoadSprites(1, 1, 138, 46, true, BMP_TITLE_CLOUD_1, "title_cloud_1.bmp") ||
            !LoadSprites(1, 1, 106, 46, true, BMP_TITLE_CLOUD_2, "title_cloud_2.bmp") ||
            !LoadSprites(1, 1, 66, 22, true, BMP_TITLE_CLOUD_3, "title_cloud_3.bmp") ||
            !LoadSprites(18, 1, 16, 16, true, BMP_LEVEL_MINI_TILES, "level_mini_tiles.bmp") ||
            !LoadSprites(5, 1, 24, 20, true, BMP_LEVEL_MINI_BOMBERS, "level_mini_bombers.bmp") ||
            !LoadSprites(7, 5, 42, 44, true, BMP_ARENA_BOMBER_DEATH, "arena_bomber_death.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_LIFT, "arena_bomber_lift.bmp") ||
            !LoadSprites(20, 8, 42, 44, true, BMP_ARENA_BOMBER_THROW, "arena_bomber_throw.bmp") ||
            !LoadSprites(8, 8, 42, 44, true, BMP_ARENA_BOMBER_PUNCH, "arena_bomber_punch.bmp") ||
            !LoadSprites(4, 8, 42, 44, true, BMP_ARENA_BOMBER_STUNT, "arena_bomber_stunt.bmp") ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_ARROWS, "arena_arrows.bmp") ||
            !LoadSprites(1, 1, 30, 32, true, BMP_MENU_HAND_TITLE, "menu_hand_title.bmp") ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_REMOTE_BOMB, "arena_remote_bomb.bmp")
            )
        {
            // Failure, get out (error is logged by the LoadSprites() method)
            return false;
        }

        // Save origin where to draw from
        m_ViewOriginX = (Width - VIEW_WIDTH) / 2;
        m_ViewOriginY = (Height - VIEW_HEIGHT) / 2;

#ifdef DIRECTX
        m_VideoDX.SetOrigin(m_ViewOriginX, m_ViewOriginY);
#else
        m_VideoSDL.SetOrigin(m_ViewOriginX, m_ViewOriginY);
#endif
    }

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::Create(EDisplayMode DisplayMode)
{
    ASSERT(DisplayMode != DISPLAYMODE_NONE);

    // According to the new display mode to set
    switch (DisplayMode)
    {
    case DISPLAYMODE_FULL1: return Create(320, 240, true);
    case DISPLAYMODE_FULL2: return Create(512, 384, true);
    case DISPLAYMODE_FULL3: return Create(640, 480, true);
    case DISPLAYMODE_WINDOWED: return Create(VIEW_WIDTH, VIEW_HEIGHT, false);
    default: return false; // Should never happen
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDisplay::Destroy(void)
{
    // Destroy DirectDraw/SDLVideo interface and the sprite tables
#ifdef DIRECTX
    m_VideoDX.Destroy();
#else
    m_VideoSDL.Destroy();
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::IsDisplayModeAvailable(EDisplayMode DisplayMode)
{
    ASSERT(DisplayMode != DISPLAYMODE_NONE);

    // According to the display mode to test
    switch (DisplayMode)
    {
#ifdef DIRECTX
    case DISPLAYMODE_FULL1: return m_VideoDX.IsModeAvailable(320, 240, 32);
    case DISPLAYMODE_FULL2: return m_VideoDX.IsModeAvailable(512, 384, 32);
    case DISPLAYMODE_FULL3: return m_VideoDX.IsModeAvailable(640, 480, 32);
#else
    case DISPLAYMODE_FULL1: return m_VideoSDL.IsModeAvailable(320, 240, 32);
    case DISPLAYMODE_FULL2: return m_VideoSDL.IsModeAvailable(512, 384, 32);
    case DISPLAYMODE_FULL3: return m_VideoSDL.IsModeAvailable(640, 480, 32);
#endif
    case DISPLAYMODE_WINDOWED: return true;
    default: return false; // Should never happen
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID, const char *file)
{

#ifndef LOAD_RESOURCES_FROM_FILES

#ifdef WIN32
    // Load the bitmap as a resource
    HBITMAP hBitmap = (HBITMAP)LoadImage(m_hModule, MAKEINTRESOURCE(BMP_ID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    // If it failed
    if (hBitmap == NULL)
    {
        // Log failure
        theLog.WriteLine("Display         => !!! Could not load resource image (%d) and create handle to bitmap.", BMP_ID);
        theLog.LogLastError();

        // Get out
        return false;
    }
#endif
    // Create the sprites by giving the sprite table information and the handle to the bitmap.
    // If it fails
#ifdef DIRECTX
    if (!m_VideoDX.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID, hBitmap))
#else
#ifdef WIN32
    if (!m_VideoSDL.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID, hBitmap))
#else
    if (!m_VideoSDL.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID))
#endif
#endif
    {
        // Get out, failure
        return false;
    }

#ifdef WIN32
    // We no longer need the hBitmap so delete it
    // If it fails
    if (DeleteObject(hBitmap) == 0)
    {
        // Log failure
        theLog.WriteLine("Display         => !!! Could not delete handle to bitmap.");
        theLog.LogLastError();

        // Get out, failure
        return false;
    }
#endif

#else

#ifdef DIRECTX
    if (!m_VideoDX.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID, file))
#else
    if (!m_VideoSDL.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID, file))
#endif
    {
        // Get out, failure
        return false;
    }

#endif

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

