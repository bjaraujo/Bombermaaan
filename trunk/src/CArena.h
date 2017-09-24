/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold

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
 *  \file CArena.h
 *  \brief Header file of the arena
 */

#ifndef __CARENA_H__
#define __CARENA_H__

#include "CArenaCloser.h"
#include "CBomber.h"
#include "CExplosion.h"
#include "CBomb.h"
#include "CItem.h"
#include "CWall.h"
#include "CFloor.h"

class CClock;
class CDisplay;
class CSound;
//class COptions;
class CArenaSnapshot;

#include "COptions.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// The EBlockHas enum allows a combination of flags to describe 
// exactly what can be seen (but very basic information about bombers) 
// on a specific block.
// Example : if there is a soft wall with an item bomb below,
// the following flags must be turned on : BLOCKHAS_FLOOR,
// BLOCKHAS_WALL, BLOCKHAS_WALLSOFT, BLOCKHAS_ITEM, BLOCKHAS_ITEMBOMB.

typedef int TBlockHas;

#define BLOCKHAS_NONE                   (1 <<  0) //!< The block has nothing in it
#define BLOCKHAS_FLOOR                  (1 <<  1) //!< The block has a floor
#define BLOCKHAS_WALL                   (1 <<  2) //!< The block has a wall of any kind
#define BLOCKHAS_WALLSOFT               (1 <<  3) //!< The block has a soft wall
#define BLOCKHAS_WALLHARD               (1 <<  4) //!< The block has a hard wall
#define BLOCKHAS_WALLFALLING            (1 <<  5) //!< The block has a falling wall
#define BLOCKHAS_WALLBURNING            (1 <<  6) //!< The block has a burning wall
#define BLOCKHAS_ITEM                   (1 <<  7) //!< The block has an item of any kind
#define BLOCKHAS_ITEMBOMB               (1 <<  8) //!< The block has a bomb item
#define BLOCKHAS_ITEMFLAME              (1 <<  9) //!< The block has a flame item
#define BLOCKHAS_ITEMROLLER             (1 << 10) //!< The block has a roller item
#define BLOCKHAS_ITEMSKULL              (1 << 11) //!< The block has a skull item
#define BLOCKHAS_ITEMKICK               (1 << 12) //!< The block has a kick item
#define BLOCKHAS_ITEMBURNING            (1 << 13) //!< The block has a burning item
#define BLOCKHAS_BOMB                   (1 << 14) //!< The block has a bomb
#define BLOCKHAS_FLAME                  (1 << 15) //!< The block has a flame
#define BLOCKHAS_EXPLOSION              (1 << 16) //!< The block has a center of explosion
#define BLOCKHAS_BOMBER                 (1 << 17) //!< The block has at least one bomber
#define BLOCKHAS_BOMBERALIVE            (1 << 18) //!< The block has at least one alive bomber
#define BLOCKHAS_BOMBERDYING            (1 << 19) //!< The block has at least one dying bomber
#define BLOCKHAS_FLOORWITHMOVEEFFECT    (1 << 20) //!< The block makes a bomb move
#define BLOCKHAS_ITEMTHROW              (1 << 21) //!< The block has a throw glove item (blue glove)
#define BLOCKHAS_ITEMPUNCH              (1 << 22) //!< The block has a punch glove item (red glove)
#define BLOCKHAS_ITEMREMOTE             (1 << 23) //!< The block has a remote fuse bomb item
#define BLOCKHAS_ITEMSHIELD             (1 << 24) //!< The block has a shield item
#define BLOCKHAS_ITEMSTRONGWEAK         (1 << 25) //!< The block has a strong/weak item

