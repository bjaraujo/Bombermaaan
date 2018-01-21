// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007-2010 Bernd Arnold
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
 *  \file CGame.cpp
 *  \brief The core of the program, handling sub-components, program control
 */

#ifdef ALLEGRO
    #inlcude "alegro.h"
#endif

#ifdef SDL
    #include "SDL.h"
#endif

#include "StdAfx.h"

#include "CGame.h"
#include "CWindow.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CDrawGame.h"
#include "CWinner.h"
#include "CVictory.h"
#include "CMatch.h"
#include "CScores.h"
#include "CControls.h"

#include "Bombermaaan.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Define this if the log file should be enabled
#define ENABLE_LOG

// Define this if the debug log file should be enabled
// #define ENABLE_DEBUG_LOG

// Define this if the console window should be enabled
// #define ENABLE_CONSOLE

// Define this if you want the console to filter repeated message
//#define ENABLE_CONSOLE_REPEATED_MESSAGE_FILTERING

// Define this if debug keys should be enabled
//#define ENABLE_DEBUG_KEYS

// Define this if the game should update normally even when the window does not have the focus
// Note : when the window does not have the focus, you will not have the input focus.
#define ENABLE_UPDATE_WHEN_WINDOW_IS_INACTIVE

// Define this if you want sound and music in the game
#define ENABLE_SOUND

// Define the name of the DLL (where sprites and sound samples are stored)
#ifdef WIN32
#define NAME_OF_BOMBERMAN_DLL "Bombermaaan32.dll"
#else
#define NAME_OF_BOMBERMAN_DLL "libBombermaaan32.so"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


#ifdef WIN32
CGame::CGame(HINSTANCE hInstance, const char* pCommandLine)
    : CWindow(hInstance, pCommandLine, IDI_BOMBER)
#else
CGame::CGame (HINSTANCE hInstance, char** pCommandLine)
    : CWindow (hInstance, "Bomberman", IDI_BOMBER)
