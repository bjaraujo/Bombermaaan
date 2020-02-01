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
 *  \file CBomber.h
 *  \brief Header file of the bomber
 */

#ifndef __CBOMBER_H__
#define __CBOMBER_H__

#include "CBomberMove.h"
#include "CElement.h"
#include "COptions.h"

class CArena;
class CDisplay;
class CBomb;
class CArenaSnapshot;
class COptions;
class CTeam;

#include "CBomb.h"
#include "CItem.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a bomber action command

enum EBomberAction
{
    BOMBERACTION_NONE, //!< None
    BOMBERACTION_ACTION1, //!< Drop or hold a bomb
    BOMBERACTION_ACTION2 //!< Stop or punch a bomb
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the state of a bomber : what is he doing? what is happening to him?
enum EBomberState
{
    BOMBERSTATE_WALK, //!< Not holding a bomb ; walking or not walking.
    BOMBERSTATE_WALK_HOLD, //!< Holding a bomb ; walking or not walking.
    BOMBERSTATE_DEATH, //!< Dying
    BOMBERSTATE_LIFT, //!< Lifting a bomb
    BOMBERSTATE_THROW, //!< Throwing a bomb
    BOMBERSTATE_PUNCH, //!< Punching a bomb
    BOMBERSTATE_STUNT, //!< Stunt by a bomb that bounced on his head
    MAX_NUMBER_OF_STATES
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a sprite table with bomber sprites inside
struct SBomberSpriteTable
{
    int SpriteTableNumber; //!< Number of the sprite table
    int NumberOfSpritesPerColor; //!< Number of sprites representing a single bomber color (white, red, etc).
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the dead state of the bomber

enum EDead
{
    DEAD_ALIVE, //!< Healthy
    DEAD_DYING, //!< Almost dead
    DEAD_DEAD //!< Dead
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Describes a sickness
 *
 *  \sa CBomber::ItemEffect()
 */

enum ESick
{
    SICK_NOTSICK = -1, //!< No sickness
    SICK_INVERTION = 0, //!< Inverted controls
    SICK_SMALLFLAME, //!< Bombs with small flames
    SICK_CONSTIPATED, //!< Cannot drop bomb
    SICK_COLIC, //!< Always drop bomb
    SICK_SLOW, //!< Slow move
    SICK_FAST, //!< Fast move
    SICK_INERTIA, //!< Always move
    SICK_LONGBOMB, //!< Bombs tick twice longer
    SICK_SHORTBOMB, //!< Bombs tick twice shorter
    SICK_INVISIBILITY, //!< Bomber is invisible
    SICK_FLAMEPROOF, //!< Bomber cannot burn
    NUMBER_SICKNESSES //!< Number of sicknesses
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! An element in the arena which represents a bomber.
class CBomber : public CElement
{
private:
    CBomberMove m_BomberMove; //!< Bomber movement manager object.
    EBomberAction m_BomberAction; //!< Describes the action that the owner player currently wants the bomber to perform.
    EBomberAction m_LastBomberAction; //!< Action from last game update
    int m_Sprite; //!< Current bomber's sprite to display
    int m_SpriteOverlay; //!< Current bomber's sprite overlay to display
    int m_AnimationSprites[5]; //!< Sprite numbers for animations, their values depend on bomber direction
    int m_Page; //!< Current sprite page number to use when displaying
    float m_Timer; //!< Time counter for general use (walk and death animation, delay...)
    float m_SickTimer; //!< Time counter for sickness flashing animation
    int m_TotalBombs; //!< Number of bombs the bomber can drop together
    int m_UsedBombs; //!< Current number of its bombs ticking in the arena
    int m_FlameSize; //!< Size of the flames when one of his bombs explodes
    int m_Speed; //!< Current speed of the bomber in pixels per second (when not sick)
    ESick m_Sickness; //!< Type of current sickness
    int m_NumberOfBombItems; //!< Number of picked up bomb items
    int m_NumberOfFlameItems; //!< Number of picked up flame items
    int m_NumberOfRollerItems; //!< Number of picked up roller items
    int m_NumberOfKickItems; //!< Number of picked up kick items
    int m_NumberOfThrowItems; //!< Number of picked up throw items
    int m_NumberOfPunchItems; //!< Number of picked up punch items
    int m_NumberOfRemoteItems; //!< Number of picked up remote controler items
    bool m_isStrongWeak; //!< Bomber is strong
    float m_ShieldTime; //!< Shield time
    float m_TimeSinceLastSick; //!< Time since last sick event
    bool m_ReturnedItems; //!< Did the bomber return the items he picked up to the arena?
    int m_Player; //!< Number of the player represented by the bomber
    EDead m_Dead; //!< Dead state : alive, dying or dead
    bool m_Neighbours[MAX_PLAYERS]; //!< Bombers who are on the same block as this bomber
    bool m_JustGotSick; //!< True if the bomber just got sick
    float m_LiftingTimeElapsed; //!< How many seconds have elapsed since we started lifting a bomb?
    float m_ThrowingTimeElapsed; //!< How many seconds have elapsed since we started throwing a bomb?
    float m_PunchingTimeElapsed; //!< How many seconds have elapsed since we started punching a bomb?
    float m_StuntTimeElapsed; //!< How many seconds have elapsed since we got stunt?
    EBomberState m_BomberState; //!< State of the bomber, describes what he is currently doing.
    int m_BombIndex; //!< Index of the bomb the bomber is either holding, lifting or punching (when the bomber is throwing, this index is -1).
    static SBomberSpriteTable m_BomberSpriteTables[MAX_NUMBER_OF_STATES]; //!< Information about the sprite table to use for each bomber state.
    bool m_MakeInvisible; //!< If true, the bomber isn't visible in the arena (used for contamination)
    bool m_HasExisted; //!< If true, this bomber exists or has existed in this match (m_Exist is set to false after the bomber' death)
    COptions* p_Options; //!< Pointer to the COptions object
    EBomberAction m_CountBomberActionDuration;
    float m_BomberActionDuration;
    bool m_dropMassBombPossible;

