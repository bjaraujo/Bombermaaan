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
 *  \file CMosaic.h
 *  \brief Header file of the mosaic
 */

#ifndef __CMOSAIC_H__
#define __CMOSAIC_H__

class CDisplay;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CMosaic
{
private:

    CDisplay*   m_pDisplay;                     //!< Link to the display object to use to draw the mosaic
    int         m_SpriteTable;                  //!< Sprite table of the mosaic tile sprite to use
    int         m_Sprite;                       //!< Number of the mosaic tile sprite to use
    int         m_SpriteLayer;                  //!< Sprite layer where to draw the mosaic
    int         m_PriorityInLayer;              //!< Priority to use in the sprite layer where to draw the mosaic
    int         m_SpriteWidth;                  //!< Width in pixels of the mosaic tile sprite
    int         m_SpriteHeight;                 //!< Height in pixels of the mosaic tile sprite
    int         m_TileCountX;                   //!< How many mosaic tiles to draw horizontally?
    int         m_TileCountY;                   //!< How many mosaic tiles to draw vertically?
    float       m_SpeedX;                       //!< Scrolling speed X of the mosaic
    float       m_SpeedY;                       //!< Scrolling speed Y of the mosaic
    float       m_BackgroundPositionX;          //!< Top left corner of the animated tiled background
    float       m_BackgroundPositionY;          //!< This position is modified to animate the background on the game view
    int         m_iBackgroundPositionX;         //!< Integer coordinates of the position above,
    int         m_iBackgroundPositionY;         //!< these coordinates _are_ in the screen coordinates.

public:

                CMosaic (void);
                ~CMosaic (void);
    inline void SetDisplay (CDisplay* pDisplay);
    void        Create (int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer, int SpriteWidth, int SpriteHeight, int TileCountX, int TileCountY, float SpeedX, float SpeedY);
    void        Destroy (void);
    void        Update (float DeltaTime);              //!< Update the mosaic state
    void        Display (void);                        //!< Display the mosaic in its current state
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMosaic::SetDisplay (CDisplay* pDisplay)
{
    m_pDisplay = pDisplay;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMOSAIC_H__