#endif
{
    m_GameMode = GAMEMODE_NONE;
    m_hModule = NULL;
#ifdef WIN32
    m_hInstance = hInstance;
#else
    m_hInstance = NULL;
#endif
    SEED_RANDOM((unsigned)time(NULL));

    //
    // Set the window title
    //

    std::string windowTitle = "Bombermaaan ";

    windowTitle.append(APP_VERSION_INFO);

    windowTitle.append(" - Compiled ");

    // At the end, the windowTitle is "... - Compiled YYYY-MM-DD"

    // Year
    windowTitle.append(__DATE__ + 7);
    windowTitle.append("-");

    // Month: Jan/Jun/Jul, Feb, Mar/May, Apr/Aug, Sep, Oct, Nov, Dec
    // Inspired by the file TortoiseProc.cpp from TortoiseSVN
    if (__DATE__[0] == 'J') {
        if (__DATE__[1] == 'a')      { windowTitle.append("01"); }
        else if (__DATE__[2] == 'n') { windowTitle.append("06"); }
        else                             { windowTitle.append("07"); }
    }
    else if (__DATE__[0] == 'F') {
        windowTitle.append("02");
    }
    else if (__DATE__[0] == 'M') {
        if (__DATE__[2] == 'r')      { windowTitle.append("03"); }
        else                             { windowTitle.append("05"); }
    }
    else if (__DATE__[0] == 'A') {
        if (__DATE__[1] == 'p')      { windowTitle.append("04"); }
        else                             { windowTitle.append("08"); }
    }
    else if (__DATE__[0] == 'S') {
        windowTitle.append("09");
    }
    else if (__DATE__[0] == 'O') {
        windowTitle.append("10");
    }
    else if (__DATE__[0] == 'N') {
        windowTitle.append("11");
    }
    else if (__DATE__[0] == 'D') {
        windowTitle.append("12");
    }
    else {
        // Should never be reached
        ASSERT(0);
    }
    windowTitle.append("-");

    // Day (ensure for leading zero when day is 1 to 9)
    if (__DATE__[4] == ' ') {
        windowTitle.append("0");
        windowTitle.append(__DATE__ + 5, 1);
    }
    else {
        windowTitle.append(__DATE__ + 4, 2);
    }

#ifdef DIRECTX_VIDEO
    SetWindowText(m_hWnd, windowTitle.c_str());
#else
    // keep the window text in mind
    m_WindowTitle = windowTitle;
#endif

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CGame::~CGame()
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Creates the main parts of the game and establishes relationships to members
 **/
#ifdef WIN32
bool CGame::Create(const char* pCommandLine)
#else
bool CGame::Create (char **pCommandLine, int pCommandLineCount)
#endif
{
    // If certain strings are detected in the command line arguments...
    // There is no check if the parameters are surrounded by spaces, or at the beginning of the line,
    // or the end. So "-----__/-h999" would also match (the -h is found).
#ifdef WIN32
    if (strstr(pCommandLine, "-?") != NULL ||
        strstr(pCommandLine, "--help") != NULL ||  // Not really necessary, since "-?" already did the job
        strstr(pCommandLine, "--license") != NULL ||
        strstr(pCommandLine, "--show-license") != NULL ||
        strstr(pCommandLine, "/?") != NULL)
#else
    bool helpRequested = false;

    for (int i = 0; i < pCommandLineCount; i++)
    {
        if (strncmp(pCommandLine[i], "-h", 2) == 0 ||
            strncmp(pCommandLine[i], "--help", 6) == 0 ||
            strncmp(pCommandLine[i], "--license", 6) == 0 ||
            strncmp(pCommandLine[i], "--show-license", 6) == 0 ||
            strncmp(pCommandLine[i], "/?", 6) == 0)
        {
            helpRequested = true;
            break;
        }
    }

    if (helpRequested)
#endif
    {
        // Display a message box
#ifdef WIN32
        MessageBox(NULL,
#else
        fprintf(stdout,
#endif
            "Bombermaaan\n"
            "Copyright (C) 2000-2002, 2007 Thibaut Tollemer\n"
            "Copyright (C) 2007, 2008 Bernd Arnold\n"
            "Copyright (C) 2008 Jerome Bigot\n"
            "Copyright (C) 2008 Markus Drescher\n"
            "Copyright (C) 2016 Billy Araujo\n"
            "\n"
            "Bombermaaan is free software: you can redistribute it and/or modify\n"
            "it under the terms of the GNU General Public License as published by\n"
            "the Free Software Foundation, either version 3 of the License, or\n"
            "(at your option) any later version.\n"
            "\n"
            "Bombermaaan is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU General Public License for more details.\n"
            "\n"
            "You should have received a copy of the GNU General Public License\n"
            "along with Bombermaaan.  If not, see <http://www.gnu.org/licenses/>.\n"
#ifdef WIN32
            , "Bombermaaan", MB_ICONINFORMATION);
#else
            );
#endif
        // Return false so the program will terminate after the message box was closed
        return false;
    }


    //! @see ENABLE_CONSOLE
#ifdef ENABLE_CONSOLE

    // Open the console window
    theConsole.Open ();

#ifndef ENABLE_CONSOLE_REPEATED_MESSAGE_FILTERING

    theConsole.SetFilterRepeatedMessages(false);

#endif

#endif


    // A folder where log file and configuration file are stored.
    // Is %APPDATA%\Bombermaaan when called with --use-appdata-dir (see below) on Windows
    // and $HOME/.Bombermaaan when NOT called with --ignore-home-dir (see below) on Linux.
    std::string dynamicDataFolder;

#ifdef WIN32
    // The "--use-appdata-dir" switch creates config and log file in the user's %APPDATA% directory (Windows)
    bool useAppDataFolder = (strstr(pCommandLine, "--use-appdata-dir") != NULL);
#else
    // In linux do the opposite - it's more useful if we save our data in ~/.Bombermaaan
    bool useAppDataFolder = true;

    for (int i = 0; i < pCommandLineCount; i++)
    {
        if (strncmp(pCommandLine[i], "--ignore-home-dir", 22) == 0)
        {
            useAppDataFolder = false;
            break;
        }
    }
#endif

    std::string pgmDirectory;
#ifdef WIN32
    // Set the current directory to the directory where the Bombermaaan exe file resides
    // __argv[0] is the full path including the exe file name
    // If we append a "\.." to the full path, we get the location where the dll and exe file(s) are placed
    pgmDirectory.append(__argv[0]);
    pgmDirectory.append("\\..");
    SetCurrentDirectory(pgmDirectory.c_str());
#else
    // determine level path by resolving the symlink /proc/self/exe
    // (assume something like /usr/bin/Bombermaaan or /usr/games/Bombermaaan)
    // -> /usr/bin/../share/games/bombermaaan/Levels
    struct stat selftest;
    if (stat("/proc/self/exe", &selftest) == 0)
    {
        char buf[1024];
        char *lastSlash;
        char *beforeLastSlash;
        int bytes = readlink("/proc/self/exe", buf, 1024);

        if (bytes > 0)
        {
            buf[bytes] = '\0';

            // now we have the path of the app, strip app name.
            lastSlash = strrchr(buf, '/');
            if (lastSlash == NULL)
            {
                pgmDirectory.append( "" );
            }
            else if (lastSlash == buf)
            {
                // root directory
                pgmDirectory.append( "/" );
            }
            else
            {
                // find out ../
                char *tempPath = new char [lastSlash - buf + 1];
                if (tempPath == NULL) return false;

                strncpy(tempPath, buf, lastSlash - buf);
                tempPath[lastSlash - buf] = '\0';

                beforeLastSlash = strrchr(tempPath, '/');
                if (beforeLastSlash == NULL || beforeLastSlash == tempPath)
                {
                    pgmDirectory.append( "/" );
                    delete[] tempPath;
                }
                else
                {
                    char *appendPath = new char [beforeLastSlash - tempPath + 2];
                    if (appendPath == NULL) return false;

                    // copy path (include slash)
                    strncpy(appendPath, tempPath, beforeLastSlash - tempPath + 1);
                    appendPath[beforeLastSlash - tempPath + 1] = '\0';
                    pgmDirectory.append( appendPath );
                    delete[] tempPath;
                    delete[] appendPath;
                }
            }
        }
        else
        {
            // assume that we're in the correct working dir
            pgmDirectory.append( "" );
        }
    }
    else
    {
        // assume that we're in the correct working dir
        pgmDirectory.append( "" );
    }

    pgmDirectory.append ( "share/games/bombermaaan" );

    // check for existance
    if (stat(pgmDirectory.c_str(), &selftest) == -1 || !S_ISDIR(selftest.st_mode))
    {
        pgmDirectory.clear(); // use current directory
    }
#endif

    if (useAppDataFolder)
    {
        // Check for the Bombermaaan directory in the appdata folder
#ifdef WIN32
        const char *appDataPath = getenv("APPDATA");
#else
        const char *appDataPath = getenv( "HOME" );
#endif
        if (!appDataPath) {
#ifdef WIN32
            MessageBox(m_hWnd,
                "Could not get the user application folder (%APPDATA%).\nBombermaaan terminates.",
                "Error", MB_OK | MB_ICONERROR);
#else
            fprintf(stderr, "Could not determine home directory ($HOME).\nBombermaaan terminates.\n");
#endif
            return false;
        }

        // Store the Bombermaaan folder name
        dynamicDataFolder = appDataPath;
#ifdef WIN32
        dynamicDataFolder.append("\\Bombermaaan\\");

        // Create the Bombermaaan directory
        if (!CreateDirectory(dynamicDataFolder.c_str(), NULL)) {
            // Exit the game if the folder cannot be created and it doesn't exist already
            if (GetLastError() != ERROR_ALREADY_EXISTS) {
                std::string errorMsg = "Could not create folder '";
                errorMsg.append(dynamicDataFolder);
                errorMsg.append("'.\nBombermaaan cannot run without this folder.");
                MessageBox(m_hWnd,
                    errorMsg.c_str(),
                    "Error", MB_OK | MB_ICONERROR);
                return false;
            }
        }
#else
        dynamicDataFolder.append( "/.Bombermaaan/" );
        struct stat dirStatus;

        if (stat(dynamicDataFolder.c_str(), &dirStatus) == 0 && S_ISDIR(dirStatus.st_mode))
        {
            // dir exists
        }
        else if (mkdir(dynamicDataFolder.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
        {
            fprintf(stderr, "Could not create folder '%s'.\nBombermaaan cannot run without this folder.\n", dynamicDataFolder.c_str());
            return false;
        }
#endif

    }
    else {
        // The current folder
#ifdef WIN32
        dynamicDataFolder = ".\\";
#else
        dynamicDataFolder = "./";
#endif
    }

    //! @see ENABLE_LOG

#ifdef ENABLE_LOG

    std::string logFileName;
    logFileName.append(dynamicDataFolder);
    logFileName.append("log.txt");

    // Open the log file
    theLog.Open(logFileName.c_str());

#endif

    //! @see ENABLE_DEBUG_LOG

#ifdef ENABLE_DEBUG_LOG

    std::string debugLogFileName;
    debugLogFileName.append( dynamicDataFolder );
    debugLogFileName.append( "debug.log" );

    // Open the log file
    debugLog.Open( debugLogFileName.c_str() );

#endif

    // Log date and time of compile and the full path to the exe file
    theLog.WriteLine("Game            => Bombermaaan " APP_VERSION_INFO);
    theLog.WriteLine("Game            => Built at " __TIME__ " on " __DATE__ ".");
    theLog.WriteLine("Game            => Program name: '%s'.",
#ifdef WIN32
        __argv[0]);
#else
        pCommandLine[0] );
#endif

    theDebug.SetGame(this);
    theDebug.SetTimer(&m_Timer);
    theDebug.SetMatch(&m_Match);
    theDebug.Create();

    // If the resource file does not exist
#ifdef WIN32
    if (GetFileAttributes(NAME_OF_BOMBERMAN_DLL) == (DWORD)-1)
    {
        // Failure
        theLog.WriteLine("Game            => !!! Could not find " NAME_OF_BOMBERMAN_DLL ".");

        MessageBox(m_hWnd, "Could not find " NAME_OF_BOMBERMAN_DLL "!", "Error", MB_OK | MB_ICONERROR);

        // Get out
        return false;
    }

    //! Establish connection to the DLL
    m_hModule = LoadLibrary(NAME_OF_BOMBERMAN_DLL);

    // If it failed
    if (m_hModule == NULL)
    {
        // Log error
        theLog.WriteLine("Game            => !!! Could not load " NAME_OF_BOMBERMAN_DLL ".");
        theLog.LogLastError();

        // Get out
        return false;
    }
#endif

#ifdef ALLEGRO

    allegro_init();

    install_timer();
    install_keyboard();
    install_joystick(JOY_TYPE_AUTODETECT);
    install_sound(DIGI_AUTODETECT, MIDI_DETECT, NULL);

    set_color_depth(32);

#endif // ALLEGRO

#ifdef SDL
    #ifdef DIRECTX_VIDEO
        if ((SDL_Init(SDL_INIT_AUDIO) == -1)) // in WIN32 we need AUDIO for SDL_mixer (replacing FMOD)
    #else
        if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) == -1))
    #endif // DIRECTX_VIDEO
        {
            theLog.WriteLine("Game            => !!! Could not initialise SDL library");
            theLog.LogLastError();
            return false;
        }
