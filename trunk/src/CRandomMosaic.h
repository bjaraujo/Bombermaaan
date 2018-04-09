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
 *  \file CRandomMosaic.h
 *  \brief Header file of a random mosaic
 */

#ifndef __CRANDOMMOSAIC_H__
#define __CRANDOMMOSAIC_H__

class CMosaic;
class CDisplay;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SMosaicTileProperties
{
    int SpriteTable;
    int Sprite;
    int CountX;
    int CountY;
    int Width;
    int Height;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EMosaicColor
{
    MOSAICCOLOR_PURPLE,
    MOSAICCOLOR_GREEN,
    MOSAICCOLOR_BLUE,
    MOSAICCOLOR_RED
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CRandomMosaic
{
private:

    static SMosaicTileProperties m_MosaicTileProperties[4][2];

public:

    static CMosaic* CreateRandomMosaic (CDisplay* pDisplay, 
                                        int SpriteLayer, 
                                        int PriorityInLayer, 
                                        float SpeedX, 
                                        float SpeedY, 
                                        EMosaicColor Color,
                                        bool IsSolid);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CRANDOMMOSAIC_H__
