/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008 Jerome Bigot
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
 *  \file CItem.h
 *  \brief Header file of an item
 */

#ifndef __CITEM_H__
#define __CITEM_H__

#include "CElement.h"

class CArena;
class CDisplay;
class CArenaSnapshot;

#include "CExplosion.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Describe a type of item
 *
 *  \sa CBomber::ItemEffect()
 */

enum EItemType
{
    ITEM_NONE,
    ITEM_BOMB,          //!< Bomb item allows a bomber to drop more bombs
    ITEM_FLAME,         //!< Flame item allows a bomber to drop more powerful bombs
    ITEM_KICK,          //!< Kick item allows a bomber to kick bombs
    ITEM_ROLLER,        //!< Roller item allows a bomber to walk faster
    ITEM_SKULL,         //!< Skull item gives a bomber a sickness.
    ITEM_THROW,         //!< Throw glove item allows a bomber to throw bombs
    ITEM_PUNCH,         //!< Boxing glove item allows a bomber to punch bombs
    ITEM_REMOTE,        //!< Remote bombs item allows a bomber to remotely control bomb fuse
    ITEM_SHIELD,        //!< Shield item allows a bomber to be resistance to flames
    ITEM_STRONGWEAK,    //!< Strong item allows a bomber to be strong or weak
    NUMBER_OF_ITEMS     //!< The number of items (this includes ITEM_NONE)
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a possible kind of place for new items. See CItem::CreateItems()

enum EItemPlace
{
    ITEMPLACE_FLOOR,            //!< Create new items on the floor only
    ITEMPLACE_SOFTWALLS         //!< Create new items under soft walls only
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the flying state of an item. Is is flying? In which direction?

enum EItemFlying
{
    ITEMFLYING_NONE = -1,       //!< The item is not flying
    ITEMFLYING_UP = 0,          //!< The item is flying and moving up
    ITEMFLYING_DOWN,            //!< The item is flying and moving down
    ITEMFLYING_LEFT,            //!< The item is flying and moving left
    ITEMFLYING_RIGHT            //!< The item is flying and moving right
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! An element in the arena which represents an item.
class CItem : public CElement
{
private:

    float               m_fX;                       //!< Float position X (in pixels) in the arena
    float               m_fY;                       //!< Float position Y (in pixels) in the arena
    int                 m_iX;                       //!< Integer position X (in pixels) in the arena
    int                 m_iY;                       //!< Integer position Y (in pixels) in the arena
    int                 m_BlockX;                   //!< Position X (in blocks) in the arena grid
    int                 m_BlockY;                   //!< Position Y (in blocks) in the arena grid
    int                 m_Sprite;                   //!< Current item sprite to use when displaying the item
    int                 m_Sprite0;                  //!< First sprite number of the item flash animation
    int                 m_Sprite1;                  //!< Second sprite number of the item flash animation
    float               m_Timer;                    //!< Time counter for animation
    bool                m_Dead;                     //!< Should the item be deleted by the arena?
    bool                m_Burning;                  //!< Is the item burning?
    EItemType           m_Type;                     //!< Type of this item
    bool                m_Fumes;                    //!< Is the fumes animation playing?
    int                 m_FumeSprite;               //!< Current fume frame number to use to get each fume sprite number.
    EItemFlying         m_Flying;                   //!< Is the item currently flying and in which direction?
    float               m_FlyTime;                  //!< How long (in seconds) has the item been flying?
                                                    
    void                SetSprites (void);          //!< Set the sprite numbers according to the item type
        
public:

                        CItem (void);               //!< Constructor (initialize the base class)
    virtual             ~CItem (void);              //!< Destructor (uninitialize the base class)
    void                Create (int BlockX, int BlockY, EItemType Type, bool Fumes, bool FlyingRandom); //!< Initialize the item
    void                Destroy (void);             //!< Uninitialize the item
    bool                Update (float DeltaTime);   //!< Update the item and return whether the item should be deleted by the arena
    void                Display (void);             //!< Display the item
    void                OnWriteSnapshot (CArenaSnapshot& Snapshot);
    void                OnReadSnapshot (CArenaSnapshot& Snapshot);
    void                Burn (EBurnDirection BurnDirection); //!< Make the item react when an explosion hits this item. The direction of the hit is specified.
    void                Crush (void);               //!< Make the item react when crushed by a wall.
    static bool         CreateItems(CArena *pArena, EItemPlace ItemPlace, int NumberOfItemBombs, int NumberOfItemFlames, int NumberOfItemRollers, int NumberOfItemKicks, int NumberOfItemSkulls, int NumberOfItemThrow, int NumberOfItemPunch, int NumberOfItemRemote, int NumberOfItemShield, int NumberOfItemStrongWeak); //!< Create the specified amount of items in an arena, in the specified EItemPlace.
    inline int          GetBlockX (void);           //!< Return the block position X of the item
    inline int          GetBlockY (void);           //!< Return the block position Y of the item
    inline bool         IsBurning (void);           //!< Return whether the item is burning
    inline EItemType    GetType (void);             //!< Return the type of the item
    inline bool         IsFlying (void);            //!< Return whether the item is currently flying.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CItem::GetBlockX (void) 
{ 
    return m_BlockX; 
}

inline int CItem::GetBlockY (void) 
{ 
    return m_BlockY; 
}

inline bool CItem::IsBurning (void) 
{ 
    return m_Burning; 
}

inline EItemType CItem::GetType (void) 
{ 
    return m_Type; 
}

inline bool CItem::IsFlying (void)
{
    return m_Flying != ITEMFLYING_NONE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