#endif // SDL

    if (!m_Options.Create(useAppDataFolder, dynamicDataFolder, pgmDirectory))
    {
        // Get out
        return false;
    }

    // Set the objects the input object has to communicate with
#ifdef WIN32
    m_Input.SetInstanceHandle(m_hInstance);
    m_Input.SetWindowHandle(m_hWnd);
#endif

    m_Input.SetOptions(&m_Options);
    m_Input.SetTimer(&m_Timer);

    // Set the objects the display object has to communicate with
#ifdef WIN32
    m_Display.SetWindowHandle(m_hWnd);
    m_Display.SetModuleHandle(m_hModule);
#else
    m_Display.SetModuleHandle (NULL);
#endif

#ifdef ALLEGRO

    set_window_title(m_WindowTitle.c_str());

#else

    #ifndef DIRECTX_VIDEO
        SDL_WM_SetCaption(m_WindowTitle.c_str(), NULL);
    #endif
#endif

    // Set the objects the match object has to communicate with
    m_Match.SetDisplay(&m_Display);
    m_Match.SetInput(&m_Input);
    m_Match.SetOptions(&m_Options);
    m_Match.SetTimer(&m_Timer);
    m_Match.SetScores(&m_Scores);
    m_Match.SetSound(&m_Sound);

#ifdef NETWORK_MODE
    m_Match.SetNetwork(&m_Network);
