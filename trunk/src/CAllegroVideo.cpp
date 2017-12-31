// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CAllegroVideo.cpp
 *  \brief Allegro video on Linux
 */

#include "StdAfx.h"

#include "CAllegroVideo.h"
#include "BombermaaanIco.h"

static const char* GetAllegroVideoError();
static void AddDisplayMode(int width, int height, int depth, vector<SDisplayMode>& displayModes);

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CAllegroVideo::CAllegroVideo(void)
{
    m_hWnd = NULL;
    m_pBackBuffer = NULL;
    m_pPrimary = NULL;
    m_Width = 0;
    m_Height = 0;
    m_Depth = 0;
    m_FullScreen = false;
    m_ColorKey = 0;
    m_OriginX = 0;
    m_OriginY = 0;
    m_rcScreen = Allegro_Rect();
    m_rcViewport = Allegro_Rect();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CAllegroVideo::~CAllegroVideo(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

static void AddDisplayMode(int width, int height, int depth, vector<SDisplayMode>& displayModes)
{
    // The DirectInput device that will be created
    SDisplayMode DisplayMode;
    DisplayMode.Width = width;
    DisplayMode.Height = height;
    DisplayMode.Depth = depth;

    displayModes.push_back(DisplayMode);

    return;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CAllegroVideo::Create(int Width, int Height, int Depth, bool FullScreen)
{
    // Set the display properties
    m_Width = Width;
    m_Height = Height;
    m_Depth = Depth;
    m_FullScreen = FullScreen;

    m_pBackBuffer = NULL;
    m_pPrimary = NULL;
    m_ColorKey = 0;

    Allegro_Rect **modes;

    int i;
    bool validMode = false; // is this video mode valid?

    // Enumerate all display modes (without taking refresh rates into account)
    modes = Allegro_ListModes(NULL, Allegro_HWSURFACE | Allegro_DOUBLEBUF);

    // some mode available?
    if (modes == (Allegro_Rect **)0) {
        // Log failure
        theLog.WriteLine("AllegroVideo        => !!! Could not find any video modes.");

        // Get out
        return false;
    }
    else if (modes == (Allegro_Rect **)-1) {
        // Log success
        theLog.WriteLine("AllegroVideo        => All modes available");

        // so, add 240x234 (window), 320 x 240, 512 x 384, 640 x 480 in 32 bits
        AddDisplayMode(240, 234, 32, m_AvailableDisplayModes);
        AddDisplayMode(320, 240, 32, m_AvailableDisplayModes);
        AddDisplayMode(512, 384, 32, m_AvailableDisplayModes);
        AddDisplayMode(640, 480, 32, m_AvailableDisplayModes);

        // so this mode is possible
        validMode = true;
    }
    else {
        // enumerate modes and add certain
        for (i = 0; modes[i]; ++i) {
            if ((modes[i]->w == 240 && modes[i]->h == 234) ||
                (modes[i]->w == 320 && modes[i]->h == 240) ||
                (modes[i]->w == 512 && modes[i]->h == 384) ||
                (modes[i]->w == 640 && modes[i]->h == 480)) {
                AddDisplayMode(modes[i]->w, modes[i]->h, 32, m_AvailableDisplayModes);

                // is our requested mode possbile?
                if (modes[i]->w == m_Width && modes[i]->h == m_Height) {
                    validMode = true;
                }
            }
        }

    }

    if (!validMode) {
        // Log failure
        theLog.WriteLine("AllegroVideo        => !!! Requested video mode %dx%d not found.", m_Width, m_Height);

        // Get out
        return false;
    }

    // If desired mode is windowed mode
    if (!m_FullScreen)
    {
        // Log that windowed mode is being initialized
        theLog.WriteLine("AllegroVideo        => Initializing AllegroVideo interface for windowed mode %dx%d.", m_Width, m_Height);

        // Get normal windowed mode
        m_pPrimary = set_gfx_mode(GFX_AUTODETECT_WINDOWED, m_Width, m_Height, 0, 0);

        if (m_pPrimary == NULL) {
            // Log failure
            theLog.WriteLine("AllegroVideo        => !!! Requested video mode could not be set. (primary surface)");

            // Get out
            return false;
        }
    }
    // If desired mode is a fullscreen mode
    else
    {
        // Log that fullscreen mode is being initialized
        theLog.WriteLine("AllegroVideo        => Initializing AllegroVideo interface for fullscreen mode %dx%dx%d.", m_Width, m_Height, m_Depth);

        // Get fullscreen mode
        m_pPrimary = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, m_Width, m_Height, 0, 0);

        if (m_pPrimary == NULL) {
            // Log failure
            theLog.WriteLine("AllegroVideo        => !!! Requested video mode could not be set. (primary/backbuffer)");

            // Get out
            return false;
        }

    }

    // Get the rects of the viewport and screen bounds
    m_rcViewport.x = 0;
    m_rcViewport.y = 0;

    m_rcViewport.w = m_Width;
    m_rcViewport.h = m_Height;

    memcpy(&m_rcScreen, &m_rcViewport, sizeof(Allegro_Rect));

    // show cursor depending on windowed/fullscreen mode
    if (m_FullScreen)
    {
        show_os_cursor(1);
    }
    
    /*
    Allegro_RWops *rwIcon = Allegro_RWFromMem(BombermaaanIcon, sizeof(BombermaaanIcon));

    Allegro_Surface *icon = Allegro_LoadBMP_RW(rwIcon, 0);

    if (icon != NULL) {
        if (Allegro_SetColorKey(icon, Allegro_SRCCOLORKEY, Allegro_MapRGB(icon->format,
            0x00, 0xff, 0x00)) == 0) {
            Allegro_WM_SetIcon(icon, NULL);
        }
    }
    else {
        printf("could not load icon.\n");
    }

    Allegro_FreeSurface(icon);
    Allegro_FreeRW(rwIcon);
    */
    
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

// Destroys the AllegroVideo interface

void CAllegroVideo::Destroy(void)
{
    // Free drawing requests, sprite tables, surfaces...
    FreeSprites();

    // If a AllegroVideo object exists
    if (m_pPrimary != NULL)
    {
        // If we are in fullscreen mode
        if (m_FullScreen)
        {
            // Show the cursor because we are in windowed mode
            Allegro_ShowCursor(true);
        }

        // If the back buffer surface exists
        if (m_pBackBuffer != NULL)
        {
            // Release it
            Allegro_FreeSurface(m_pBackBuffer);
            m_pBackBuffer = NULL;

            // Log release
            theLog.WriteLine("AllegroVideo        => Backbuffer surface was released.");
        }

        // If the primary surface exists
        if (m_pPrimary != NULL)
        {
            // Release it
            Allegro_FreeSurface(m_pPrimary);
            m_pPrimary = NULL;

            // Log release
            theLog.WriteLine("AllegroVideo        => Primary surface was released.");
        }

        // Log release
        theLog.WriteLine("AllegroVideo        => AllegroVideo object was released.");
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Updates the display by blitting the back buffer
// surface on the primary surface.

void CAllegroVideo::UpdateScreen(void)
{
    HRESULT hRet;

    while (true)
    {
        // Update the primary surface by flipping backbuffer and primary surface
        hRet = Allegro_Flip(m_pPrimary);

        // If it worked fine
        if (hRet == 0)
        {
            // Get out
            break;
        }

        // Log failure
        if (hRet != 0) {
            theLog.WriteLine("AllegroVideo        => !!! Updating failed (switching primary/backbuffer).");
            theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Updates the object : this updates the drawing zones
// in case the window moves.

void CAllegroVideo::OnWindowMove()
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAllegroVideo::DrawSprite(int PositionX,
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

    // Bound always into range
    SpriteTable = MAX(0, MIN(SpriteTable, (int)m_SpriteTables.size() - 1));
    Sprite = MAX(0, MIN(Sprite, (int)m_SpriteTables[SpriteTable].size() - 1));

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
    m_DrawingRequests.push(DrawingRequest);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAllegroVideo::DrawDebugRectangle(int PositionX,
    int PositionY,
    int w, int h,
    Uint8 r, Uint8 g, Uint8 b,
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

void CAllegroVideo::RemoveAllDebugRectangles()
{
    m_DebugDrawingRequests.clear();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Makes the display black.

void CAllegroVideo::Clear()
{
    HRESULT hRet;
    hRet = Allegro_FillRect(m_pPrimary, &m_rcViewport, 0);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

WORD CAllegroVideo::GetNumberOfBits(DWORD dwMask)
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

bool CAllegroVideo::SetTransparentColor(int Red, int Green, int Blue)
{
    // Get the pixel format of the primary surface
    Allegro_PixelFormat *pf = m_pPrimary->format;

    m_ColorKey = Allegro_MapRGB(pf, Red, Green, Blue);

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CAllegroVideo::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, HBITMAP hBitmap)
{

    SSurface Surface;

    // Create a AllegroVideo surface for this bitmap
    Allegro_Surface *ddsd = NULL;

#ifdef WIN32

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

    GetObject(hBitmap, sizeof(Bitmap), &Bitmap);

    Uint32 rmask, gmask, bmask, amask;

#if Allegro_BYTEORDER == Allegro_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    Allegro_Surface *surf = Allegro_CreateRGBSurface(Allegro_SWSURFACE, Bitmap.bmWidth, Bitmap.bmHeight, Bitmap.bmBitsPixel, rmask, gmask, bmask, amask);

    BYTE* bits = new BYTE[Bitmap.bmWidthBytes * Bitmap.bmHeight];
    BYTE* temp = new BYTE[Bitmap.bmWidthBytes * Bitmap.bmHeight];

    memcpy(temp, Bitmap.bmBits, Bitmap.bmWidthBytes * Bitmap.bmHeight);

    BYTE *ptemp;
    BYTE *pbits = bits;

    for (int y = Bitmap.bmHeight - 1; y >= 0; y--)
    {

        ptemp = temp + y * Bitmap.bmWidthBytes;

        for (int x = 0; x < Bitmap.bmWidthBytes; x++)
        {
            *pbits = *ptemp;
            pbits++;
            ptemp++;
        }

    }

    delete[] temp;

    LONG a = Bitmap.bmWidthBytes;

    int p = 0;

    //Now reverse BGR data to be RGB
    for (int j = 0; j < Bitmap.bmHeight; j++)
    {
        for (int i = 0; i < Bitmap.bmWidthBytes - 3; i += 3)
        {

            p = Bitmap.bmWidthBytes * j + i;

            BYTE aux = bits[p];
            bits[p] = bits[p + 2];
            bits[p + 2] = aux;

        }
    }

    //Now just copy bits onto surface
    memcpy(surf->pixels, bits, Bitmap.bmWidthBytes * Bitmap.bmHeight);

    delete[] bits;

    //Finally, convert surface to display format so it displays correctly

    ddsd = Allegro_DisplayFormat(surf);

    Allegro_FreeSurface(surf);


#else
    Allegro_RWops *rwBitmap;

    DWORD DataSize;
    LPVOID pData;

    // Prepare a new surface from the BMP
    DataSize = GetObject(hBitmap, 0, &pData);
    if (DataSize == 0)
    {
        // Log failure
        theLog.WriteLine("AllegroVideo        => !!! Could not get the bitmap's (res id: %d) attributes.", hBitmap);
        theLog.LogLastError();

        // Get out
        return false;
    }

    rwBitmap = Allegro_RWFromMem(pData, DataSize);

    ddsd = Allegro_LoadBMP_RW(rwBitmap, 0);

    Allegro_FreeRW(rwBitmap);

#endif

    // If it failed
    if (ddsd == NULL)
    {
        // Log failure
        theLog.WriteLine("AllegroVideo        => !!! Could not create surface.");
        theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());

        // Get out
        return false;
    }

    // Blit the bitmap onto the AllegroVideo surface
    Surface.pSurface = ddsd;

    //----------------------------------------------
    // Set the surface transparency color if needed
    //----------------------------------------------

    // If the sprite table uses transparency
    if (Transparent)
    {
        // Apply the color key to the surface
        HRESULT hRet = Allegro_SetColorKey(ddsd, Allegro_SRCCOLORKEY | Allegro_RLEACCEL, Allegro_MapRGBA(ddsd->format, 0x00, 0xff, 0x00, 0xff));

        // If it failed
        if (hRet != 0)
        {
            // Log failure
            theLog.WriteLine("AllegroVideo        => !!! Could not set colorkey.");
            theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());

            // Get out
            return false;
        }
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

bool CAllegroVideo::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, const char* file)
{
    
    SSurface Surface;
    
    std::string path(IMAGE_FOLDER);

#ifdef WIN32
    path.append("\\");
#else
    path.append("/");
#endif

    path.append(file);

    Allegro_Surface *ddsd = Allegro_LoadBMP(path.c_str());

    // If it failed
    if (ddsd == NULL)
    {
        // Log failure
        theLog.WriteLine("AllegroVideo        => !!! Could not create surface.");
        theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());

        // Get out
        return false;
    }

    // Blit the bitmap onto the AllegroVideo surface
    Surface.pSurface = ddsd;

    //----------------------------------------------
    // Set the surface transparency color if needed
    //----------------------------------------------

    // If the sprite table uses transparency
    if (Transparent)
    {
        // Apply the color key to the surface
        HRESULT hRet = Allegro_SetColorKey(ddsd, Allegro_SRCCOLORKEY | Allegro_RLEACCEL, Allegro_MapRGBA(ddsd->format, 0x00, 0xff, 0x00, 0xff));

        // If it failed
        if (hRet != 0)
        {
            // Log failure
            theLog.WriteLine("AllegroVideo        => !!! Could not set colorkey.");
            theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());

            // Get out
            return false;
        }
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

void CAllegroVideo::FreeSprites(void)
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
            Allegro_FreeSurface(m_Surfaces[i].pSurface);
            m_Surfaces[i].pSurface = NULL;
        }
    }

    // Remove all surfaces
    m_Surfaces.clear();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CAllegroVideo::UpdateAll(void)
{

    // While all the drawing requests have not been executed
    while (!m_DrawingRequests.empty())
    {
        // Save the top drawing request
        const SDrawingRequest &DR = m_DrawingRequests.top();

        // Save the sprite as specified by this drawing request
        const SSprite *pSprite = &m_SpriteTables[DR.SpriteTable][DR.Sprite];

        // Build a RECT structure containing the zone to draw
        Allegro_Rect SourceRect;
        SourceRect.x = DR.ZoneX1;
        SourceRect.y = DR.ZoneY1;
        SourceRect.w = DR.ZoneX2 - DR.ZoneX1;
        SourceRect.h = DR.ZoneY2 - DR.ZoneY1;

        Allegro_Rect DestRect;
        DestRect.x = DR.PositionX;
        DestRect.y = DR.PositionY;
        DestRect.w = 0;
        DestRect.h = 0;

        // Blit the surface zone on the back buffer
        if (Allegro_BlitSurface(m_Surfaces[pSprite->SurfaceNumber].pSurface, &SourceRect, m_pPrimary, &DestRect) < 0) {
            // blitting failed
            theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());
        }

        // Pop the drawing request to go to the next
        m_DrawingRequests.pop();
    }

    vector<SDebugDrawingRequest>::iterator it;

    // Debug rectangles?
    for (it = m_DebugDrawingRequests.begin(); it < m_DebugDrawingRequests.end(); it++)
    {
        // Save the top drawing request
        const SDebugDrawingRequest &DR = *it;

        // Build a RECT structure containing the zone to draw
        Allegro_Rect SourceRect;
        SourceRect.x = DR.ZoneX1;
        SourceRect.y = DR.ZoneY1;
        SourceRect.w = DR.ZoneX2 - DR.ZoneX1;
        SourceRect.h = DR.ZoneY2 - DR.ZoneY1;

        Allegro_Rect DestRect;
        DestRect.x = DR.PositionX;
        DestRect.y = DR.PositionY;
        DestRect.w = 0;
        DestRect.h = 0;

        Uint32 rmask, gmask, bmask, amask;
        Uint8 r, g, b;

        r = DR.R;
        g = DR.G;
        b = DR.B;
#if Allegro_BYTEORDER == Allegro_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif

        // create surface
        Allegro_Surface *rectangle = Allegro_CreateRGBSurface(Allegro_HWSURFACE | Allegro_SRCALPHA, SourceRect.w, SourceRect.h, 32, rmask, gmask, bmask, amask);

        Allegro_Surface *reals = NULL;

        Allegro_SetAlpha(rectangle, Allegro_SRCALPHA | Allegro_RLEACCEL, 128);

        // fill with rectangle
        if (rectangle != NULL && Allegro_FillRect(rectangle, NULL, Allegro_MapRGBA(rectangle->format, r, g, b, 128)) == 0)
        {
            reals = Allegro_DisplayFormatAlpha(rectangle);

            // Blit the surface zone on the back buffer
            if (reals != NULL && Allegro_BlitSurface(reals, &SourceRect, m_pPrimary, &DestRect) < 0) {
                // blitting failed
                theLog.WriteLine("AllegroVideo        => !!! AllegroVideo error is : %s.", GetAllegroVideoError());
            }
        }

        Allegro_FreeSurface(rectangle);
        if (reals != NULL)
            Allegro_FreeSurface(reals);

        // do not Pop the drawing request (there is a separate function)
        //m_DebugDrawingRequests.pop();
    }
    UpdateScreen();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CAllegroVideo::IsModeAvailable(int Width, int Height, int Depth)
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

static const char* GetAllegroVideoError()
{
    return Allegro_GetError();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