// note hat (1 << 31) is the maximum value for a 32-bit integer
// BLOCKHAS_ITEMx, where x = {BOMB,FLAME,ROLLER,KICK,THROW,PUNCH,REMOTE} are not necessary

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_FLOORS      (ARENA_WIDTH * ARENA_HEIGHT)
#define MAX_WALLS       (2 * ARENA_WIDTH * ARENA_HEIGHT)        // Don't forget the falling walls in 
#define MAX_BOMBS       50
#define MAX_ITEMS       50
#define MAX_EXPLOSIONS  50
#define MAX_BOMBERS     MAX_PLAYERS
#define MAX_TEAMS       MAX_PLAYERS

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CArena
{
private:

    CDisplay*               m_pDisplay;             //!< Display object to pass to elements so they can display something
    CSound*                 m_pSound;               //!< Sound object to pass to elements so they can make sound
    COptions*               m_pOptions;             //!< Options object to use to get information about what the user chose
    CFloor                  m_Floors [MAX_FLOORS];
    CWall                   m_Walls [MAX_WALLS];
    CBomb                   m_Bombs [MAX_BOMBS];
    CItem                   m_Items [MAX_ITEMS];
    CExplosion              m_Explosions [MAX_EXPLOSIONS];
    CBomber                 m_Bombers [MAX_BOMBERS];
    CArenaCloser            m_ArenaCloser;
    TBlockHas               m_BlockHas [ARENA_WIDTH][ARENA_HEIGHT]; //!< Simplified view of the arena made using the element containers. This allows a fast access to the basic information of the arena.
    int                     m_BombsInUse;           //!< A counter for bombs in use
    bool                    m_Prediction;

                            CArena (const CArena& Arena);
    void                    ClearBlock (int X, int Y);
    void                    UpdateElements (float DeltaTime);
    void                    UpdateView (void);
    inline bool             GetBlockHas (int BlockX, int BlockY, TBlockHas BlockHas);
    inline void             SetBlockHas (int BlockX, int BlockY, TBlockHas BlockHas);
    void                    DeleteBomber       (int Index);
    void                    DeleteBomb         (int Index);
    void                    DeleteWall         (int Index);
    void                    DeleteItem         (int Index);
    void                    DeleteExplosion    (int Index);
    void                    DeleteFloor        (int Index);

public:
    
                            CArena (void);
                            ~CArena (void);
    CArena &                operator = (const CArena &Arena);

    inline void             SetDisplay (CDisplay *pDisplay);
    inline void             SetSound (CSound *pSound);
    inline void             SetOptions (COptions *pOptions);
    
    void                    Create (void);
    void                    Destroy (void);
    void                    Update (float DeltaTime);
    void                    UpdateSingleBomber (int Player, float DeltaTime);
    void                    Display (void);
    
    void                    WriteSnapshot (CArenaSnapshot& Snapshot);
    void                    ReadSnapshot (CArenaSnapshot& Snapshot);

    inline bool             IsPrediction();
    inline void             SetPrediction (bool Active);

    inline CArenaCloser&    GetArenaCloser (void);
    
    inline CBomber &        GetBomber    (int Index);
    inline CBomb &          GetBomb      (int Index);
    inline CWall &          GetWall      (int Index);
    inline CItem &          GetItem      (int Index);
    inline CExplosion &     GetExplosion (int Index);
    inline CFloor &         GetFloor     (int Index);
    
    inline int              MaxFloors      (void);
    inline int              MaxWalls       (void);
    inline int              MaxBombs       (void);
    inline int              MaxItems       (void);
    inline int              MaxExplosions  (void);
    inline int              MaxBombers     (void);
    inline int              MaxTeams       (void);

    inline int              BombsInUse     (void);
        
    void                    NewFloor        (int BlockX, int BlockY, EBlockType BlockType);
    void                    NewWall         (int BlockX, int BlockY, EWallType Type);
    void                    NewBomb         (int BlockX, int BlockY, int FlameSize, float TimeLeft, int OwnerPlayer);
    void                    NewItem         (int BlockX, int BlockY, EItemType Type, bool Fumes, bool FlyingRandom);
    void                    NewExplosion    (int BlockX, int BlockY, int FlameSize);
    void                    NewBomber       (int BlockX, int BlockY, int Player);
    
    EFloorAction            GetFloorAction  ( int BlockX, int BlockY );
    
    inline int              ToBlock (int Position);
    inline int              ToPosition (int Block);
    
    inline bool             IsWall          (int BlockX, int BlockY);
    inline bool             IsSoftWall      (int BlockX, int BlockY);
    inline bool             IsHardWall      (int BlockX, int BlockY);
    inline bool             IsFallingWall   (int BlockX, int BlockY);
    inline bool             IsBurningWall   (int BlockX, int BlockY);
    inline bool             IsItem          (int BlockX, int BlockY);
    inline bool             IsBombItem      (int BlockX, int BlockY);
    inline bool             IsFlameItem     (int BlockX, int BlockY);
    inline bool             IsRollerItem    (int BlockX, int BlockY);
    inline bool             IsKickItem      (int BlockX, int BlockY);
    inline bool             IsSkullItem     (int BlockX, int BlockY);
    inline bool             IsThrowItem     (int BlockX, int BlockY);
    inline bool             IsPunchItem     (int BlockX, int BlockY);
    inline bool             IsRemoteItem    (int BlockX, int BlockY);
    inline bool             IsBurningItem   (int BlockX, int BlockY);
    inline bool             IsBomb          (int BlockX, int BlockY);
    inline bool             IsFlame         (int BlockX, int BlockY);
    inline bool             IsExplosion     (int BlockX, int BlockY);
    inline bool             IsBomber        (int BlockX, int BlockY);
    inline bool             IsAliveBomber   (int BlockX, int BlockY);
    inline bool             IsDyingBomber   (int BlockX, int BlockY);
    inline bool             IsFloor         (int BlockX, int BlockY);
    inline bool             IsFloorWithMoveEffect( int BlockX, int BlockY );
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline bool CArena::IsPrediction()
{
    return m_Prediction;
}

inline void CArena::SetPrediction (bool Active)
{
    m_Prediction = Active;
}

inline CArenaCloser& CArena::GetArenaCloser (void)
{
    return m_ArenaCloser;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline CBomber &CArena::GetBomber (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_BOMBERS);

    return m_Bombers[Index];
}

inline CBomb &CArena::GetBomb (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_BOMBS);

    return m_Bombs[Index];
}

