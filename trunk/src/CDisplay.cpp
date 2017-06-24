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
#ifdef DIRECTX_DRAW
    if (!m_DirectDraw.IsModeSet (Width, Height, Depth, FullScreen))
#else
    if (!m_SDLVideo.IsModeSet(Width, Height, Depth, FullScreen))
#endif
    {
        // Destroy DirectDraw/SDLVideo interface and the sprite tables
        Destroy();

        // If DirectDraw/SDLVideo object creation failed
#ifdef DIRECTX_DRAW
        if (!m_DirectDraw.Create (Width, Height, Depth, FullScreen))
#else
        if (!m_SDLVideo.Create(Width, Height, Depth, FullScreen))
#endif
        {
            // Get out
            return false;
        }

#ifdef DIRECTX_DRAW
        // Set the RGB color for transparent pixels in sprites
        // If it failed
        if (!m_DirectDraw.SetTransparentColor (0, 255, 0))
        {
            // Get out, it failed
            return false;
        }
#endif

#ifndef LOAD_RESOURCES_FROM_FILES
        // Load the sprite tables. If at least one sprite table could not be loaded
        if (
            !LoadSprites(2, 1, 32, 32, false, BMP_ARENA_FLOOR) ||
            !LoadSprites(7, 1, 32, 32, true, BMP_ARENA_WALL) ||
            !LoadSprites(28, 1, 32, 32, true, BMP_ARENA_FLAME) ||
            !LoadSprites(18, 1, 32, 32, false, BMP_ARENA_ITEM) ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_BOMB) ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK) ||
            !LoadSprites(7, 1, 52, 54, true, BMP_ARENA_FIRE) ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK_HOLD) ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_FLY) ||
            !LoadSprites(1, 1, 480, 26, false, BMP_BOARD_BACKGROUND) ||
            !LoadSprites(12, 1, 7, 10, true, BMP_BOARD_TIME) || // 10
            !LoadSprites(2, 1, 15, 7, true, BMP_BOARD_CLOCK_TOP) ||
            !LoadSprites(8, 1, 15, 13, true, BMP_BOARD_CLOCK_BOTTOM) ||
            !LoadSprites(6, 1, 6, 8, true, BMP_BOARD_SCORE) ||
            !LoadSprites(5, 2, 14, 14, true, BMP_BOARD_HEADS) ||
            !LoadSprites(1, 1, 480, 442, false, BMP_DRAWGAME_MAIN) ||
            !LoadSprites(2, 1, 68, 96, false, BMP_DRAWGAME_FLAG) ||
            !LoadSprites(4, 1, 20, 62, true, BMP_DRAWGAME_FUMES) ||
            !LoadSprites(4, 5, 24, 32, true, BMP_WINNER_BOMBER) ||
            !LoadSprites(1, 1, 64, 32, false, BMP_WINNER_BACKGROUND_1) ||
            !LoadSprites(16, 1, 22, 22, true, BMP_WINNER_COIN) || // 20
            !LoadSprites(4, 1, 6, 6, true, BMP_WINNER_LIGHTS) ||
            !LoadSprites(4, 2, 16, 16, true, BMP_WINNER_SPARKS) ||
            !LoadSprites(1, 1, 158, 16, true, BMP_WINNER_TITLE) ||
            !LoadSprites(1, 1, 32, 405, false, BMP_VICTORY_WALL) ||
            !LoadSprites(9, 1, 14, 16, true, BMP_VICTORY_CROWD) ||
            !LoadSprites(14, 5, 36, 61, true, BMP_VICTORY_BOMBER) ||
            !LoadSprites(1, 1, 192, 60, true, BMP_VICTORY_TITLE) ||
            !LoadSprites(46, 6, 10, 10, true, BMP_GLOBAL_FONT) ||
            !LoadSprites(1, 1, 64, 32, false, BMP_MENU_BACKGROUND_1) ||
            !LoadSprites(5, 2, 21, 19, true, BMP_MENU_BOMBER) || // 30
            !LoadSprites(1, 1, 420, 362, true, BMP_MENU_FRAME_1) ||
            !LoadSprites(2, 1, 15, 16, true, BMP_MENU_HAND) ||
            !LoadSprites(5, 1, 23, 23, true, BMP_WINNER_CROSS) ||
            !LoadSprites(5, 5, 14, 15, true, BMP_VICTORY_CONFETTIS_LARGE) ||
            !LoadSprites(5, 5, 13, 14, true, BMP_VICTORY_CONFETTIS_MEDIUM) ||
            !LoadSprites(5, 5, 10, 10, true, BMP_VICTORY_CONFETTIS_SMALL) ||
            !LoadSprites(1, 1, 200, 36, true, BMP_PAUSE) ||
            !LoadSprites(1, 1, 200, 36, true, BMP_HURRY) ||
            !LoadSprites(1, 1, 154, 93, true, BMP_MENU_FRAME_2) ||
            !LoadSprites(3, 4, 32, 32, true, BMP_ARENA_FUMES) || // 40
            !LoadSprites(1, 1, 14, 14, true, BMP_BOARD_DRAWGAME) ||
            !LoadSprites(1, 1, 480, 442, false, BMP_TITLE_BACKGROUND) ||
            !LoadSprites(1, 1, 480, 126, true, BMP_TITLE_BOMBERS) ||
            !LoadSprites(1, 1, 298, 139, true, BMP_TITLE_TITLE) ||
            !LoadSprites(2, 5, 128, 26, true, BMP_TITLE_MENU_ITEMS) ||
            !LoadSprites(1, 1, 64, 32, false, BMP_CONTROLS_BACKGROUND_1) ||
            !LoadSprites(1, 1, 82, 41, false, BMP_CONTROLS_BACKGROUND_2) ||
            !LoadSprites(1, 1, 82, 41, false, BMP_CONTROLS_BACKGROUND_3) ||
            !LoadSprites(1, 1, 82, 41, false, BMP_MENU_BACKGROUND_2) ||
            !LoadSprites(1, 1, 82, 41, false, BMP_MENU_BACKGROUND_3) || // 50
            !LoadSprites(1, 1, 82, 41, false, BMP_WINNER_BACKGROUND_2) ||
            !LoadSprites(1, 1, 82, 41, false, BMP_WINNER_BACKGROUND_3) ||
            !LoadSprites(1, 1, 138, 46, true, BMP_TITLE_CLOUD_1) ||
            !LoadSprites(1, 1, 106, 46, true, BMP_TITLE_CLOUD_2) ||
            !LoadSprites(1, 1, 66, 22, true, BMP_TITLE_CLOUD_3) ||
            !LoadSprites(17, 1, 16, 16, true, BMP_LEVEL_MINI_TILES) ||
            !LoadSprites(5, 1, 24, 20, true, BMP_LEVEL_MINI_BOMBERS) ||
            !LoadSprites(7, 5, 42, 44, true, BMP_ARENA_BOMBER_DEATH) ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_LIFT) ||
            !LoadSprites(20, 8, 42, 44, true, BMP_ARENA_BOMBER_THROW) || // 60
            !LoadSprites(8, 8, 42, 44, true, BMP_ARENA_BOMBER_PUNCH) ||
            !LoadSprites(4, 8, 42, 44, true, BMP_ARENA_BOMBER_STUNT) ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_ARROWS) || // 63
            !LoadSprites(1, 1, 30, 32, true, BMP_MENU_HAND_TITLE) ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_REMOTE_BOMB)
            )
        {
            // Failure, get out (error is logged by the LoadSprites() method)
            return false;
        }
