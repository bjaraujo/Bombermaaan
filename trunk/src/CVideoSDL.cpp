// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CVideoSDL.cpp
 *  \brief SDL video on Linux
 */

#include "StdAfx.h"

#include "BombermaaanIco.h"
#include "CVideoSDL.h"

static const char* GetSDLVideoError();

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVideoSDL::CVideoSDL()
{    
    m_hWnd = nullptr;
    m_pPrimary = nullptr;
    m_pWindow = nullptr;
    m_pRenderer = nullptr;
    m_pTexture = nullptr;
    m_Width = 0;
    m_Height = 0;
    m_Depth = 0;
    m_FullScreen = false;
    m_ColorKey = 0;
    m_OriginX = 0;
    m_OriginY = 0;
    m_rcScreen = SDL_Rect();
    m_rcViewport = SDL_Rect();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVideoSDL::~CVideoSDL()
{
    // Nothing to do
}

void CVideoSDL::SetProgramFolder(const std::string& pgmFolder) 
{ 
    m_pgmFolder = pgmFolder; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoSDL::Create(int Width, int Height, int Depth, bool FullScreen)
{
    // Set the display properties
    m_Width = Width;
    m_Height = Height;
    m_Depth = Depth;
    m_FullScreen = FullScreen;

    m_pPrimary = nullptr;
    m_ColorKey = 0;

    if (m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
    }

    m_pWindow = SDL_CreateWindow("Bombermaaan",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          m_Width, m_Height, FullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    if (m_pWindow == nullptr)
    {
        // Log failure
        theLog.WriteLine("SDLVideo        => !!! Window could not be created.");

        // Get out
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == nullptr)
    {
        // Log failure
        theLog.WriteLine("SDLVideo        => !!! Renderer could not be created.");

        // Get out
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(m_pRenderer, m_Width, m_Height);

    m_pPrimary = SDL_CreateRGBSurface(0, m_Width, m_Height, m_Depth,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

    m_pTexture = SDL_CreateTexture(m_pRenderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            m_Width, m_Height);

    // Get the rects of the viewport and screen bounds
    m_rcViewport.x = 0;
    m_rcViewport.y = 0;

    m_rcViewport.w = m_Width;
    m_rcViewport.h = m_Height;

    memcpy(&m_rcScreen, &m_rcViewport, sizeof(SDL_Rect));

    // show cursor depending on windowed/fullscreen mode
    SDL_ShowCursor(!m_FullScreen);

    SDL_RWops* rwIcon = SDL_RWFromMem(BombermaaanIcon, sizeof(BombermaaanIcon));

    SDL_Surface* icon = SDL_LoadBMP_RW(rwIcon, 0);

    if (icon != nullptr)
    {
        if (SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 0x00, 0xff, 0x00)) == 0)
        {
            //SDL_WM_SetIcon(icon, nullptr);
        }
    }
    else
    {
        printf("could not load icon.\n");
    }

    SDL_FreeSurface(icon);
    SDL_FreeRW(rwIcon);

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

// Destroys the SDLVideo interface

void CVideoSDL::Destroy()
{
    // Free drawing requests, sprite tables, surfaces...
    FreeSprites();

    // If a SDLVideo object exists
    if (m_pPrimary)
    {
        // Release it
        SDL_FreeSurface(m_pPrimary);

        // If we are in fullscreen mode
        if (m_FullScreen)
        {
            // Show the cursor because we are in windowed mode
            SDL_ShowCursor(true);
        }

        // Log release
        theLog.WriteLine("SDLVideo        => Primary surface was released.");
    }

    if (m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Updates the display by blitting the back buffer
// surface on the primary surface.

void CVideoSDL::UpdateScreen()
{
    SDL_UpdateTexture(m_pTexture, nullptr, m_pPrimary->pixels, m_pPrimary->pitch);
    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, nullptr);
    SDL_RenderPresent(m_pRenderer);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Updates the object : this updates the drawing zones
// in case the window moves.

void CVideoSDL::OnWindowMove() {}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoSDL::DrawSprite(int PositionX, int PositionY, RECT* pZone, RECT* pClip, int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer)
{
    // Prepare a drawing request
    SDrawingRequest DrawingRequest;

    // Save the sprite pointer
    SSprite* pSprite = &m_SpriteTables[SpriteTable][Sprite];

    // If we have to take care of clipping
    if (pClip != nullptr)
    {
        // Save the sprite size
        int SpriteSizeX = pSprite->ZoneX2 - pSprite->ZoneX1;
        int SpriteSizeY = pSprite->ZoneY2 - pSprite->ZoneY1;

        // If the sprite is completely out of the clip rect
        if (PositionX >= pClip->right || PositionY >= pClip->bottom || PositionX + SpriteSizeX < pClip->left || PositionY + SpriteSizeY < pClip->top)
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

void CVideoSDL::DrawDebugRectangle(int PositionX, int PositionY, int w, int h, Uint8 r, Uint8 g, Uint8 b, int SpriteLayer, int PriorityInLayer)
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

void CVideoSDL::RemoveAllDebugRectangles() { m_DebugDrawingRequests.clear(); }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Makes the display black.

void CVideoSDL::Clear() { SDL_RenderClear(m_pRenderer); }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

WORD CVideoSDL::GetNumberOfBits(DWORD dwMask)
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

bool CVideoSDL::SetTransparentColor(int Red, int Green, int Blue)
{
    // Get the pixel format of the primary surface
    /*SDL_PixelFormat* pf = m_pSurface->format;

    m_ColorKey = SDL_MapRGB(pf, Red, Green, Blue);*/

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoSDL::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID, HBITMAP hBitmap)
{

    SSurface Surface;

    // Create a SDLVideo surface for this bitmap
    SDL_Surface* ddsd = nullptr;

#ifdef WIN32

    // Info structure on the bitmap, contains the size info
    BITMAP Bitmap;

    // Get the bitmap's attributes
    // If it fails
    if (GetObject(hBitmap, sizeof(Bitmap), &Bitmap) == 0)
    {
        // Log failure
        theLog.WriteLine("SDLVideo      => !!! Could not get the bitmap's attributes.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    GetObject(hBitmap, sizeof(Bitmap), &Bitmap);

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
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

    SDL_Surface* surf = SDL_CreateRGBSurface(0, Bitmap.bmWidth, Bitmap.bmHeight, Bitmap.bmBitsPixel, rmask, gmask, bmask, amask);

    BYTE* bits = new BYTE[Bitmap.bmWidthBytes * Bitmap.bmHeight];
    BYTE* temp = new BYTE[Bitmap.bmWidthBytes * Bitmap.bmHeight];

    memcpy(temp, Bitmap.bmBits, Bitmap.bmWidthBytes * Bitmap.bmHeight);

    BYTE* ptemp;
    BYTE* pbits = bits;

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

    ddsd = SDL_ConvertSurfaceFormat(surf, SDL_GetWindowPixelFormat(m_pWindow), 0);

    SDL_FreeSurface(surf);

#else

    SDL_RWops* rwBitmap;

    DWORD DataSize;
    LPVOID pData;

    // Prepare a new surface from the BMP
    DataSize = GetObject(hBitmap, 0, &pData);
    if (DataSize == 0)
    {
        // Log failure
        theLog.WriteLine("SDLVideo        => !!! Could not get the bitmap's (res id: %d) attributes.", hBitmap);
        theLog.LogLastError();

        // Get out
        return false;
    }

    rwBitmap = SDL_RWFromMem(pData, DataSize);

    ddsd = SDL_LoadBMP_RW(rwBitmap, 0);

    SDL_FreeRW(rwBitmap);

#endif

    // If it failed
    if (ddsd == nullptr)
    {
        // Log failure
        theLog.WriteLine("SDLVideo        => !!! Could not create surface.");
        theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());

        // Get out
        return false;
    }

    // Blit the bitmap onto the SDLVideo surface
    Surface.pSurface = ddsd;
    Surface.BlitParameters = 0;

    //----------------------------------------------
    // Set the surface transparency color if needed
    //----------------------------------------------

    // If the sprite table uses transparency
    if (Transparent)
    {
        // Apply the color key to the surface
        HRESULT hRet = SDL_SetColorKey(ddsd, SDL_TRUE, SDL_MapRGBA(ddsd->format, 0x00, 0xff, 0x00, 0xff));

        // If it failed
        if (hRet != 0)
        {
            // Log failure
            theLog.WriteLine("SDLVideo        => !!! Could not set colorkey.");
            theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());

            // Get out
            return false;
        }
    }

    //-----------------------
    // Store the new surface
    //-----------------------

    // Add the surface to the surface container
    //SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(m_pRenderer, ddsd);
    m_Surfaces.push_back(Surface);
    //SDL_FreeSurface(ddsd);

    //---------------------------
    // Create the sprite table
    //---------------------------

    // Prepare a sprite table
    std::vector<SSprite> SpriteTable;

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
            Sprite.SurfaceNumber = m_Surfaces.size() - 1; // The surface we just added to the container
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
    m_SpriteTables[BMP_ID] = SpriteTable;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoSDL::LoadSprites(int SpriteTableWidth, int SpriteTableHeight, int SpriteWidth, int SpriteHeight, bool Transparent, int BMP_ID, const char* file)
{

    SSurface Surface;

    std::string path(m_pgmFolder);
#ifdef WIN32
    path.append("\\");
#else
    path.append("/");
#endif
    path.append(IMAGE_FOLDER);
#ifdef WIN32
    path.append("\\");
#else
    path.append("/");
#endif
    path.append(file);

    SDL_Surface* ddsd = SDL_LoadBMP(path.c_str());

    // If it failed
    if (ddsd == nullptr)
    {
        // Log failure
        theLog.WriteLine("SDLVideo        => !!! Could not create surface.");
        theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());

        // Get out
        return false;
    }

    // Blit the bitmap onto the SDLVideo surface
    Surface.pSurface = ddsd;
    Surface.BlitParameters = 0;

    //----------------------------------------------
    // Set the surface transparency color if needed
    //----------------------------------------------

    // If the sprite table uses transparency
    if (Transparent)
    {
        // Apply the color key to the surface
        HRESULT hRet = SDL_SetColorKey(ddsd, SDL_TRUE, SDL_MapRGBA(ddsd->format, 0x00, 0xff, 0x00, 0xff));

        // If it failed
        if (hRet != 0)
        {
            // Log failure
            theLog.WriteLine("SDLVideo        => !!! Could not set colorkey.");
            theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());

            // Get out
            return false;
        }
    }

    //-----------------------
    // Store the new surface
    //-----------------------

    // Add the surface to the surface container
    //SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(m_pRenderer, ddsd);
    m_Surfaces.push_back(Surface);
    //SDL_FreeSurface(ddsd);

    //---------------------------
    // Create the sprite table
    //---------------------------

    // Prepare a sprite table
    std::vector<SSprite> SpriteTable;

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
            Sprite.SurfaceNumber = m_Surfaces.size() - 1; // The surface we just added to the container
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
    m_SpriteTables[BMP_ID] = SpriteTable;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoSDL::FreeSprites()
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
        if (m_Surfaces[i].pSurface != nullptr)
        {
            // Release it
            SDL_FreeSurface(m_Surfaces[i].pSurface);
            m_Surfaces[i].pSurface = nullptr;
        }
    }

    // Remove all surfaces
    m_Surfaces.clear();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVideoSDL::UpdateAll()
{
    // While all the drawing requests have not been executed
    while (!m_DrawingRequests.empty())
    {
        // Save the top drawing request
        const SDrawingRequest& DR = m_DrawingRequests.top();

        // Save the sprite as specified by this drawing request
        const SSprite* pSprite = &m_SpriteTables[DR.SpriteTable][DR.Sprite];

        // Build a RECT structure containing the zone to draw
        SDL_Rect SourceRect;
        SourceRect.x = DR.ZoneX1;
        SourceRect.y = DR.ZoneY1;
        SourceRect.w = DR.ZoneX2 - DR.ZoneX1;
        SourceRect.h = DR.ZoneY2 - DR.ZoneY1;

        SDL_Rect DestRect;
        DestRect.x = DR.PositionX;
        DestRect.y = DR.PositionY;
        DestRect.w = 0;
        DestRect.h = 0;

        // Blit the surface zone on the back buffer
        if (SDL_BlitSurface(m_Surfaces[pSprite->SurfaceNumber].pSurface, &SourceRect, m_pPrimary, &DestRect) < 0)
        {
            // blitting failed
            theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());
        }

        // Pop the drawing request to go to the next
        m_DrawingRequests.pop();
    }

    std::vector<SDebugDrawingRequest>::iterator it;

    // Debug rectangles?
    for (it = m_DebugDrawingRequests.begin(); it < m_DebugDrawingRequests.end(); it++)
    {
        // Save the top drawing request
        const SDebugDrawingRequest& DR = *it;

        // Build a RECT structure containing the zone to draw
        SDL_Rect SourceRect;
        SourceRect.x = DR.ZoneX1;
        SourceRect.y = DR.ZoneY1;
        SourceRect.w = DR.ZoneX2 - DR.ZoneX1;
        SourceRect.h = DR.ZoneY2 - DR.ZoneY1;

        SDL_Rect DestRect;
        DestRect.x = DR.PositionX;
        DestRect.y = DR.PositionY;
        DestRect.w = 0;
        DestRect.h = 0;

        Uint32 rmask, gmask, bmask, amask;
        Uint8 r, g, b;

        r = DR.R;
        g = DR.G;
        b = DR.B;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
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
        SDL_Surface* rectangle = SDL_CreateRGBSurface(0, SourceRect.w, SourceRect.h, 32, rmask, gmask, bmask, amask);

        SDL_Surface* reals = nullptr;

        SDL_SetSurfaceAlphaMod(rectangle, 128);

        // fill with rectangle
        if (rectangle != nullptr && SDL_FillRect(rectangle, nullptr, SDL_MapRGBA(rectangle->format, r, g, b, 128)) == 0)
        {
            reals = SDL_ConvertSurfaceFormat(rectangle, SDL_GetWindowPixelFormat(m_pWindow), 0);

            // Blit the surface zone on the back buffer
            if (reals != nullptr && SDL_BlitSurface(reals, &SourceRect, m_pPrimary, &DestRect) < 0)
            {
                // blitting failed
                theLog.WriteLine("SDLVideo        => !!! SDLVideo error is : %s.", GetSDLVideoError());
            }
        }

        SDL_FreeSurface(rectangle);
        if (reals != nullptr)
            SDL_FreeSurface(reals);

        // do not Pop the drawing request (there is a separate function)
        //m_DebugDrawingRequests.pop();
    }
    UpdateScreen();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CVideoSDL::IsModeAvailable(int Width, int Height, int Depth)
{
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

static const char* GetSDLVideoError() { return SDL_GetError(); }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
