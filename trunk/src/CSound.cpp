// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007 Bernd Arnold
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
 *  \file CSound.cpp
 *  \brief Sound
 */

#include "StdAfx.h"
#include "CSound.h"

#include "res.h"

// now using SDL_Mixer library
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CSound::CSound(void)
{
    // No connection to the resources yet
    m_hModule = NULL;

    // Sound is unpaused
    m_GlobalPause = false;

    // Reset the sample and song pointers
    int i;

    for (i = 0; i < NUM_SAMPLES; i++) {
        m_Samples[i] = NULL;
    }

    m_CurrentSong = NULL;
    m_ESong = SONG_NONE;

    m_SoundOK = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CSound::~CSound(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CSound::Create(void)
{

    if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0) {
        // failure
        theLog.WriteLine("Sound           => !!! Could not initialise SDL_mixer Library. Error is : %s", Mix_GetError());

        // Get out
        return false;
    }

    Mix_Volume(-1, MIX_MAX_VOLUME);

    Mix_AllocateChannels(32); // this was the default in FMOD

    if (!LoadSample(SAMPLE_BOMB_DROP, SND_BOMB_DROP, "bomb_drop.ogg") ||
        !LoadSample(SAMPLE_BOMBER_DEATH, SND_BOMBER_DEATH, "bomber_death.ogg") ||
        !LoadSample(SAMPLE_BOMB_BOUNCE, SND_BOMB_BOUNCE, "bomb_bounce.ogg") ||
        !LoadSample(SAMPLE_BOMBER_PUNCH, SND_BOMBER_PUNCH, "bomber_punch.ogg") ||
        !LoadSample(SAMPLE_BOMBER_THROW, SND_BOMBER_THROW, "bomber_throw.ogg") ||
        !LoadSample(SAMPLE_BOMBER_LOSE_ITEM, SND_BOMBER_LOSE_ITEM, "bomber_lose_item.ogg") ||
        !LoadSample(SAMPLE_BREAK_1, SND_BREAK_1, "break_1.ogg") ||
        !LoadSample(SAMPLE_BREAK_2, SND_BREAK_2, "break_2.ogg") ||
        !LoadSample(SAMPLE_DRAW_GAME, SND_DRAW_GAME, "draw_game.ogg") ||
        !LoadSample(SAMPLE_DRAW_GAME_VOICE, SND_DRAW_GAME_VOICE, "draw_game_voice.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_01_1, SND_EXPLOSION_01_1, "explosion_01_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_01_2, SND_EXPLOSION_01_2, "explosion_01_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_02_1, SND_EXPLOSION_02_1, "explosion_02_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_02_2, SND_EXPLOSION_02_2, "explosion_02_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_03_1, SND_EXPLOSION_03_1, "explosion_03_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_03_2, SND_EXPLOSION_03_2, "explosion_03_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_04_1, SND_EXPLOSION_04_1, "explosion_04_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_04_2, SND_EXPLOSION_04_2, "explosion_04_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_05_1, SND_EXPLOSION_05_1, "explosion_05_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_05_2, SND_EXPLOSION_05_2, "explosion_05_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_06_1, SND_EXPLOSION_06_1, "explosion_06_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_06_2, SND_EXPLOSION_06_2, "explosion_06_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_07_1, SND_EXPLOSION_07_1, "explosion_07_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_07_2, SND_EXPLOSION_07_2, "explosion_07_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_08_1, SND_EXPLOSION_08_1, "explosion_08_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_08_2, SND_EXPLOSION_08_2, "explosion_08_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_09_1, SND_EXPLOSION_09_1, "explosion_09_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_09_2, SND_EXPLOSION_09_2, "explosion_09_2.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_10_1, SND_EXPLOSION_10_1, "explosion_10_1.ogg") ||
        !LoadSample(SAMPLE_EXPLOSION_10_2, SND_EXPLOSION_10_2, "explosion_10_2.ogg") ||
        !LoadSample(SAMPLE_HURRY, SND_HURRY, "hurry.ogg") ||
        !LoadSample(SAMPLE_ITEM_FUMES, SND_ITEM_FUMES, "item_fumes.ogg") ||
        !LoadSample(SAMPLE_MENU_NEXT, SND_MENU_NEXT, "menu_next.ogg") ||
        !LoadSample(SAMPLE_MENU_PREVIOUS, SND_MENU_PREVIOUS, "menu_previous.ogg") ||
        !LoadSample(SAMPLE_MENU_BEEP, SND_MENU_BEEP, "menu_beep.ogg") ||
        !LoadSample(SAMPLE_MENU_ERROR, SND_MENU_ERROR, "menu_error.ogg") ||
        !LoadSample(SAMPLE_PAUSE, SND_PAUSE, "pause.ogg") ||
        !LoadSample(SAMPLE_PICK_ITEM_1, SND_PICK_ITEM_1, "pick_item_1.ogg") ||
        !LoadSample(SAMPLE_PICK_ITEM_2, SND_PICK_ITEM_2, "pick_item_2.ogg") ||
        !LoadSample(SAMPLE_RING_DING, SND_RING_DING, "ring_ding.ogg") ||
        !LoadSample(SAMPLE_SICK_1, SND_SICK_1, "sick_1.ogg") ||
        !LoadSample(SAMPLE_SICK_2, SND_SICK_2, "sick_2.ogg") ||
        !LoadSample(SAMPLE_SICK_3, SND_SICK_3, "sick_3.ogg") ||
        !LoadSample(SAMPLE_VICTORY, SND_VICTORY, "victory.ogg") ||
        !LoadSample(SAMPLE_VICTORY_VOICE, SND_VICTORY_VOICE, "victory_voice.ogg") ||
        !LoadSample(SAMPLE_WALL_CLAP_1, SND_WALL_CLAP_1, "wall_clap_1.ogg") ||
        !LoadSample(SAMPLE_WALL_CLAP_2, SND_WALL_CLAP_2, "wall_clap_2.ogg") ||
        !LoadSample(SAMPLE_WINNER, SND_WINNER, "winner.ogg"))
    { // songs are loaded when they are needed
      // Failure, get out (error is logged by the LoadSample() and LoadSong() methods)
        return false;
    }

    m_SoundOK = true;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::Destroy(void)
{
    int i;

    // If the sound works
    if (m_SoundOK)
    {
        // halt playback on all channels
        Mix_HaltChannel(-1);

        // Stop and free all samples that are playing
        for (i = 0; i < NUM_SAMPLES; i++)
        {
            if (m_Samples[i] != NULL)
            {
                Mix_FreeChunk(m_Samples[i]);

                // Free the sample slot
                m_Samples[i] = NULL;
            }
        }

        // Free the current song
        if (m_CurrentSong != NULL)
        {
            Mix_FreeMusic(m_CurrentSong);

            // Free the song slot
            m_CurrentSong = NULL;
        }
    }

    Mix_CloseAudio();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CSound::LoadSample(ESample Sample, int ResourceID, const char *file)
{

#ifndef LOAD_RESOURCES_FROM_FILES

    SDL_RWops *rwSample;

    // Check if the sample slot is free
    ASSERT(m_Samples[Sample] == NULL);

    LPVOID pData;
    DWORD DataSize;

    // If we could not get the sound resource information
    if (!GetSoundResource(ResourceID, pData, DataSize))
    {
        // Get out
        return false;
    }

    // Convert pData to SDL_RWops
    rwSample = SDL_RWFromMem(pData, DataSize);

    // Open Sample
    m_Samples[Sample] = Mix_LoadWAV_RW(rwSample, 0);
    SDL_FreeRW(rwSample);

#else

    // Check if the sample slot is free
    ASSERT(m_Samples[Sample] == NULL);

    std::string path(SOUND_FOLDER);

#ifdef WIN32
    path.append("\\");
#else
    path.append("/");
#endif

    path.append(file);

    m_Samples[Sample] = Mix_LoadWAV(path.c_str());

#endif

    if (!m_Samples[Sample])
    {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not open sample %d because %s", ResourceID, Mix_GetError());

        // Get out
        return false;
    }

    // Everything went right
    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::FreeSample(ESample Sample)
{
    // If the sample slot is not free
    if (m_Samples[Sample] != NULL)
    {
        // halt playback on all channels
        Mix_HaltChannel(-1);

        // free sample
        Mix_FreeChunk(m_Samples[Sample]);

        // Free the sample slot
        m_Samples[Sample] = NULL;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CSound::LoadSong(ESong Song, int ResourceID, const char* file)
{

    // Check if the song slot is free
    ASSERT(m_CurrentSong == NULL);

#ifndef LOAD_RESOURCES_FROM_FILES

    SDL_RWops *rwSong;

    LPVOID pData;
    DWORD DataSize;

    // If we could not get the sound resource information
    if (!GetSoundResource(ResourceID, pData, DataSize))
    {
        // Get out
        return false;
    }

    // Convert pData to SDL_RWops
    rwSong = SDL_RWFromMem(pData, DataSize);

    // Open Sample
    m_CurrentSong = Mix_LoadMUS_RW(rwSong);
    SDL_FreeRW(rwSong);

#else

    std::string path(SOUND_FOLDER);

#ifdef WIN32
    path.append("\\");
#else
    path.append("/");
#endif

    path.append(file);

    // Open Sample
    m_CurrentSong = Mix_LoadMUS(path.c_str());

#endif

    if (!m_CurrentSong) {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not load song %d because %s.", ResourceID, Mix_GetError());

        // Get out
        return false;
    }

    // Everything went right
    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::FreeSong(ESong Song)
{
    // If the song slot is not free
    if (m_CurrentSong != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(m_CurrentSong);

        // Free the song slot
        m_CurrentSong = NULL;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CSound::GetSoundResource(int ResourceID, LPVOID &pData, DWORD &DataSize)
{
    // Find the resource file
    HRSRC hResource = FindResource(m_hModule, MAKEINTRESOURCE(ResourceID), "SOUND");

    // If it failed
    if (hResource == NULL)
    {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not find resource.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Load the resource file into memory
    HGLOBAL hGlobal = LoadResource(m_hModule, hResource);

    // If it failed
    if (hGlobal == NULL)
    {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not load resource.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Get the address of the resource file in memory
    pData = LockResource(hGlobal);

    // If it failed
    if (pData == NULL)
    {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not lock resource.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Get the size (in bytes) of the PNG resource file
    DataSize = SizeofResource(m_hModule, hResource);

    // If it failed
    if (DataSize == 0)
    {
        // Log failure
        theLog.WriteLine("Sound           => !!! Could not get resource size.");
        theLog.LogLastError();

        // Get out
        return false;
    }

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::SetPause(bool Pause)
{
    // If the sound works
    if (m_SoundOK)
    {
        if (Pause)
        {
            Mix_PauseMusic();
        }
        else
        {
            Mix_ResumeMusic();
        }

        m_GlobalPause = Pause;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::PlaySong(ESong Song)
{
    bool result = true;
    int VolumePerCent = 25;

    // If the sound works
    if (m_SoundOK)
    {
        // If a song exists free it unless it is the same
        if (m_CurrentSong != NULL && m_ESong != Song) FreeSong(Song);

        // note: songs are loaded dynamically because of an error in libmikmod (used by SDL_mixer)
        // load new song (if necessary)
        if (m_ESong != Song || m_CurrentSong == NULL) {
            switch (Song) {
            case SONG_MATCH_MUSIC_1_NORMAL:
                result = LoadSong(SONG_MATCH_MUSIC_1_NORMAL, SND_MATCH_MUSIC_1_NORMAL, "match_music_1_normal.mod");
                break;
            case SONG_MATCH_MUSIC_1_FAST:
                result = LoadSong(SONG_MATCH_MUSIC_1_FAST, SND_MATCH_MUSIC_1_FAST, "match_music_1_fast.mod");
                break;
            case SONG_MENU_MUSIC:
                result = LoadSong(SONG_MENU_MUSIC, SND_MENU_MUSIC, "menu_music.s3m");
                break;
            case SONG_CONTROLS_MUSIC:
                result = LoadSong(SONG_CONTROLS_MUSIC, SND_CONTROLS_MUSIC, "controls_music.s3m");
                break;
            case SONG_TITLE_MUSIC:
                result = LoadSong(SONG_TITLE_MUSIC, SND_TITLE_MUSIC, "title_music.s3m");
                break;
            default:
                result = false;
            }
        }

        if (result)
        {
            // Start playing this song (-1 infinite loop)
            Mix_PlayMusic(m_CurrentSong, -1);
            Mix_VolumeMusic(VolumePerCent * MIX_MAX_VOLUME / 100);

            m_ESong = Song;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::StopSong(ESong Song)
{
    // If the sound works
    if (m_SoundOK)
    {
        // If the song exists
        if (m_CurrentSong != NULL)
        {
            // Stop playing current song (we don't know which one is playing)
            FreeSong(m_ESong);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::PlaySample(ESample Sample)
{
    // If the sound works
    if (m_SoundOK)
    {
        // If the sample exists
        if (m_Samples[Sample] != NULL)
        {
            // Start playing this sample
            Mix_PlayChannel(-1, m_Samples[Sample], 0);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::StopAllSamples()
{
    // If the sound works
    if (m_SoundOK)
    {
        // Halt all channels
        Mix_HaltChannel(-1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::SetSampleVolume(int VolumePerCent)
{
    // If the sound works
    if (m_SoundOK)
    {
        // Set the volume of all samples
        Mix_Volume(-1, VolumePerCent * MIX_MAX_VOLUME / 100);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSound::SetSongVolume(ESong Song, int VolumePerCent)
{
    // If the sound works
    if (m_SoundOK)
    {
        // If this song exists
        if (m_CurrentSong != NULL)
        {
            Mix_VolumeMusic(VolumePerCent * MIX_MAX_VOLUME / 100);
        }
    }
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