#endif

    // Set the objects the demo object has to communicate with
    m_Demo.SetDisplay(&m_Display);
    m_Demo.SetInput(&m_Input);
    m_Demo.SetOptions(&m_Options);
    m_Demo.SetTimer(&m_Timer);
    m_Demo.SetScores(&m_Scores);
    m_Demo.SetSound(&m_Sound);

    // Set the objects the draw game object has to communicate with
    m_DrawGame.SetDisplay(&m_Display);
    m_DrawGame.SetInput(&m_Input);
    m_DrawGame.SetTimer(&m_Timer);
    m_DrawGame.SetScores(&m_Scores);
    m_DrawGame.SetOptions(&m_Options);
    m_DrawGame.SetSound(&m_Sound);

    // Set the objects the winner object has to communicate with
    m_Winner.SetDisplay(&m_Display);
    m_Winner.SetInput(&m_Input);
    m_Winner.SetOptions(&m_Options);
    m_Winner.SetTimer(&m_Timer);
    m_Winner.SetScores(&m_Scores);
    m_Winner.SetMatch(&m_Match);
    m_Winner.SetSound(&m_Sound);

    // Set the objects the victory object has to communicate with
    m_Victory.SetDisplay(&m_Display);
    m_Victory.SetInput(&m_Input);
    m_Victory.SetOptions(&m_Options);
    m_Victory.SetTimer(&m_Timer);
    m_Victory.SetScores(&m_Scores);
    m_Victory.SetSound(&m_Sound);

    // Set the objects the scores object has to communicate with
    m_Scores.SetOptions(&m_Options);

    // Set the objects the menu object has to communicate with
    m_Menu.SetDisplay(&m_Display);
    m_Menu.SetInput(&m_Input);
    m_Menu.SetOptions(&m_Options);
    m_Menu.SetTimer(&m_Timer);
    m_Menu.SetSound(&m_Sound);
    m_Menu.SetScores(&m_Scores);

    // Set the objects the title object has to communicate with
    m_Title.SetDisplay(&m_Display);
    m_Title.SetInput(&m_Input);
    m_Title.SetOptions(&m_Options);
    m_Title.SetTimer(&m_Timer);
    m_Title.SetSound(&m_Sound);

    // Set the objects the controls object has to communicate with
    m_Controls.SetDisplay(&m_Display);
    m_Controls.SetInput(&m_Input);
    m_Controls.SetOptions(&m_Options);
    m_Controls.SetTimer(&m_Timer);
    m_Controls.SetSound(&m_Sound);

    // Set the objects the credits object has to communicate with
    m_Credits.SetDisplay(&m_Display);
    m_Credits.SetInput(&m_Input);
    m_Credits.SetOptions(&m_Options);
    m_Credits.SetTimer(&m_Timer);
    m_Credits.SetSound(&m_Sound);

    // Set the objects the victory object has to communicate with
    m_MenuYesNo.SetDisplay(&m_Display);
    m_MenuYesNo.SetInput(&m_Input);
    m_MenuYesNo.SetTimer(&m_Timer);
    m_MenuYesNo.SetSound(&m_Sound);

    // Set the objects the sound object has to communicate with