inline CWall &CArena::GetWall (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_WALLS);

    return m_Walls[Index];
}

inline CItem &CArena::GetItem (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_ITEMS);

    return m_Items[Index];
}

inline CExplosion &CArena::GetExplosion (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_EXPLOSIONS);

    return m_Explosions[Index];
}

inline CFloor &CArena::GetFloor (int Index)
{
    ASSERT (Index >= 0 && Index < MAX_FLOORS);

    return m_Floors[Index];
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CArena::MaxFloors (void)   
{ 
    return MAX_FLOORS; 
}

inline int CArena::MaxWalls (void)   
{ 
    return MAX_WALLS; 
}

inline int CArena::MaxBombs (void)   
{ 
    return MAX_BOMBS; 
}

inline int CArena::MaxItems (void)   
{ 
    return MAX_ITEMS; 
}

inline int CArena::MaxExplosions (void)   
{ 
    return MAX_EXPLOSIONS; 
}

inline int CArena::MaxBombers (void)   
{ 
    return MAX_BOMBERS; 
}

inline int CArena::MaxTeams(void)
{
    return MAX_TEAMS;
}

/**
 * @brief   return the bomb counter
 * @return  how many bombs are currently used
 */
inline int CArena::BombsInUse (void)   
{ 
    return m_BombsInUse; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline bool CArena::GetBlockHas (int BlockX, int BlockY, TBlockHas BlockHas)
{
    // Check coordinates
    ASSERT (BlockX >= 0 && BlockX < ARENA_WIDTH);    
    ASSERT (BlockY >= 0 && BlockY < ARENA_HEIGHT);

    // Return if the specified block has the required combination
    return (m_BlockHas[BlockX][BlockY] & BlockHas) != 0;
}

inline void CArena::SetBlockHas (int BlockX, int BlockY, TBlockHas BlockHas)
{
    // Check coordinates
    ASSERT (BlockX >= 0 && BlockX < ARENA_WIDTH);
    ASSERT (BlockY >= 0 && BlockY < ARENA_HEIGHT);

    m_BlockHas[BlockX][BlockY] |= BlockHas;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CArena::SetDisplay (CDisplay *pDisplay)
{
    // Save the display object pointer to pass to elements
    m_pDisplay = pDisplay;
    
    // Set the display object for already existing elements
    int Index;

    for (Index = 0 ; Index < MaxFloors() ; Index++)
        m_Floors[Index].SetDisplay (pDisplay);

    for (Index = 0 ; Index < MaxWalls() ; Index++)
        m_Walls[Index].SetDisplay (pDisplay);

    for (Index = 0 ; Index < MaxBombs() ; Index++)
        m_Bombs[Index].SetDisplay (pDisplay);

    for (Index = 0 ; Index < MaxBombers() ; Index++)
        m_Bombers[Index].SetDisplay (pDisplay);

    for (Index = 0 ; Index < MaxItems() ; Index++)
        m_Items[Index].SetDisplay (pDisplay);

    for (Index = 0 ; Index < MaxExplosions() ; Index++)
        m_Explosions[Index].SetDisplay (pDisplay);
}

inline void CArena::SetSound (CSound *pSound)
{
    // Save the sound object pointer to pass to elements
    m_pSound = pSound;
    
    // Set the sound object for already existing elements
    int Index;

    for (Index = 0 ; Index < MaxFloors() ; Index++)
        m_Floors[Index].SetSound (pSound);

    for (Index = 0 ; Index < MaxWalls() ; Index++)
        m_Walls[Index].SetSound (pSound);

    for (Index = 0 ; Index < MaxBombs() ; Index++)
        m_Bombs[Index].SetSound (pSound);

    for (Index = 0 ; Index < MaxBombers() ; Index++)
        m_Bombers[Index].SetSound (pSound);

    for (Index = 0 ; Index < MaxItems() ; Index++)
        m_Items[Index].SetSound (pSound);

    for (Index = 0 ; Index < MaxExplosions() ; Index++)
        m_Explosions[Index].SetSound (pSound);
}

inline void CArena::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
    m_ArenaCloser.SetOptions (pOptions);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline bool CArena::IsWall (int BlockX, int BlockY) 
{ 
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_WALL); 
}

inline bool CArena::IsSoftWall (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_WALLSOFT); 
}

inline bool CArena::IsHardWall (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_WALLHARD); 
}

