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
 *  \file CFont.h
 *  \brief Header file of the font
 */

#ifndef __CFONT_H__
#define __CFONT_H__

class CDisplay;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EFontColor
{
    FONTCOLOR_BLUE,
    FONTCOLOR_YELLOW,
    FONTCOLOR_RED,
    FONTCOLOR_GREEN,
    FONTCOLOR_WHITE,
    FONTCOLOR_BLACK
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EShadowDirection
{
    SHADOWDIRECTION_UP,
    SHADOWDIRECTION_DOWN,
    SHADOWDIRECTION_LEFT,
    SHADOWDIRECTION_RIGHT,
    SHADOWDIRECTION_UPLEFT,
    SHADOWDIRECTION_UPRIGHT,
    SHADOWDIRECTION_DOWNLEFT,
    SHADOWDIRECTION_DOWNRIGHT
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CFont
{
private:

    CDisplay*       m_pDisplay;             //!< We need a display object to draw the string characters
    int             m_TextColorOffset;      //!< Sprite offset to use to write text using the selected font color
    int             m_ShadowColorOffset;    //!< Sprite offset to use to write text shadow using the selected font color
    int             m_SpriteLayer;          //!< Sprite layer in which string characters will be drawn
    bool            m_DrawShadow;           //!< Do we have to draw a shadow under the string we draw?
    int             m_ShadowOffsetX;        //!< Offset to apply to text position in order to get shadow position
    int             m_ShadowOffsetY;

    void            DrawString (int PositionX, int PositionY, const char *pString);
    int             GetColorOffset (EFontColor FontColor);

public:

                    CFont (void);
                    ~CFont (void);
    inline void     SetDisplay (CDisplay *pDisplay);
    void            Create (void);
    void            Destroy (void);
    inline void     SetShadow (bool DrawShadow);
    inline void     SetTextColor (EFontColor FontColor);
    inline void     SetShadowColor (EFontColor FontColor);
    void            SetShadowDirection (EShadowDirection ShadowDirection);
    inline void     SetSpriteLayer (int SpriteLayer);
    void            Draw (int PositionX, int PositionY, const char *pString, ...);
    void            DrawCenteredX (int BorderLeft, int BorderRight, int PositionY, const char *pString, ...);
    void            DrawCenteredY (int PositionX, int BorderUp, int BorderDown, const char *pString, ...);
    void            DrawCenteredXY (int BorderUp, int BorderDown, int BorderLeft, int BorderRight, const char *pString, ...);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CFont::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

inline void CFont::SetSpriteLayer (int SpriteLayer)
{
    m_SpriteLayer = SpriteLayer;
}

inline void CFont::SetShadow (bool DrawShadow)
{
    m_DrawShadow = DrawShadow;
}

inline void CFont::SetTextColor (EFontColor FontColor)
{
    m_TextColorOffset = GetColorOffset (FontColor);
}

inline void CFont::SetShadowColor (EFontColor FontColor)
{
    m_ShadowColorOffset = GetColorOffset (FontColor);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CFONT_H__