    CTeam* p_Team;

    void Animate(float DeltaTime);
    void ReturnItems(float DeltaTime); //!< Manage the return of the items this bomber owns if he is dead
    void Action(); //!< Make the bomber perform the bomber action he was ordered.
    void Contamination(); //!< Manage sickness contamination (contaminate if sick), update contamination members.
    void UsedBombs(); //!< Update the number of bombs the bomber is currently using.
    void Heal(); //!< Remove the bomber sickness if he is sick.
    bool TryMove(float fPixels); //!< Make the bomber move in the current direction (bomber move) and return if he could move.
    void Die(); //!< Make the bomber start dying.
    float GetBombTime(); //!< Get the current bomb time, i.e. the time left before a bomb this bomber drops will explode. It depends on current sickness.
    void MakeBombFly(EBombFlightType FlightType); //!< Makes the bomber's bomb fly now.

protected:
    // These methods are protected so that bombers can check
    // each other's positions for the contamination ability.

    inline void SetSickness(ESick Sickness); //!< Set the current sickness of the bomber.
    inline int GetX(); //!< Get the integer X position (in pixels) of the bomber in the arena
    inline int GetY(); //!< Get the integer Y position (in pixels) of the bomber in the arena

public:
    CBomber(); //!< Constructor (initialize the base class)
    virtual ~CBomber(); //!< Destructor (uninitialize the base class)
    inline void SetArena(CArena* pArena); //!< Redefinition of the inherited method.
    void Create(int BlockX, int BlockY, int Player, COptions* options); //!< Initialize the bomber
    void Destroy(); //!< Uninitialize the bomber
    bool Update(float DeltaTime); //!< Update the bomber. Return whether the element should be deleted by the arena.
    void Display(); //!< Draw the bomber sprite in the right layer.
    void OnWriteSnapshot(CArenaSnapshot& Snapshot);
    void OnReadSnapshot(CArenaSnapshot& Snapshot);
    void Command(EBomberMove BomberMove, EBomberAction BomberAction); //!< Give a move and action order to the bomber for next update.
    void Burn(); //!< Make the bomber react when he is burnt by an explosion
    void Crush(); //!< Make the bomber react when he is crushed by a wall
    void Stunt(); //!< Make the bomber react when a bomb bounces on his head
    void ItemEffect(EItemType Type); //!< Apply the effects of the specified item type on the bomber.
    void TryKickBomb(int BlockX, int BlockY, EBombKick BombKick); //!< Kick a bomb if there is one on the specified block.
    int GetFlameSize(); //!< Get the current flame size of the bombs dropped by the bomber. Depends on its current sickness.
    int GetPixelsPerSecond(); //!< Return the speed (in pixels per second) the bomber can walk.
    bool IsObstacle(int BlockX, int BlockY); //!< Return whether there is a wall or a bomb on the specified block.
    inline bool IsAlive(); //!< Return whether the bomber is alive (not dead and not dying)
    inline bool IsDying(); //!< Return whether the bomber is dying
    inline bool IsDead(); //!< Return whether the bomber is dead (but not dying)
    inline bool IsActor(); //!< Return whether the bomber can currently do something (i.e. execute commands).
    inline int GetBlockX(); //!< Return the block position X of the bomber
    inline int GetBlockY(); //!< Return the block position Y of the bomber
    inline int GetPlayer(); //!< Return the number of the player who controls this bomber
    inline ESick GetSickness(); //!< Return the bomber's sickness value
    inline bool CanKickBombs(); //!< Return whether the bomber is able to kick bombs
    inline bool CanThrowBombs(); //!< Return whether the bomber is able to throw bombs
    inline bool CanPunchBombs(); //!< Return whether the bomber is able to punch bombs
    inline bool CanRemoteFuseBombs(); //!< Return whether the bomber is able to remote fuse bombs
    inline bool HasShield(); //!< Return whether the bomber has shield
    inline float TimeSinceLastSick(); //!< Return time after last sick event
    inline int GetUsedBombsCount(); //!< Return how many bombs the bomber are currently ticking in the arena
    inline int GetBombItemsCount(); //!< Return how many bomb items the bomber has picked up
    inline int GetFlameItemsCount(); //!< Return how many flame items the bomber has picked up
    inline int GetRollerItemsCount(); //!< Return how many roller items the bomber has picked up
    inline int GetTotalBombs(); //!< Return how many bombs the bomber can currently drop
    inline EBomberState GetState(); //!< Return the state of the bomber
    inline int GetBombIndex(); //!< Return the index of the bomb the bomber is possibly holding, lifting, or punching (if the bomber is throwing, this index is -1).
    inline bool HasExisted(); //!< Return the has existed status variable
    inline void ResetHasExisted(); //!< Reset the existed status variable to false
    inline EBomberType GetBomberType(); //!< Return the bomber type (@see EBomberType)

