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

#ifdef DIRECTX
#include "CVideoDX.h"
#else
#include "CVideoSDL.h"
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
#ifdef DIRECTX
    CVideoDX        m_VideoDX;         //!< Object used for display
#else
    CVideoSDL       m_VideoSDL;           //!< Object used for display
#endif
    int             m_ViewOriginX;      //!< Top left corner of the game view
    int             m_ViewOriginY;

    bool            Create (int Width, int Height, bool FullScreen); //!< (Re)Create the DirectDraw/SDLVideo interface and (re)load the sprite tables given the resolution
    bool            LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID, const char *file); //!< Load a sprite table given its bitmap data and its properties.

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
#ifndef DIRECTX
    inline CVideoSDL& GetSDLVideo(void);
#endif
    bool            IsDisplayModeAvailable (EDisplayMode DisplayMode);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CDisplay::SetWindowHandle (HWND hWnd)
{
#ifdef DIRECTX
    m_VideoDX.SetWindowHandle (hWnd);
#else
    m_VideoSDL.SetWindowHandle (hWnd);
#endif
}

inline void CDisplay::SetModuleHandle (HMODULE hModule)
{
    m_hModule = hModule;
}

inline void CDisplay::SetOrigin (int OriginX, int OriginY)
{
#ifdef DIRECTX
    m_VideoDX.SetOrigin (m_ViewOriginX + OriginX, m_ViewOriginY + OriginY);
#else
    m_VideoSDL.SetOrigin (m_ViewOriginX + OriginX, m_ViewOriginY + OriginY);
#endif
}

inline void CDisplay::Clear (void)
{
#ifdef DIRECTX
    m_VideoDX.Clear ();
#else
    m_VideoSDL.Clear ();
#endif
}

inline void CDisplay::Update (void)
{
#ifdef DIRECTX
    m_VideoDX.UpdateAll ();
#else
    m_VideoSDL.UpdateAll ();
#endif
}

inline void CDisplay::OnWindowMove (void)
{
#ifdef DIRECTX
    m_VideoDX.OnWindowMove ();
#else
    m_VideoSDL.OnWindowMove ();
#endif
}

inline void CDisplay::OnPaint (void)
{
#ifdef DIRECTX
    m_VideoDX.UpdateScreen ();
#else
    m_VideoSDL.UpdateScreen ();
#endif
}

inline void CDisplay::DrawSprite (int PositionX, int PositionY, RECT *pZone, RECT *pClip, int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer)
{
#ifdef DIRECTX
    m_VideoDX.DrawSprite (PositionX, PositionY, pZone, pClip, SpriteTable, Sprite, SpriteLayer, PriorityInLayer);
#else
    m_VideoSDL.DrawSprite (PositionX, PositionY, pZone, pClip, SpriteTable, Sprite, SpriteLayer, PriorityInLayer);
#endif
}

inline void CDisplay::DrawDebugRectangle (int PositionX, int PositionY, int w, int h, BYTE r, BYTE g, BYTE b, int SpriteLayer, int PriorityInLayer)
{
#ifdef DIRECTX
    m_VideoDX.DrawDebugRectangle (PositionX, PositionY, w, h, r, g, b, SpriteLayer, PriorityInLayer);
#else
    m_VideoSDL.DrawDebugRectangle (PositionX, PositionY, w, h, r, g, b, SpriteLayer, PriorityInLayer);
#endif
}

inline void CDisplay::RemoveAllDebugRectangles (void)
{
#ifdef DIRECTX
    m_VideoDX.RemoveAllDebugRectangles();
#else
    m_VideoSDL.RemoveAllDebugRectangles();
#endif
}

#ifndef DIRECTX
inline CVideoSDL& CDisplay::GetSDLVideo(void) {
    return m_VideoSDL;
}
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CDISPLAY_H__