#ifdef WIN32
    m_Sound.SetModuleHandle(m_hModule);
#else
    m_Sound.SetModuleHandle (NULL);
#endif

    // If creating the display and setting the display mode failed
    if (!m_Display.Create(m_Options.GetDisplayMode()))
    {
        // Get out
        return false;
    }

    // If creating the input failed
    if (!m_Input.Create())
    {
        // Get out
        return false;
    }

    //! @see ENABLE_SOUND
#ifdef ENABLE_SOUND

    // If creating the sound failed
    if (!m_Sound.Create())
    {
        // Get out
        return false;
    }

    m_Sound.SetSongVolume(SONG_MATCH_MUSIC_1_NORMAL, 65);
    m_Sound.SetSongVolume(SONG_MATCH_MUSIC_1_FAST, 65);
    m_Sound.SetSongVolume(SONG_MENU_MUSIC, 65);
    m_Sound.SetSampleVolume(100);

#endif

    m_MenuYesNo.Create();

#ifdef NETWORK_MODE
    char IpAddressString[32];
    const char *pos;

#ifdef WIN32
    pos = strstr(pCommandLine, "--client");
    if (pos != NULL)
    {
        strcpy(IpAddressString, pos + 9);
        OutputDebugString("*** STARTING GAME AS CLIENT\n");
        m_Network.SetNetworkMode(NETWORKMODE_CLIENT);
    }
    else
    {
        pos = strstr(pCommandLine, "-c");
        if (pos != NULL)
        {
            strcpy(IpAddressString, pos + 3);
            OutputDebugString("*** STARTING GAME AS CLIENT\n");
            m_Network.SetNetworkMode(NETWORKMODE_CLIENT);
        }
    }

    pos = strstr(pCommandLine, "--host");
    if (pos != NULL)
    {
        strcpy(IpAddressString, pos + 6);
        OutputDebugString("*** STARTING GAME AS HOST\n");
        m_Network.SetNetworkMode(NETWORKMODE_HOST);
    }
    else
    {
        pos = strstr(pCommandLine, "-h");
        if (pos != NULL)
        {
            strcpy(IpAddressString, pos + 3);
            OutputDebugString("*** STARTING GAME AS HOST\n");
            m_Network.SetNetworkMode(NETWORKMODE_HOST);
        }
    }
#else
    for (int i = 0; i < pCommandLineCount; i++)
    {
        if (strncmp(pCommandLine[i], "-h", 2) == 0 ||
            strncmp(pCommandLine[i], "--host", 6) == 0)
        {
            printf("*** STARTING GAME AS HOST\n");
            m_Network.SetNetworkMode(NETWORKMODE_HOST);
            break;
        }
        else if ((strncmp(pCommandLine[i], "-c", 2) == 0 ||
            strncmp(pCommandLine[i], "--client", 8) == 0) &&
            pCommandLineCount > i + 1)
        {
            printf("*** STARTING GAME AS CLIENT\n");
            m_Network.SetNetworkMode(NETWORKMODE_CLIENT);

            strcpy(IpAddressString, pCommandLine[i+1]);
            break;
        }
    }