inline bool CArena::IsFallingWall (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_WALLFALLING); 
}

inline bool CArena::IsBurningWall (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_WALLBURNING); 
}

inline bool CArena::IsItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEM); 
}

inline bool CArena::IsBombItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMBOMB); 
}

inline bool CArena::IsFlameItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMFLAME); 
}

inline bool CArena::IsRollerItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMROLLER); 
}

inline bool CArena::IsKickItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMKICK); 
}

inline bool CArena::IsSkullItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMSKULL); 
}

inline bool CArena::IsThrowItem (int BlockX, int BlockY)
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMTHROW); 
}

inline bool CArena::IsPunchItem (int BlockX, int BlockY)
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMPUNCH); 
}

inline bool CArena::IsRemoteItem (int BlockX, int BlockY)
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMREMOTE); 
}

inline bool CArena::IsBurningItem (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_ITEMBURNING); 
}

inline bool CArena::IsBomb (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_BOMB); 
}

inline bool CArena::IsFlame (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_FLAME); 
}

inline bool CArena::IsExplosion (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_EXPLOSION); 
}

inline bool CArena::IsBomber (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBER); 
}

inline bool CArena::IsAliveBomber (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBERALIVE); 
}

inline bool CArena::IsDyingBomber (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_BOMBERDYING); 
}

inline bool CArena::IsFloor (int BlockX, int BlockY) 
{
    return GetBlockHas(BlockX, BlockY, BLOCKHAS_FLOOR); 
}

inline bool CArena::IsFloorWithMoveEffect (int BlockX, int BlockY)
{
    ASSERT( IsFloor( BlockX, BlockY ) );

    return GetBlockHas( BlockX, BlockY, BLOCKHAS_FLOORWITHMOVEEFFECT );
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CArena::ToBlock (int Position)  
{ 
    return (Position / BLOCK_SIZE); 
}

inline int CArena::ToPosition (int Block)  
{ 
    return (Block * BLOCK_SIZE); 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif //__CARENA_H__
