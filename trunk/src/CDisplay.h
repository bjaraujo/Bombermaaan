/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
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
 *  \file CDisplay.h
 *  \brief Header file of the display
 */

#ifndef __CDISPLAY_H__
#define __CDISPLAY_H__

#ifdef WIN32
#include "CDirectDraw.h"
#else
#include "CSDLVideo.h"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EDisplayMode
{
    DISPLAYMODE_NONE,
    DISPLAYMODE_FULL1,
    DISPLAYMODE_FULL2,
    DISPLAYMODE_FULL3,
    DISPLAYMODE_WINDOWED
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CDisplay
{
private:

    HMODULE         m_hModule;          //!< Connection to the resources
#ifdef WIN32
    CDirectDraw  	m_DirectDraw;         //!< Object used for display
#else
    CSDLVideo  	 	m_SDLVideo;           //!< Object used for display
#endif
    int             m_ViewOriginX;      //!< Top left corner of the game view
    int             m_ViewOriginY;
    
    bool            Create (int Width, int Height, bool FullScreen); //!< (Re)Create the DirectDraw/SDLVideo interface and (re)load the sprite tables given the resolution
    bool            LoadSprites (int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID); //!< Load a sprite table given its bitmap data and its properties.

public:

                    CDisplay (void);    //!< Initialize some members
                    ~CDisplay (void);   //!< Does nothing
    inline void     SetWindowHandle (HWND hWnd); //!< Set the handle of the window DirectDraw/SDLVideo has to work with
    inline void     SetModuleHandle (HMODULE hModule); //!< Set the handle of the module linked to the resources
    bool            Create (EDisplayMode DisplayMode); //!< (Re)Create the DirectDraw/SDLVideo interface and (re)load the sprite tables given the display mode
    void            Destroy (void);     //!< Destroy the DirectDraw/SDLVideo interface and the sprite tables
    inline void     OnWindowMove (void); //!< Has to be called when the window moves (WM_MOVE)
    inline void     OnPaint (void);     //!< Has to be called when the window has to be repainted (WM_PAINT)
    inline void     Clear (void);       //!< Make the window's client area black
    inline void     Update (void);      //!< Draw the sprites that DrawSprite recorded and update the window's client area
    inline void     SetOrigin (int OriginX, int OriginY); //!< Set the origin to draw from the game view origin
    inline void     DrawSprite (int PositionX, int PositionY, RECT *pZone, RECT *pClip, int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer); //!< Record a drawing request that will be executed on next call to Update
    inline void     DrawDebugRectangle (int PositionX, int PositionY, int w, int h, BYTE r, BYTE g, BYTE b, int SpriteLayer, int PriorityInLayer); //!< Record a drawing request for debug purposes
    inline void     RemoveAllDebugRectangles(void);
#ifndef WIN32
    inline CSDLVideo& GetSDLVideo(void);
#endif
    bool            IsDisplayModeAvailable (EDisplayMode DisplayMode);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CDisplay::SetWindowHandle (HWND hWnd)
{
#ifdef WIN32
    m_DirectDraw.SetWindowHandle (hWnd);
#else
    m_SDLVideo.SetWindowHandle (hWnd);
#endif
}

inline void CDisplay::SetModuleHandle (HMODULE hModule)
{
    m_hModule = hModule;
}

inline void CDisplay::SetOrigin (int OriginX, int OriginY)
{
#ifdef WIN32
    m_DirectDraw.SetOrigin (m_ViewOriginX + OriginX, m_ViewOriginY + OriginY);
#else
    m_SDLVideo.SetOrigin (m_ViewOriginX + OriginX, m_ViewOriginY + OriginY);
#endif
}

inline void CDisplay::Clear (void)
{
#ifdef WIN32
    m_DirectDraw.Clear ();
#else
    m_SDLVideo.Clear ();
#endif
}

inline void CDisplay::Update (void)
{
#ifdef WIN32
    m_DirectDraw.UpdateAll ();
#else
    m_SDLVideo.UpdateAll ();
#endif
}

inline void CDisplay::OnWindowMove (void)
{
#ifdef WIN32
    m_DirectDraw.OnWindowMove ();
#else
    m_SDLVideo.OnWindowMove ();
#endif
}

inline void CDisplay::OnPaint (void)
{
#ifdef WIN32
    m_DirectDraw.UpdateScreen ();
#else
    m_SDLVideo.UpdateScreen ();
#endif
}

inline void CDisplay::DrawSprite (int PositionX, int PositionY, RECT *pZone, RECT *pClip, int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer)
{
#ifdef WIN32
    m_DirectDraw.DrawSprite (PositionX, PositionY, pZone, pClip, SpriteTable, Sprite, SpriteLayer, PriorityInLayer);
#else
    m_SDLVideo.DrawSprite (PositionX, PositionY, pZone, pClip, SpriteTable, Sprite, SpriteLayer, PriorityInLayer);
#endif
}

inline void CDisplay::DrawDebugRectangle (int PositionX, int PositionY, int w, int h, BYTE r, BYTE g, BYTE b, int SpriteLayer, int PriorityInLayer)
{
#ifdef WIN32
    m_DirectDraw.DrawDebugRectangle (PositionX, PositionY, w, h, r, g, b, SpriteLayer, PriorityInLayer);
#else
    m_SDLVideo.DrawDebugRectangle (PositionX, PositionY, w, h, r, g, b, SpriteLayer, PriorityInLayer);
#endif
}

inline void CDisplay::RemoveAllDebugRectangles (void)
{
#ifdef WIN32
    m_DirectDraw.RemoveAllDebugRectangles();
#else
    m_SDLVideo.RemoveAllDebugRectangles();
#endif
}

#ifndef WIN32
inline CSDLVideo& CDisplay::GetSDLVideo(void) {
    return m_SDLVideo;
}
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CDISPLAY_H__
