/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer

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
 *  \file CArenaCloser.h
 *  \brief Header file of the arena closer
 */

#ifndef __CARENACLOSER_H__
#define __CARENACLOSER_H__

class CArena;
class CArenaSnapshot;
class COptions;

#include <deque>

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a block position to close when the arena is closing
struct SClosePosition
{                       
    int X;    //!< Block X position
    int Y;    //!< Block Y position
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

typedef std::deque<SClosePosition> TClosure;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! This class manages the closure of an arena.
class CArenaCloser
{
private:

    CArena*         m_pArena;                           //!< Arena to close
    COptions*       m_pOptions;                         //!< Options object where to find the arena closure settings for this match
    TClosure        m_ClosureData;                      //!< Sequence of block positions which have to be closed in this order.
    float           m_TimeBetweenTwoBlockClosures;      //!< How many seconds should elapse after closing a block and before closing the next block?
    float           m_TimeLeftBeforeClosingNextBlock;   //!< Time left in seconds before the next block should start closing
    bool            m_IsClosing;                        //!< Is the arena currently closing?

    void            CreateSpiralClosing (void);         //!< Prepare the closure data for a spiral arena closure
    void            CreateHorizontalClosing (void);     //!< Prepare the closure data for an horizontal arena closure
    void            CreateVerticalClosing (void);       //!< Prepare the closure data for a vertical arena closure
    
public:
    
                    CArenaCloser (void);                        //!< Constructor
                    ~CArenaCloser (void);                       //!< Destructor
    inline void     SetArena (CArena *pArena);                  //!< Set the arena to close
    inline void     SetOptions (COptions *pOptions);            //!< Set the options object to use
    void            Create (void);                              //!< Initialize the arena closer (this doesn't start closure)
    void            Destroy (void);                             //!< Uninitialize the arena closer (frees any allocated memory)
    void            Start (void);                               //!< Start to close the arena
    void            Stop (void);                                //!< Stop closing the arena
    void            Update (float DeltaTime);                   //!< Close the arena if closure has started
    void            WriteSnapshot (CArenaSnapshot& Snapshot);
    void            ReadSnapshot (CArenaSnapshot& Snapshot);
    inline bool     IsClosing (void);                           //!< Is the arena currently closing?
    inline int      GetNextBlockPositionX (int Index);          //!< Get the X position (in blocks) of the block that will be closed next
    inline int      GetNextBlockPositionY (int Index);          //!< Get the Y position (in blocks) of the block that will be closed next
    inline int      GetNumberOfBlocksLeft (void);               //!< Get the number of blocks the arena closer still has to close.
    inline float    GetTimeBetweenTwoBlockClosures (void);      //!< Get the time (in seconds) that elapses between the closure of one block and the closure of the next block
    inline float    GetTimeLeftBeforeClosingNextBlock (void);   //!< Get the time left (in seconds) before the next block should start closing
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CArenaCloser::SetArena (CArena *pArena)
{
    m_pArena = pArena;
}

inline void CArenaCloser::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
}

inline bool CArenaCloser::IsClosing (void)
{
    return m_IsClosing;
}

inline int CArenaCloser::GetNextBlockPositionX (int Index)
{
    return m_ClosureData[Index].X;
}

inline int CArenaCloser::GetNextBlockPositionY (int Index)
{
    return m_ClosureData[Index].Y;
}

inline int CArenaCloser::GetNumberOfBlocksLeft (void)
{
    return m_ClosureData.size();
}

inline float CArenaCloser::GetTimeBetweenTwoBlockClosures (void)
{
    return m_TimeBetweenTwoBlockClosures;
}

inline float CArenaCloser::GetTimeLeftBeforeClosingNextBlock (void)
{
    return m_TimeLeftBeforeClosingNextBlock;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif //__CARENACLOSER_H__
