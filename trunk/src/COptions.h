/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008, 2010 Bernd Arnold
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
 *  \file COptions.h
 *  \brief Header file for the game options
 */

#ifndef __COPTIONS_H__
#define __COPTIONS_H__

#include "CDisplay.h"
#include "CItem.h"
#include "CLevel.h"
#include "CTeam.h"
#include "tinyxml.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the type of a bomber
enum EBomberType
{
    BOMBERTYPE_OFF,     //!< The bomber is not playing
    BOMBERTYPE_MAN,     //!< The bomber is controlled by a local human player
    BOMBERTYPE_COM,     //!< The bomber is controlled by the computer
    BOMBERTYPE_NET      //!< The bomber is controlled by a human player on the network
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EBattleMode
{
    BATTLEMODE_SINGLE,     //!< Single battle mode
    BATTLEMODE_TEAM        //!< Team battle mode
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EBomberTeam
{
    BOMBERTEAM_A,     //!< The bomber team A
    BOMBERTEAM_B,     //!< The bomber team B
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EActionAIAlive
{
    ACTIONONLYAIPLAYERSALIVE_CONTINUEGAME,      //!< The game continues when only AI players are alive
    ACTIONONLYAIPLAYERSALIVE_STARTCLOSING,      //!< The arena starts closing when only AI players are alive
    ACTIONONLYAIPLAYERSALIVE_ENDMATCHDRAWGAME,  //!< The match ends and there is a draw game when only AI players are alive
    ACTIONONLYAIPLAYERSALIVE_SPEEDUPGAME        //!< The game speed is increased
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_PLAYER_INPUT        10
#define NUM_CONTROLS            6

#define CONTROL_UP      0
#define CONTROL_DOWN    1
#define CONTROL_LEFT    2
#define CONTROL_RIGHT   3
#define CONTROL_ACTION1 4
#define CONTROL_ACTION2 5

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Contains every option in the game and manages the configuration file
class COptions
{
private:

    int                 m_TimeStartMinutes;             //!< How many minutes in the time when a battle starts?
    int                 m_TimeStartSeconds;             //!< How many seconds in the time when a battle starts?
    int                 m_TimeUpMinutes;                //!< How many minutes in the time when the arena starts closing?
    int                 m_TimeUpSeconds;                //!< How many seconds in the time when the arena starts closing?
    EBomberType         m_BomberType [MAX_PLAYERS];     //!< Bomber type for each player
    EBomberTeam         m_BomberTeam[MAX_PLAYERS];      //!< Bomber team for each player
    EBattleMode         m_BattleMode;                   //!< Battle mode single / team
    int                 m_PlayerCount;                  //!< Total number of players in the battle
    int                 m_BattleCount;                  //!< How many battles to win in order to be victorious
    int                 m_PlayerInput [MAX_PLAYERS];    //!< Player input to use for each player
    EDisplayMode        m_DisplayMode;                  //!< Current display mode to use in the CDisplay object
    int                 m_Control[MAX_PLAYER_INPUT][NUM_CONTROLS]; //!< Control number to use for each player input and for each control
    int                 m_Level;
    std::vector<CLevel> m_Levels;
    std::string         configFileName;                 //!< Full name of the config file (including path)
    std::string         oldconfigFileName;              //!< Full name of the old (binary) config file (including path)

    void                SetDefaultValues();             //!< Set the default configuration values
    void                WriteXMLData();                 //!< Write the options to the XML based configuration file
    void                ReadIntFromXML( TiXmlDocument &doc, std::string configNode, std::string attrName, int *value );
    bool                LoadLevels( std::string appDataFolder, std::string pgmFolder );              //!< Load game levels data and names from the level directory.
    bool                LoadConfiguration (void);       //!< Load the configuration file, create default if it does not exist.
                        
public:                 
                        
                        COptions (void);                //!< Constructor.
                        COptions(const COptions& another);
                        ~COptions (void);               //!< Destructor. Do nothing.
    COptions&           operator = (const COptions& Copy);    //!< Operator = used to copy an option object.
    bool                Create( bool useAppDataFolder, std::string dynamicDataFolder, std::string pgmFolder );  //!< Load the options. Create the configuration file if it doesn't exist.
    void                Destroy (void);                 //!< Free allocated memory.
    void                SaveBeforeExit (void);          //!< Write the options to the configuration file
    inline int          GetTimeStartMinutes (void);     //!< Get how many minutes in the time when a battle starts
    inline int          GetTimeStartSeconds (void);     //!< Get how many seconds in the time when a battle starts
    inline int          GetTimeUpMinutes (void);        //!< Get how many minutes in the time when the arena starts closing
    inline int          GetTimeUpSeconds (void);        //!< Get how many seconds in the time when the arena starts closing
    inline void         SetTimeStart (int TimeStartMinutes, int TimeStartSeconds); //!< Set the time when a battle starts
    inline void         SetTimeUp (int TimeUpMinutes, int TimeUpSeconds); //!< Set the time when the arena starts closing
    inline EBomberType  GetBomberType (int Player);     //!< Get the bomber type of the specified player
    inline void         SetBomberType (int Player, EBomberType BomberType); //!< Set the bomber type of the specified player
    inline int          GetBattleCount (void);          //!< Get how many battles to win in order to be victorious
    inline void         SetBattleCount (int BattleCount); //!< Set how many battles to win in order to be victorious
    inline int          GetPlayerInput (int Player);    //!< Get the player input to use for the specified player
    inline void         SetPlayerInput (int Player, int PlayerInput); //!< Set the player input to use for the specified player
    inline void         SetDisplayMode (EDisplayMode DisplayMode); //!< Set the display mode to use in the CDisplay object
    inline EDisplayMode GetDisplayMode (void);          //!< Set the display mode to use in the CDisplay object
    inline int          GetControl (int PlayerInput, int Control);
    inline void         SetControl (int PlayerInput, int Control, int Value);
    inline EBlockType   GetBlockType (int X, int Y);
    inline int          GetNumberOfItemsInWalls (EItemType ItemType);
    inline int          GetInitialBomberSkills (EBomberSkills BomberSkill);
    inline void         SetLevel (int Level);
    inline int          GetLevel (void);
    inline int          GetNumberOfLevels (void);
    inline const char*  GetLevelName (void);
    inline EActionAIAlive   GetOption_ActionWhenOnlyAIPlayersLeft();

    inline void         SetBattleMode(EBattleMode BattleMode);
    inline EBattleMode  GetBattleMode();

    inline EBomberTeam  GetBomberTeam(int Player);                          //!< Get the bomber team of the specified player
    inline void         SetBomberTeam(int Player, EBomberTeam BomberTeam); //!< Set the bomber team of the specified player

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int COptions::GetTimeStartMinutes (void)
{
    return m_TimeStartMinutes;
}

inline int COptions::GetTimeStartSeconds (void)
{
    return m_TimeStartSeconds;
}

inline int COptions::GetTimeUpMinutes (void)
{
    return m_TimeUpMinutes;
}

inline int COptions::GetTimeUpSeconds (void)
{
    return m_TimeUpSeconds;
}

inline void COptions::SetTimeStart (int TimeStartMinutes, int TimeStartSeconds)
{
    m_TimeStartMinutes = TimeStartMinutes;
    m_TimeStartSeconds = TimeStartSeconds;
}

inline void COptions::SetTimeUp (int TimeUpMinutes, int TimeUpSeconds)
{
    m_TimeUpMinutes = TimeUpMinutes;
    m_TimeUpSeconds = TimeUpSeconds;
}

inline EBomberType COptions::GetBomberType (int Player)
{
    return m_BomberType[Player];
}

inline void COptions::SetBomberType (int Player, EBomberType BomberType)
{
    m_BomberType[Player] = BomberType;
}

inline void COptions::SetBattleMode(EBattleMode BattleMode)
{
    m_BattleMode = BattleMode;
}

inline EBattleMode COptions::GetBattleMode(void)
{
    return m_BattleMode;
}

inline int COptions::GetBattleCount (void)
{
    return m_BattleCount;
}

inline void COptions::SetBattleCount (int BattleCount)
{
    m_BattleCount = BattleCount;
}

inline int COptions::GetPlayerInput (int Player)
{
    ASSERT (Player >= 0 && Player < MAX_PLAYERS);

    return m_PlayerInput[Player];
}

inline void COptions::SetPlayerInput (int Player, int PlayerInput)
{
    ASSERT (Player >= 0 && Player < MAX_PLAYERS);
    //TODO: ASSERT PlayerInput

    m_PlayerInput[Player] = PlayerInput;
}

inline void COptions::SetDisplayMode (EDisplayMode DisplayMode)
{
    m_DisplayMode = DisplayMode;
}

inline EDisplayMode COptions::GetDisplayMode (void)
{
    return m_DisplayMode;
}

inline int COptions::GetControl (int PlayerInput, int Control)
{
    ASSERT (PlayerInput >= 0 && PlayerInput < MAX_PLAYER_INPUT);
    ASSERT (Control >= 0 && Control < NUM_CONTROLS);

    return m_Control[PlayerInput][Control];
}

inline void COptions::SetControl (int PlayerInput, int Control, int Value)
{
    ASSERT (PlayerInput >= 0 && PlayerInput < MAX_PLAYER_INPUT);
    ASSERT (Control >= 0 && Control < NUM_CONTROLS);

    m_Control[PlayerInput][Control] = Value;
}

inline EBlockType COptions::GetBlockType (int X, int Y)
{
    ASSERT (m_Level >= 0 && m_Level < (int)m_Levels.size());

    return m_Levels.at(m_Level).GetBlockType(X,Y);
}

inline int COptions::GetNumberOfItemsInWalls ( EItemType ItemType )
{
    ASSERT (m_Level >= 0 && m_Level < (int)m_Levels.size()); // #3078839

    return m_Levels.at(m_Level).GetNumberOfItemsInWalls(ItemType);
}

inline int COptions::GetInitialBomberSkills ( EBomberSkills BomberSkill )
{
    // #3078839
    ASSERT (m_Level >= 0 && m_Level < (int)m_Levels.size()); // #3078839

    return m_Levels.at(m_Level).GetInitialBomberSkills(BomberSkill);
}

inline void COptions::SetLevel (int Level)
{
    ASSERT (Level >= 0 && Level < (int)m_Levels.size()); // #3078839
    m_Level = Level;
}

inline int COptions::GetLevel (void)
{
    return m_Level;
}

inline int COptions::GetNumberOfLevels (void)
{
    return m_Levels.size();
}

inline const char* COptions::GetLevelName (void)
{
    ASSERT (m_Level >= 0 && m_Level < (int)m_Levels.size()); // #3078839
    return m_Levels.at( m_Level ).GetLevelName();
}

inline EActionAIAlive COptions::GetOption_ActionWhenOnlyAIPlayersLeft()
{
    //! @TODO This should really be an option
    return ACTIONONLYAIPLAYERSALIVE_CONTINUEGAME;
    // return ACTIONONLYAIPLAYERSALIVE_SPEEDUPGAME;
    // return ACTIONONLYAIPLAYERSALIVE_ENDMATCHDRAWGAME;
}

inline EBomberTeam COptions::GetBomberTeam(int Player)
{
    return m_BomberTeam[Player];
}

inline void COptions::SetBomberTeam(int Player, EBomberTeam BomberTeam)
{
    m_BomberTeam[Player] = BomberTeam;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __COPTIONS_H__

