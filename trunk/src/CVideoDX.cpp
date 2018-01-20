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
 *  \file CVideoDX.cpp
 *  \brief Direct draw function calls on Windows
 */

#include "StdAfx.h"
#include "CVideoDX.h"

static const char* GetDirectDrawError(HRESULT hRet);
static HRESULT WINAPI AddDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVideoDX::CVideoDX(void)
{
    m_hWnd = NULL;
    m_pDD = NULL;
    m_pBackBuffer = NULL;
    m_pPrimary = NULL;
    m_Width = 0;
    m_Height = 0;
    m_Depth = 0;
    m_FullScreen = false;
    m_ColorKey = 0;
    m_OriginX = 0;
    m_OriginY = 0;
		
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVideoDX::~CVideoDX(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

static HRESULT WINAPI AddDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext)
{
    // The DirectInput device that will be created
    vector<SDisplayMode>* pDisplayModes = (vector<SDisplayMode>*)lpContext;

    SDisplayMode DisplayMode;
    DisplayMode.Width = lpDDSurfaceDesc->dwWidth;
    DisplayMode.Height = lpDDSurfaceDesc->dwHeight;
    DisplayMode.Depth = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;

    pDisplayModes->push_back(DisplayMode);

    // Continue enumerating display modes       
    return DDENUMRET_OK;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoDX::Create(int Width, int Height, int Depth, bool FullScreen)
{
    //! Set the display properties
    m_Width = Width;
    m_Height = Height;
    m_Depth = Depth;
    m_FullScreen = FullScreen;

    m_pDD = NULL;
    m_pBackBuffer = NULL;
    m_pPrimary = NULL;
    m_ColorKey = 0;

    HRESULT hRet;
    DDSURFACEDESC2 ddsd;

    //! Create directdraw object
    hRet = DirectDrawCreateEx(NULL, (LPVOID *)&m_pDD, IID_IDirectDraw7, NULL);

    // If it failed
    if (hRet != DD_OK)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not create DirectDraw object.");
        theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

        // Get out
        return false;
    }
    // If it was successful
    else
    {
        // Log success
        theLog.WriteLine("DirectDraw      => DirectDraw object was created.");
    }

    //! Enumerate all display modes (without taking refresh rates into account)
    m_pDD->EnumDisplayModes(0, NULL, (LPVOID *)&m_AvailableDisplayModes, AddDisplayMode);

    // If desired mode is windowed mode
    if (!m_FullScreen)
    {
        // Log that windowed mode is being initialized
        theLog.WriteLine("DirectDraw      => Initializing DirectDraw interface for windowed mode %dx%d.", m_Width, m_Height);

        // Get normal windowed mode
        hRet = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not set cooperative level.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Cooperative level was set.");
        }

        // Create the primary surface
        ZeroMemory(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        hRet = m_pDD->CreateSurface(&ddsd, &m_pPrimary, NULL);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not create primary surface.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Primary surface was created.");
        }

        // Create a clipper object since this is for a windowed render
        LPDIRECTDRAWCLIPPER pClipper;
        hRet = m_pDD->CreateClipper(0, &pClipper, NULL);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not create clipper object.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Clipper object was created.");
        }

        // Associate the clipper with the window
        pClipper->SetHWnd(0, m_hWnd);
        m_pPrimary->SetClipper(pClipper);
        pClipper->Release();
        pClipper = NULL;

        // Resize the window to adjust the client area size
        RECT rc;
        SetRect(&rc, 0, 0, m_Width, m_Height);
        AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (int)GetMenu(m_hWnd), GetWindowExStyle(m_hWnd));
        SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);

        // Get the rects of the viewport and screen bounds
        GetClientRect(m_hWnd, &m_rcViewport);
        GetClientRect(m_hWnd, &m_rcScreen);
        ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.left);
        ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.right);

        // Get the backbuffer
        ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        ddsd.dwWidth = m_Width;
        ddsd.dwHeight = m_Height;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        hRet = m_pDD->CreateSurface(&ddsd, &m_pBackBuffer, NULL);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not create backbuffer surface.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Backbuffer surface was created.");
        }

        // In windowed mode, the cursor is visible by default
        ShowCursor(true);
    }
    // If desired mode is a fullscreen mode
    else
    {
        // Log that fullscreen mode is being initialized
        theLog.WriteLine("DirectDraw      => Initializing DirectDraw interface for fullscreen mode %dx%dx%d.", m_Width, m_Height, m_Depth);

        // Get exclusive mode
        hRet = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not set cooperative level.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Cooperative level was set.");
        }

        // Set the video mode
        hRet = m_pDD->SetDisplayMode(m_Width, m_Height, m_Depth, 0, 0);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not set display mode.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Display mode was set.");
        }

        // Create the primary surface with 1 back buffer
        ZeroMemory(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
        ddsd.dwBackBufferCount = 1;
        hRet = m_pDD->CreateSurface(&ddsd, &m_pPrimary, NULL);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not create primary surface.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Primary surface was created.");
        }

        DDSCAPS2 ddscaps;
        ZeroMemory(&ddscaps, sizeof(ddscaps));
        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
        hRet = m_pPrimary->GetAttachedSurface(&ddscaps, &m_pBackBuffer);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not get backbuffer surface.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectDraw      => Getting backbuffer surface was successful.");
        }

        // Get the dimensions of the viewport and screen bounds
        // Store the rectangle which contains the renderer
        SetRect(&m_rcViewport, 0, 0, m_Width, m_Height);
        memcpy(&m_rcScreen, &m_rcViewport, sizeof(RECT));

        // In fullscreen mode, the cursor is not visible by default
        ShowCursor(false);
    }

    // Clear the back buffer surface
    Clear();

    // Reset origin
    m_OriginX = 0;
    m_OriginY = 0;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Destroys the directdraw interface

