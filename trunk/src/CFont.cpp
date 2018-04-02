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
 *  \file CFont.cpp
 *  \brief Font
 */

#include "StdAfx.h"
#include "CFont.h"
#include "CDisplay.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_STRING_LENGTH               2048    //!< Maximum length for a string to draw
#define CHAR_COUNT_PER_FONTCOLOR        46      //!< Number of characters per font color in the font sprite table
#define CHAR_WIDTH                      10      //!< Size (in pixels) of one character
#define CHAR_HEIGHT                     10
#define CHAR_SPACE                      1       //!< Space (in pixels) between two chars when drawing a string

// Character offset stuff
#define LETTERS_CHAR_OFFSET_BEGIN       0       //!< Beginning character offset for letters
#define NUMBERS_CHAR_OFFSET_BEGIN       26      //!< Beginning character offset for numbers
#define SPECIAL_CHAR_OFFSET_BEGIN       36      //!< Beginning character offset for special characters
#define MINUS_CHAR_OFFSET               0       //!< Char offset for '-' from special characters begin offset
#define PLUS_CHAR_OFFSET                1       //!< Char offset for '+' from special characters begin offset
#define PERIOD_CHAR_OFFSET              2       //!< Char offset for '.' from special characters begin offset
#define COLON_CHAR_OFFSET               3       //!< Char offset for ':' from special characters begin offset
#define EXCLAMATION_CHAR_OFFSET         4       //!< Char offset for '!' from special characters begin offset
#define INTERROGATIVE_CHAR_OFFSET       5       //!< Char offset for '?' from special characters begin offset
#define COMMA_CHAR_OFFSET               6       //!< Char offset for ',' from special characters begin offset
#define LEFTPARENTHESIS_CHAR_OFFSET     7       //!< Char offset for '(' from special characters begin offset
#define RIGHTPARENTHESIS_CHAR_OFFSET    8       //!< Char offset for ')' from special characters begin offset
#define AT_CHAR_OFFSET                  9       //!< Char offset for '@' from special characters begin offset

#define TEXT_PRIORITY                   1       //!< Priority for the text in the sprite layer
#define SHADOW_PRIORITY                 0       //!< Priority for the text shadow in the sprite layer

