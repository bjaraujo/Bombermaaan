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
 *  \file CRandomMosaic.cpp
 *  \brief A random mosaic
 */

#include "StdAfx.h"
#include "CRandomMosaic.h"
#include "CMosaic.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

SMosaicTileProperties CRandomMosaic::m_MosaicTileProperties [4][2] =
{
    // PURPLE
    {
        { BMP_PURPLE_BACKGROUND_SOLID, 0, 7, 12, 82, 41 },
        { BMP_PURPLE_BACKGROUND_BOMB, 0, 7, 12, 82, 41 },
    },
    
    // GREEN
    {
        { BMP_GREEN_BACKGROUND_SOLID, 0, 7, 12, 82, 41 },
        { BMP_GREEN_BACKGROUND_BOMB, 0, 7, 12, 82, 41 },
    },

    // BLUE
    {
        { BMP_BLUE_BACKGROUND_SOLID, 0, 7, 12, 82, 41 },
        { BMP_BLUE_BACKGROUND_BOMB, 0, 7, 12, 82, 41 },
    },

    // RED
    {
        { BMP_RED_BACKGROUND_SOLID, 0, 7, 12, 82, 41 },
        { BMP_RED_BACKGROUND_BOMB, 0, 7, 12, 82, 41 },
    }

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMosaic* CRandomMosaic::CreateRandomMosaic (CDisplay* pDisplay, 
                                            int SpriteLayer, 
                                            int PriorityInLayer, 
                                            float SpeedX, 
                                            float SpeedY, 
                                            EMosaicColor Color,
                                            bool IsSolid)
{

    SMosaicTileProperties* pMosaicTileProperties;

    if (IsSolid)
        pMosaicTileProperties = &m_MosaicTileProperties[(int)Color][0];
    else
        pMosaicTileProperties = &m_MosaicTileProperties[(int)Color][1];

    CMosaic* pNewMosaic = new CMosaic;
    
    pNewMosaic->SetDisplay (pDisplay);
    pNewMosaic->Create (pMosaicTileProperties->SpriteTable, 
                        pMosaicTileProperties->Sprite, 
                        SpriteLayer, 
                        PriorityInLayer, 
                        pMosaicTileProperties->Width, 
                        pMosaicTileProperties->Height, 
                        pMosaicTileProperties->CountX, 
                        pMosaicTileProperties->CountY, 
                        SpeedX,
                        SpeedY);

    return pNewMosaic;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
