/************************************************************************************

    Copyright (C) 2017 Billy Araujo

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
 *  \file CAllegroVideo.h
 *  \brief Header file of the Allegro video
 */

#ifndef __CALLEGROVIDEO_H__
#define __CALLEGROVIDEO_H__

#include "allegro.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SSprite
{
    int SurfaceNumber;          //!< Number of the surface in CDisplay that will be the source
    int ZoneX1;                 // Top-left corner in the source surface
    int ZoneY1;
    int ZoneX2;                 // Bottom-right corner in the source surface
    int ZoneY2;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SDisplayMode
{
    int Width;
    int Height;
    int Depth;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Drawing requests are stored in a CSpriteManager
// instance. They describe a sprite to draw, and
// where to draw it, and when to draw to it (using
// sprites layers and PriorityInLayer-inside-layer.

struct SDrawingRequest
{
    int PositionX;        //!< Position X where to draw the sprite (using CDisplay origin)
    int PositionY;        //!< Position Y where to draw the sprite (using CDisplay origin)
    int ZoneX1;           // Zone to draw in the selected sprite
    int ZoneY1;
    int ZoneX2;
    int ZoneY2;
    int SpriteTable;      //!< Number of the sprite page where to the sprite is
    int Sprite;           //!< Number of the sprite to draw
    int SpriteLayer;      //!< Number of the layer where the sprite has to be drawn
    int PriorityInLayer;  //!< PriorityInLayer value inside the layer.

    #define PRIORITY_UNUSED     -1
    
    // Operators used by STL's PriorityInLayer_queue when sorting
    // The top layer on the screen is the greatest layer number
    // The top priority is the greatest priority value

    bool operator < (const SDrawingRequest &DR) const 
    { 
        return SpriteLayer > DR.SpriteLayer 
               ||
               (
                SpriteLayer == DR.SpriteLayer &&
                PriorityInLayer > DR.PriorityInLayer
               ); 
    }
    bool operator == (const SDrawingRequest &DR) const 
    { 
        return SpriteLayer == DR.SpriteLayer &&
               PriorityInLayer == DR.PriorityInLayer; 
    }
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Drawing request for debug purposes

struct SDebugDrawingRequest
{
    int PositionX;        //!< Position X where to draw the sprite (using CDisplay origin)
    int PositionY;        //!< Position Y where to draw the sprite (using CDisplay origin)
    int ZoneX1;           // Zone to draw in the selected sprite
    int ZoneY1;
    int ZoneX2;
    int ZoneY2;
    
    // rectangle colour
    Uint8 R;
    Uint8 G;
    Uint8 B;
    
    int SpriteLayer;      //!< Number of the layer where the sprite has to be drawn
    int PriorityInLayer;  //!< PriorityInLayer value inside the layer.

    #define PRIORITY_UNUSED     -1
    
    // Operators used by STL's PriorityInLayer_queue when sorting
    // The top layer on the screen is the greatest layer number
    // The top priority is the greatest priority value

    bool operator < (const SDebugDrawingRequest &DR) const 
    { 
        return SpriteLayer > DR.SpriteLayer 
               ||
               (
                SpriteLayer == DR.SpriteLayer &&
                PriorityInLayer > DR.PriorityInLayer
               ); 
    }
    bool operator == (const SDebugDrawingRequest &DR) const 
    { 
        return SpriteLayer == DR.SpriteLayer &&
               PriorityInLayer == DR.PriorityInLayer; 
    }
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SSurface
{
    struct Allegro_Surface        *pSurface;            //!< Allegro surface
    DWORD                   BlitParameters;     //!< Parameter when blitting, depends on if the surface is transparent
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CAllegroVideo
{
private:

    HWND                    m_hWnd;                         //!< Window handle
    Allegro_Rect            m_rcScreen;                     //!< Window rect in screen coordinates
    Allegro_Rect            m_rcViewport;                   //!< Window rect in client coordinates
    int                     m_Width;                        //!< Display width when fullscreen
    int                     m_Height;                       //!< Display height when fullscreen
    int                     m_Depth;                        //!< Display depth when fullscreen
    bool                    m_FullScreen;                   //!< Is it fullscreen?
    Allegro_Surface         *m_pBackBuffer;                 //!< Backbuffer surface
    Allegro_Surface         *m_pPrimary;                    //!< Primary surface
    vector<SSurface>        m_Surfaces;                     //!< Surfaces
    DWORD                   m_ColorKey;                     //!< Color key for transparent surfaces
    priority_queue<SDrawingRequest> m_DrawingRequests;      //!< Automatically sorted drawing requests queue
    vector<SDebugDrawingRequest> m_DebugDrawingRequests;    //!< vector of drawing requests for debugging purposes
    vector<vector<SSprite> > m_SpriteTables;                //!< Available sprite tables
    int                     m_OriginX;                      //!< Origin position where to draw from
    int                     m_OriginY;
    vector<SDisplayMode>    m_AvailableDisplayModes;

private:

    WORD                    GetNumberOfBits (DWORD dwMask);
    
public:

    CAllegroVideo (void);
    ~CAllegroVideo (void);


    inline void             SetWindowHandle (HWND hWnd);
    bool                    Create (int Width, int Height, int Depth, bool FullScreen);
    void                    Destroy (void);
    bool                    SetTransparentColor (int Red, int Green, int Blue);
    bool                    LoadSprites (int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, HBITMAP hBitmap);
    bool                    LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, const char* file);
    void                    FreeSprites(void);
    void                    OnWindowMove (void);
    inline void             OnPaint (void);
    void                    Clear (void);
    void                    UpdateAll (void);
    void                    UpdateScreen (void);
    inline void             SetOrigin (int OriginX, int OriginY);
    inline void             SetNewPrimary (Allegro_Surface *pSurface);
    void                    DrawSprite (int PositionX, int PositionY, RECT *pZone, RECT *pClip, int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer);
    void                    DrawDebugRectangle (int PositionX, int PositionY, int w, int h, Uint8 r, Uint8 g, Uint8 b, int SpriteLayer, int PriorityInLayer);
    void                    RemoveAllDebugRectangles ();
    inline bool             IsModeSet (int Width, int Height, int Depth, bool FullScreen);
    bool                    IsModeAvailable (int Width, int Height, int Depth);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CAllegroVideo::SetWindowHandle (HWND hWnd)
{
    m_hWnd = hWnd;
}

inline bool CAllegroVideo::IsModeSet (int Width, int Height, int Depth, bool FullScreen)
{
    return m_Width == Width     && 
           m_Height == Height   && 
           m_Depth == Depth     && 
           m_FullScreen == FullScreen;
}

inline void CAllegroVideo::OnPaint (void)
{
    UpdateScreen ();
}

inline void CAllegroVideo::SetOrigin (int OriginX, int OriginY)
{
    m_OriginX = OriginX;
    m_OriginY = OriginY;
}

inline void CAllegroVideo::SetNewPrimary (Allegro_Surface *pSurface)
{
    m_pPrimary = pSurface;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************




#endif
