// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold
    Copyright (C) 2008 Jerome Bigot
    Copyright (C) 2010 Markus Drescher

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
 *  \file CBomber.cpp
 *  \brief The bombers
 *
 *  Jouabilite :
 *  Pour dans longtemps, pour rendre les deplacements encore mieux.
 *  Si a gauche le bomber est bloque, on regarde la direction qu'il
 *  avait avant de vouloir tourner, par exemple haut, et dans ce cas
 *  qqsoit sa position (mais on verifie qd meme s'il peut aller par la)
 *  il ira en haut pour eviter le mur.
 *  Comment peut il etre vraiment bloque ? il suffit qu'il soit une
 *  direction comme droite et qu'il veuille aller a droite...
 *
 *  Bugs :
 *  - Pas bon : plus un player est proche de zero, plus il a de
 *    chances d'etre contamine (voir CBomber::Contamination())
 *  - Pendant une colique, le player peut demander sans cesse action2
 *    et ainsi ne pas poser de bombe
 *
 *  Optims :
 *  - des ToBlock(...) sont recalcules inutilement
 *  - dans CBomber::Move, deux memes CanMove sont calcules inutilement
 *  - dans CBomber::CanMove, CANMOVE_AVOID et CANMOVE_TURN, on s'en fout...
 *  - l'animation du walking, c'est cradoooooooooo
 *  - j'ai deja essaye mais ca faisait mourir le bomber facilement contre un mur
 *    Faire en sorte que TryMove, s'il modifie m_Y par exemple, update m_iY et m_BomberMove.GetBlockY(),
 *    plutot qu'on ait a updater m_iX, m_iY, m_BomberMove.GetBlockX() et m_BomberMove.GetBlockY() a la fin de CBomber::Move.
 *
 *  bug contamination : lorsqu'il y a plus de deux joueurs sur une case qui se suivent,
 *  avec un joueur malade, y a plein de contamination alors qu'il faudrait pas.
 */


#include "StdAfx.h"
#include "CBomber.h"
#include "CTeam.h"
#include "CArena.h"
#include "CBomb.h"
#include "CDisplay.h"
#include "CItem.h"
#include "CSound.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

SBomberSpriteTable CBomber::m_BomberSpriteTables[MAX_NUMBER_OF_STATES] =
{
    { BMP_ARENA_BOMBER_WALK, 12 }, // Walk
    { BMP_ARENA_BOMBER_WALK_HOLD, 12 }, // Walk and hold bomb
    { BMP_ARENA_BOMBER_DEATH, 7 }, // Death
    { BMP_ARENA_BOMBER_LIFT, 12 }, // Lift bomb
    { BMP_ARENA_BOMBER_THROW, 20 }, // Throw bomb
    { BMP_ARENA_BOMBER_PUNCH, 8 }, // Punch bomb
    { BMP_ARENA_BOMBER_STUNT, 4 }  // Stunt
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Bomber speeds (in pixels per second)
#define SPEED_SLOW      76                //!< Speed with SLOW sickness
#define SPEED_FAST      250               //!< Speed with FAST sickness
#define SPEED_NORMAL    120               //!< Normal speed
#define SPEED_INC       25                //!< Speed increase each time a roller item is picked up

//! Sick flashing animation times (in seconds)
#define ANIMSICK_TIME1      0.090f
#define ANIMSICK_TIME2      (ANIMSICK_TIME1 * 2)

//! Walk animation times (in seconds)
#define ANIMWALK_TIME1      0.150f
#define ANIMWALK_TIME2      (ANIMWALK_TIME1 * 2)
#define ANIMWALK_TIME3      (ANIMWALK_TIME1 * 3)
#define ANIMWALK_TIME4      (ANIMWALK_TIME1 * 4)

//! Dying animation times (in seconds)
#define ANIMDYING_TIME1     0.600f
#define ANIMDYING_TIME2     (ANIMDYING_TIME1 * 1.2)
#define ANIMDYING_TIME3     (ANIMDYING_TIME1 * 1.3)
#define ANIMDYING_TIME4     (ANIMDYING_TIME1 * 1.4)
#define ANIMDYING_TIME5     (ANIMDYING_TIME1 * 1.5)
#define ANIMDYING_TIME6     (ANIMDYING_TIME1 * 1.6)
#define ANIMDYING_TIME7     (ANIMDYING_TIME1 * 1.7)

//! Bomb lifting animation times (in seconds)
#define ANIMBOMBLIFTING_TIME1     0.060f
#define ANIMBOMBLIFTING_TIME2     (ANIMBOMBLIFTING_TIME1 * 2)
#define ANIMBOMBLIFTING_TIME3     (ANIMBOMBLIFTING_TIME1 * 3)

//! Bomb throwing animation times (in seconds)
#define ANIMBOMBTHROWING_TIME1     0.040f
#define ANIMBOMBTHROWING_TIME2     0.080f
#define ANIMBOMBTHROWING_TIME3     0.120f
#define ANIMBOMBTHROWING_TIME4     0.200f
#define ANIMBOMBTHROWING_TIME5     0.280f

//! Bomb punching animation times (in seconds)
#define ANIMBOMBPUNCHING_TIME1     0.080f
#define ANIMBOMBPUNCHING_TIME2     0.220f
#define ANIMBOMBPUNCHING_TIME3     0.320f

//! Animation sprites when walking (holding bomb and not holding bomb)
#define BOMBERSPRITE_DOWN0      0
#define BOMBERSPRITE_DOWN1      1
#define BOMBERSPRITE_DOWN2      2
#define BOMBERSPRITE_RIGHT0     3
#define BOMBERSPRITE_RIGHT1     4
#define BOMBERSPRITE_RIGHT2     5
#define BOMBERSPRITE_LEFT0      6
#define BOMBERSPRITE_LEFT1      7
#define BOMBERSPRITE_LEFT2      8
#define BOMBERSPRITE_UP0        9
#define BOMBERSPRITE_UP1        10
#define BOMBERSPRITE_UP2        11

//! Animation sprites when dying
#define BOMBERSPRITE_DYING0     0
#define BOMBERSPRITE_DYING1     1
#define BOMBERSPRITE_DYING2     2
#define BOMBERSPRITE_DYING3     3
#define BOMBERSPRITE_DYING4     4
#define BOMBERSPRITE_DYING5     5
#define BOMBERSPRITE_DYING6     6

//! Animation sprites when lifting a bomb
#define BOMBERSPRITE_LIFTING_DOWN_0     0
#define BOMBERSPRITE_LIFTING_DOWN_1     1
#define BOMBERSPRITE_LIFTING_DOWN_2     2
#define BOMBERSPRITE_LIFTING_RIGHT_0    3
#define BOMBERSPRITE_LIFTING_RIGHT_1    4
#define BOMBERSPRITE_LIFTING_RIGHT_2    5
#define BOMBERSPRITE_LIFTING_LEFT_0     6
#define BOMBERSPRITE_LIFTING_LEFT_1     7
#define BOMBERSPRITE_LIFTING_LEFT_2     8
#define BOMBERSPRITE_LIFTING_UP_0       9
#define BOMBERSPRITE_LIFTING_UP_1       10
#define BOMBERSPRITE_LIFTING_UP_2       11

//! Animation sprites when throwing a bomb
#define BOMBERSPRITE_THROWING_DOWN_0     0
#define BOMBERSPRITE_THROWING_DOWN_1     1
#define BOMBERSPRITE_THROWING_DOWN_2     2
#define BOMBERSPRITE_THROWING_DOWN_3     3
#define BOMBERSPRITE_THROWING_DOWN_4     4
#define BOMBERSPRITE_THROWING_RIGHT_0    5
#define BOMBERSPRITE_THROWING_RIGHT_1    6
#define BOMBERSPRITE_THROWING_RIGHT_2    7
#define BOMBERSPRITE_THROWING_RIGHT_3    8
#define BOMBERSPRITE_THROWING_RIGHT_4    9
#define BOMBERSPRITE_THROWING_LEFT_0     10
#define BOMBERSPRITE_THROWING_LEFT_1     11
#define BOMBERSPRITE_THROWING_LEFT_2     12
#define BOMBERSPRITE_THROWING_LEFT_3     13
#define BOMBERSPRITE_THROWING_LEFT_4     14
#define BOMBERSPRITE_THROWING_UP_0       15
#define BOMBERSPRITE_THROWING_UP_1       16
#define BOMBERSPRITE_THROWING_UP_2       17
#define BOMBERSPRITE_THROWING_UP_3       18
#define BOMBERSPRITE_THROWING_UP_4       19

//! Animation sprites when punching a bomb
#define BOMBERSPRITE_PUNCHING_DOWN_0     0
#define BOMBERSPRITE_PUNCHING_DOWN_1     1
#define BOMBERSPRITE_PUNCHING_RIGHT_0    2
#define BOMBERSPRITE_PUNCHING_RIGHT_1    3
#define BOMBERSPRITE_PUNCHING_LEFT_0     4
#define BOMBERSPRITE_PUNCHING_LEFT_1     5
#define BOMBERSPRITE_PUNCHING_UP_0       6
#define BOMBERSPRITE_PUNCHING_UP_1       7

//! Flamesize when the bomber has the SMALLFLAME sickness
#define FLAMESIZE_SMALLFLAME    1

//! Time left before a bomb the bomber dropped explodes
#define BOMBTIMELEFT_NORMAL         2.0f
#define BOMBTIMELEFT_SICKLONGBOMB   4.0f    // When bomber has LONGBOMB sickness
#define BOMBTIMELEFT_SICKSHORTBOMB  1.0f    // When bomber has SHORTBOMB sickness

//! Wait before rint the items the bomber picked up if bomber is dead (in seconds)
#define RETURNITEMS_WAIT        0.8f

// Offset when drawing the bomber sprite
#define BOMBER_OFFSETX  (-6)
#define BOMBER_OFFSETY  (-12)

/**
 *  \brief Manhattan distance
 *  Used for contamination to determine if a bomber can be considered to be NEAR another bomber.
 *  A bomber is near another if abs(x2-x1) + abs(y2-y1) <= CONTAMINATION_NEAR.
 *  This is the manhattan distance in pixels (http://en.wikipedia.org/wiki/Manhattan_distance).
 */
#define CONTAMINATION_NEAR      9        

 //! Max time since last sick
#define MAX_TIME_SINCE_LAST_SICK 3.0

//! Bomber sprite layer
#define BOMBER_SPRITELAYER  50

#define SICK_SPRITE_ROW_FULL        (MAX_PLAYERS + 0)       //!< The row with the full black bomber sprites (this is the number of maximum players)
#define SICK_SPRITE_ROW_SHADOW      (MAX_PLAYERS + 1)       //!< The row with the black shadow bomber sprites (one row below SICK_SPRITE_ROW_FULL)
#define SICK_SPRITE_ROW_BRIGHT      (MAX_PLAYERS + 2)       //!< The row with the black shadow bomber sprites (one row below SICK_SPRITE_ROW_FULL)

/**
 *  \brief Fuse only the first bomb found?
 *  If set to true, only the first bomb will be fused by the remote control of a bomber.
 *  If set to false, all bombs are immediately fused.
 *  First means: the first bomb found in the bomb array, not the first in time.
 */
#define REMOTE_FUSE_ONLY_FIRST_BOMB         true

#define SHIELD_TIME 15.0f

#define STRONGWEAK_TIME 20.0f

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBomber::CBomber(void) : CElement()
{
    m_HasExisted = false; // the bomber did not exist, yet.

    // Initialize pointer
    p_Options = NULL;

    m_BomberAction = BOMBERACTION_NONE;
    m_LastBomberAction = BOMBERACTION_NONE;
    m_Sprite = 0;
    m_SpriteOverlay = 0;
    m_Page = 0;
    m_Timer = 0;
    m_SickTimer = 0.0f;
    m_TotalBombs = 0;
    m_UsedBombs = 0;
    m_FlameSize = 0;
    m_Speed = 0;
    m_Sickness = SICK_NOTSICK;
    m_NumberOfBombItems = 0;
    m_NumberOfFlameItems = 0;
    m_NumberOfRollerItems = 0;
    m_NumberOfRemoteItems = 0;
    m_NumberOfKickItems = 0;
    m_NumberOfThrowItems = 0;
    m_NumberOfPunchItems = 0;
    m_isStrongWeak = false;
    m_ShieldTime = 0.0f;
    m_TimeSinceLastSick = 0.0f;
    m_ReturnedItems = false;
    m_Player = 0;
    m_Dead = DEAD_ALIVE;
    m_JustGotSick = false;
    m_LiftingTimeElapsed = 0.0f;
    m_ThrowingTimeElapsed = 0.0f;
    m_PunchingTimeElapsed = 0.0f;
    m_StuntTimeElapsed = 0.0f;
    m_BomberState = BOMBERSTATE_WALK;
    m_BombIndex = 0;
    m_MakeInvisible = false;
    m_CountBomberActionDuration = BOMBERACTION_NONE;
    m_BomberActionDuration = 0.0f;
    m_dropMassBombPossible = false;

    for (int i = 0; i < 5; i++)
        m_AnimationSprites[i] = BOMBERSPRITE_DOWN0;

    for (int p = 0; p < m_pArena->MaxBombers(); p++)
        m_Neighbours[p] = false;

    p_Team = NULL;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBomber::~CBomber(void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::Create(int BlockX, int BlockY, int Player, COptions* options)
{
    CElement::Create();

    m_HasExisted = true; // the bomber exists now (must not be reset unless the match is finished/canceled)

    p_Options = options;

    m_BomberMove.Create(BlockX, BlockY, Player);

    m_BomberAction = BOMBERACTION_NONE;
    m_LastBomberAction = BOMBERACTION_NONE;
    m_BomberState = BOMBERSTATE_WALK;

    m_Dead = DEAD_ALIVE;

    m_FlameSize = options->GetInitialBomberSkills(BOMBERSKILL_FLAME);
    m_TotalBombs = options->GetInitialBomberSkills(BOMBERSKILL_BOMBS);
    m_UsedBombs = 0;
    m_Speed = SPEED_NORMAL;

    m_Timer = 0.0f;

    m_Player = Player;

    m_Sickness = SICK_NOTSICK;

    m_SickTimer = 0.0f;

    m_ShieldTime = (float) (options->GetInitialBomberSkills(BOMBERSKILL_SHIELDITEMS) * SHIELD_TIME);

    // Initial bomber direction is down
    m_AnimationSprites[0] = BOMBERSPRITE_DOWN0;
    m_AnimationSprites[1] = BOMBERSPRITE_DOWN1;
    m_AnimationSprites[2] = BOMBERSPRITE_DOWN2;

    m_Page = m_BomberSpriteTables[m_BomberState].SpriteTableNumber;

    Animate(0.0f);

    m_NumberOfBombItems = options->GetInitialBomberSkills(BOMBERSKILL_BOMBITEMS);
    m_NumberOfFlameItems = options->GetInitialBomberSkills(BOMBERSKILL_FLAMEITEMS);
    m_NumberOfRollerItems = options->GetInitialBomberSkills(BOMBERSKILL_ROLLERITEMS);
    m_NumberOfKickItems = options->GetInitialBomberSkills(BOMBERSKILL_KICKITEMS);
    m_NumberOfThrowItems = options->GetInitialBomberSkills(BOMBERSKILL_THROWITEMS);
    m_NumberOfPunchItems = options->GetInitialBomberSkills(BOMBERSKILL_PUNCHITEMS);
    m_NumberOfRemoteItems = options->GetInitialBomberSkills(BOMBERSKILL_REMOTEITEMS);

    // increase initial speed
    m_Speed += SPEED_INC * m_NumberOfRollerItems;

    m_ReturnedItems = false;

    for (int p = 0; p < m_pArena->MaxBombers(); p++)
    {
        m_Neighbours[p] = false;
    }

    m_LiftingTimeElapsed = 0.0f;
    m_ThrowingTimeElapsed = 0.0f;
    m_PunchingTimeElapsed = 0.0f;
    m_StuntTimeElapsed = 0.0f;

    m_BombIndex = -1;

    m_CountBomberActionDuration = BOMBERACTION_NONE;
    m_BomberActionDuration = 0.0f;
    m_dropMassBombPossible = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::Destroy(void)
{
    m_BomberMove.Destroy();

    CElement::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::Die(void)
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber dying [id=%d, x=%02d, y=%02d].", m_Player, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY());
#endif

    if (theDebug.CanBombersDie())
    {
        // If the bomber has not won the current match
        if (!p_Team->IsVictorious())
        {
            // Make the bomber start dying : set the dead state
            // and reset the timer.
            if (m_Dead == DEAD_ALIVE)
            {
                // If the bomber was doing something with a bomb
                if (m_BombIndex != -1)
                {
                    // If the bomber is just lifting this bomb
                    if (m_BomberState == BOMBERSTATE_LIFT) {
                        // End the lifting and make the bomber hold the bomb
                        m_pArena->GetBomb(m_BombIndex).SetBeingHeld();
                        m_BomberState = BOMBERSTATE_WALK_HOLD;
                    }

                    // If the bomber died while holding or throwing a bomb
                    if (m_BomberState == BOMBERSTATE_WALK_HOLD ||
                        m_BomberState == BOMBERSTATE_THROW)
                    {
                        // Make him throw the bomb (with no bomber throwing animation)
                        MakeBombFly(BOMBFLIGHTTYPE_THROW);
                    }
                    // If the bomber died while punching a bomb
                    else if (m_BomberState == BOMBERSTATE_PUNCH)
                    {
                        // Make him punch the bomb now (with no bomber punching animation).
                        // Yes, that's strange, since he is dying now. But we have to release the bomb
                        // otherwise it won't explode (bug tracker #2160381). Besides, this can only happen
                        // during the first animation sequence (ANIMBOMBPUNCHING_TIME1) so the human
                        // players should not notice since it's just 0.08 seconds currently.
                        MakeBombFly(BOMBFLIGHTTYPE_PUNCH);
                    }
                    //! @todo check if another else assert(0) makes sense since we still have a bomb (m_BombIndex != -1)

                }

                m_Dead = DEAD_DYING;
                m_BomberState = BOMBERSTATE_DEATH;
                m_Timer = 0.0f;
                m_ShieldTime = 0.0f;

                // Play the bomber death sound
                m_pSound->PlaySample(SAMPLE_BOMBER_DEATH);

            }
        }

        // Call animate whenever bomber changes state to update m_Page and m_Sprite correctly
        Animate(0.0f);

    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called by explosions being touched by this bomber

void CBomber::Burn()
{

#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber burning [id=%d, x=%02d, y=%02d].", m_Player, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY());
#endif

    // The bomber cannot die by flames if he/she has the flameproof contamination or shield or is strong
    if (m_Sickness != SICK_FLAMEPROOF && m_ShieldTime <= 0.0f) {
        Die();
    }
    else
    {
        // Bomb destroys shield time
        if (m_ShieldTime > 0.75f)
            m_ShieldTime = 0.75f;
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called by walls being touched by this bomber

void CBomber::Crush()
{
#ifdef _DEBUG
    debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber crushing [id=%d, x=%02d, y=%02d].", m_Player, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY());
#endif

    Die();

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * Give orders to the bomber.
 * The bomber stores these orders as is if he's not sick. However, a
 * sickness makes him not understand the orders correctly, so they are
 * modified then stored.
 */

void CBomber::Command(EBomberMove BomberMove, EBomberAction BomberAction)
{
    // The given BomberMove and BomberAction will be modified 
    // according to the bomber's sickness. When updating the bomber, 
    // these modified BomberMove and BomberAction will be used.

    m_BomberMove.Command(BomberMove);

    switch (m_Sickness)
    {
        // Sicknesses that don't affect the action
    case SICK_NOTSICK:
    case SICK_SLOW:
    case SICK_FAST:
    case SICK_SMALLFLAME:
    case SICK_LONGBOMB:
    case SICK_SHORTBOMB:
    case SICK_INVERTION:
    case SICK_INERTIA:
    case SICK_INVISIBILITY:
    {
        m_BomberAction = BomberAction;
        break;
    }

    case SICK_FLAMEPROOF:
    case SICK_CONSTIPATED:
    {
        if (m_BomberAction == BOMBERACTION_ACTION1)
            m_BomberAction = BOMBERACTION_NONE;

        break;
    }

    case SICK_COLIC:
    {
        if (m_BomberAction == BOMBERACTION_NONE)
            m_BomberAction = BOMBERACTION_ACTION1;

        break;
    }
    default:
        break;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * Make the bomber not sick anymore and reset
 * the sick timer.
 *
 * \sa m_Sickness, m_SickTimer
 */

void CBomber::Heal()
{
    if (m_Sickness != SICK_NOTSICK)
    {
        m_Sickness = SICK_NOTSICK;
        m_SickTimer = 0.0f;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Makes the bomber perform an action if he has to.

void CBomber::Action()
{
    // If the bomber is alive (not dead and not dying)
    if (m_Dead == DEAD_ALIVE)
    {
        // If the bomber is holding a bomb
        if (m_BomberState == BOMBERSTATE_WALK_HOLD)
        {
            // Verify the bomber can throw bombs since he holds one
            assert(CanThrowBombs());

            // In order to pick up its bomb, the user had to press action1.
            // If the user releases action1, then the bomber must throw the bomb.

            // If action1 is not pressed
            if (m_BomberAction != BOMBERACTION_ACTION1)
            {
#ifdef _DEBUG
                debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber throwing bomb [bomber=%d, bomb=%02d].", m_Player, m_BombIndex);
#endif

                // Play the sound the bomber does when he throws a bomb
                m_pSound->PlaySample(SAMPLE_BOMBER_THROW);

                // Switch to the state where the bomber will throw the bomb
                m_BomberState = BOMBERSTATE_THROW;
                m_ThrowingTimeElapsed = 0.0f;
            }
        }
        // If the bomber is just standing/walking without holding a bomb
        else if (m_BomberState == BOMBERSTATE_WALK)
        {
            // If he wants to drop a bomb or hold a bomb
            if (m_BomberAction == BOMBERACTION_ACTION1)
            {
                // If the bomber can throw bombs
                // and the player didn't press action1 last time
                // and the bomber is on a block with a bomb
                if (CanThrowBombs() &&
                    m_LastBomberAction != BOMBERACTION_ACTION1 &&
                    m_pArena->IsBomb(m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY()))
                {
#ifdef _DEBUG
                    debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber lifting bomb [bomber=%d, x=%02d, y=%02d].", m_Player, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY());
#endif

                    // Switch to the state where the bomber will lift the bomb
                    m_BomberState = BOMBERSTATE_LIFT;
                    m_LiftingTimeElapsed = 0.0f;

                    // Find the bomb that is here
                    for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
                    {
                        // Test existence and position
                        if (m_pArena->GetBomb(Index).Exist() &&
                            m_pArena->GetBomb(Index).GetBlockX() == m_BomberMove.GetBlockX() &&
                            m_pArena->GetBomb(Index).GetBlockY() == m_BomberMove.GetBlockY() &&
                            m_pArena->GetBomb(Index).IsOnFloor())
                        {
#ifdef _DEBUG
                            debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber lifting bomb [bomber=%d, bomb=%02d, x=%02d, y=%02d].", m_Player, Index, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY());
#endif

                            // Save the bomb index
                            m_BombIndex = Index;

                            // Tell the bomb it is being lifted
                            m_pArena->GetBomb(Index).SetBeingLifted();
                        }
                    }
                }
                // If the bomber cannot throw bombs
                // or the player keeps pressing action1 
                // or there is no bomb to hold
                else
                {
                    // Then try to drop a bomb

                    // If he is able to drop a bomb
                    if (m_UsedBombs < m_TotalBombs)
                    {
                        bool dropMassBombNow = false && // <-- false prevents the bomb-mass feature NOW...   m_hasMassBombFeature == true &&
                            (m_BomberActionDuration > 1.0f) && m_dropMassBombPossible;

                        // If no wall and no bomb and no explosion at his position
                        if ((!IsObstacle(m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY()) &&
                            !m_pArena->IsExplosion(m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY()))
                            || (m_dropMassBombPossible && dropMassBombNow))
                        {
                            // So we can limit the number of dropped bombs
                            int droppedBombsNow = 0;

                            // @todo The maximum number of bombs should depend on the bomber's skills (extra item 'mass bomb drop').
                            // @todo Maybe more bombs are only dropped if the player holds the action button for a longer time.
                            // In mass-bomb mode this is the number of additional bombs
                            int maxBombs = (dropMassBombNow ? 4 : 1);

                            // The mass-bomb drop is only possible if we pressed the button less than a second
                            // and the bomber didn't move
                            m_dropMassBombPossible = (m_BomberActionDuration < 1.0f) && (m_BomberMove.GetMove() == BOMBERMOVE_NONE);

                            // Determine the bomber's last move, that is, in which direction he
                            // is looking. The bombs are dropped in front of him.
                            int deltax, deltay;
                            switch (m_BomberMove.GetLastRealMove())
                            {
                            case BOMBERMOVE_UP:    deltax = 0; deltay = -1; break;
                            case BOMBERMOVE_DOWN:  deltax = 0; deltay = 1; break;
                            case BOMBERMOVE_LEFT:  deltax = -1; deltay = 0; break;
                            case BOMBERMOVE_RIGHT: deltax = 1; deltay = 0; break;
                            default:               deltax = 0; deltay = 0; assert(false); break;  // Prevents "variable not initialized" warning -- should never happen
                            }

                            // Start with the bomber's current position
                            int x, y;
                            x = m_BomberMove.GetBlockX();
                            y = m_BomberMove.GetBlockY();

                            if (dropMassBombNow)
                            {
                                // Move to the next position, since we already placed a bomb just a second ago
                                // at the bomber's position
                                x += deltax;
                                y += deltay;

                                // The mass-bomb drop isn't possible from now on
                                m_dropMassBombPossible = false;
                            }

                            while (true) {

                                // Create the bomb (unless it is possible)
                                if (m_pArena->BombsInUse() >= m_pArena->MaxBombs()) break;

#ifdef _DEBUG
                                debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber dropping bomb [bomber=%d, x=%02d, y=%02d, used=%02d, total=%02d].", m_Player, x, y, m_UsedBombs, m_TotalBombs);
#endif

                                m_pArena->NewBomb(x, y, GetFlameSize(), GetBombTime(), m_Player);

                                // One more used and dropped bomb
                                m_UsedBombs++;
                                droppedBombsNow++;

                                // Move to the next position
                                x += deltax;
                                y += deltay;

                                // Check if we should end the mass bomb drop
                                // We can at least drop one bomb, so the checks are at the end here
                                if (m_UsedBombs >= m_TotalBombs) break;
                                if (droppedBombsNow >= maxBombs) break;
                                if (IsObstacle(x, y)) break;
                                if (m_pArena->IsExplosion(x, y)) break;

                            }

                            // Play the drop sound
                            m_pSound->PlaySample(SAMPLE_BOMB_DROP);
                        }
                    }
                }
            }
            // If the player either wants to stop a bomb he kicked, or to punch a bomb in front of him 
            else if (m_LastBomberAction != BOMBERACTION_ACTION2 && m_BomberAction == BOMBERACTION_ACTION2)
            {
                // If the bomber can kick bombs
                if (CanKickBombs())
                {
                    // Find the bombs that are still moving and that 
                    // were just kicked by this player.
                    for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
                    {
                        // Test existence and kicker player number
                        if (m_pArena->GetBomb(Index).Exist() &&
                            m_pArena->GetBomb(Index).GetKickerPlayer() == m_Player)
                        {
                            // Tell the bomb this bomber kicked to
                            // stop moving as soon as possible
                            m_pArena->GetBomb(Index).StopMoving();
                        }
                    }
                }

                bool bomberHasPunchedBomb = false;

                // If the bomber can punch bombs
                if (CanPunchBombs())
                {
                    // Coordinates of the block in front of the bomber
                    int FrontBlockX = -1;
                    int FrontBlockY = -1;

                    // Determine the coordinates of the block that is in front of the bomber
                    switch (m_BomberMove.GetLastRealMove())
                    {
                    case BOMBERMOVE_UP:
                        FrontBlockX = m_BomberMove.GetBlockX();
                        FrontBlockY = m_BomberMove.GetBlockY() - 1;
                        break;

                    case BOMBERMOVE_DOWN:
                        FrontBlockX = m_BomberMove.GetBlockX();
                        FrontBlockY = m_BomberMove.GetBlockY() + 1;
                        break;

                    case BOMBERMOVE_LEFT:
                        FrontBlockX = m_BomberMove.GetBlockX() - 1;
                        FrontBlockY = m_BomberMove.GetBlockY();
                        break;

                    case BOMBERMOVE_RIGHT:
                        FrontBlockX = m_BomberMove.GetBlockX() + 1;
                        FrontBlockY = m_BomberMove.GetBlockY();
                        break;
                    default:
                        break;
                    }

                    // Check if the EBomberMove was correct
                    ASSERT(FrontBlockX != -1);
                    ASSERT(FrontBlockY != -1);

                    // If there is a bomb in front of the bomber
                    if (m_pArena->IsBomb(FrontBlockX, FrontBlockY))
                    {
                        // Find the bomb that is in front of the bomber
                        for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
                        {
                            // Test existence and position
                            if (m_pArena->GetBomb(Index).Exist() &&
                                m_pArena->GetBomb(Index).GetBlockX() == FrontBlockX &&
                                m_pArena->GetBomb(Index).GetBlockY() == FrontBlockY &&
                                !m_pArena->GetBomb(Index).IsBeingPunched())
                            {
#ifdef _DEBUG
                                debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber punching bomb [bomber=%d, x=%02d, y=%02d, bomb=%02d].", m_Player, m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY(), Index);
#endif

                                // Tell the bomb it is being punched
                                m_pArena->GetBomb(Index).SetBeingPunched();

                                // Save the bomb index for later use (during the punching bomber animation)
                                m_BombIndex = Index;

                                // Play the punch sound
                                m_pSound->PlaySample(SAMPLE_BOMBER_PUNCH);

                                // Switch to the state where the bomber will punch the bomb
                                m_BomberState = BOMBERSTATE_PUNCH;
                                m_PunchingTimeElapsed = 0.0f;

                                // Set we have punched a bomb right now (used for remote fusing bombs, see below)
                                bomberHasPunchedBomb = true;

                                break;
                            }
                        }
                    }
                }

                // If the bomber can remote fuse bombs and we didn't punch a bomb (see above)
                if (CanRemoteFuseBombs() && !bomberHasPunchedBomb)
                {
                    float timeMax = 0.0f;   // time elapsed since bomb was created
                    int   bombTimeMax = -1; // index with the bomb which exists the longest time ago

                    // Find the first bomb, i.e. the one which was planted first, to fuse.
                    for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
                    {
                        CBomb& myBomb = m_pArena->GetBomb(Index); // help variable

                        // Test existence and kicker player number
                        if (myBomb.Exist() && myBomb.IsRemote() &&
                            myBomb.GetOwnerPlayer() == m_Player &&
                            myBomb.GetElapsedTime() > timeMax)
                        {
                            if (REMOTE_FUSE_ONLY_FIRST_BOMB) {
                                // look for the bomb which exists the longest time ago
                                timeMax = myBomb.GetElapsedTime();
                                bombTimeMax = Index;
                            }
                            else {
#ifdef _DEBUG
                                debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber fusing bomb [bomber=%d, bomb=%02d].", m_Player, Index);
#endif

                                myBomb.Burn();
                            }
                        }
                    }

                    if (REMOTE_FUSE_ONLY_FIRST_BOMB && bombTimeMax > -1) {
                        debugLog.WriteDebugMsg(DEBUGSECT_BOMBER, "Bomber fusing first bomb [bomber=%d, bomb=%02d].", m_Player, bombTimeMax);
                        // found it, detonate.
                        m_pArena->GetBomb(bombTimeMax).Burn();
                    }
                }

            }
        }

        // Remember bomber action
        m_LastBomberAction = m_BomberAction;

        // Call animate whenever bomber changes state to update m_Page and m_Sprite correctly
        Animate(0.0f);

    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * Get the current flame size of the bomber. It depends on sicknesses.
 * If the bomber has the SMALLFLAME sickness, the bomb will have a limited
 * falme size.
 *
 * \sa m_Sickness, m_FlameSize
 */

int CBomber::GetFlameSize(void)
{
    return (m_Sickness != SICK_SMALLFLAME ? m_FlameSize : FLAMESIZE_SMALLFLAME);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * Get the time a bomb will tick before it will explode. The time depends on
 * the bomber's sickness.
 * If the bomber has the LONGBOMB or SHORTBOMB sickness, the time is adjusted.
 *
 * \sa m_Sickness
 */

float CBomber::GetBombTime(void)
{
    switch (m_Sickness)
    {
    case SICK_LONGBOMB:  return BOMBTIMELEFT_SICKLONGBOMB;
    case SICK_SHORTBOMB: return BOMBTIMELEFT_SICKSHORTBOMB;
    default:             return BOMBTIMELEFT_NORMAL;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

int CBomber::GetPixelsPerSecond(void)
{
    switch (m_Sickness)
    {
    case SICK_SLOW: return SPEED_SLOW;
    case SICK_FAST: return SPEED_FAST;
    default:        return m_Speed;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Update the number of used bombs : if some of his
// bombs are dead, the bomber can drop more bombs.

void CBomber::UsedBombs()
{
    // Decrease the number of used bombs if some are dead
    // Scan the bombs
    for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
    {
        // Test existence, owner player and dead state
        if (m_pArena->GetBomb(Index).Exist() &&
            m_pArena->GetBomb(Index).GetOwnerPlayer() == m_Player &&
            m_pArena->GetBomb(Index).IsDead())
        {
            // The bomb is owned by this bomber and it's dead
            // Check the bomb : the arena will now be able to delete it
            m_pArena->GetBomb(Index).SetChecked();
            // Update the number of used bombs
            m_UsedBombs--;
        }
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Updates the sprite to display. This function prepares the
 *  sprite table, updates stunt times,
 *  handles the punch/throw/kick animations and updates the bomber state.
 *
 *  \sa m_BomberState, MakeBombFly(), Display()
 */

void CBomber::Animate(float DeltaTime)
{
    // Bomber state to set before exiting this method
    EBomberState NewBomberState = m_BomberState;

    // By default, the bomber can be seen in the arena
    m_MakeInvisible = false;
    
    m_SpriteOverlay = 0;

    m_Sprite = 0;

    // If the bomber is alive (not dead and not dying)
    if (m_Dead == DEAD_ALIVE)
    {
        if (m_BomberState == BOMBERSTATE_STUNT)
        {
            m_StuntTimeElapsed += DeltaTime;

            // The sprite depends on the direction where the 
            // bomber was going the last time he really moved.
            switch (m_BomberMove.GetLastRealMove())
            {
            case BOMBERMOVE_DOWN: m_Sprite = 0; break;
            case BOMBERMOVE_RIGHT: m_Sprite = 1; break;
            case BOMBERMOVE_LEFT: m_Sprite = 2; break;
            case BOMBERMOVE_UP: m_Sprite = 3; break;
            default: break;
            }

            if (m_StuntTimeElapsed >= 1.0f)
            {
                NewBomberState = BOMBERSTATE_WALK;
                m_StuntTimeElapsed = 0.0f;
            }
        }
        // If the bomber is not currently lifting or throwing a bomb
        else if (m_BomberState == BOMBERSTATE_WALK ||
            m_BomberState == BOMBERSTATE_WALK_HOLD)
        {
            // If the bomber has to move (no sickness) or he has the inertia 
            // sickness and he could move, then play the walk animation
            if (m_BomberMove.GetMove() != BOMBERMOVE_NONE &&
                (m_Sickness != SICK_INERTIA || m_BomberMove.CouldMove()))
            {
                // The animation sprites depend on the direction 
                // where the bomber is going. Compute them.
                switch (m_BomberMove.GetMove())
                {
                case BOMBERMOVE_DOWN:
                    m_AnimationSprites[0] = BOMBERSPRITE_DOWN0;
                    m_AnimationSprites[1] = BOMBERSPRITE_DOWN1;
                    m_AnimationSprites[2] = BOMBERSPRITE_DOWN2;
                    break;

                case BOMBERMOVE_RIGHT:
                    m_AnimationSprites[0] = BOMBERSPRITE_RIGHT0;
                    m_AnimationSprites[1] = BOMBERSPRITE_RIGHT1;
                    m_AnimationSprites[2] = BOMBERSPRITE_RIGHT2;
                    break;

                case BOMBERMOVE_LEFT:
                    m_AnimationSprites[0] = BOMBERSPRITE_LEFT0;
                    m_AnimationSprites[1] = BOMBERSPRITE_LEFT1;
                    m_AnimationSprites[2] = BOMBERSPRITE_LEFT2;
                    break;

                case BOMBERMOVE_UP:
                    m_AnimationSprites[0] = BOMBERSPRITE_UP0;
                    m_AnimationSprites[1] = BOMBERSPRITE_UP1;
                    m_AnimationSprites[2] = BOMBERSPRITE_UP2;
                    break;
                default:
                    break;
                }

                // Play animation
                m_Timer += DeltaTime;

                // Animate
                if (m_Timer < ANIMWALK_TIME1)  m_Sprite = m_AnimationSprites[1];
                else if (m_Timer < ANIMWALK_TIME2)  m_Sprite = m_AnimationSprites[0];
                else if (m_Timer < ANIMWALK_TIME3)  m_Sprite = m_AnimationSprites[1];
                else if (m_Timer < ANIMWALK_TIME4)  m_Sprite = m_AnimationSprites[2];
                else
                {
                    // Loop animation
                    m_Sprite = m_AnimationSprites[1];
                    m_Timer = 0.0f;
                }
            }
            // Just standing, set the sprite 
            else
            {
                m_Timer = 0.0f;

                // The sprite depends on the direction where the 
                // bomber was going the last time he really moved.
                switch (m_BomberMove.GetLastRealMove())
                {
                case BOMBERMOVE_DOWN: m_Sprite = BOMBERSPRITE_DOWN1;   break;
                case BOMBERMOVE_RIGHT: m_Sprite = BOMBERSPRITE_RIGHT1;  break;
                case BOMBERMOVE_LEFT: m_Sprite = BOMBERSPRITE_LEFT1;   break;
                case BOMBERMOVE_UP: m_Sprite = BOMBERSPRITE_UP1;     break;
                default: break;
                }
            }
        }
        // If the bomber is currently lifting a bomb
        else if (m_BomberState == BOMBERSTATE_LIFT)
        {
            // The sprites showing a bomber lifting a bomb depend
            // on the direction where the bomber was going the last 
            // time he really moved.
            switch (m_BomberMove.GetLastRealMove())
            {
            case BOMBERMOVE_DOWN:
                m_AnimationSprites[0] = BOMBERSPRITE_LIFTING_DOWN_0;
                m_AnimationSprites[1] = BOMBERSPRITE_LIFTING_DOWN_1;
                m_AnimationSprites[2] = BOMBERSPRITE_LIFTING_DOWN_2;
                break;

            case BOMBERMOVE_RIGHT:
                m_AnimationSprites[0] = BOMBERSPRITE_LIFTING_RIGHT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_LIFTING_RIGHT_1;
                m_AnimationSprites[2] = BOMBERSPRITE_LIFTING_RIGHT_2;
                break;

            case BOMBERMOVE_LEFT:
                m_AnimationSprites[0] = BOMBERSPRITE_LIFTING_LEFT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_LIFTING_LEFT_1;
                m_AnimationSprites[2] = BOMBERSPRITE_LIFTING_LEFT_2;
                break;

            case BOMBERMOVE_UP:
                m_AnimationSprites[0] = BOMBERSPRITE_LIFTING_UP_0;
                m_AnimationSprites[1] = BOMBERSPRITE_LIFTING_UP_1;
                m_AnimationSprites[2] = BOMBERSPRITE_LIFTING_UP_2;
                break;
            default:
                break;
            }

            // Increase bomb lifting time
            m_LiftingTimeElapsed += DeltaTime;

            if (m_BombIndex != -1)
            {

                // Animate the bomber who lifts a bomb
                if (m_LiftingTimeElapsed < ANIMBOMBLIFTING_TIME1) m_Sprite = m_AnimationSprites[0];
                else if (m_LiftingTimeElapsed < ANIMBOMBLIFTING_TIME2) m_Sprite = m_AnimationSprites[1];
                else if (m_LiftingTimeElapsed < ANIMBOMBLIFTING_TIME3) m_Sprite = m_AnimationSprites[2];
                else
                {
                    // Finish bomb lifting animation
                    m_Sprite = m_AnimationSprites[2];
                    m_LiftingTimeElapsed = 0.0f;

                    if (m_pArena->GetBomb(m_BombIndex).IsBeingLifted())
                    { 
                        // The bomber now holds the bomb
                        m_pArena->GetBomb(m_BombIndex).SetBeingHeld();
                        NewBomberState = BOMBERSTATE_WALK_HOLD;
                    }
                    else
                        NewBomberState = BOMBERSTATE_WALK;

                }

            }
        }
        // If the bomber is currently throwing a bomb
        else if (m_BomberState == BOMBERSTATE_THROW)
        {
            // The sprites showing a bomber lifting a bomb depend
            // on the direction where the bomber was going the last 
            // time he really moved.
            switch (m_BomberMove.GetLastRealMove())
            {
            case BOMBERMOVE_DOWN:
                m_AnimationSprites[0] = BOMBERSPRITE_THROWING_DOWN_0;
                m_AnimationSprites[1] = BOMBERSPRITE_THROWING_DOWN_1;
                m_AnimationSprites[2] = BOMBERSPRITE_THROWING_DOWN_2;
                m_AnimationSprites[3] = BOMBERSPRITE_THROWING_DOWN_3;
                m_AnimationSprites[4] = BOMBERSPRITE_THROWING_DOWN_4;
                break;

            case BOMBERMOVE_RIGHT:
                m_AnimationSprites[0] = BOMBERSPRITE_THROWING_RIGHT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_THROWING_RIGHT_1;
                m_AnimationSprites[2] = BOMBERSPRITE_THROWING_RIGHT_2;
                m_AnimationSprites[3] = BOMBERSPRITE_THROWING_RIGHT_3;
                m_AnimationSprites[4] = BOMBERSPRITE_THROWING_RIGHT_4;
                break;

            case BOMBERMOVE_LEFT:
                m_AnimationSprites[0] = BOMBERSPRITE_THROWING_LEFT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_THROWING_LEFT_1;
                m_AnimationSprites[2] = BOMBERSPRITE_THROWING_LEFT_2;
                m_AnimationSprites[3] = BOMBERSPRITE_THROWING_LEFT_3;
                m_AnimationSprites[4] = BOMBERSPRITE_THROWING_LEFT_4;
                break;

            case BOMBERMOVE_UP:
                m_AnimationSprites[0] = BOMBERSPRITE_THROWING_UP_0;
                m_AnimationSprites[1] = BOMBERSPRITE_THROWING_UP_1;
                m_AnimationSprites[2] = BOMBERSPRITE_THROWING_UP_2;
                m_AnimationSprites[3] = BOMBERSPRITE_THROWING_UP_3;
                m_AnimationSprites[4] = BOMBERSPRITE_THROWING_UP_4;
                break;
            default:
                break;
            }

            // Increase bomb throwing time
            m_ThrowingTimeElapsed += DeltaTime;

            // Animate the bomber who is throwing a bomb
            if (m_ThrowingTimeElapsed < ANIMBOMBTHROWING_TIME1) m_Sprite = m_AnimationSprites[0];
            else if (m_ThrowingTimeElapsed < ANIMBOMBTHROWING_TIME2) m_Sprite = m_AnimationSprites[1];
            else if (m_ThrowingTimeElapsed < ANIMBOMBTHROWING_TIME3)
            {
                // This is the animation frame where the bomber 
                // visually throws the bomb, so let's really throw 
                // the bomb now.

                m_Sprite = m_AnimationSprites[2];

                // This code block is likely to be called more than once,
                // so we ensure it will be called only once by testing if
                // the bomb has not been thrown yet.
                if (m_BombIndex != -1)
                {
                    MakeBombFly(BOMBFLIGHTTYPE_THROW);
                }
            }
            else if (m_ThrowingTimeElapsed < ANIMBOMBTHROWING_TIME4) m_Sprite = m_AnimationSprites[3];
            else if (m_ThrowingTimeElapsed < ANIMBOMBTHROWING_TIME5) m_Sprite = m_AnimationSprites[4];
            else
            {
                // Finish bomb throwing animation
                m_Sprite = m_AnimationSprites[4];
                m_ThrowingTimeElapsed = 0.0f;

                // The bomber now doesn't have any bomb in his hands, back to normal state.
                NewBomberState = BOMBERSTATE_WALK;
            }
        }
        // If the bomber is currently punching a bomb
        else if (m_BomberState == BOMBERSTATE_PUNCH)
        {
            // The sprites showing a bomber punching a bomb depend
            // on the direction where the bomber was going the last 
            // time he really moved.
            switch (m_BomberMove.GetLastRealMove())
            {
            case BOMBERMOVE_DOWN:
                m_AnimationSprites[0] = BOMBERSPRITE_PUNCHING_DOWN_0;
                m_AnimationSprites[1] = BOMBERSPRITE_PUNCHING_DOWN_1;
                break;

            case BOMBERMOVE_RIGHT:
                m_AnimationSprites[0] = BOMBERSPRITE_PUNCHING_RIGHT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_PUNCHING_RIGHT_1;
                break;

            case BOMBERMOVE_LEFT:
                m_AnimationSprites[0] = BOMBERSPRITE_PUNCHING_LEFT_0;
                m_AnimationSprites[1] = BOMBERSPRITE_PUNCHING_LEFT_1;
                break;

            case BOMBERMOVE_UP:
                m_AnimationSprites[0] = BOMBERSPRITE_PUNCHING_UP_0;
                m_AnimationSprites[1] = BOMBERSPRITE_PUNCHING_UP_1;
                break;
            default:
                break;
            }

            // Increase bomb punching time
            m_PunchingTimeElapsed += DeltaTime;

            // Animate the bomber who is punching a bomb
            if (m_PunchingTimeElapsed < ANIMBOMBPUNCHING_TIME1) m_Sprite = m_AnimationSprites[0];
            else if (m_PunchingTimeElapsed < ANIMBOMBPUNCHING_TIME2)
            {
                // This is the animation frame where the bomber visually
                // punches the bomb, so let's really punch the bomb now

                m_Sprite = m_AnimationSprites[1];

                // This code block is likely to be called more than once,
                // so we ensure it will be called only once by testing if
                // the bomb has not been punched yet.
                if (m_BombIndex != -1)
                {
                    MakeBombFly(BOMBFLIGHTTYPE_PUNCH);
                }
            }
            else if (m_PunchingTimeElapsed < ANIMBOMBPUNCHING_TIME3) m_Sprite = m_AnimationSprites[0];
            else
            {
                // Finish bomb punching animation
                m_Sprite = m_AnimationSprites[0];
                m_PunchingTimeElapsed = 0.0f;

                // Switch the bomber back to its normal state.
                NewBomberState = BOMBERSTATE_WALK;
            }
        }
    }
    // If the bomber is dying (between dead and alive)
    // Then play the dying-bomber animation
    else if (m_Dead == DEAD_DYING)
    {
        if (m_Timer < ANIMDYING_TIME1) m_Sprite = BOMBERSPRITE_DYING0;
        else if (m_Timer < ANIMDYING_TIME2) m_Sprite = BOMBERSPRITE_DYING1;
        else if (m_Timer < ANIMDYING_TIME3) m_Sprite = BOMBERSPRITE_DYING2;
        else if (m_Timer < ANIMDYING_TIME4) m_Sprite = BOMBERSPRITE_DYING3;
        else if (m_Timer < ANIMDYING_TIME5) m_Sprite = BOMBERSPRITE_DYING4;
        else if (m_Timer < ANIMDYING_TIME6) m_Sprite = BOMBERSPRITE_DYING5;
        else if (m_Timer < ANIMDYING_TIME7) m_Sprite = BOMBERSPRITE_DYING6;
        else
        {
            // Finished animation, bomber is dead
            m_Dead = DEAD_DEAD;
            // Wait before returning the items the bomber has picked up
            m_Timer = RETURNITEMS_WAIT;
        }

        // Play animation
        m_Timer += DeltaTime;
    }

    // Set the sprite table to use for the current state of the bomber
    m_Page = m_BomberSpriteTables[m_BomberState].SpriteTableNumber;

    // If bomber is not sick or he is dying
    if (m_Sickness == SICK_NOTSICK || m_Dead != DEAD_ALIVE)
    {

        if (m_ShieldTime > 0.0f)
        {
            if (m_ShieldTime > 2.0f)
                m_SpriteOverlay = m_Sprite + SICK_SPRITE_ROW_BRIGHT * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            else
            {
                // Flash
                if (int(m_ShieldTime * 10) % 2 == 0)
                    m_SpriteOverlay = m_Sprite + SICK_SPRITE_ROW_BRIGHT * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            }
        }

        // Give him its player color
        m_Sprite += m_Player * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;

    }
    // If he has shield, is sick and alive
    else
    {
        // Make the bomber flash between sick sprite and normal colored sprite

        if (m_SickTimer < ANIMSICK_TIME1)
        {
            // Sick color
            if (m_Sickness == SICK_INVISIBILITY)
            {
                // The arena-bomber-alive sprites have an additional series for the invisible bomber (only the bomber's border can be seen)
                m_Sprite += SICK_SPRITE_ROW_SHADOW * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            }
            else 
            {
                if (m_Sickness == SICK_FLAMEPROOF)
                {
                    m_SpriteOverlay = m_Sprite + SICK_SPRITE_ROW_BRIGHT * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
                    // Give him its player color
                    m_Sprite += m_Player * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
                }
                else
                    m_Sprite += SICK_SPRITE_ROW_FULL * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            }

        }
        else if (m_SickTimer < ANIMSICK_TIME2)
        {
            // Player color
            m_Sprite += m_Player * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;

            // If it is the invisibility contamination, make the bomber invisible during this time
            if (m_Sickness == SICK_INVISIBILITY) {
                m_MakeInvisible = true;
            }
        }
        else
        {
            // Reset timer
            m_SickTimer = 0.0f;
            // Sick color
            if (m_Sickness == SICK_INVISIBILITY) {
                // The arena-bomber-alive sprites have an additional series for the invisible bomber (only the bomber's border can be seen)
                m_Sprite += SICK_SPRITE_ROW_SHADOW * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            }
            else 
            {
                if (m_Sickness == SICK_FLAMEPROOF)
                {
                    m_SpriteOverlay = m_Sprite + SICK_SPRITE_ROW_BRIGHT * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
                    // Give him its player color
                    m_Sprite += m_Player * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
                }
                else
                    m_Sprite += SICK_SPRITE_ROW_FULL * m_BomberSpriteTables[m_BomberState].NumberOfSpritesPerColor;
            }
        }

        // Play sick animation
        m_SickTimer += DeltaTime;
    }

    // Set the new bomber state
    m_BomberState = NewBomberState;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Tells the bomber's bomb that it should fly now. The bomb is forced to
 *  fly to the direction the bomber is looking into (in which direction he last moved).
 *  Resets #m_BombIndex so the bomber has no bomb from this time.
 */

void CBomber::MakeBombFly(EBombFlightType FlightType)
{
    // There must be a bomb 
    ASSERT(m_BombIndex != -1);

    // Direction in which the bomb is to be thrown
    EBombFly BombFly = BOMBFLY_NONE;

    // The direction where the bomber was going the last 
    // time he really moved, will be the direction where
    // we will throw the bomb.
    switch (m_BomberMove.GetLastRealMove())
    {
    case BOMBERMOVE_DOWN: BombFly = BOMBFLY_DOWN;  break;
    case BOMBERMOVE_RIGHT: BombFly = BOMBFLY_RIGHT; break;
    case BOMBERMOVE_LEFT: BombFly = BOMBFLY_LEFT;  break;
    case BOMBERMOVE_UP: BombFly = BOMBFLY_UP;    break;
    default: break;
    }

    // Make the bomb fly in the chosen direction
    m_pArena->GetBomb(m_BombIndex).StartFlying(BombFly, FlightType);

    // Now we don't have the bomb anymore in our hands.
    // This line is important for the caller of this method.
    m_BombIndex = -1;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::Contamination()
{
    // If this bomber is alive then he can contaminate other bombers if he's sick
    if (m_Dead == DEAD_ALIVE)
    {
        // Scan the bombers
        for (int Player = 0; Player < m_pArena->MaxBombers(); Player++)
        {
            // If this player is not ourself
            // and this player exists and is alive,
            // and we are really close to him (pixel distance test)
            if (Player != m_Player &&
                m_pArena->GetBomber(Player).Exist() &&
                m_pArena->GetBomber(Player).IsAlive() &&
                m_pArena->GetBomber(Player).TimeSinceLastSick() > MAX_TIME_SINCE_LAST_SICK &&
                !m_pArena->GetBomber(Player).HasShield() &&
                ABS(m_pArena->GetBomber(Player).GetX() - m_BomberMove.GetX()) +
                ABS(m_pArena->GetBomber(Player).GetY() - m_BomberMove.GetY()) <= CONTAMINATION_NEAR)
            {
                // If this player is not registered as one of our neighbours
                // and we are sick, and we didn't just get sick in this game frame
                if (!m_Neighbours[Player] && m_Sickness != SICK_NOTSICK && !m_JustGotSick)
                {
                    // Then we can contaminate him! Here are some explanations
                    // about the neighbour stuff. The neighbours are checked each
                    // game frame in order to know when to contaminate : contaminating
                    // a bomber can only be done if this bomber wasn't a neighbour
                    // on last game frame, but is now a neighbour on this game frame.
                    // The "just got sick" variable forbids a bomber that was just
                    // contaminated to contaminate again its new neighbour that just
                    // contaminated him.

                    // Swap sickness
                    ESick mySickness = m_Sickness;
                    m_Sickness = m_pArena->GetBomber(Player).GetSickness();
                    m_pArena->GetBomber(Player).SetSickness(mySickness);

                    // Play the contamination sound
                    m_pSound->PlaySample(SAMPLE_SICK_3);

                    // One contamination only
                    break;

                }

                // Register this player as one of our neighbours
                m_Neighbours[Player] = true;

                break;
            }
            // If this player is not ourself and not close to us
            else
            {
                // It's not one of our neighbours we could contaminate
                m_Neighbours[Player] = false;
            }
        }

        // From now on, if we are sick, we can contaminate another bomber
        m_JustGotSick = false;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  The sprite table is prepared by Animate().
 */

void CBomber::Display(void)
{
    // If bomber is not dead and the bomber is not invisible
    if (m_Dead != DEAD_DEAD && m_MakeInvisible == false)
    {

        // Add the sprite in the layer. Priority in bomber sprite layer depends on m_iY.
        m_pDisplay->DrawSprite(m_BomberMove.GetX() + BOMBER_OFFSETX,
            m_BomberMove.GetY() + BOMBER_OFFSETY,
            NULL,                            // Draw entire sprite
            NULL,                            // No need to clip
            m_Page,
            m_Sprite,
            BOMBER_SPRITELAYER,
            m_BomberMove.GetY());

        if (m_SpriteOverlay > 0)
        {

            m_pDisplay->DrawSprite(m_BomberMove.GetX() + BOMBER_OFFSETX,
                m_BomberMove.GetY() + BOMBER_OFFSETY,
                NULL,                            // Draw entire sprite
                NULL,                            // No need to clip
                m_Page,
                m_SpriteOverlay,
                BOMBER_SPRITELAYER,
                m_BomberMove.GetY() + 1);

        }

    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::OnWriteSnapshot(CArenaSnapshot& Snapshot)
{
    int i;

    m_BomberMove.WriteSnapshot(Snapshot);

    Snapshot.WriteInteger(m_BomberAction);
    Snapshot.WriteInteger(m_LastBomberAction);
    Snapshot.WriteInteger(m_Sprite);

    for (i = 0; i < 5; i++)
        Snapshot.WriteInteger(m_AnimationSprites[i]);

    Snapshot.WriteInteger(m_Page);
    Snapshot.WriteFloat(m_Timer);
    Snapshot.WriteFloat(m_SickTimer);
    Snapshot.WriteInteger(m_TotalBombs);
    Snapshot.WriteInteger(m_UsedBombs);
    Snapshot.WriteInteger(m_FlameSize);
    Snapshot.WriteInteger(m_Speed);
    Snapshot.WriteInteger(m_Sickness);
    Snapshot.WriteInteger(m_NumberOfBombItems);
    Snapshot.WriteInteger(m_NumberOfFlameItems);
    Snapshot.WriteInteger(m_NumberOfRollerItems);
    Snapshot.WriteInteger(m_NumberOfKickItems);
    Snapshot.WriteInteger(m_NumberOfThrowItems);
    Snapshot.WriteInteger(m_NumberOfPunchItems);
    Snapshot.WriteInteger(m_NumberOfRemoteItems);

    Snapshot.WriteBoolean(m_ReturnedItems);
    Snapshot.WriteInteger(m_Player);
    Snapshot.WriteInteger(m_Dead);

    for (i = 0; i < MAX_PLAYERS; i++)
        Snapshot.WriteBoolean(m_Neighbours[i]);

    Snapshot.WriteBoolean(m_JustGotSick);
    Snapshot.WriteFloat(m_LiftingTimeElapsed);
    Snapshot.WriteFloat(m_ThrowingTimeElapsed);
    Snapshot.WriteFloat(m_PunchingTimeElapsed);
    Snapshot.WriteFloat(m_StuntTimeElapsed);
    Snapshot.WriteInteger(m_BomberState);
    Snapshot.WriteInteger(m_BombIndex);
    Snapshot.WriteBoolean(m_MakeInvisible);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CBomber::OnReadSnapshot(CArenaSnapshot& Snapshot)
{
    int i;

    m_BomberMove.ReadSnapshot(Snapshot);

    Snapshot.ReadInteger((int*)&m_BomberAction);
    Snapshot.ReadInteger((int*)&m_LastBomberAction);
    Snapshot.ReadInteger(&m_Sprite);

    for (i = 0; i < 5; i++)
        Snapshot.ReadInteger(&m_AnimationSprites[i]);

    Snapshot.ReadInteger(&m_Page);
    Snapshot.ReadFloat(&m_Timer);
    Snapshot.ReadFloat(&m_SickTimer);
    Snapshot.ReadInteger(&m_TotalBombs);
    Snapshot.ReadInteger(&m_UsedBombs);
    Snapshot.ReadInteger(&m_FlameSize);
    Snapshot.ReadInteger(&m_Speed);
    Snapshot.ReadInteger((int*)&m_Sickness);
    Snapshot.ReadInteger(&m_NumberOfBombItems);
    Snapshot.ReadInteger(&m_NumberOfFlameItems);
    Snapshot.ReadInteger(&m_NumberOfRollerItems);
    Snapshot.ReadInteger(&m_NumberOfKickItems);
    Snapshot.ReadInteger(&m_NumberOfThrowItems);
    Snapshot.ReadInteger(&m_NumberOfPunchItems);
    Snapshot.ReadInteger(&m_NumberOfRemoteItems);
    Snapshot.ReadBoolean(&m_ReturnedItems);
    Snapshot.ReadInteger(&m_Player);
    Snapshot.ReadInteger((int*)&m_Dead);

    for (i = 0; i < MAX_PLAYERS; i++)
        Snapshot.ReadBoolean(&m_Neighbours[i]);

    Snapshot.ReadBoolean(&m_JustGotSick);
    Snapshot.ReadFloat(&m_LiftingTimeElapsed);
    Snapshot.ReadFloat(&m_ThrowingTimeElapsed);
    Snapshot.ReadFloat(&m_PunchingTimeElapsed);
    Snapshot.ReadFloat(&m_StuntTimeElapsed);
    Snapshot.ReadInteger((int*)&m_BomberState);
    Snapshot.ReadInteger(&m_BombIndex);
    Snapshot.ReadBoolean(&m_MakeInvisible);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// If the bomber is dead, recreate on the floor the 
// items the bomber picked up (except skull item).
// This will be done when time is up (little delay
// before rint the items)

void CBomber::ReturnItems(float DeltaTime)
{
    // If the bomber's dead
    if (m_Dead == DEAD_DEAD && !m_ReturnedItems)
    {
        // Update time left
        m_Timer -= DeltaTime;

        // If time is up
        if (m_Timer <= 0.0f)
        {
            // Try to create items to return the ones the bomber picked up in the arena
            // If at least one item was created (it's possible the bomber had no item or there wasn't any free block)
            if (CItem::CreateItems(m_pArena,
                ITEMPLACE_FLOOR,
                m_NumberOfBombItems,
                m_NumberOfFlameItems,
                m_NumberOfRollerItems,
                m_NumberOfKickItems,
                0,
                m_NumberOfThrowItems,
                m_NumberOfPunchItems,
                m_NumberOfRemoteItems,
                0,
                0))
            {
                // Play the item fumes sound
                m_pSound->PlaySample(SAMPLE_ITEM_FUMES);
            }

            // The items were returned
            m_ReturnedItems = true;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Update the bomber according to the last frametime

bool CBomber::Update(float DeltaTime)
{

    //! Manage the bomber's movement by calling CBomberMove::Update()
    m_BomberMove.Update(DeltaTime);

    m_ShieldTime -= DeltaTime;

    if (m_ShieldTime < 0.0f)
    {
        m_ShieldTime = 0.0f;

        if (m_isStrongWeak)
        {
            m_isStrongWeak = false;
            m_Speed = SPEED_NORMAL;
            m_FlameSize = 2;
        }
    }

    m_TimeSinceLastSick += DeltaTime;

    // Calculate button-press duration
    // Remember how long the action button has been pressed
    if (m_BomberAction == BOMBERACTION_NONE) {
        // Reset
        m_CountBomberActionDuration = BOMBERACTION_NONE;
        m_BomberActionDuration = 0.0f;
    }
    else if (m_BomberAction == m_CountBomberActionDuration) {
        m_BomberActionDuration += DeltaTime;
        //theLog.WriteLine( "DEBUG: Duration this button pressed=%f",m_BomberActionDuration);
    }
    else {
        m_CountBomberActionDuration = m_BomberAction;
        m_BomberActionDuration = 0.0f;
    }

    //! Update sub-components
    UsedBombs();                //! - Update the number of used bombs by calling UsedBombs().
    Action();                   //! - Drop a bomb or stop a bomb he kicked if needed by calling Action().
    Contamination();            //! - Manage contaminations by calling Contamination().
    Animate(DeltaTime);         //! - Animation (update sprite and sprite table) by calling Animate().
    ReturnItems(DeltaTime);     //! - Return the items the bomber picked up if he's dead by calling ReturnItems().

    /**
     * The bomber can only be deleted by the arena:
     * - if he's dead and
     * - if he has returned the items he has picked up and
     * - if all of his bombs exploded
     */
    return (m_Dead == DEAD_DEAD && m_ReturnedItems && m_UsedBombs == 0);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Make the bomber have the effects of an item he picked up
void CBomber::ItemEffect(EItemType Type)
{
    //! If the bomber is sick and picks up an item, he will now be healthy again,
    //! and the bad, bad skull item escapes from within the bomber to fly somewhere else.
    if (m_Sickness != SICK_NOTSICK)
    {
        m_pArena->NewItem(m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY(), ITEM_SKULL, false, true);
    }

    //! If the bomber has picked up a skull item he gets sick.
    if (Type == ITEM_SKULL)
    {
        // If bombers can be sick
        if (theDebug.CanBombersBeSick())
        {
            // The bomber is now sick (random sickness)
            //! \sa m_Sickness
            m_Sickness = ESick(RANDOM(NUMBER_SICKNESSES));

            m_ShieldTime = 0.0f;

            // Play a random skull item sound
            m_pSound->PlaySample(RANDOM(100) >= 50 ? SAMPLE_SICK_1 : SAMPLE_SICK_2);
        }
    }
    // If the item the bomber has picked up isn't a skull item
    else
    {
        // The effect depends on the item type
        switch (Type)
        {
        case ITEM_BOMB:
        {
            // The bomber can carry more bombs
            m_TotalBombs++;

            // One more picked up bomb item
            m_NumberOfBombItems++;

            break;
        }

        case ITEM_FLAME:
        {
            // More powerful explosions
            m_FlameSize++;

            // One more picked up flame item
            m_NumberOfFlameItems++;

            break;
        }

        case ITEM_KICK:
        {
            if (theDebug.CanBombersKick())
            {
                // One more picked up kick item
                m_NumberOfKickItems++;
            }

            break;
        }

        case ITEM_ROLLER:
        {
            // The bomber speed is increased
            m_Speed += SPEED_INC;

            // One more picked up roller item
            m_NumberOfRollerItems++;

            break;
        }

        case ITEM_THROW:
        {
            // One more picked up throw item
            m_NumberOfThrowItems++;

            break;
        }

        case ITEM_PUNCH:
        {
            // One more picked up punch item
            m_NumberOfPunchItems++;

            break;
        }

        case ITEM_REMOTE:
        {
            // One more picked up remote controler item
            m_NumberOfRemoteItems++;

            break;
        }

        case ITEM_SHIELD:
        {
            // Picked up shield
            m_ShieldTime += SHIELD_TIME;

            break;
        }

        case ITEM_STRONGWEAK:
        {
            m_isStrongWeak = true;

            // Picked up strong/weak
            if (RANDOM(100) >= 25)
            {
                m_ShieldTime += SHIELD_TIME;
                m_Speed = SPEED_FAST;
                m_FlameSize = 10;
                m_NumberOfRemoteItems = 1;
            }
            else
            {
                m_ShieldTime = 0.0f;
                m_Speed = SPEED_SLOW;
                m_FlameSize = 1;
                m_NumberOfRemoteItems = 0;
            }

            break;
        }

        default:
        {
            assert(0);
        }
        } // switch

        // The bomber is not sick anymore
        Heal();

        // Play a random pick sound
        m_pSound->PlaySample((RANDOM(100) >= 50 ? SAMPLE_PICK_ITEM_1 : SAMPLE_PICK_ITEM_2));
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Tell if there is an obstacle at (BlockX,BlockY).
// An obstacle is a wall or a bomb for bombers.

bool CBomber::IsObstacle(int BlockX, int BlockY)
{
    return (m_pArena->IsWall(BlockX, BlockY) ||
        m_pArena->IsBomb(BlockX, BlockY));
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Kick a bomb if there is one at (BlockX,BlockY), 
// and if the bomber is able to.

void CBomber::TryKickBomb(int BlockX, int BlockY, EBombKick BombKick)
{
    // Try only if bomber is able to kick bombs
    if (CanKickBombs())
    {
        // If there is a bomb at BlockX,BlockY
        if (m_pArena->IsBomb(BlockX, BlockY))
        {
            // Seek this bomb
            for (int Index = 0; Index < m_pArena->MaxBombs(); Index++)
            {
                // Test existence and position
                if (m_pArena->GetBomb(Index).Exist() &&
                    m_pArena->GetBomb(Index).GetBlockX() == BlockX &&
                    m_pArena->GetBomb(Index).GetBlockY() == BlockY)
                {
                    // There is a bomb, kick it and give the bomb the bomber player 
                    // number for the bombers to be able to find it.
                    m_pArena->GetBomb(Index).StartMoving(BombKick, m_Player);

                    // There can't be two bombs on the same block
                    break;
                }
            }
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CBomber::Stunt(void)
{
    // A bomb bouncing on the bomber's head doesn't do anything if he is dying
    if (m_BomberState == BOMBERSTATE_DEATH)
        return;

    // Reset the stunt timer, so that a stunt bomber can be stunt for a longer time
    m_StuntTimeElapsed = 0.0f;

    // Check the bomber state before making the bomber stunt
    // If the bomber still has the bomb
    if (m_BombIndex != -1)
    {

        // If the bomber is just lifting this bomb
        if (m_BomberState == BOMBERSTATE_LIFT) {
            // End the lifting and make the bomber hold the bomb
            m_pArena->GetBomb(m_BombIndex).SetBeingHeld();
            m_BomberState = BOMBERSTATE_WALK_HOLD;
        }

        switch (m_BomberState)
        {
            // If the bomber is holding or throwing a bomb
        case BOMBERSTATE_WALK_HOLD:
        case BOMBERSTATE_THROW:
            // Throw the bomb in the current direction he is heading
            MakeBombFly(BOMBFLIGHTTYPE_THROW);
            break;
        case BOMBERSTATE_PUNCH:
            // Make him punch the bomb now (with no bomber punching animation).
            MakeBombFly(BOMBFLIGHTTYPE_PUNCH);
            break;
        default:
            break;
        }
    }


    /**
     * The bomber will lose one item if it has at least one. The descending order is:
     * - remote control
     * - punch
     * - throw
     * - kick
     * - bomb
     * - flame
     * - roller
     * The item (if any) taken from the bomber is populated to the arena so everyone can catch it.
     */

    EItemType ItemType = ITEM_NONE;

    if (m_NumberOfRemoteItems > 0)
    {
        m_NumberOfRemoteItems--;
        ItemType = ITEM_REMOTE;
    }
    else if (m_NumberOfPunchItems > 0)
    {
        m_NumberOfPunchItems--;
        ItemType = ITEM_PUNCH;
    }
    else if (m_NumberOfThrowItems > 0)
    {
        m_NumberOfThrowItems--;
        ItemType = ITEM_THROW;
    }
    else if (m_NumberOfKickItems > 0)
    {
        m_NumberOfKickItems--;
        ItemType = ITEM_KICK;
    }
    else if (m_NumberOfBombItems > 0)
    {
        m_NumberOfBombItems--;
        ItemType = ITEM_BOMB;
    }
    else if (m_NumberOfFlameItems > 0)
    {
        m_NumberOfFlameItems--;
        ItemType = ITEM_FLAME;
    }
    else if (m_NumberOfRollerItems > 0)
    {
        m_NumberOfRollerItems--;
        ItemType = ITEM_ROLLER;
    }
    else if (m_ShieldTime > 0.0f)
    {
        m_ShieldTime = 0.0f;
        ItemType = ITEM_SHIELD;
    }

    if (ItemType != ITEM_NONE)
    {
        m_pArena->NewItem(m_BomberMove.GetBlockX(), m_BomberMove.GetBlockY(), ItemType, false, true);
    }

    m_pSound->PlaySample(SAMPLE_BOMBER_LOSE_ITEM);

    // The bomber is now stunt
    m_BomberState = BOMBERSTATE_STUNT;

    // Call animate whenever bomber changes state to update m_Page and m_Sprite correctly
    Animate(0.0f);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
