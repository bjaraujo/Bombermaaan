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
 *  \file CElement.h
 *  \brief An element in the arena
 */

#ifndef __CELEMENT_H__
#define __CELEMENT_H__

class CDisplay;
class CSound;
class CArena;
class CArenaSnapshot;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The base class for every element of the arena.
class CElement
{
private:

    bool                m_Exist;                            //!< Does the element exist? (is it created?)
                                                            
protected:                                                  
                                                            
    CArena*             m_pArena;                           //!< Link to the parent arena in which this element is
    CDisplay*           m_pDisplay;                         //!< Link to the display object to use
    CSound*             m_pSound;                           //!< Link to the sound object to use
                                                            
    void                Create (void);                      //!< Initialize the element. You need to call this on creation ( Create() )of the inherited class.
    void                Destroy (void);                     //!< Uninitialize the element. You need to call this on destruction ( Destroy() ) of the inherited class.
    virtual void        OnWriteSnapshot (CArenaSnapshot& Snapshot) = 0;
    virtual void        OnReadSnapshot (CArenaSnapshot& Snapshot) = 0;
                                                            
public:                                                     
                                                            
                        CElement();                         //!< Base constructor. Some member initializations
    virtual             ~CElement();                        //!< Base destructor. Does nothing.
    virtual inline void SetArena (CArena *pArena);          //!< Set link to the parent arena in which this element is
    virtual inline void SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    virtual inline void SetSound (CSound *pSound);          //!< Set link to the sound object to use
    virtual bool        Update (float DeltaTime) = 0;       //!< Update the element. Return whether the element should be deleted by the arena.
    virtual void        Display (void) = 0;                 //!< Display the element.
    void                WriteSnapshot (CArenaSnapshot& Snapshot);
    void                ReadSnapshot (CArenaSnapshot& Snapshot);
    inline bool         Exist (void);                       //!< Return whether the element exists (i.e. should be updated and displayed)
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CElement::SetArena (CArena *pArena)
{
    m_pArena = pArena;
}

inline void CElement::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

inline void CElement::SetSound (CSound *pSound)
{
    m_pSound = pSound;
}

inline bool CElement::Exist (void)
{
    return m_Exist;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  //!< __CELEMENT_H__