#define SHADOW_ABS_OFFSET_X             2       //!< ABSOLUTE offset to apply to text position in order to get
#define SHADOW_ABS_OFFSET_Y             2       //!< the shadow position. Sign depends on shadow direction.

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CFont::CFont (void)
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pDisplay = NULL;
    
    m_SpriteLayer = 0;
    m_DrawShadow = false;    
    m_TextColorOffset = 0;
    m_ShadowColorOffset = 0;
    m_ShadowOffsetX = 0;
    m_ShadowOffsetY = 0;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CFont::~CFont (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::Create (void)
{
    // Check if all the objects to communicate with are set
    ASSERT (m_pDisplay != NULL);

    // Set default text/shadow color
    m_TextColorOffset = GetColorOffset (FONTCOLOR_WHITE);
    m_ShadowColorOffset = GetColorOffset (FONTCOLOR_BLACK);

    // Draw the characters in sprite layer 0 by default
    m_SpriteLayer = 0;

    // By default don't draw any text shadow
    m_DrawShadow = false;

    // Set default shadow direction
    SetShadowDirection (SHADOWDIRECTION_UP);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::Destroy (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::DrawString (int PositionX, int PositionY, const char *pString)
{
    int CharacterOffset;        // Character offset to use to get the character sprite to
                                // draw (not taking font color into account)
    
    // While this not the end of string
    while (*pString != '\0')
    {
        // If the current character to draw is a lower case letter
        if (*pString >= 'a' && *pString <= 'z')
        {
            // Set the offset of the character to draw (not taking font color into account)
            CharacterOffset = LETTERS_CHAR_OFFSET_BEGIN + *pString - 'a';
        }
        // If the current character to draw is an upper case letter
        else if (*pString >= 'A' && *pString <= 'Z')
        {
            // Set the offset of the character to draw (not taking font color into account)
            CharacterOffset = LETTERS_CHAR_OFFSET_BEGIN + *pString - 'A';
        }
        // If the current character to draw is a number
        else if (*pString >= '0' && *pString <= '9')
        {
            // Set the offset of the character to draw (not taking font color into account)
            CharacterOffset = NUMBERS_CHAR_OFFSET_BEGIN + *pString - '0';
        }
        // Else it's a special character or a character the font doesn't have
        else
        {
            // Identify the character and set the offset of the character 
            // to draw (not taking font color into account)
            switch (*pString)
            {
                case '.' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + PERIOD_CHAR_OFFSET;           break;
                case ',' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + COMMA_CHAR_OFFSET;            break;
                case '!' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + EXCLAMATION_CHAR_OFFSET;      break;
                case '?' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + INTERROGATIVE_CHAR_OFFSET;    break;
                case '(' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + LEFTPARENTHESIS_CHAR_OFFSET;  break;
                case ')' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + RIGHTPARENTHESIS_CHAR_OFFSET; break;
                case '-' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + MINUS_CHAR_OFFSET;            break;
                case '+' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + PLUS_CHAR_OFFSET;             break;
                case '@' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + AT_CHAR_OFFSET;               break;
                case ':' : CharacterOffset = SPECIAL_CHAR_OFFSET_BEGIN + COLON_CHAR_OFFSET;            break;

                // Is it a character that is unsupported by the font?
                // In that case we won't draw any character.
                default  : CharacterOffset = -1; break;          // Remember the character is unsupported
            }
        }

        // If the character to draw is supported
        if (CharacterOffset != -1)
        {
            // Draw current character in string. The offset corresponding to
            // the current font color is added to the character offset in order
            // to get the chosen color for characters.
            m_pDisplay->DrawSprite (PositionX, 
                                    PositionY, 
                                    NULL, 
                                    NULL, 
                                    BMP_GLOBAL_FONT,
                                    m_TextColorOffset + CharacterOffset, 
                                    m_SpriteLayer, 
                                    TEXT_PRIORITY);

            // If we have to draw a shadow under the text
            if (m_DrawShadow)
            {
                // Draw the text shadow
                m_pDisplay->DrawSprite (PositionX + m_ShadowOffsetX,    // Apply shadow offset which depends
                                        PositionY + m_ShadowOffsetY,    // on the chosen shadow direction.
                                        NULL, 
                                        NULL, 
                                        BMP_GLOBAL_FONT,
                                        m_ShadowColorOffset + CharacterOffset, 
                                        m_SpriteLayer, 
                                        SHADOW_PRIORITY);
            }
        }

        // Update position where to draw the next character
        PositionX += CHAR_WIDTH + CHAR_SPACE;

        // Point to next character in string
        pString++;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::Draw (int PositionX, int PositionY, const char *pString, ...)
{
    // Format the given string using the given arguments ("..." parameter)
    char String [MAX_STRING_LENGTH];
    va_list argList;
    va_start (argList, pString);
    vsprintf (String, pString, argList);
    va_end (argList);

    // Draw the given string at given position
    DrawString (PositionX, PositionY, String);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::DrawCenteredX (int BorderLeft, int BorderRight, int PositionY, const char *pString, ...)
{
    // Format the given string using the given arguments ("..." parameter)
    char String [MAX_STRING_LENGTH];
    va_list argList;
    va_start (argList, pString);
    vsprintf (String, pString, argList);
    va_end (argList);

    // Compute X position so that the string we write is centered between the two borders
    int PositionX = ((BorderRight - BorderLeft) - (strlen(String) * (CHAR_WIDTH + CHAR_SPACE) - CHAR_SPACE)) / 2;

    // Draw the given string at center position
    DrawString (PositionX, PositionY, String);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::DrawCenteredY (int PositionX, int BorderUp, int BorderDown, const char *pString, ...)
{
    // Format the given string using the given arguments ("..." parameter)
    char String [MAX_STRING_LENGTH];
    va_list argList;
    va_start (argList, pString);
    vsprintf (String, pString, argList);
    va_end (argList);

    // Compute Y position so that the string we write is centered between the two borders
    int PositionY = ((BorderDown - BorderUp) - CHAR_HEIGHT) / 2;

    // Draw the given string at center position
    DrawString (PositionX, PositionY, String);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::DrawCenteredXY (int BorderUp, int BorderDown, int BorderLeft, int BorderRight, const char *pString, ...)
{
    // Format the given string using the given arguments ("..." parameter)
    char String [MAX_STRING_LENGTH];
    va_list argList;
    va_start (argList, pString);
    vsprintf (String, pString, argList);
    va_end (argList);

    // Compute X position so that the string we write is centered between the two borders
    int PositionX = ((BorderRight - BorderLeft) - (strlen(String) * (CHAR_WIDTH + CHAR_SPACE) - CHAR_SPACE)) / 2;

    // Compute Y position so that the string we write is centered between the two borders
    int PositionY = ((BorderDown - BorderUp) - CHAR_HEIGHT) / 2;

    // Draw the given string at center position
    DrawString (PositionX, PositionY, String);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

int CFont::GetColorOffset (EFontColor FontColor)
{
    // Set the color sprite offset according to the chosen font color
    switch (FontColor)
    {
        case FONTCOLOR_BLUE     : return  0 * CHAR_COUNT_PER_FONTCOLOR;
        case FONTCOLOR_YELLOW   : return  1 * CHAR_COUNT_PER_FONTCOLOR;
        case FONTCOLOR_RED      : return  2 * CHAR_COUNT_PER_FONTCOLOR;
        case FONTCOLOR_GREEN    : return  3 * CHAR_COUNT_PER_FONTCOLOR;
        case FONTCOLOR_WHITE    : return  4 * CHAR_COUNT_PER_FONTCOLOR;
        case FONTCOLOR_BLACK    : return  5 * CHAR_COUNT_PER_FONTCOLOR;
    }

    return 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFont::SetShadowDirection (EShadowDirection ShadowDirection)
{
    // Set the shadow offset according to the selected shadow direction
    switch (ShadowDirection)
    {
        case SHADOWDIRECTION_UP : 
            m_ShadowOffsetX = 0;
            m_ShadowOffsetY = -SHADOW_ABS_OFFSET_Y;
            break;

        case SHADOWDIRECTION_DOWN : 
            m_ShadowOffsetX = 0;
            m_ShadowOffsetY = SHADOW_ABS_OFFSET_Y;
            break;
        
        case SHADOWDIRECTION_LEFT : 
            m_ShadowOffsetX = -SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = 0;
            break;
        
        case SHADOWDIRECTION_RIGHT : 
            m_ShadowOffsetX = SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = 0;
            break;
        
        case SHADOWDIRECTION_UPLEFT : 
            m_ShadowOffsetX = -SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = -SHADOW_ABS_OFFSET_Y;
            break;
        
        case SHADOWDIRECTION_UPRIGHT : 
            m_ShadowOffsetX = SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = -SHADOW_ABS_OFFSET_Y;
            break;
        
        case SHADOWDIRECTION_DOWNLEFT : 
            m_ShadowOffsetX = -SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = SHADOW_ABS_OFFSET_Y;
            break;
        
        case SHADOWDIRECTION_DOWNRIGHT : 
            m_ShadowOffsetX = SHADOW_ABS_OFFSET_X;
            m_ShadowOffsetY = SHADOW_ABS_OFFSET_Y;
            break;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