#endif

    if (m_Network.NetworkMode() != NETWORKMODE_UNKNOWN)
    {
        if (!m_Network.Connect(IpAddressString, 1234))
        {
            Destroy();
            return false;
        }

        // Set the current game mode
        StartGameMode(GAMEMODE_MATCH);
    }
    else
    {
        // Set the current game mode
        StartGameMode(GAMEMODE_TITLE);
    }

#else

    // Set the current game mode
    StartGameMode(GAMEMODE_TITLE);

#endif

    // Log that initialization is complete
    theLog.WriteLine("Game            => Game initialization is complete!");

    // Leave a blank line between "game loop" stuff and initialization
    theLog.Write("\n");

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CGame::Destroy(void)
{
    // Leave a blank line between "game loop" stuff and shutdown
    theLog.Write("\n");

    // Log that shutdown begins
    theLog.WriteLine("Game            => Game shutdown will now begin.");

    // Terminate game mode and set no game mode
    FinishGameMode();

#ifdef NETWORK_MODE
    m_Network.Disconnect();
#endif

#ifdef ENABLE_SOUND

    m_Sound.Destroy();

#endif

    m_Input.Destroy();
    m_Display.Destroy();

    m_Options.SaveBeforeExit();
    m_Options.Destroy();
    m_MenuYesNo.Destroy();

    theDebug.Destroy();

#ifdef ALLEGRO

    allegro_exit();

#else

    SDL_Quit(); // shutdown SDL library

#endif

    // If there is a connection to the resources
    if (m_hModule != NULL)
    {
        // Close the connection to the resources
#ifdef WIN32
        FreeLibrary(m_hModule);
#endif
        m_hModule = NULL;
    }

#ifdef ENABLE_CONSOLE

    // Close the console window
    theConsole.Close ();

#endif

#ifdef ENABLE_LOG

    // Close the log file
    theLog.Close();

#endif

#ifdef ENABLE_DEBUG_LOG

    // Close the debug log file
    debugLog.Close ();

#endif

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CModeScreen* CGame::GetGameModeObject(EGameMode GameMode)
{
    // According to the specified game mode, return a pointer to the object manager of this mode
    switch (GameMode)
    {
    case GAMEMODE_TITLE:  return &m_Title;    break;
    case GAMEMODE_DEMO:  return &m_Demo;     break;
    case GAMEMODE_MENU:  return &m_Menu;     break;
    case GAMEMODE_MATCH:  return &m_Match;    break;
    case GAMEMODE_WINNER:  return &m_Winner;   break;
    case GAMEMODE_DRAWGAME:  return &m_DrawGame; break;
    case GAMEMODE_VICTORY:  return &m_Victory;  break;
    case GAMEMODE_CONTROLS:  return &m_Controls; break;
    case GAMEMODE_GREETS:  return &m_Credits; break;
    case GAMEMODE_EXIT:  break;
    default:  break;
    }

    // There is no object manager for this game mode
    return NULL;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  This method is called when the window is active.
 */

void CGame::OnWindowActive(void)
{
    // Prepare a game mode variable to save the mode to set
    EGameMode NextGameMode = m_GameMode;

    m_Timer.Update();
    m_Input.GetMainInput().Update();

    //! If the menu yes/no is not active
    if (!m_MenuYesNo.IsActive())
    {
        //! - Update the object corresponding to the current game mode and let it tell the next game mode
        CModeScreen* modeScreen = GetGameModeObject(m_GameMode);
        if (modeScreen != NULL)
            NextGameMode = modeScreen->Update();

    }

    //! If the mode screen object corresponding to the current game mode
    //! is not asking for changing the game mode
    if (NextGameMode == m_GameMode)
    {
        //! - Then let the menu yes/no ask for changing the game mode if needed.
        //! - Manage the menu yes/no and get the appropriate game mode to set
        NextGameMode = m_MenuYesNo.Update(m_GameMode);

        if (NextGameMode == GAMEMODE_TITLE)
            m_Menu.SetMenuMode(MENUMODE_BOMBER);

    }

    //! Make the display black
    m_Display.Clear();

    //! Display the object corresponding to the current game mode
    CModeScreen* modeScreen = GetGameModeObject(m_GameMode);

    if (modeScreen != NULL)
        modeScreen->Display();

    //! Display the menu yes/no if needed
    m_MenuYesNo.Display();

    //! Display everything (CDisplay::Update())
    m_Display.Update();

    //! If the next game mode is different from the current game mode
    if (NextGameMode != m_GameMode)
    {
        //! - Change the game mode
        FinishGameMode();                      //!< @see FinishGameMode()
        StartGameMode(NextGameMode);           //!< @see StartGameMode()
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Set a new game mode. Creates the object corresponding to the new game mode.
 */

void CGame::StartGameMode(EGameMode GameMode)
{
    // Set the new game mode
    m_GameMode = GameMode;

    // If we must exit the game
    if (m_GameMode == GAMEMODE_EXIT)
    {
        // Come back to windowed mode in order to avoid a kind
        // of bug with VC++. Otherwise the VC++ window would be
        // resized to the size of the fullscreen mode that was
        // set when exiting.
        m_Display.Create(DISPLAYMODE_WINDOWED);

#ifdef ALLEGRO

        set_close_button_callback();

#else

        // Close the window
        #ifdef DIRECTX_VIDEO
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        #else
            SDL_Event quitevent;

            quitevent.type = SDL_QUIT;
            quitevent.quit.type = SDL_QUIT;

            SDL_PushEvent(&quitevent);
        #endif
#endif

    }
    // If we don't have to exit the game
    else
    {
        // Create the object corresponding to the new game mode
        CModeScreen* modeScreen = GetGameModeObject(m_GameMode);
        if (modeScreen != NULL)
            modeScreen->Create();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Finish the current game mode
 */

void CGame::FinishGameMode(void)
{
    //! Destroy the object corresponding to the new game mode
    CModeScreen* modeScreen = GetGameModeObject(m_GameMode);
    if (modeScreen != NULL)
        modeScreen->Destroy();

    //! Set no game mode
    m_GameMode = GAMEMODE_NONE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// This is called when the active state of the window
// changes. This is NOT called on window creation.

void CGame::OnActivateApp(WPARAM wParam, LPARAM lParam)
{
#ifdef ENABLE_SOUND
    static bool  soundWasPausedWhenLosingFocus = false;
#endif


    CWindow::OnActivateApp(wParam, lParam);

#ifdef ENABLE_UPDATE_WHEN_WINDOW_IS_INACTIVE
    m_Active = true;
#endif

    // If the window gets the focus
    if (m_Active)
    {
        // Resume the timer
        m_Timer.Resume();

#ifdef ENABLE_SOUND
        // Unpause the sound
        if (soundWasPausedWhenLosingFocus) {
            m_Sound.SetPause(false);
            soundWasPausedWhenLosingFocus = false;
        }
#endif

        // Open the needed players inputs according to current game mode
        // Create the object corresponding to the new game mode
        CModeScreen* modeScreen = GetGameModeObject(m_GameMode);
        if (modeScreen != NULL)
            modeScreen->OpenInput();
    }
    // If the window loses the focus
    else
    {
        // Pause the timer
        m_Timer.Pause();

#ifdef ENABLE_SOUND
        // Pause the sound when it is not already paused (by a pause in a match for example)
        if (!m_Sound.IsPaused()) {
            m_Sound.SetPause(true);
            soundWasPausedWhenLosingFocus = true;
        }
#endif

        // Close the needed players inputs according to current game mode
        // Create the object corresponding to the new game mode
        if (GetGameModeObject(m_GameMode) != NULL)
            GetGameModeObject(m_GameMode)->CloseInput();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called when the window moves.

void CGame::OnMove(WPARAM wParam, LPARAM lParam)
{
    CWindow::OnMove(wParam, lParam);

    // Create the display manage the movement of the window
    m_Display.OnWindowMove();
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// When the window is active and a key is pressed down,
// this method will be called.

void CGame::OnKeyDown(WPARAM wParam, LPARAM lParam)
{


}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When the window is active and a key is released,
// this method will be called.

void CGame::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
#ifdef ENABLE_DEBUG_KEYS

    // Read the key that is released and apply changes to the game if needed (for debugging)
    theDebug.HandleKey(wParam, lParam);

#endif // ENABLE_DEBUG_KEYS

    // If the CTRL key is not pressed while the key specified by wParam is released
#ifdef WIN32
    if (!(GetKeyState(VK_CONTROL) & 0x8000))
#else
    if (!(lParam & KMOD_CTRL))
#endif
    {
        EDisplayMode DisplayMode = DISPLAYMODE_NONE;

        // Assume we have to change the display mode
        bool SetDisplayMode = true;

#ifdef ALLEGRO

        // TODO:

#else

    #ifdef DIRECTX_VIDEO
            //! Change display mode if this is a F1-F4 key
            switch (wParam)
            {
                //! Display modes #1 and #2 are not available in the 32-pixels version
                //! since the screen isn't large enough (so disable F1 and F2 keys)
            case VK_F3: DisplayMode = DISPLAYMODE_FULL3; break;
            case VK_F4: DisplayMode = DISPLAYMODE_WINDOWED; break;
            default: SetDisplayMode = false; break;
            }
    #else
            //! Change display mode if this is a F1-F4 key
            switch (wParam)
            {
                //! Display modes #1 and #2 are not available in the 32-pixels version
                //! since the screen isn't large enough (so disable F1 and F2 keys)
            case SDLK_F3: DisplayMode = DISPLAYMODE_FULL3; break;
            case SDLK_F4: DisplayMode = DISPLAYMODE_WINDOWED; break;
            default: SetDisplayMode = false; break;
            }
    #endif
#endif

        // If we have to change the display mode
        // and the new display mode to set is available on the graphic card
        if (SetDisplayMode && m_Display.IsDisplayModeAvailable(DisplayMode))
        {
            // Set the display mode
            m_Display.Create(DisplayMode);

            // Save it in the options
            m_Options.SetDisplayMode(DisplayMode);
        }

    }
    else {

        //! Quickly exit the game with Ctrl + F12
        if (wParam == VK_F12)
        {
            FinishGameMode();
            StartGameMode(GAMEMODE_EXIT);
        }

    }
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


/**
 *  If the window has to be repainted, the display
 *  must be updated to redraw the game screen.
 */

void CGame::OnPaint(WPARAM wParam, LPARAM lParam)
{
    // Create the display manage this repainting
    //! @see CDisplay::OnPaint()
    m_Display.OnPaint();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Handles system commands
 *
 *  \todo: SCREEN SAVER DISABLING DOES NOT WORK
 **/

bool CGame::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
    // Check what is the system command
#ifdef WIN32
    switch (wParam)
    {
        // If it's a monitor power command or a screen saver execution
    case SC_MONITORPOWER:
    case SC_SCREENSAVE:
    {
        // Do NOT let Windows handle these commands in order
        // to disable these two system commands when the game is running
        return false;
    }
    }
#endif

    // Make Windows handle these commands
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Handles the WM_SIZE message (Sent after the window's size has changed)

void CGame::OnSize(WPARAM wParam, LPARAM lParam)
{
#ifndef WIN32
    // Rework necessary
    // Removed due to B
#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When the window is active and a joystick axis changes,
// this method will be called.
#ifndef DIRECTX_INPUT
void CGame::OnJoystickAxis(WPARAM wParam, LPARAM lParam)
{
    SDL_JoyAxisEvent *jaxis;

    if (wParam != 0) jaxis = (SDL_JoyAxisEvent *)wParam;
    else return;

    CMainInput m_pMainInput = m_Input.GetMainInput();
    CInputSDL *m_pInput = m_pMainInput.GetInput();

    // update main menu input
    if (jaxis->axis == 0) { // X axis
        m_pInput->SetJoystickAxisX(jaxis->which, jaxis->value);
    }
    else if (jaxis->axis == 1) { // Y axis
        m_pInput->SetJoystickAxisY(jaxis->which, jaxis->value);
    }

    m_pMainInput.Update();

    // update player input (for each player with keyboard)
    CPlayerInput m_pPlayerInput;
    int i = NUMBER_OF_KEYBOARD_CONFIGURATIONS + jaxis->which;

    m_pPlayerInput = m_Input.GetPlayerInput(i);
    m_pInput = m_pPlayerInput.GetDirectInput();

    if (jaxis->axis == 0) { // X axis
        m_pInput->SetJoystickAxisX(jaxis->which, jaxis->value);
    }
    else if (jaxis->axis == 1) { // Y axis
        m_pInput->SetJoystickAxisY(jaxis->which, jaxis->value);
    }

    m_pPlayerInput.Update();

    return;
}
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When the window is active and a joystick button is pressed/released,
// this method will be called.

#ifndef DIRECTX_INPUT
void CGame::OnJoystickButton(WPARAM wParam, LPARAM lParam)
{
    SDL_JoyButtonEvent *jbutton;

    if (wParam != 0) jbutton = (SDL_JoyButtonEvent *)wParam;
    else return;

    CMainInput m_pMainInput = m_Input.GetMainInput();

    CInputSDL *m_pDirectInput = m_pMainInput.GetInput();

    // update main menu input
    m_pDirectInput->SetJoystickButton(jbutton->which, jbutton->button,
        (jbutton->state == SDL_PRESSED));
    m_pMainInput.Update();

    // update player input (for each player with keyboard)
    CPlayerInput m_pPlayerInput;
    int i = NUMBER_OF_KEYBOARD_CONFIGURATIONS + jbutton->which;

    m_pPlayerInput = m_Input.GetPlayerInput(i);
    m_pDirectInput = m_pPlayerInput.GetDirectInput();

    m_pDirectInput->SetJoystickButton(jbutton->which, jbutton->button,
        (jbutton->state == SDL_PRESSED));
    m_pPlayerInput.Update();

}
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