#else
        if (
            !LoadSprites(2, 1, 32, 32, false, BMP_ARENA_FLOOR, "images/arena_floor.bmp") ||
            !LoadSprites(7, 1, 32, 32, true, BMP_ARENA_WALL, "images/arena_wall.bmp") ||
            !LoadSprites(28, 1, 32, 32, true, BMP_ARENA_FLAME, "images/arena_flame.bmp") ||
            !LoadSprites(18, 1, 32, 32, false, BMP_ARENA_ITEM, "images/arena_item.bmp") ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_BOMB, "images/arena_bomb.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK, "images/arena_bomber_walk.bmp") ||
            !LoadSprites(7, 1, 52, 54, true, BMP_ARENA_FIRE, "images/arena_fire.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_WALK_HOLD, "images/arena_bomber_walk_hold.bmp") ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_FLY, "images/arena_fly.bmp") ||
            !LoadSprites(1, 1, 480, 26, false, BMP_BOARD_BACKGROUND, "images/board_background.bmp") ||
            !LoadSprites(12, 1, 7, 10, true, BMP_BOARD_TIME, "images/board_time.bmp") || // 10
            !LoadSprites(2, 1, 15, 7, true, BMP_BOARD_CLOCK_TOP, "images/board_clock_top.bmp") ||
            !LoadSprites(8, 1, 15, 13, true, BMP_BOARD_CLOCK_BOTTOM, "images/board_clock_bottom.bmp") ||
            !LoadSprites(6, 1, 6, 8, true, BMP_BOARD_SCORE, "images/board_score.bmp") ||
            !LoadSprites(5, 2, 14, 14, true, BMP_BOARD_HEADS, "images/board_heads.bmp") ||
            !LoadSprites(1, 1, 480, 442, false, BMP_DRAWGAME_MAIN, "images/drawgame_main.bmp") ||
            !LoadSprites(2, 1, 68, 96, false, BMP_DRAWGAME_FLAG, "images/drawgame_flag.bmp") ||
            !LoadSprites(4, 1, 20, 62, true, BMP_DRAWGAME_FUMES, "images/drawgame_fumes.bmp") ||
            !LoadSprites(4, 5, 24, 32, true, BMP_WINNER_BOMBER, "images/winner_bomber.bmp") ||
            !LoadSprites(1, 1, 64, 32, false, BMP_WINNER_BACKGROUND_1, "images/winner_background_1.bmp") ||
            !LoadSprites(16, 1, 22, 22, true, BMP_WINNER_COIN, "images/winner_coin.bmp") || // 20
            !LoadSprites(4, 1, 6, 6, true, BMP_WINNER_LIGHTS, "images/winner_lights.bmp") ||
            !LoadSprites(4, 2, 16, 16, true, BMP_WINNER_SPARKS, "images/winner_sparks.bmp") ||
            !LoadSprites(1, 1, 158, 16, true, BMP_WINNER_TITLE, "images/winner_title.bmp") ||
            !LoadSprites(1, 1, 32, 405, false, BMP_VICTORY_WALL, "images/victory_wall.bmp") ||
            !LoadSprites(9, 1, 14, 16, true, BMP_VICTORY_CROWD, "images/victory_crowd.bmp") ||
            !LoadSprites(14, 5, 36, 61, true, BMP_VICTORY_BOMBER, "images/victory_bomber.bmp") ||
            !LoadSprites(1, 1, 192, 60, true, BMP_VICTORY_TITLE, "images/victory_title.bmp") ||
            !LoadSprites(46, 6, 10, 10, true, BMP_GLOBAL_FONT, "images/global_font.bmp") ||
            !LoadSprites(1, 1, 64, 32, false, BMP_MENU_BACKGROUND_1, "images/menu_background_1.bmp") ||
            !LoadSprites(5, 2, 21, 19, true, BMP_MENU_BOMBER, "images/menu_bomber.bmp") || // 30
            !LoadSprites(1, 1, 420, 362, true, BMP_MENU_FRAME_1, "images/menu_frame_1.bmp") ||
            !LoadSprites(2, 1, 15, 16, true, BMP_MENU_HAND, "images/menu_hand.bmp") ||
            !LoadSprites(5, 1, 23, 23, true, BMP_WINNER_CROSS, "images/winner_cross.bmp") ||
            !LoadSprites(5, 5, 14, 15, true, BMP_VICTORY_CONFETTIS_LARGE, "images/victory_confettis_large.bmp") ||
            !LoadSprites(5, 5, 13, 14, true, BMP_VICTORY_CONFETTIS_MEDIUM, "images/victory_confettis_medium.bmp") ||
            !LoadSprites(5, 5, 10, 10, true, BMP_VICTORY_CONFETTIS_SMALL, "images/victory_confettis_small.bmp") ||
            !LoadSprites(1, 1, 200, 36, true, BMP_PAUSE, "images/arena_pause.bmp") ||
            !LoadSprites(1, 1, 200, 36, true, BMP_HURRY, "images/arena_hurry.bmp") ||
            !LoadSprites(1, 1, 154, 93, true, BMP_MENU_FRAME_2, "images/menu_frame_2.bmp") ||
            !LoadSprites(3, 4, 32, 32, true, BMP_ARENA_FUMES, "images/arena_fumes.bmp") || // 40
            !LoadSprites(1, 1, 14, 14, true, BMP_BOARD_DRAWGAME, "images/board_drawgame.bmp") ||
            !LoadSprites(1, 1, 480, 442, false, BMP_TITLE_BACKGROUND, "images/title_background.bmp") ||
            !LoadSprites(1, 1, 480, 126, true, BMP_TITLE_BOMBERS, "images/title_bombers.bmp") ||
            !LoadSprites(1, 1, 298, 139, true, BMP_TITLE_TITLE, "images/title_title.bmp") ||
            !LoadSprites(2, 5, 128, 26, true, BMP_TITLE_MENU_ITEMS, "images/title_menu_items.bmp") ||
            !LoadSprites(1, 1, 64, 32, false, BMP_CONTROLS_BACKGROUND_1, "images/controls_background_1.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_CONTROLS_BACKGROUND_2, "images/controls_background_2.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_CONTROLS_BACKGROUND_3, "images/controls_background_3.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_MENU_BACKGROUND_2, "images/menu_background_2.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_MENU_BACKGROUND_3, "images/menu_background_3.bmp") || // 50
            !LoadSprites(1, 1, 82, 41, false, BMP_WINNER_BACKGROUND_2, "images/winner_background_2.bmp") ||
            !LoadSprites(1, 1, 82, 41, false, BMP_WINNER_BACKGROUND_3, "images/winner_background_3.bmp") ||
            !LoadSprites(1, 1, 138, 46, true, BMP_TITLE_CLOUD_1, "images/title_cloud_1.bmp") ||
            !LoadSprites(1, 1, 106, 46, true, BMP_TITLE_CLOUD_2, "images/title_cloud_2.bmp") ||
            !LoadSprites(1, 1, 66, 22, true, BMP_TITLE_CLOUD_3, "images/title_cloud_3.bmp") ||
            !LoadSprites(17, 1, 16, 16, true, BMP_LEVEL_MINI_TILES, "images/level_mini_tiles.bmp") ||
            !LoadSprites(5, 1, 24, 20, true, BMP_LEVEL_MINI_BOMBERS, "images/level_mini_bombers.bmp") ||
            !LoadSprites(7, 5, 42, 44, true, BMP_ARENA_BOMBER_DEATH, "images/arena_bomber_death.bmp") ||
            !LoadSprites(12, 8, 42, 44, true, BMP_ARENA_BOMBER_LIFT, "images/arena_bomber_lift.bmp") ||
            !LoadSprites(20, 8, 42, 44, true, BMP_ARENA_BOMBER_THROW, "images/arena_bomber_throw.bmp") || // 60
            !LoadSprites(8, 8, 42, 44, true, BMP_ARENA_BOMBER_PUNCH, "images/arena_bomber_punch.bmp") ||
            !LoadSprites(4, 8, 42, 44, true, BMP_ARENA_BOMBER_STUNT, "images/arena_bomber_stunt.bmp") ||
            !LoadSprites(4, 1, 32, 32, true, BMP_ARENA_ARROWS, "images/arena_arrows.bmp") || // 63
            !LoadSprites(1, 1, 30, 32, true, BMP_MENU_HAND_TITLE, "images/menu_hand_title.bmp") ||
            !LoadSprites(3, 1, 32, 32, true, BMP_ARENA_REMOTE_BOMB, "images/arena_remote_bomb.bmp")
            )
        {
            // Failure, get out (error is logged by the LoadSprites() method)
            return false;
        }
#endif

        // Save origin where to draw from
        m_ViewOriginX = (Width - VIEW_WIDTH) / 2;
        m_ViewOriginY = (Height - VIEW_HEIGHT) / 2;

#ifdef DIRECTX_DRAW
        m_DirectDraw.SetOrigin(m_ViewOriginX, m_ViewOriginY);
#else
        m_SDLVideo.SetOrigin(m_ViewOriginX, m_ViewOriginY);
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
#ifdef DIRECTX_DRAW
    m_DirectDraw.Destroy();
#else
    m_SDLVideo.Destroy();
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
#ifdef DIRECTX_DRAW
    case DISPLAYMODE_FULL1: return m_DirectDraw.IsModeAvailable(320, 240, 32);
    case DISPLAYMODE_FULL2: return m_DirectDraw.IsModeAvailable(512, 384, 32);
    case DISPLAYMODE_FULL3: return m_DirectDraw.IsModeAvailable(640, 480, 32);
#else
    case DISPLAYMODE_FULL1: return m_SDLVideo.IsModeAvailable(320, 240, 32);
    case DISPLAYMODE_FULL2: return m_SDLVideo.IsModeAvailable(512, 384, 32);
    case DISPLAYMODE_FULL3: return m_SDLVideo.IsModeAvailable(640, 480, 32);
#endif
    case DISPLAYMODE_WINDOWED: return true;
    default: return false; // Should never happen
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID)
{
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
#ifdef DIRECTX_DRAW
    if (!m_DirectDraw.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, hBitmap))
#else
#ifdef WIN32
    if (!m_SDLVideo.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, hBitmap))
#else
    if (!m_SDLVideo.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, BMP_ID))
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

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID, const char *file)
{

#ifdef DIRECTX_DRAW
    if (!m_DirectDraw.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, file))
#else
    if (!m_SDLVideo.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, file))
#endif
    {
        // Get out, failure
        return false;
    }

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

