/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007 Bernd Arnold

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
 *  \file CSound.h
 *  \brief Header file of the sound
 */

#ifndef __CSOUND_H__
#define __CSOUND_H__

#ifdef ALLEGRO
    #include "allegro.h"
    #include "winalleg.h"
#else
    #include "SDL.h"
    #include "SDL_mixer.h"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum ESample
{
    SAMPLE_BOMB_DROP = 0,
    SAMPLE_BOMBER_DEATH,
    SAMPLE_BOMB_BOUNCE,
    SAMPLE_BOMBER_PUNCH,
    SAMPLE_BOMBER_THROW,
    SAMPLE_BOMBER_LOSE_ITEM,
    SAMPLE_BREAK_1,
    SAMPLE_BREAK_2,
    SAMPLE_DRAW_GAME,
    SAMPLE_DRAW_GAME_VOICE,
    SAMPLE_EXPLOSION_01_1,
    SAMPLE_EXPLOSION_01_2,
    SAMPLE_EXPLOSION_02_1,
    SAMPLE_EXPLOSION_02_2,
    SAMPLE_EXPLOSION_03_1,
    SAMPLE_EXPLOSION_03_2,
    SAMPLE_EXPLOSION_04_1,
    SAMPLE_EXPLOSION_04_2,
    SAMPLE_EXPLOSION_05_1,
    SAMPLE_EXPLOSION_05_2,
    SAMPLE_EXPLOSION_06_1,
    SAMPLE_EXPLOSION_06_2,
    SAMPLE_EXPLOSION_07_1,
    SAMPLE_EXPLOSION_07_2,
    SAMPLE_EXPLOSION_08_1,
    SAMPLE_EXPLOSION_08_2,
    SAMPLE_EXPLOSION_09_1,
    SAMPLE_EXPLOSION_09_2,
    SAMPLE_EXPLOSION_10_1,
    SAMPLE_EXPLOSION_10_2,
    SAMPLE_HURRY,
    SAMPLE_ITEM_FUMES,
    SAMPLE_MENU_NEXT,
    SAMPLE_MENU_PREVIOUS,
    SAMPLE_MENU_BEEP,
    SAMPLE_MENU_ERROR,
    SAMPLE_PAUSE,
    SAMPLE_PICK_ITEM_1,
    SAMPLE_PICK_ITEM_2,
    SAMPLE_RING_DING,
    SAMPLE_SICK_1,
    SAMPLE_SICK_2,
    SAMPLE_SICK_3,
    SAMPLE_VICTORY,
    SAMPLE_VICTORY_VOICE,
    SAMPLE_WALL_CLAP_1,
    SAMPLE_WALL_CLAP_2,
    SAMPLE_WINNER,
    NUM_SAMPLES
};

enum ESong
{
    SONG_NONE = -1,
    SONG_MATCH_MUSIC_1_NORMAL = 0,
    SONG_MATCH_MUSIC_1_FAST,
    SONG_MENU_MUSIC,
    SONG_CONTROLS_MUSIC = 3,
    SONG_GREET_MUSIC = SONG_CONTROLS_MUSIC,
    SONG_TITLE_MUSIC,
    NUM_SONGS
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! CSound handles the songs and samples
class CSound
{
private:

    HMODULE m_hModule;                          //!< Connection to the resources
    bool m_GlobalPause;                         //!< Is the sound paused?
    bool m_SoundOK;                             //!< Could SDL_mixer be initialized? This may be false if there is no sound card
    Mix_Chunk *m_Samples[NUM_SAMPLES];          //!< The available samples
    Mix_Music *m_CurrentSong;                   //!< The current song
    ESong m_ESong;                              //!< current song number

    bool GetSoundResource(int ResourceID, LPVOID &pData, DWORD &DataSize);
    bool LoadSample(ESample Sample, int ResourceID, const char *file);

    void FreeSample(ESample Sample);
    bool LoadSong(ESong Song, int ResourceID, const char* file);
    void FreeSong(ESong Song);

public:

    CSound(void);
    ~CSound(void);

    inline void SetModuleHandle(HMODULE hModule);             //!< Set the connection to the resources
    bool        Create(void);                                 //!< Initialize the object
    void        Destroy(void);                                //!< Uninitialize the object
    void        SetPause(bool Pause);                         //!< Pause or resume the sound
    inline bool IsPaused(void);
    void        PlaySong(ESong Song);                         //!< Start playing a song
    void        StopSong(ESong Song);                         //!< Stop playing a specific song
    void        PlaySample(ESample Sample);                   //!< Start playing a sample
    void        StopAllSamples();                              //!< Stops all samples
    void        SetSampleVolume(int VolumePerCent);           //!< Set the volume for all samples
    void        SetSongVolume(ESong Song, int VolumePerCent); //!< Set the volume for a specific song
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CSound::SetModuleHandle(HMODULE hModule)
{
    m_hModule = hModule;
}

inline bool CSound::IsPaused(void)
{
    return m_GlobalPause;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CSOUND_H__
