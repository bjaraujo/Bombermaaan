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

CDisplay::CDisplay (void)
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

CDisplay::~CDisplay (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::Create (int Width, int Height, bool FullScreen)
{
    // Check if we have a connection with the resources
#ifdef WIN32
    ASSERT (m_hModule != NULL);
#endif
    
    int Depth = (FullScreen ? 32 : 0);
	
    // If no display mode has been set yet or the current display mode is not the right one
#ifdef DIRECTX
    if (!m_DirectDraw.IsModeSet (Width, Height, Depth, FullScreen))
#else
    if (!m_SDLVideo.IsModeSet (Width, Height, Depth, FullScreen))
#endif
    {
		    // Destroy DirectDraw/SDLVideo interface and the sprite tables
        Destroy ();
                
        // If DirectDraw/SDLVideo object creation failed
#ifdef DIRECTX
        if (!m_DirectDraw.Create (Width, Height, Depth, FullScreen))
#else
        if (!m_SDLVideo.Create (Width, Height, Depth, FullScreen))
#endif
        {
            // Get out
            return false;
        }

#ifdef DIRECTX
        // Set the RGB color for transparent pixels in sprites
        // If it failed
        if (!m_DirectDraw.SetTransparentColor (0, 255, 0))
        {
            // Get out, it failed
            return false;
        }
#endif        

        // Load the sprite tables. If at least one sprite table could not be loaded
        if (
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (2,      1,      32,     32,     false,    BMP_ARENA_FLOOR               ) ||
            !LoadSprites (7,      1,      32,     32,     true,     BMP_ARENA_WALL                ) ||
            !LoadSprites (28,     1,      32,     32,     true,     BMP_ARENA_FLAME               ) ||
            !LoadSprites (16,     1,      32,     32,     false,    BMP_ARENA_ITEM                ) ||
            !LoadSprites (3,      1,      32,     32,     true,     BMP_ARENA_BOMB                ) ||			
            !LoadSprites (12,     7,      42,     44,     true,     BMP_ARENA_BOMBER_WALK         ) ||
            !LoadSprites (7,      1,      52,     54,     true,     BMP_ARENA_FIRE                ) ||
            !LoadSprites (12,     7,      42,     44,     true,     BMP_ARENA_BOMBER_WALK_HOLD    ) ||
            !LoadSprites (4,      1,      32,     32,     true,     BMP_ARENA_FLY                 ) ||
            !LoadSprites (1,      1,      480,    26,     false,    BMP_BOARD_BACKGROUND          ) ||
#else
            !LoadSprites (2,      1,      16,     16,     false,    BMP_ARENA_FLOOR               ) ||
            !LoadSprites (7,      1,      16,     16,     true,     BMP_ARENA_WALL                ) ||
            !LoadSprites (28,     1,      16,     16,     true,     BMP_ARENA_FLAME               ) ||
            !LoadSprites (16,     1,      16,     16,     false,    BMP_ARENA_ITEM                ) ||
            !LoadSprites (3,      1,      16,     16,     true,     BMP_ARENA_BOMB                ) ||
            !LoadSprites (12,     7,      21,     22,     true,     BMP_ARENA_BOMBER_WALK         ) ||
            !LoadSprites (7,      1,      26,     27,     true,     BMP_ARENA_FIRE                ) ||
            !LoadSprites (12,     7,      21,     22,     true,     BMP_ARENA_BOMBER_WALK_HOLD    ) ||
            !LoadSprites (4,      1,      16,     16,     true,     BMP_ARENA_FLY                 ) ||
            !LoadSprites (1,      1,      240,    26,     false,    BMP_BOARD_BACKGROUND          ) ||
#endif
            !LoadSprites (12,     1,      7,      10,     true,     BMP_BOARD_TIME                ) || // 10
            !LoadSprites (2,      1,      15,     7,      true,     BMP_BOARD_CLOCK_TOP           ) ||
            !LoadSprites (8,      1,      15,     13,     true,     BMP_BOARD_CLOCK_BOTTOM        ) ||
            !LoadSprites (6,      1,      6,      8,      true,     BMP_BOARD_SCORE               ) ||
            !LoadSprites (5,      2,      14,     14,     true,     BMP_BOARD_HEADS               ) ||
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (1,      1,      480,    442,    false,    BMP_DRAWGAME_MAIN             ) ||
			!LoadSprites (2,      1,      68,     96,     false,    BMP_DRAWGAME_FLAG             ) ||
            !LoadSprites (4,      1,      20,     62,     true,     BMP_DRAWGAME_FUMES            ) ||
#else
			!LoadSprites (1,      1,      240,    234,    false,    BMP_DRAWGAME_MAIN             ) ||
			!LoadSprites (2,      1,      34,     48,     false,    BMP_DRAWGAME_FLAG             ) ||
            !LoadSprites (4,      1,      10,     31,     true,     BMP_DRAWGAME_FUMES            ) ||
#endif
            !LoadSprites (4,      5,      24,     32,     true,     BMP_WINNER_BOMBER             ) ||
            !LoadSprites (1,      1,      64,     32,     false,    BMP_WINNER_BACKGROUND_1       ) ||
            !LoadSprites (16,     1,      22,     22,     true,     BMP_WINNER_COIN               ) || // 20
            !LoadSprites (4,      1,      6,      6,      true,     BMP_WINNER_LIGHTS             ) ||
            !LoadSprites (4,      2,      16,     16,     true,     BMP_WINNER_SPARKS             ) ||
            !LoadSprites (1,      1,      158,    16,     true,     BMP_WINNER_TITLE              ) ||
            !LoadSprites (1,      1,      32,     405,    false,    BMP_VICTORY_WALL              ) ||
            !LoadSprites (9,      1,      14,     16,     true,     BMP_VICTORY_CROWD             ) ||
            !LoadSprites (14,     5,      36,     61,     true,     BMP_VICTORY_BOMBER            ) ||
            !LoadSprites (1,      1,      192,    60,     true,     BMP_VICTORY_TITLE             ) ||
            !LoadSprites (46,     6,      10,     10,     true,     BMP_GLOBAL_FONT               ) ||
            !LoadSprites (1,      1,      64,     32,     false,    BMP_MENU_BACKGROUND_1         ) ||
            !LoadSprites (5,      2,      21,     19,     true,     BMP_MENU_BOMBER               ) || // 30
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (1,      1,      420,    362,    true,     BMP_MENU_FRAME_1              ) ||
#else
            !LoadSprites (1,      1,      210,    181,    true,     BMP_MENU_FRAME_1              ) ||
#endif
            !LoadSprites (2,      1,      15,     16,     true,     BMP_MENU_HAND                 ) ||
            !LoadSprites (5,      1,      23,     23,     true,     BMP_WINNER_CROSS              ) ||
            !LoadSprites (5,      5,      14,     15,     true,     BMP_VICTORY_CONFETTIS_LARGE   ) ||
            !LoadSprites (5,      5,      13,     14,     true,     BMP_VICTORY_CONFETTIS_MEDIUM  ) ||
            !LoadSprites (5,      5,      10,     10,     true,     BMP_VICTORY_CONFETTIS_SMALL   ) ||
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (1,      1,     202,     48,     true,     BMP_PAUSE                     ) ||
            !LoadSprites (1,      1,     200,     36,     true,     BMP_HURRY                     ) ||
#else
            !LoadSprites (1,      1,      69,     16,     true,     BMP_PAUSE                     ) ||
            !LoadSprites (1,      1,     105,     16,     true,     BMP_HURRY                     ) ||
#endif
            !LoadSprites (1,      1,     154,     93,     true,     BMP_MENU_FRAME_2              ) ||
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (3,      4,      32,     32,     true,     BMP_ARENA_FUMES               ) || // 40
#else
            !LoadSprites (3,      4,      16,     16,     true,     BMP_ARENA_FUMES               ) || // 40
#endif
            !LoadSprites (1,      1,      14,     14,     true,     BMP_BOARD_DRAWGAME            ) ||
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (1,      1,     480,    442,     false,    BMP_TITLE_BACKGROUND          ) ||
#else
            !LoadSprites (1,      1,     240,    234,     false,    BMP_TITLE_BACKGROUND          ) ||
#endif
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (1,      1,     480,    126,     true,     BMP_TITLE_BOMBERS             ) ||
            !LoadSprites (1,      1,     298,    139,     true,     BMP_TITLE_TITLE               ) ||
            !LoadSprites (2,      5,     128,     26,     true,     BMP_TITLE_MENU_ITEMS          ) ||
#else
            !LoadSprites (1,      1,     240,    126,     true,     BMP_TITLE_BOMBERS             ) ||
            !LoadSprites (1,      1,     168,     77,     true,     BMP_TITLE_TITLE               ) ||
            !LoadSprites (2,      5,      64,     13,     true,     BMP_TITLE_MENU_ITEMS          ) ||
#endif
            !LoadSprites (1,      1,      64,     32,     false,    BMP_CONTROLS_BACKGROUND_1     ) ||
            !LoadSprites (1,      1,      82,     41,     false,    BMP_CONTROLS_BACKGROUND_2     ) ||
            !LoadSprites (1,      1,      82,     41,     false,    BMP_CONTROLS_BACKGROUND_3     ) ||
            !LoadSprites (1,      1,      82,     41,     false,    BMP_MENU_BACKGROUND_2         ) ||
            !LoadSprites (1,      1,      82,     41,     false,    BMP_MENU_BACKGROUND_3         ) || // 50
            !LoadSprites (1,      1,      82,     41,     false,    BMP_WINNER_BACKGROUND_2       ) ||
            !LoadSprites (1,      1,      82,     41,     false,    BMP_WINNER_BACKGROUND_3       ) ||
            !LoadSprites (1,      1,     138,     46,     true,     BMP_TITLE_CLOUD_1             ) ||
            !LoadSprites (1,      1,     106,     46,     true,     BMP_TITLE_CLOUD_2             ) ||
            !LoadSprites (1,      1,      66,     22,     true,     BMP_TITLE_CLOUD_3             ) ||
#ifdef USE_32_PIXELS_PER_BLOCK
            !LoadSprites (16,     1,      16,     16,     true,     BMP_LEVEL_MINI_TILES          ) ||
            !LoadSprites (5,      1,      24,     20,     true,     BMP_LEVEL_MINI_BOMBERS        ) ||
            !LoadSprites (7,      5,      42,     44,     true,     BMP_ARENA_BOMBER_DEATH        ) ||
            !LoadSprites (12,     7,      42,     44,     true,     BMP_ARENA_BOMBER_LIFT         ) || 
            !LoadSprites (20,     7,      42,     44,     true,     BMP_ARENA_BOMBER_THROW        ) || // 60
            !LoadSprites (8,      7,      42,     44,     true,     BMP_ARENA_BOMBER_PUNCH        ) ||
            !LoadSprites (4,      7,      42,     44,     true,     BMP_ARENA_BOMBER_STUNT        ) ||
            !LoadSprites (4,      1,      32,     32,     true,     BMP_ARENA_ARROWS              ) || // 63
            !LoadSprites (1,      1,      30,     32,     true,     BMP_MENU_HAND_TITLE           ) ||
			!LoadSprites (3,      1,      32,     32,     true,     BMP_ARENA_REMOTE_BOMB         ) 
#else
            !LoadSprites (16,     1,       8,      8,     true,     BMP_LEVEL_MINI_TILES          ) ||
            !LoadSprites (5,      1,      12,     10,     true,     BMP_LEVEL_MINI_BOMBERS        ) ||
            !LoadSprites (7,      5,      21,     22,     true,     BMP_ARENA_BOMBER_DEATH        ) ||
            !LoadSprites (12,     7,      21,     22,     true,     BMP_ARENA_BOMBER_LIFT         ) || 
            !LoadSprites (20,     7,      21,     22,     true,     BMP_ARENA_BOMBER_THROW        ) || // 60
            !LoadSprites (8,      7,      21,     22,     true,     BMP_ARENA_BOMBER_PUNCH        ) ||
            !LoadSprites (4,      7,      21,     22,     true,     BMP_ARENA_BOMBER_STUNT        ) ||
            !LoadSprites (4,      1,      16,     16,     true,     BMP_ARENA_ARROWS              ) || // 63
            !LoadSprites (1,      1,      15,     16,     true,     BMP_MENU_HAND_TITLE           ) ||
			!LoadSprites (3,      1,      16,     16,     true,     BMP_ARENA_REMOTE_BOMB         ) 
#endif
           )
        {
            // Failure, get out (error is logged by the LoadSprites() method)
            return false;
        }

        // Save origin where to draw from
        m_ViewOriginX = (Width - VIEW_WIDTH) / 2;
        m_ViewOriginY = (Height - VIEW_HEIGHT) / 2;

#ifdef DIRECTX
        m_DirectDraw.SetOrigin (m_ViewOriginX, m_ViewOriginY);
#else
        m_SDLVideo.SetOrigin (m_ViewOriginX, m_ViewOriginY);
#endif
    }

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::Create (EDisplayMode DisplayMode)
{
    ASSERT (DisplayMode != DISPLAYMODE_NONE);
    
    // According to the new display mode to set
    switch (DisplayMode)
    {
        case DISPLAYMODE_FULL1    : return Create (320, 240, true);
        case DISPLAYMODE_FULL2    : return Create (512, 384, true);
        case DISPLAYMODE_FULL3    : return Create (640, 480, true);
        case DISPLAYMODE_WINDOWED : return Create (VIEW_WIDTH, VIEW_HEIGHT, false);
        default                   : return false; // Should never happen
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDisplay::Destroy (void)
{
    // Destroy DirectDraw/SDLVideo interface and the sprite tables
#ifdef DIRECTX
    m_DirectDraw.Destroy ();
#else
    m_SDLVideo.Destroy ();
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::IsDisplayModeAvailable (EDisplayMode DisplayMode)
{
    ASSERT (DisplayMode != DISPLAYMODE_NONE);

    // According to the display mode to test
    switch (DisplayMode)
    {
#ifdef DIRECTX
		case DISPLAYMODE_FULL1    : return m_DirectDraw.IsModeAvailable (320, 240, 32);
        case DISPLAYMODE_FULL2    : return m_DirectDraw.IsModeAvailable (512, 384, 32);
        case DISPLAYMODE_FULL3    : return m_DirectDraw.IsModeAvailable (640, 480, 32);
#else
        case DISPLAYMODE_FULL1    : return m_SDLVideo.IsModeAvailable (320, 240, 16);
        case DISPLAYMODE_FULL2    : return m_SDLVideo.IsModeAvailable (512, 384, 16);
        case DISPLAYMODE_FULL3    : return m_SDLVideo.IsModeAvailable (640, 480, 16);
#endif
        case DISPLAYMODE_WINDOWED : return true;
        default                   : return false; // Should never happen
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CDisplay::LoadSprites (int SpriteTableWidth, 
                            int SpriteTableHeight, 
                            int SpriteWidth, 
                            int SpriteHeight, 
                            bool Transparent,
                            int BMP_ID)
{
#ifdef WIN32
    // Load the bitmap as a resource
    HBITMAP hBitmap = (HBITMAP) LoadImage (m_hModule, MAKEINTRESOURCE(BMP_ID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    // If it failed
    if (hBitmap == NULL)
    {
        // Log failure
        theLog.WriteLine ("Display         => !!! Could not load resource image (%d) and create handle to bitmap.", BMP_ID);
        theLog.LogLastError ();

        // Get out
        return false;
    }
#endif
    // Create the sprites by giving the sprite table information and the handle to the bitmap.
    // If it fails
#ifdef DIRECTX
    if (!m_DirectDraw.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, hBitmap))
#else
	if (!m_SDLVideo.LoadSprites(SpriteTableWidth, SpriteTableHeight, SpriteWidth, SpriteHeight, Transparent, hBitmap))
#endif
    {
        // Get out, failure
        return false;
    }

#ifdef WIN32
    // We no longer need the hBitmap so delete it
    // If it fails
    if (DeleteObject (hBitmap) == 0)
    {
        // Log failure
        theLog.WriteLine ("Display         => !!! Could not delete handle to bitmap.");
        theLog.LogLastError ();

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