    inline void SetTeam(CTeam* pTeam); //!< Set the team
    inline CTeam* GetTeam(); //!< Return the team
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CBomber::SetArena(CArena* pArena)
{
    CElement::SetArena(pArena);
    m_BomberMove.SetArena(pArena);
}

inline bool CBomber::IsAlive() { return m_Dead == DEAD_ALIVE; }

inline bool CBomber::IsDying() { return m_Dead == DEAD_DYING; }

inline bool CBomber::IsDead() { return m_Dead == DEAD_DEAD; }

inline bool CBomber::IsActor() { return m_Dead == DEAD_ALIVE && m_BomberState != BOMBERSTATE_STUNT; }

inline int CBomber::GetBlockX() { return m_BomberMove.GetBlockX(); }

inline int CBomber::GetBlockY() { return m_BomberMove.GetBlockY(); }

inline int CBomber::GetPlayer() { return m_Player; }

inline ESick CBomber::GetSickness() { return m_Sickness; }

inline bool CBomber::CanKickBombs() { return m_NumberOfKickItems > 0; }

inline bool CBomber::CanThrowBombs() { return m_NumberOfThrowItems > 0; }

inline bool CBomber::CanPunchBombs() { return m_NumberOfPunchItems > 0; }

/**
 *  The bomber can remote fuse bombs, if all of these conditions are met:
 *  - he has at least one remote item
 *  - he doesn't have one of these sicknesses: long ticking bomb, short ticking bomb, colic
 *
 *  If he can't remote fuse bombs, the dropped bombs will tick normally and explode after
 *  its time is up.
 */

inline bool CBomber::CanRemoteFuseBombs() { return m_NumberOfRemoteItems > 0; }

inline bool CBomber::HasShield() { return m_ShieldTime > 0.0f; }

inline float CBomber::TimeSinceLastSick() { return m_TimeSinceLastSick; }

inline int CBomber::GetUsedBombsCount() { return m_UsedBombs; }

inline int CBomber::GetBombItemsCount() { return m_NumberOfBombItems; }

inline int CBomber::GetFlameItemsCount() { return m_NumberOfFlameItems; }

inline int CBomber::GetRollerItemsCount() { return m_NumberOfRollerItems; }

inline int CBomber::GetTotalBombs() { return m_NumberOfBombItems + 1; }

inline int CBomber::GetX() { return m_BomberMove.GetX(); }

inline int CBomber::GetY() { return m_BomberMove.GetY(); }

inline void CBomber::SetSickness(ESick Sickness)
{
    m_Sickness = Sickness;
    m_JustGotSick = true;
    m_TimeSinceLastSick = 0.0;

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber getting sick [id=%d, sickness=%02d].", m_Player, Sickness);
#endif
}

inline EBomberState CBomber::GetState() { return m_BomberState; }

inline int CBomber::GetBombIndex()
{
    ASSERT(m_BombIndex != -1);
    return m_BombIndex;
}

inline bool CBomber::HasExisted() { return m_HasExisted; }

inline void CBomber::ResetHasExisted() { m_HasExisted = false; }

inline EBomberType CBomber::GetBomberType() { return p_Options->GetBomberType(m_Player); }

inline void CBomber::SetTeam(CTeam* pTeam) { p_Team = pTeam; }

inline CTeam* CBomber::GetTeam() { return p_Team; }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