void CVideoDX::Destroy(void)
{
    // Free drawing requests, sprite tables, surfaces...
    FreeSprites();

    // If a DirectDraw object exists
    if (m_pDD != NULL)
    {
        // If we are in fullscreen mode
        if (m_FullScreen)
        {
            // Set cooperative level to normal (for windowed mode)
            HRESULT hRet = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);

            // If it failed
            if (hRet != DD_OK)
            {
                // Log failure
                theLog.WriteLine("DirectDraw      => !!! Could not set cooperative level.");
                theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

                // Get out
                return;
            }
            // If it was successful
            else
            {
                // Log success
                theLog.WriteLine("DirectDraw      => Cooperative level was set.");
            }

            // Show the cursor because we are in windowed mode
            ShowCursor(true);
        }

        // If the back buffer surface exists
        if (m_pBackBuffer != NULL)
        {
            // Release it
            m_pBackBuffer->Release();
            m_pBackBuffer = NULL;

            // Log release
            theLog.WriteLine("DirectDraw      => Backbuffer surface was released.");
        }

        // If the primary surface exists
        if (m_pPrimary != NULL)
        {
            // Release it
            m_pPrimary->Release();
            m_pPrimary = NULL;

            // Log release
            theLog.WriteLine("DirectDraw      => Primary surface was released.");
        }

        // Release the DirectDraw object
        m_pDD->Release();
        m_pDD = NULL;

        // Log release
        theLog.WriteLine("DirectDraw      => DirectDraw object was released.");
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Updates the display by blitting the back buffer surface on the primary surface.

void CVideoDX::UpdateScreen(void)
{
    HRESULT hRet;

    while (true)
    {
        // If we are in windowed mode
        if (!m_FullScreen)
        {
            // Update the primary surface with by blitting the backbuffer on the primary surface
            hRet = m_pPrimary->Blt(&m_rcScreen, m_pBackBuffer, &m_rcViewport, DDBLT_WAIT, NULL);
        }
        // If we are in fullscreen mode
        else
        {
            // Update the primary surface by flipping backbuffer and primary surface
            hRet = m_pPrimary->Flip(NULL, DDFLIP_WAIT);
        }

        // If it worked fine
        if (hRet == DD_OK)
        {
            // Get out
            break;
        }

        // If the primary surface was lost
        if (hRet == DDERR_SURFACELOST)
        {
            // Log the primary surface was lost
            theLog.WriteLine("DirectDraw      => !!! Primary surface is lost.");

            // Try to restore it
            hRet = m_pPrimary->Restore();

            // If it failed
            if (hRet != DD_OK)
            {
                // Log failure
                theLog.WriteLine("DirectDraw      => !!! Could not restore primary surface.");
                theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

                // Get out
                break;
            }
        }

        // If the primary surface is too busy
        //! TODO: Check if this should be an equal compare instead of not equal
        if (hRet != DDERR_WASSTILLDRAWING)
        {
            // Log the primary surface is too busy
            theLog.WriteLine("DirectDraw      => !!! Primary surface is too busy to be updated.");

            // Get out
            break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Update the drawing zones in case the window moves.

void CVideoDX::OnWindowMove()
{
    // Update the window rect that is used when updating the screen
    GetClientRect(m_hWnd, &m_rcViewport);
    GetClientRect(m_hWnd, &m_rcScreen);
    ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.left);
    ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.right);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Store a sprite in the drawing requests list
 *
 *  @param PositionX the x coordinate where the upper left corner should be placed (relative to origin)
 *  @param PositionY the y coordinate where the upper left corner should be placed (relative to origin)
 *
 *  @see SetOrigin()
 */

void CVideoDX::DrawSprite(int PositionX,
    int PositionY,
    RECT *pZone,
    RECT *pClip,
    int SpriteTable,
    int Sprite,
    int SpriteLayer,
    int PriorityInLayer)
{

    // Check if the parameters are valid
    ASSERT(SpriteTable >= 0 && SpriteTable < (int)m_SpriteTables.size());
    ASSERT(Sprite >= 0 && Sprite < (int)m_SpriteTables[SpriteTable].size());

    // Prepare a drawing request
    SDrawingRequest DrawingRequest;

    // Save the sprite pointer
    SSprite *pSprite = &m_SpriteTables[SpriteTable][Sprite];

    // If we have to take care of clipping
    if (pClip != NULL)
    {
        // Save the sprite size
        int SpriteSizeX = pSprite->ZoneX2 - pSprite->ZoneX1;
        int SpriteSizeY = pSprite->ZoneY2 - pSprite->ZoneY1;

        // If the sprite is completely out of the clip rect
        if (PositionX >= pClip->right ||
            PositionY >= pClip->bottom ||
            PositionX + SpriteSizeX < pClip->left ||
            PositionY + SpriteSizeY < pClip->top)
        {
            // Get out, don't even register the drawing request
            return;
        }

        // If the sprite has to be clipped on the left
        if (PositionX < pClip->left)
        {
            // Clip on the left
            DrawingRequest.PositionX = pClip->left;
            DrawingRequest.ZoneX1 = pSprite->ZoneX1 + pClip->left - PositionX;
        }
        // If no clip on the left is needed
        else
        {
            // Don't clip
            DrawingRequest.PositionX = PositionX;
            DrawingRequest.ZoneX1 = pSprite->ZoneX1;
        }

        // If the sprite has to be clipped on the top
        if (PositionY < pClip->top)
        {
            // Clip on the top
            DrawingRequest.PositionY = pClip->top;
            DrawingRequest.ZoneY1 = pSprite->ZoneY1 + pClip->top - PositionY;
        }
        // If no clip on the top is needed
        else
        {
            // Don't clip
            DrawingRequest.PositionY = PositionY;
            DrawingRequest.ZoneY1 = pSprite->ZoneY1;
        }

        // If the sprite has to be clipped on the right
        if (PositionX + SpriteSizeX >= pClip->right)
        {
            // Clip on the right
            DrawingRequest.ZoneX2 = pSprite->ZoneX1 + pClip->right - PositionX;
        }
        // If no clip on the right is needed
        else
        {
            // Don't clip
            DrawingRequest.ZoneX2 = pSprite->ZoneX2;
        }

        // If the sprite has to be clipped on the bottom
        if (PositionY + SpriteSizeY >= pClip->bottom)
        {
            // Clip on the bottom
            DrawingRequest.ZoneY2 = pSprite->ZoneY1 + pClip->bottom - PositionY;
        }
        // If no clip on the bottom is needed
        else
        {
            // Don't clip
            DrawingRequest.ZoneY2 = pSprite->ZoneY2;
        }
    }
    // If we don't have to care about clipping
    else
    {
        // Use the desired position
        DrawingRequest.PositionX = PositionX;
        DrawingRequest.PositionY = PositionY;

        // Use the zone of the sprite
        DrawingRequest.ZoneX1 = pSprite->ZoneX1;
        DrawingRequest.ZoneY1 = pSprite->ZoneY1;
        DrawingRequest.ZoneX2 = pSprite->ZoneX2;
        DrawingRequest.ZoneY2 = pSprite->ZoneY2;
    }

    // Finish preparing the drawing request
    DrawingRequest.PositionX += m_OriginX;
    DrawingRequest.PositionY += m_OriginY;
    DrawingRequest.SpriteTable = SpriteTable;
    DrawingRequest.Sprite = Sprite;
    DrawingRequest.SpriteLayer = SpriteLayer;
    DrawingRequest.PriorityInLayer = PriorityInLayer;

    // Store it (automatic sort)
    //! @see m_DrawingRequests
    m_DrawingRequests.push(DrawingRequest);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoDX::DrawDebugRectangle(int PositionX,
    int PositionY,
    int w, int h,
    BYTE r, BYTE g, BYTE b,
    int SpriteLayer,
    int PriorityInLayer)
{
    // Prepare a drawing request
    SDebugDrawingRequest DrawingRequest;

    // Use the desired position
    DrawingRequest.PositionX = PositionX;
    DrawingRequest.PositionY = PositionY;

    // Use the zone of the sprite
    DrawingRequest.ZoneX1 = 0;
    DrawingRequest.ZoneY1 = 0;
    DrawingRequest.ZoneX2 = w;
    DrawingRequest.ZoneY2 = h;

    // rectangle colour
    DrawingRequest.R = r;
    DrawingRequest.G = g;
    DrawingRequest.B = b;

    // Finish preparing the drawing request
    DrawingRequest.PositionX += m_OriginX;
    DrawingRequest.PositionY += m_OriginY;
    DrawingRequest.SpriteLayer = SpriteLayer;
    DrawingRequest.PriorityInLayer = PriorityInLayer;

    // Store it (automatic sort)
    m_DebugDrawingRequests.push_back(DrawingRequest);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoDX::RemoveAllDebugRectangles()
{
    m_DebugDrawingRequests.clear();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Makes the display black.

void CVideoDX::Clear()
{
    DDBLTFX blt = { 0 };
    blt.dwSize = sizeof(blt);
    blt.dwFillColor = 0;
    m_pBackBuffer->Blt(NULL, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &blt);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

WORD CVideoDX::GetNumberOfBits(DWORD dwMask)
{
    WORD wBits = 0;
    while (dwMask)
    {
        dwMask = dwMask & (dwMask - 1);
        wBits++;
    }
    return wBits;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoDX::SetTransparentColor(int Red, int Green, int Blue)
{
    // Get the pixel format of the primary surface
    DDPIXELFORMAT pf;
    ZeroMemory(&pf, sizeof(pf));
    pf.dwSize = sizeof(pf);
    HRESULT hRet = m_pPrimary->GetPixelFormat(&pf);

    // If it failed
    if (hRet != DD_OK)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not get the primary surface pixel format.");
        theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

        // Get out
        return false;
    }

    // Compute how many bits for each RGB color component in a pixel data
    WORD wRBitCount = GetNumberOfBits(pf.dwRBitMask);
    WORD wGBitCount = GetNumberOfBits(pf.dwGBitMask);
    WORD wBBitCount = GetNumberOfBits(pf.dwBBitMask);

    // Compute RGB color components to use for the color key 
    // according to transparency color RGB components
    DWORD r = (Red >> (8 - wRBitCount)) << (wGBitCount + wBBitCount);
    DWORD g = (Green >> (8 - wGBitCount)) << wBBitCount;
    DWORD b = (Blue >> (8 - wBBitCount));

    // Compute the color key to apply to the surface
    m_ColorKey = (r | g | b);

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoDX::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, HBITMAP hBitmap)
{
    HRESULT hRet;

    // Info structure on the bitmap, contains the size info
    BITMAP Bitmap;

    // Get the bitmap's attributes
    // If it fails
    if (GetObject(hBitmap, sizeof(Bitmap), &Bitmap) == 0)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not get the bitmap's attributes.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Prepare a new surface
    SSurface Surface;

    // Create a DirectDraw surface for this bitmap
    DDSURFACEDESC2 ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    ddsd.dwWidth = Bitmap.bmWidth;
    ddsd.dwHeight = Bitmap.bmHeight;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    hRet = m_pDD->CreateSurface(&ddsd, &Surface.pSurface, NULL);

    // If it failed
    if (hRet != DD_OK)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not create surface.");
        theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

        // Get out
        return false;
    }

    // Create a device context that is compatible with the current display
    HDC hdcSrc = CreateCompatibleDC(NULL);

    // If it failed
    if (hdcSrc == NULL)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not create a device context.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Link the device context we created with the bitmap's hBitmap
    HGDIOBJ hGdiObj = SelectObject(hdcSrc, hBitmap);

    // If it failed
    if (hGdiObj == NULL || (DWORD)hGdiObj == GDI_ERROR)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not select the bitmap into the device context.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Handle to a DC on the DirectDraw surface
    HDC hdcDest;

    // Get a DC on the surface we will return, to copy the bitmap into the surface with the GDI
    hRet = Surface.pSurface->GetDC(&hdcDest);

    // If it failed
    if (hRet != DD_OK)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not get a device context.");
        theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

        // Get out
        return false;
    }

    // Blit the bitmap onto the DirectDraw surface
    // If it fails
    if (BitBlt(hdcDest, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY) == 0)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not blit the bitmap to the DirectDraw surface.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Release the DirectDraw surface's device context
    hRet = Surface.pSurface->ReleaseDC(hdcDest);

    // If it failed
    if (hRet != DD_OK)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not release the DirectDraw surface's device context.");
        theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

        // Get out
        return false;
    }

    // If deleting the device context we created with CreateCompatibleDC fails
    if (DeleteDC(hdcSrc) == 0)
    {
        // Log failure
        theLog.WriteLine("DirectDraw      => !!! Could not delete the device context.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    //----------------------------------------------
    // Set the surface transparency color if needed
    //----------------------------------------------

    // If the sprite table uses transparency
    if (Transparent)
    {
        // Set parameter to use when blitting the surface : use transparency
        Surface.BlitParameters = DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY;

        // Prepare color key structure
        DDCOLORKEY CKey;
        CKey.dwColorSpaceLowValue = CKey.dwColorSpaceHighValue = m_ColorKey;

        // Apply the color key to the surface
        hRet = Surface.pSurface->SetColorKey(DDCKEY_SRCBLT, &CKey);

        // If it failed
        if (hRet != DD_OK)
        {
            // Log failure
            theLog.WriteLine("DirectDraw      => !!! Could not set colorkey.");
            theLog.WriteLine("DirectDraw      => !!! DirectDraw error is : %s.", GetDirectDrawError(hRet));

            // Get out
            return false;
        }
    }
    else
    {
        // Set parameter to use when blitting the surface : no transparency
        Surface.BlitParameters = DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY;
    }

    //-----------------------
    // Store the new surface
    //-----------------------

    // Add the surface to the surface container
    m_Surfaces.push_back(Surface);

    //---------------------------
    // Create the sprite table
    //---------------------------

    // Prepare a sprite table
    vector<SSprite> SpriteTable;

    // Variable rectangle coordinates that will be passed during sprite creations
    int ZoneX1 = 1;
    int ZoneY1 = 1;
    int ZoneX2 = 1 + SpriteWidth;
    int ZoneY2 = 1 + SpriteHeight;

    // Scan all the sprites in this surface
    for (int Y = 0; Y < SpriteTableHeight; Y++)
    {
        for (int X = 0; X < SpriteTableWidth; X++)
        {
            // Prepare a sprite
            SSprite Sprite;
            Sprite.SurfaceNumber = m_Surfaces.size() - 1;       // The surface we just added to the container
            Sprite.ZoneX1 = ZoneX1;
            Sprite.ZoneY1 = ZoneY1;
            Sprite.ZoneX2 = ZoneX2;
            Sprite.ZoneY2 = ZoneY2;

            // Advance the rectangle on the row
            ZoneX1 += SpriteWidth + 1;
            ZoneX2 += SpriteWidth + 1;

            // Add the sprite to the sprite table
            SpriteTable.push_back(Sprite);
        }

        // Back to beginning of row
        ZoneX1 = 1;
        ZoneX2 = 1 + SpriteWidth;

        // Make the rectangle go down
        ZoneY1 += SpriteHeight + 1;
        ZoneY2 += SpriteHeight + 1;
    }

    // Store the sprite table
    m_SpriteTables.push_back(SpriteTable);

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoDX::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, const char* file)
{

    // TODO:

	return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoDX::FreeSprites(void)
{
    // Empty drawing requests queue
    while (!m_DrawingRequests.empty())
        m_DrawingRequests.pop();

    // Remove all sprite tables
    m_SpriteTables.clear();

    // Scan all the surfaces
    for (unsigned int i = 0; i < m_Surfaces.size(); i++)
    {
        // If the surface exists
        if (m_Surfaces[i].pSurface != NULL)
        {
            // Release it
            m_Surfaces[i].pSurface->Release();
            m_Surfaces[i].pSurface = NULL;
        }
    }

    // Remove all surfaces
    m_Surfaces.clear();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Processes all drawing requests and calls UpdateScreen
 *
 *  Blit every sprite from the drawing request list to the back buffer surface.
 *
 *  @see UpdateScreen()
 */

void CVideoDX::UpdateAll(void)
{
    // we need the pixel format for drawing the debug rectangles
    static DDPIXELFORMAT pf;
    static bool pixelFormatSet = false;

    // how many bits for each RGB color component in a pixel data
    static WORD wRBitCount = (WORD)-1;
    static WORD wGBitCount = (WORD)-1;
    static WORD wBBitCount = (WORD)-1;
    static DWORD BPP;

    // While all the drawing requests have not been executed
    while (!m_DrawingRequests.empty())
    {
        // Save the top drawing request
        const SDrawingRequest &DR = m_DrawingRequests.top();

        // Save the sprite as specified by this drawing request
        const SSprite *pSprite = &m_SpriteTables[DR.SpriteTable][DR.Sprite];

        // Build a RECT structure containing the zone to draw
        RECT SourceRect;
        SourceRect.left = DR.ZoneX1;
        SourceRect.top = DR.ZoneY1;
        SourceRect.right = DR.ZoneX2;
        SourceRect.bottom = DR.ZoneY2;

        // Blit the surface zone on the back buffer
        m_pBackBuffer->BltFast(DR.PositionX,
            DR.PositionY,
            m_Surfaces[pSprite->SurfaceNumber].pSurface,
            &SourceRect,
            m_Surfaces[pSprite->SurfaceNumber].BlitParameters);

        // Pop the drawing request to go to the next
        m_DrawingRequests.pop();
    }

    vector<SDebugDrawingRequest>::iterator it;

    // Debug rectangles?
    if (m_DebugDrawingRequests.size() > 0)
    {
        DDSURFACEDESC2 ddsd;

        if (!pixelFormatSet)
        {
            ZeroMemory(&pf, sizeof(pf));
            pf.dwSize = sizeof(pf);
            HRESULT hRet = m_pPrimary->GetPixelFormat(&pf);

            // If it failed
            if (hRet != DD_OK)
                return;

            pixelFormatSet = true;

            wRBitCount = GetNumberOfBits(pf.dwRBitMask);
            wGBitCount = GetNumberOfBits(pf.dwGBitMask);
            wBBitCount = GetNumberOfBits(pf.dwBBitMask);
            BPP = pf.dwRGBBitCount;
        }

        // valid pixel formats for our alpha blending method?
        if (BPP == 16 || BPP == 24 || BPP == 32)
        {

            ZeroMemory(&ddsd, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);

            // Lock Backbuffer Surface
            m_pBackBuffer->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

            for (it = m_DebugDrawingRequests.begin(); it < m_DebugDrawingRequests.end(); it++)
            {
                // Save the top drawing request
                const SDebugDrawingRequest &DR = *it;

                // determine colour
                DWORD r = (DR.R >> (8 - wGBitCount)) << (wGBitCount + wBBitCount);
                DWORD g = (DR.G >> (8 - wGBitCount)) << wBBitCount;
                DWORD b = (DR.B >> (8 - wBBitCount));

                // Rectangle for the pixels to blit
                RECT BlitRect;
                BlitRect.left = DR.PositionX;
                BlitRect.top = DR.PositionY;
                BlitRect.right = DR.PositionX + DR.ZoneX2;
                BlitRect.bottom = DR.PositionY + DR.ZoneY2;

                // check valid values
                if (BlitRect.left < 0 || BlitRect.right < 0 ||
                    BlitRect.bottom < 0 || BlitRect.top < 0)
                    continue;
                if ((DWORD)BlitRect.left > ddsd.dwWidth || (DWORD)BlitRect.right > ddsd.dwWidth ||
                    (DWORD)BlitRect.bottom > ddsd.dwHeight || (DWORD)BlitRect.top > ddsd.dwHeight)
                    continue;
                if (BlitRect.left > BlitRect.right || BlitRect.bottom < BlitRect.top)
                    continue;

                // Apply Alpha Blending
                BYTE *SurfacePointer = (BYTE*)ddsd.lpSurface;
                DWORD Result;
                DWORD SourceColour;
                DWORD DestColour = (r | g | b);
                int register X, Y;
                LONG Pitch = ddsd.lPitch;
                WORD Skip;

                switch (BPP)
                {
                case 16:
                    Skip = (WORD)(Pitch - (2 * DR.ZoneX2));
                    SurfacePointer += (BlitRect.top * Pitch) + (BlitRect.left * 2);

                    Y = DR.ZoneY2;
                    do
                    {
                        X = DR.ZoneX2;
                        do
                        {
                            SourceColour = *((WORD*)SurfacePointer);

                            Result = ((SourceColour & 0xF7DE) >> 1) +
                                ((DestColour & 0xF7DE) >> 1);

                            *((DWORD*)SurfacePointer) = Result;
                            SurfacePointer += 2;
                        } while (--X > 0);
                        SurfacePointer += Skip;
                    } while (--Y > 0);
                    break;
                case 24:
                    Skip = (WORD)(Pitch - (3 * DR.ZoneX2));
                    SurfacePointer += (BlitRect.top * Pitch) + (BlitRect.left * 3);

                    Y = DR.ZoneY2;
                    do
                    {
                        X = DR.ZoneX2;
                        do
                        {
                            SourceColour = *((DWORD*)SurfacePointer);
                            Result = ((SourceColour & 0xFEFEFE) >> 1) +
                                ((DestColour & 0xFEFEFE) >> 1);

                            *((WORD*)SurfacePointer) = (WORD)(Result & 0xFFFF);
                            SurfacePointer += 2;
                            *SurfacePointer = (BYTE)(Result >> 16);
                            SurfacePointer++;
                        } while (--X > 0);
                        SurfacePointer += Skip;
                    } while (--Y > 0);
                    break;
                case 32:
                    Skip = (WORD)(Pitch - (4 * DR.ZoneX2));
                    SurfacePointer += (BlitRect.top * Pitch) + (BlitRect.left * 4);

                    Y = DR.ZoneY2;
                    do
                    {
                        X = DR.ZoneX2;
                        do
                        {
                            SourceColour = *((DWORD*)SurfacePointer);
                            Result = ((SourceColour & 0xFEFEFE) >> 1) +
                                ((DestColour & 0xFEFEFE) >> 1);

                            *((WORD*)SurfacePointer) = (WORD)(Result & 0xFFFF);
                            SurfacePointer += 2;
                            *SurfacePointer = (BYTE)(Result >> 16);
                            SurfacePointer += 2;
                        } while (--X > 0);
                        SurfacePointer += Skip;
                    } while (--Y > 0);
                    break;
                default:
                    break;
                } // switch
            } // for
        } // if

        // Unlock Surface
        m_pBackBuffer->Unlock(NULL);
    } // if

    UpdateScreen();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoDX::IsModeAvailable(int Width, int Height, int Depth)
{
    // Scan all available display modes
    for (unsigned int i = 0; i < m_AvailableDisplayModes.size(); i++)
    {
        // If this is the display mode we are looking for
        if (m_AvailableDisplayModes[i].Width == Width &&
            m_AvailableDisplayModes[i].Height == Height &&
            m_AvailableDisplayModes[i].Depth == Depth)
        {
            // Then it's available
            return true;
        }
    }

    // The display mode was not found so it's not available
    return false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

static const char* GetDirectDrawError(HRESULT hRet)
{
    switch (hRet)
    {
    case DD_OK: return "DD_OK";
    case DDERR_ALREADYINITIALIZED: return "DDERR_ALREADYINITIALIZED";
    case DDERR_BLTFASTCANTCLIP: return "DDERR_BLTFASTCANTCLIP";
    case DDERR_CANNOTATTACHSURFACE: return "DDERR_CANNOTATTACHSURFACE";
    case DDERR_CANNOTDETACHSURFACE: return "DDERR_CANNOTDETACHSURFACE";
    case DDERR_CANTCREATEDC: return "DDERR_CANTCREATEDC";
    case DDERR_CANTDUPLICATE: return "DDERR_CANTDUPLICATE";
    case DDERR_CANTLOCKSURFACE: return "DDERR_CANTLOCKSURFACE";
    case DDERR_CANTPAGELOCK: return "DDERR_CANTPAGELOCK";
    case DDERR_CANTPAGEUNLOCK: return "DDERR_CANTPAGEUNLOCK";
    case DDERR_CLIPPERISUSINGHWND: return "DDERR_CLIPPERISUSINGHWND";
    case DDERR_COLORKEYNOTSET: return "DDERR_COLORKEYNOTSET";
    case DDERR_CURRENTLYNOTAVAIL: return "DDERR_CURRENTLYNOTAVAIL";
    case DDERR_DDSCAPSCOMPLEXREQUIRED: return "DDERR_DDSCAPSCOMPLEXREQUIRED";
    case DDERR_DCALREADYCREATED: return "DDERR_DCALREADYCREATED";
    case DDERR_DEVICEDOESNTOWNSURFACE: return "DDERR_DEVICEDOESNTOWNSURFACE";
    case DDERR_DIRECTDRAWALREADYCREATED: return "DDERR_DIRECTDRAWALREADYCREATED";
    case DDERR_EXCEPTION: return "DDERR_EXCEPTION";
    case DDERR_EXCLUSIVEMODEALREADYSET: return "DDERR_EXCLUSIVEMODEALREADYSET";
    case DDERR_EXPIRED: return "DDERR_EXPIRED";
    case DDERR_GENERIC: return "DDERR_GENERIC";
    case DDERR_HEIGHTALIGN: return "DDERR_HEIGHTALIGN";
    case DDERR_HWNDALREADYSET: return "DDERR_HWNDALREADYSET";
    case DDERR_HWNDSUBCLASSED: return "DDERR_HWNDSUBCLASSED";
    case DDERR_IMPLICITLYCREATED: return "DDERR_IMPLICITLYCREATED";
    case DDERR_INCOMPATIBLEPRIMARY: return "DDERR_INCOMPATIBLEPRIMARY";
    case DDERR_INVALIDCAPS: return "DDERR_INVALIDCAPS";
    case DDERR_INVALIDCLIPLIST: return "DDERR_INVALIDCLIPLIST";
    case DDERR_INVALIDDIRECTDRAWGUID: return "DDERR_INVALIDDIRECTDRAWGUID";
    case DDERR_INVALIDMODE: return "DDERR_INVALIDMODE";
    case DDERR_INVALIDOBJECT: return "DDERR_INVALIDOBJECT";
    case DDERR_INVALIDPARAMS: return "DDERR_INVALIDPARAMS";
    case DDERR_INVALIDPIXELFORMAT: return "DDERR_INVALIDPIXELFORMAT";
    case DDERR_INVALIDPOSITION: return "DDERR_INVALIDPOSITION";
    case DDERR_INVALIDRECT: return "DDERR_INVALIDRECT";
    case DDERR_INVALIDSTREAM: return "DDERR_INVALIDSTREAM";
    case DDERR_INVALIDSURFACETYPE: return "DDERR_INVALIDSURFACETYPE";
    case DDERR_LOCKEDSURFACES: return "DDERR_LOCKEDSURFACES";
    case DDERR_MOREDATA: return "DDERR_MOREDATA";
    case DDERR_NEWMODE: return "DDERR_NEWMODE";
    case DDERR_NO3D: return "DDERR_NO3D";
    case DDERR_NOALPHAHW: return "DDERR_NOALPHAHW";
    case DDERR_NOBLTHW: return "DDERR_NOBLTHW";
    case DDERR_NOCLIPLIST: return "DDERR_NOCLIPLIST";
    case DDERR_NOCLIPPERATTACHED: return "DDERR_NOCLIPPERATTACHED";
    case DDERR_NOCOLORCONVHW: return "DDERR_NOCOLORCONVHW";
    case DDERR_NOCOLORKEY: return "DDERR_NOCOLORKEY";
    case DDERR_NOCOLORKEYHW: return "DDERR_NOCOLORKEYHW";
    case DDERR_NOCOOPERATIVELEVELSET: return "DDERR_NOCOOPERATIVELEVELSET";
    case DDERR_NODC: return "DDERR_NODC";
    case DDERR_NODDROPSHW: return "DDERR_NODDROPSHW";
    case DDERR_NODIRECTDRAWHW: return "DDERR_NODIRECTDRAWHW";
    case DDERR_NODIRECTDRAWSUPPORT: return "DDERR_NODIRECTDRAWSUPPORT";
    case DDERR_NODRIVERSUPPORT: return "DDERR_NODRIVERSUPPORT";
    case DDERR_NOEMULATION: return "DDERR_NOEMULATION";
    case DDERR_NOEXCLUSIVEMODE: return "DDERR_NOEXCLUSIVEMODE";
    case DDERR_NOFLIPHW: return "DDERR_NOFLIPHW";
    case DDERR_NOFOCUSWINDOW: return "DDERR_NOFOCUSWINDOW";
    case DDERR_NOGDI: return "DDERR_NOGDI";
    case DDERR_NOHWND: return "DDERR_NOHWND";
    case DDERR_NOMIPMAPHW: return "DDERR_NOMIPMAPHW";
    case DDERR_NOMIRRORHW: return "DDERR_NOMIRRORHW";
    case DDERR_NOMONITORINFORMATION: return "DDERR_NOMONITORINFORMATION";
    case DDERR_NONONLOCALVIDMEM: return "DDERR_NONONLOCALVIDMEM";
    case DDERR_NOOPTIMIZEHW: return "DDERR_NOOPTIMIZEHW";
    case DDERR_NOOVERLAYDEST: return "DDERR_NOOVERLAYDEST";
    case DDERR_NOOVERLAYHW: return "DDERR_NOOVERLAYHW";
    case DDERR_NOPALETTEATTACHED: return "DDERR_NOPALETTEATTACHED";
    case DDERR_NOPALETTEHW: return "DDERR_NOPALETTEHW";
    case DDERR_NORASTEROPHW: return "DDERR_NORASTEROPHW";
    case DDERR_NOROTATIONHW: return "DDERR_NOROTATIONHW";
    case DDERR_NOSTEREOHARDWARE: return "DDERR_NOSTEREOHARDWARE";
    case DDERR_NOSTRETCHHW: return "DDERR_NOSTRETCHHW";
    case DDERR_NOSURFACELEFT: return "DDERR_NOSURFACELEFT";
    case DDERR_NOT4BITCOLOR: return "DDERR_NOT4BITCOLOR";
    case DDERR_NOT4BITCOLORINDEX: return "DDERR_NOT4BITCOLORINDEX";
    case DDERR_NOT8BITCOLOR: return "DDERR_NOT8BITCOLOR";
    case DDERR_NOTAOVERLAYSURFACE: return "DDERR_NOTAOVERLAYSURFACE";
    case DDERR_NOTEXTUREHW: return "DDERR_NOTEXTUREHW";
    case DDERR_NOTFLIPPABLE: return "DDERR_NOTFLIPPABLE";
    case DDERR_NOTFOUND: return "DDERR_NOTFOUND";
    case DDERR_NOTINITIALIZED: return "DDERR_NOTINITIALIZED";
    case DDERR_NOTLOADED: return "DDERR_NOTLOADED";
    case DDERR_NOTLOCKED: return "DDERR_NOTLOCKED";
    case DDERR_NOTPAGELOCKED: return "DDERR_NOTPAGELOCKED";
    case DDERR_NOTPALETTIZED: return "DDERR_NOTPALETTIZED";
    case DDERR_NOVSYNCHW: return "DDERR_NOVSYNCHW";
    case DDERR_NOZBUFFERHW: return "DDERR_NOZBUFFERHW";
    case DDERR_NOZOVERLAYHW: return "DDERR_NOZOVERLAYHW";
    case DDERR_OUTOFCAPS: return "DDERR_OUTOFCAPS";
    case DDERR_OUTOFMEMORY: return "DDERR_OUTOFMEMORY";
    case DDERR_OUTOFVIDEOMEMORY: return "DDERR_OUTOFVIDEOMEMORY";
    case DDERR_OVERLAPPINGRECTS: return "DDERR_OVERLAPPINGRECTS";
    case DDERR_OVERLAYCANTCLIP: return "DDERR_OVERLAYCANTCLIP";
    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE: return "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
    case DDERR_OVERLAYNOTVISIBLE: return "DDERR_OVERLAYNOTVISIBLE";
    case DDERR_PALETTEBUSY: return "DDERR_PALETTEBUSY";
    case DDERR_PRIMARYSURFACEALREADYEXISTS: return "DDERR_PRIMARYSURFACEALREADYEXISTS";
    case DDERR_REGIONTOOSMALL: return "DDERR_REGIONTOOSMALL";
    case DDERR_SURFACEALREADYATTACHED: return "DDERR_SURFACEALREADYATTACHED";
    case DDERR_SURFACEALREADYDEPENDENT: return "DDERR_SURFACEALREADYDEPENDENT";
    case DDERR_SURFACEBUSY: return "DDERR_SURFACEBUSY";
    case DDERR_SURFACEISOBSCURED: return "DDERR_SURFACEISOBSCURED";
    case DDERR_SURFACELOST: return "DDERR_SURFACELOST";
    case DDERR_SURFACENOTATTACHED: return "DDERR_SURFACENOTATTACHED";
    case DDERR_TESTFINISHED: return "DDERR_TESTFINISHED";
    case DDERR_TOOBIGHEIGHT: return "DDERR_TOOBIGHEIGHT";
    case DDERR_TOOBIGSIZE: return "DDERR_TOOBIGSIZE";
    case DDERR_TOOBIGWIDTH: return "DDERR_TOOBIGWIDTH";
    case DDERR_UNSUPPORTED: return "DDERR_UNSUPPORTED";
    case DDERR_UNSUPPORTEDFORMAT: return "DDERR_UNSUPPORTEDFORMAT";
    case DDERR_UNSUPPORTEDMASK: return "DDERR_UNSUPPORTEDMASK";
    case DDERR_UNSUPPORTEDMODE: return "DDERR_UNSUPPORTEDMODE";
    case DDERR_VERTICALBLANKINPROGRESS: return "DDERR_VERTICALBLANKINPROGRESS";
    case DDERR_VIDEONOTACTIVE: return "DDERR_VIDEONOTACTIVE";
    case DDERR_WASSTILLDRAWING: return "DDERR_WASSTILLDRAWING";
    case DDERR_WRONGMODE: return "DDERR_WRONGMODE";
    case DDERR_XALIGN: return "DDERR_XALIGN";
    default: return "Unknown DirectDraw error!";
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
