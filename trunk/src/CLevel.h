/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold
    Copyright (C) 2008 Jerome Bigot

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
 *  \file CLevel.h
 *  \brief Header file for the level
 */

#ifndef __CLEVEL_H__
#define __CLEVEL_H__

#include "CItem.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EBlockType
{
    BLOCKTYPE_HARDWALL,            //!< There must be a hard wall here
    BLOCKTYPE_SOFTWALL,            //!< There must be a soft wall here
    BLOCKTYPE_RANDOM,              //!< There must be either a soft wall, or a free place here (random)
    BLOCKTYPE_FREE,                //!< There must be a free place here
    BLOCKTYPE_WHITEBOMBER,         //!< The white bomber must start here
    BLOCKTYPE_BLACKBOMBER,         //!< The black bomber must start here
    BLOCKTYPE_REDBOMBER,           //!< The red bomber must start here
    BLOCKTYPE_BLUEBOMBER,          //!< The blue bomber must start here
    BLOCKTYPE_GREENBOMBER,         //!< The green bomber must start here
    BLOCKTYPE_MOVEBOMB_RIGHT,      //!< A bomb starts moving right if placed here
    BLOCKTYPE_MOVEBOMB_DOWN,       //!< A bomb starts moving down if placed here
    BLOCKTYPE_MOVEBOMB_LEFT,       //!< A bomb starts moving left if placed here
    BLOCKTYPE_MOVEBOMB_UP,         //!< A bomb starts moving up if placed here
    BLOCKTYPE_ITEM_BOMB,           //!< A bomb item if placed here
    BLOCKTYPE_ITEM_FLAME,          //!< A flame item if placed here
    BLOCKTYPE_ITEM_ROLLER,         //!< A roller item if placed here
    BLOCKTYPE_ITEM_KICK,           //!< A kick item if placed here
    BLOCKTYPE_ITEM_THROW,          //!< A throw item if placed here
    BLOCKTYPE_ITEM_PUNCH,          //!< A punch item if placed here
    BLOCKTYPE_ITEM_SKULL,          //!< A skull item if placed here
    BLOCKTYPE_ITEM_REMOTES,        //!< A remote item if placed here
    BLOCKTYPE_ITEM_SHIELD,         //!< A shield item if placed here
    BLOCKTYPE_ITEM_STRONGWEAK      //!< A strong/weak item if placed here
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EBomberSkills
{
    BOMBERSKILL_DUMMYFIRST,
    BOMBERSKILL_FLAME,
    BOMBERSKILL_BOMBS,
    BOMBERSKILL_BOMBITEMS,
    BOMBERSKILL_FLAMEITEMS,
    BOMBERSKILL_ROLLERITEMS,
    BOMBERSKILL_KICKITEMS,
    BOMBERSKILL_THROWITEMS,
    BOMBERSKILL_PUNCHITEMS,
    BOMBERSKILL_REMOTEITEMS,
    BOMBERSKILL_SHIELDITEMS,
    BOMBERSKILL_STRONGWEAKITEMS,
    NUMBER_OF_BOMBERSKILLS
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Initial number of items when a new arena is built
#define INITIAL_ITEMBOMB        11
#define INITIAL_ITEMFLAME       8
#define INITIAL_ITEMROLLER      7
#define INITIAL_ITEMKICK        2
#define INITIAL_ITEMSKULL       1
#define INITIAL_ITEMTHROW       2
#define INITIAL_ITEMPUNCH       2
#define INITIAL_ITEMREMOTE      2
#define INITIAL_ITEMSHIELD      1
#define INITIAL_ITEMSTRONGWEAK  1

// Initial flame size
#define INITIAL_FLAMESIZE       2

// Initial number of bombs the bomber can drop
#define INITIAL_BOMBS           1

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Contains all settings of one level
class CLevel
{
private:

    EBlockType          m_ArenaData[ARENA_WIDTH][ARENA_HEIGHT];         //!< Arena cells are walls etc.?
    std::string         m_FilenameShort;                                //!< The short level file name without path
    std::string         m_FilenameFull;                                 //!< The full name of a level file including path
    int                 m_NumberOfItemsInWalls[NUMBER_OF_ITEMS];        //!< The number of items in the soft walls
    int                 m_InitialBomberSkills[NUMBER_OF_BOMBERSKILLS];  //!< The initial bomber skills

    bool                LoadVersion1(std::ifstream& File);                    //!< Load level file version 1
    bool                LoadVersion2(std::string filename);                   //!< Load level file version 2 (requiredRemoteFuse = false) or 3 (requiredRemoteFuse = true)
    bool                CheckMaxNumberOfItems(unsigned int *sumOfMaxItems);   //!< Check if number of max items is valid
    bool                Validate();                                           //!< Check if this level is valid

public:

    CLevel(std::string filename_full, std::string filename_short);     //!< Constructor.
    ~CLevel(void);                                 //!< Destructor.
    bool                LoadFromFile();                                 //!< Load game level data from the file.
    inline EBlockType   GetBlockType(int X, int Y);
    inline int          GetNumberOfItemsInWalls(EItemType ItemType);
    inline int          GetInitialBomberSkills(EBomberSkills BomberSkill);
    inline const char*  GetLevelName(void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline EBlockType CLevel::GetBlockType(int X, int Y)
{
    ASSERT(X >= 0 && X < ARENA_WIDTH);
    ASSERT(Y >= 0 && Y < ARENA_HEIGHT);

    return m_ArenaData[X][Y];
}

inline int CLevel::GetNumberOfItemsInWalls(EItemType ItemType)
{
    ASSERT(ItemType > ITEM_NONE && ItemType < NUMBER_OF_ITEMS);

    return m_NumberOfItemsInWalls[ItemType];
}

inline int CLevel::GetInitialBomberSkills(EBomberSkills BomberSkill)
{
    ASSERT(BomberSkill > BOMBERSKILL_DUMMYFIRST && BomberSkill < NUMBER_OF_BOMBERSKILLS);

    return m_InitialBomberSkills[BomberSkill];
}

inline const char* CLevel::GetLevelName(void)
{
    return m_FilenameShort.c_str();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CLEVEL_H__
