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
 *  \file CMenuInput.h
 *  \brief Header file of the menu input
 */

#ifndef __CMENUINPUT_H__
#define __CMENUINPUT_H__

#include "CMenuBase.h"

class CMenuInput : public CMenuBase
{
private:
    int m_CursorPlayer; //!< Player the cursor hand is currently pointing to

    void OnCreate();
    void OnDestroy();
    void OnUp();
    void OnDown();
    void OnLeft();
    void OnRight();
    void OnPrevious();
    void OnNext();
    void OnUpdate();
    void OnDisplay();

public:
    CMenuInput();
    virtual ~CMenuInput();
};

#endif // __CMENUINPUT_H__
