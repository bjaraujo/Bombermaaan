// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CLevel.cpp
 *  \brief Handling a level
 */

#include "StdAfx.h"
#include "CLevel.h"
//#include "COptions.h"
//#include "CInput.h"
#include "CArena.h"
#include <sstream>
#include <iomanip>
#include "SimpleIni.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/*** // Initial number of items when a new arena is built
#define INITIAL_ITEMBOMB        11
#define INITIAL_ITEMFLAME       8
#define INITIAL_ITEMROLLER      7
#define INITIAL_ITEMKICK        2
#define INITIAL_ITEMSKULL       1
#define INITIAL_ITEMTHROW       2
#define INITIAL_ITEMPUNCH       2
#define INITIAL_ITEMREMOTE      2

// Initial flame size
#define INITIAL_FLAMESIZE       2

// Initial number of bombs the bomber can drop
#define INITIAL_BOMBS           1
***/
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CLevel::CLevel(std::string filename_full, std::string filename_short) :
    m_FilenameShort(filename_short),
    m_FilenameFull(filename_full)
{

    for (int i = 0; i < ARENA_WIDTH; i++)
    {
        for (int j = 0; j < ARENA_HEIGHT; j++)
        {
            m_ArenaData[i][j] = BLOCKTYPE_HARDWALL;
        }
    }

    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        m_NumberOfItemsInWalls[i] = 0;
    }

    for (int i = 0; i < NUMBER_OF_BOMBERSKILLS; i++)
    {
        m_InitialBomberSkills[i] = 0;
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CLevel::~CLevel(void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  @param  filename_full    The full file name of the level file
 *  @param  filename_short   The file name of the level file without path
 */

bool CLevel::LoadFromFile()
{
    bool ErrorOccurred = false;

    // Open the existing level file for reading
    std::ifstream in;
    in.open(m_FilenameFull.c_str(), std::ios_base::in);

    // If it failed
    if (!in.is_open())
    {
        theLog.WriteLine("Options         => Loading level file %s failed.", m_FilenameFull.c_str());
        // Stop loading levels
        return false;
    }

    // This is the first line for the level files beginning with version 2 (therefore "V2plus")
    std::string headerV2plus("; Bombermaaan level file version=");

    std::string s;
    std::getline(in, s);
    int LevelVersion;

    // When header string is found at the beginning of the string, find() returns 0 (offset 0)
    if (s.find(headerV2plus) == 0) {
        // We can look for the level version now
        LevelVersion = atoi(s.substr(headerV2plus.length()).c_str());
    }
    else
    {
        LevelVersion = 1;
    }

    switch (LevelVersion) {

    case 1:
        if (!LoadVersion1(in)) {
            ErrorOccurred = true;
        }
        break;

    case 2:
        if (!LoadVersion2(m_FilenameFull)) {
            ErrorOccurred = true;
        }
        break;

    default:
        theLog.WriteLine("Options         => !!! Unsupported version of level file %s.", m_FilenameShort.c_str());
        ErrorOccurred = true;
        break;

    }

    // Close the level file
    in.close();

    // Validate this level if no error occurred so far
    if (!ErrorOccurred)
    {
        ErrorOccurred = !Validate();
    }

    // If there wasn't any problem
    if (!ErrorOccurred)
    {
        theLog.WriteLine("Options         => Level file %s was successfully loaded (version %d).", m_FilenameShort.c_str(), LevelVersion);
    }
    // If there was a problem
    else
    {
        theLog.WriteLine("Options         => !!! Could not load level file %s (version %d).", m_FilenameShort.c_str(), LevelVersion);
    }

    // If we had to stop then there is a problem.
    if (ErrorOccurred)
        return false;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CLevel::LoadVersion1(std::ifstream& File) {

    bool StopReadingFile = false;
    std::filebuf *pbuf = File.rdbuf();

    // go to the beginning
    pbuf->pubseekpos(0, std::ios::in);

    // For each line of characters to read
    for (int y = 0; y < ARENA_HEIGHT; y++)
    {
        // Buffer where we'll store one line of characters. We'll read the two EOL characters as well.
        std::string Line;
        int ReadBytes;

        // Read one line of characters (including the EOL chars)
        if (File.good())
        {
            std::getline(File, Line);
            ReadBytes = Line.size();
        }
        else
        {
            ReadBytes = 0;
        }

        // Check if all the characters were read
        if (ReadBytes < ARENA_WIDTH)
        {
            // Log there is a problem
            theLog.WriteLine("Options         => !!! Level file is incorrect (Line: %d, Length: %d).", y + 1, ReadBytes);

            // Close the level file
            File.close();

            // Stop loading levels
            StopReadingFile = true;
            break;
        }

        // For each character representing a block in this line
        for (int x = 0; x < ARENA_WIDTH; x++)
        {
            // According to the character value, store the corresponding block type in the current position and level
            switch (Line.c_str()[x])
            {
            case '*': m_ArenaData[x][y] = BLOCKTYPE_HARDWALL;          break;
            case '-': m_ArenaData[x][y] = BLOCKTYPE_SOFTWALL;          break;
            case '?': m_ArenaData[x][y] = BLOCKTYPE_RANDOM;            break;
            case ' ': m_ArenaData[x][y] = BLOCKTYPE_FREE;              break;
            case '1': m_ArenaData[x][y] = BLOCKTYPE_WHITEBOMBER;       break;
            case '2': m_ArenaData[x][y] = BLOCKTYPE_BLACKBOMBER;       break;
            case '3': m_ArenaData[x][y] = BLOCKTYPE_REDBOMBER;         break;
            case '4': m_ArenaData[x][y] = BLOCKTYPE_BLUEBOMBER;        break;
            case '5': m_ArenaData[x][y] = BLOCKTYPE_GREENBOMBER;       break;
            case 'R': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_RIGHT;    break;
            case 'D': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_DOWN;     break;
            case 'L': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_LEFT;     break;
            case 'U': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_UP;       break;
            case 'B': m_ArenaData[x][y] = BLOCKTYPE_ITEM_BOMB;         break;
            case 'K': m_ArenaData[x][y] = BLOCKTYPE_ITEM_KICK;         break;
            case 'F': m_ArenaData[x][y] = BLOCKTYPE_ITEM_FLAME;        break;
            case 'S': m_ArenaData[x][y] = BLOCKTYPE_ITEM_ROLLER;       break;
            case 'P': m_ArenaData[x][y] = BLOCKTYPE_ITEM_PUNCH;        break;
            case 'T': m_ArenaData[x][y] = BLOCKTYPE_ITEM_THROW;        break;
            case 'Z': m_ArenaData[x][y] = BLOCKTYPE_ITEM_REMOTES;      break;
            case 'C': m_ArenaData[x][y] = BLOCKTYPE_ITEM_SKULL;        break;
            case 'V': m_ArenaData[x][y] = BLOCKTYPE_ITEM_SHIELD;       break;
            case 'I': m_ArenaData[x][y] = BLOCKTYPE_ITEM_STRONGWEAK;   break;
            default:
            {
                // Log there is a problem
                theLog.WriteLine("Options         => !!! Level file is incorrect (unknown character %c).", Line[x]);

                // Close the level file
                File.close();

                // Stop loading levels
                StopReadingFile = true;
                break;
            }
            }
        }

        // If there was a problem
        if (StopReadingFile)
        {
            // Stop reading this level file
            break;
        }
    }

    m_NumberOfItemsInWalls[ITEM_BOMB] = INITIAL_ITEMBOMB;
    m_NumberOfItemsInWalls[ITEM_FLAME] = INITIAL_ITEMFLAME;
    m_NumberOfItemsInWalls[ITEM_KICK] = INITIAL_ITEMKICK;
    m_NumberOfItemsInWalls[ITEM_ROLLER] = INITIAL_ITEMROLLER;
    m_NumberOfItemsInWalls[ITEM_SKULL] = INITIAL_ITEMSKULL;
    m_NumberOfItemsInWalls[ITEM_THROW] = INITIAL_ITEMTHROW;
    m_NumberOfItemsInWalls[ITEM_PUNCH] = INITIAL_ITEMPUNCH;
    m_NumberOfItemsInWalls[ITEM_REMOTE] = INITIAL_ITEMREMOTE;
    m_NumberOfItemsInWalls[ITEM_SHIELD] = INITIAL_ITEMSHIELD;
    m_NumberOfItemsInWalls[ITEM_STRONGWEAK] = INITIAL_ITEMSTRONGWEAK;

    m_InitialBomberSkills[BOMBERSKILL_FLAME] = INITIAL_FLAMESIZE;
    m_InitialBomberSkills[BOMBERSKILL_BOMBS] = INITIAL_BOMBS;
    m_InitialBomberSkills[BOMBERSKILL_BOMBITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_FLAMEITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_ROLLERITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_KICKITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_THROWITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_PUNCHITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_REMOTEITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_SHIELDITEMS] = 0;
    m_InitialBomberSkills[BOMBERSKILL_STRONGWEAKITEMS] = 0;

    return !StopReadingFile;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CLevel::LoadVersion2(std::string filename)
{
    // Define INI file
    CSimpleIniA iniFile(false, false, false);

    // Load INI file
    SI_Error rc = iniFile.LoadFile(filename.c_str());
    if (rc < 0) return false;

    std::string s;
    int value;

    // Read the width of the map and check whether it is allowed
    // At the moment the width is fix, but maybe the width can be changed in the future
    value = atoi(iniFile.GetValue("General", "Width", "0"));
    if (value != ARENA_WIDTH) {
        theLog.WriteLine("Options         => !!! Invalid arena width %d. Only %d is allowed.", value, ARENA_WIDTH);
        return false;
    }

    // Read the height of the map and check whether it is allowed
    // At the moment the height is fix, but maybe the height can be changed in the future
    value = atoi(iniFile.GetValue("General", "Height", "0"));
    if (value != ARENA_HEIGHT) {
        theLog.WriteLine("Options         => !!! Invalid arena height %d. Only %d is allowed.", value, ARENA_HEIGHT);
        return false;
    }

    // Read the maximum number of players allowed with this level
    // At the moment this must be set to 5
    // Maybe this is changed in the future
    value = atoi(iniFile.GetValue("General", "MaxPlayers", "0"));
    if (value != 5) {
        theLog.WriteLine("Options         => !!! Invalid maximum players %d. Only %d is allowed.", value, 5);
        return false;
    }

    // Read the maximum number of players allowed with this level
    // Currently this must be set to 1, though a game with 1 player is not possible
    // Maybe this is changed in the future
    value = atoi(iniFile.GetValue("General", "MinPlayers", "0"));
    if (value != 1) {
        theLog.WriteLine("Options         => !!! Invalid minimum players %d. Only %d is allowed.", value, 1);
        return false;
    }

    // Check if there is a line with the creator
    // The creator can be empty, it's not stored anywhere at the moment
    std::string creator = iniFile.GetValue("General", "Creator", "");

    // Priority line following
    // The priority setting is not used currently
    // For future use:
    // - The levels are first sorted by priority and then by the file name
    value = atoi(iniFile.GetValue("General", "Priority", "0"));

    // Comment line following (not used currently)
    std::string comment = iniFile.GetValue("General", "Comment", "");

    // Description line following (not used currently)
    std::string description = iniFile.GetValue("General", "Description", "");

    // For each line of characters to read
    for (int y = 0; y < ARENA_HEIGHT; y++)
    {
        std::ostringstream oss;
        oss << "Line." << std::setw(2) << std::setfill('0') << y;
        std::string keyName = oss.str();

        std::string arenaLine = iniFile.GetValue("Map", keyName.c_str(), "");

        if (arenaLine.length() != ARENA_WIDTH) {
            theLog.WriteLine("Options         => !!! Level file is incorrect (Line.%d wrong length %d).", y, arenaLine.length());
            return false;
        }

        // For each character representing a block in this line
        for (int x = 0; x < ARENA_WIDTH; x++)
        {
            // According to the character value, store the corresponding block type in the current position and level
            switch (arenaLine.at(x))
            {
            case '*': m_ArenaData[x][y] = BLOCKTYPE_HARDWALL;        break;
            case '-': m_ArenaData[x][y] = BLOCKTYPE_SOFTWALL;        break;
            case '?': m_ArenaData[x][y] = BLOCKTYPE_RANDOM;          break;
            case ' ': m_ArenaData[x][y] = BLOCKTYPE_FREE;            break;
            case '1': m_ArenaData[x][y] = BLOCKTYPE_WHITEBOMBER;     break;
            case '2': m_ArenaData[x][y] = BLOCKTYPE_BLACKBOMBER;     break;
            case '3': m_ArenaData[x][y] = BLOCKTYPE_REDBOMBER;       break;
            case '4': m_ArenaData[x][y] = BLOCKTYPE_BLUEBOMBER;      break;
            case '5': m_ArenaData[x][y] = BLOCKTYPE_GREENBOMBER;     break;
            case 'R': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_RIGHT;  break;
            case 'D': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_DOWN;   break;
            case 'L': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_LEFT;   break;
            case 'U': m_ArenaData[x][y] = BLOCKTYPE_MOVEBOMB_UP;     break;
            case 'B': m_ArenaData[x][y] = BLOCKTYPE_ITEM_BOMB;       break;
            case 'K': m_ArenaData[x][y] = BLOCKTYPE_ITEM_KICK;       break;
            case 'F': m_ArenaData[x][y] = BLOCKTYPE_ITEM_FLAME;      break;
            case 'S': m_ArenaData[x][y] = BLOCKTYPE_ITEM_ROLLER;     break;
            case 'P': m_ArenaData[x][y] = BLOCKTYPE_ITEM_PUNCH;      break;
            case 'T': m_ArenaData[x][y] = BLOCKTYPE_ITEM_THROW;      break;
            case 'Z': m_ArenaData[x][y] = BLOCKTYPE_ITEM_REMOTES;    break;
            case 'C': m_ArenaData[x][y] = BLOCKTYPE_ITEM_SKULL;      break;
            case 'V': m_ArenaData[x][y] = BLOCKTYPE_ITEM_SHIELD;     break;
            case 'I': m_ArenaData[x][y] = BLOCKTYPE_ITEM_STRONGWEAK; break;
            default:
            {
                // Log there is a problem
                theLog.WriteLine("Options         => !!! Level file is incorrect (unknown character %c).", arenaLine.at(x));
                return false;
            }
            }
        }

    }

    //---------------------
    // Read the ItemsInWalls values
    //---------------------

    //! @todo Replace fix value (third parameter of GetValue(...)) by default setting
    m_NumberOfItemsInWalls[ITEM_BOMB] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Bombs", "0"));
    m_NumberOfItemsInWalls[ITEM_FLAME] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Flames", "0"));
    m_NumberOfItemsInWalls[ITEM_KICK] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Kicks", "0"));
    m_NumberOfItemsInWalls[ITEM_ROLLER] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Rollers", "0"));
    m_NumberOfItemsInWalls[ITEM_SKULL] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Skulls", "0"));
    m_NumberOfItemsInWalls[ITEM_THROW] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Throws", "0"));
    m_NumberOfItemsInWalls[ITEM_PUNCH] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Punches", "0"));
    m_NumberOfItemsInWalls[ITEM_REMOTE] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Remotes", "0"));
    m_NumberOfItemsInWalls[ITEM_SHIELD] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.Shields", "0"));
    m_NumberOfItemsInWalls[ITEM_STRONGWEAK] = atoi(iniFile.GetValue("Settings", "ItemsInWalls.StrongWeak", "0"));

    //---------------------
    // Read the BomberSkillsAtStart values
    //---------------------

    m_InitialBomberSkills[BOMBERSKILL_FLAME] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.FlameSize", "0"));
    m_InitialBomberSkills[BOMBERSKILL_BOMBS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.MaxBombs", "0"));
    m_InitialBomberSkills[BOMBERSKILL_BOMBITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.BombItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_FLAMEITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.FlameItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_ROLLERITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.RollerItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_KICKITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.KickItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_THROWITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.ThrowItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_PUNCHITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.PunchItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_REMOTEITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.RemoteItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_SHIELDITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.ShieldItems", "0"));
    m_InitialBomberSkills[BOMBERSKILL_STRONGWEAKITEMS] = atoi(iniFile.GetValue("Settings", "BomberSkillsAtStart.StrongWeakItems", "0"));

    //---------------------
    // Read the ContaminationsNotUsed setting
    //---------------------

    // This setting controls which contamination should not be used in this level
    // The only one value allowed is "None" at the moment

    std::string contaminationsNotToUse = iniFile.GetValue("Settings", "ContaminationsNotUsed", "");


    // Everything went right
    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 * @brief   check if this level does not exceed the maximum possible number of items
 * @param   sumOfMaxItems   pointer to an integer variable where the sum of max items is counted
 * @return  true if the number of maximum allowed items is not exceeded, false otherwise
 */
bool CLevel::CheckMaxNumberOfItems(unsigned int *sumOfMaxItems)
{
    // check if maximum number of items is not exceeded
    // we do this, because if there is a draw game when many bombers die
    // they all lose their items at the same time.
    *sumOfMaxItems = 0;
    unsigned int i;

    // count items in walls
    for (i = ITEM_NONE + 1; i < NUMBER_OF_ITEMS; i++)
    {
        *sumOfMaxItems += m_NumberOfItemsInWalls[i];
    }

    // count initial bomber skills (note: count the worst case with five players)
    for (i = BOMBERSKILL_DUMMYFIRST + 1; i < NUMBER_OF_BOMBERSKILLS; i++)
    {
        // initial skills like bombs and flames will not be lost
        if (i != BOMBERSKILL_FLAME && i != BOMBERSKILL_BOMBS)
            *sumOfMaxItems += m_InitialBomberSkills[i] * MAX_PLAYERS;
    }

    if (*sumOfMaxItems > MAX_ITEMS)
        return false;
    else
        return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CLevel::Validate()
{
    unsigned int itemCount;

    // There may be more checks in the future

    if (!CheckMaxNumberOfItems(&itemCount)){
        // Log there is a problem
        theLog.WriteLine("Options         => !!! Level file is incorrect (Too many items: %d of %d allowed).", itemCount, MAX_ITEMS);
        return false;
    }

    return true;
}
