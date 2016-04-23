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
 *  \file CWindow.cpp
 *  \brief Implementation of the CWindow class
 */

#include "STDAFX.H"
#include "CWindow.h"

#ifndef WIN32
#include "SDL.h"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Default WinProc
// HackHackHack!!! We have to do this because the real WinProc is in the class. This is very very hard to 
// explain. Let's define two memory places where you can store a CWindow* :
// M1 --> We pass our 'this' pointer in this during CreateWindowEx(). When the WM_CREATE message will be received 
//        in DefaultWinProc, M1 will be in a member of the CREATESTRUCT (lParam contains the CREATESTRUCT).
// M2 --> This is a memory place you can access by using GetWindowLong (or Set...). When M1 is accessible, we will 
//        store M1 in M2, as M1 is permanent (until the window's destruction) and M2 is temporary (we can only get 
//        it when WM_CREATE is received).
// This hack was found in the source code of the Network tutorial of Dan Royer.
// Dan Royer's homepage : http://members.home.com/droyer
// Network tutorial found on Flipcode : http://www.flipcode.com/network
#ifdef WIN32
static LRESULT CALLBACK DefaultWinProc (HWND hwnd, unsigned int msg, WPARAM wParam, LPARAM lParam) 
{
    LPCREATESTRUCT lpcs;
    CWindow *pWindow;

    pWindow = (CWindow *)GetWindowLong (hwnd, 0);  // Pointer to the (C++ object that is the) window.
    if (!pWindow)       // Not stored yet ?
    {
        if (msg == WM_CREATE)       // The window is being created !! lParam contains the CreateStruct.
        {
            lpcs = (LPCREATESTRUCT)lParam;
            pWindow = (CWindow *)lpcs->lpCreateParams;      // Get the CWindow * we stored during CreateWindowEx
            SetWindowLong (hwnd, 0, (LONG)pWindow);

            return pWindow->WinProc (msg, wParam, lParam);  // Call its WinProc
        } 
        else return DefWindowProc( hwnd, msg, wParam, lParam );  // this should never be called
    } 
    else    // We got a CWindow * using GetWindowLong so it was already stored
    {
        return pWindow->WinProc(msg, wParam, lParam );
    }
}
#endif



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


CWindow::CWindow (HINSTANCE hInstance, const char *pWindowTitle, int IconResourceID)
{
    m_hWnd = NULL;
    m_Active = false;

    // Init the window class
#ifdef WIN32
    WNDCLASSEX WndClassEx;
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.lpszClassName = "Class name";
    WndClassEx.lpfnWndProc = DefaultWinProc;        // Biiiig Hack. See the function DefaultWinProc
    WndClassEx.style = CS_VREDRAW | CS_HREDRAW;
    WndClassEx.hInstance = hInstance;
    WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClassEx.hIconSm = NULL;
    WndClassEx.hCursor = LoadCursor (NULL, IDC_ARROW);
    WndClassEx.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    WndClassEx.lpszMenuName = NULL;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = sizeof(CWindow *);  // We'll store the 'this' pointer in the extra allocated bytes

    // Prepare the window's styles (default ones)
    DWORD Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX/* | WS_MINIMIZEBOX*/;
    DWORD ExStyle = 0;

    // If registering the window class failed
    if (!RegisterClassEx (&WndClassEx))
    {
        // Log failure
        theLog.WriteLine ("Window          => !!! Could not register window class.");
    }

    // Create the window
    m_hWnd = CreateWindowEx (ExStyle,           // Extended style
                             "Class name",      // ClassName
                             pWindowTitle,      // Title
                             Style,             // Style
                             CW_USEDEFAULT,     // Position
                             CW_USEDEFAULT,     
                             CW_USEDEFAULT,     // Size
                             CW_USEDEFAULT,     
                             NULL,              // Parent window
                             NULL,              // Menu
                             hInstance,         // Handle to instance
                             this);             // Pointer to window creation data 
                                                // (Allows us to store the 'this' pointer)

    // If it failed
    if (m_hWnd == NULL)
    {
        // Log failure
        theLog.WriteLine ("Window          => !!! Could not create the window.");
    }

    // If an icon was specified
    if (IconResourceID != -1)
    {
        // Check if the resource ID seems to be valid
        ASSERT (IconResourceID >= 0);
        
        // Load the icon specified by the resource ID
        HICON hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_BOMBER));
        ASSERT (hIcon != NULL);

        // Tell the window to set this icon
        PostMessage (m_hWnd, WM_SETICON, (WPARAM) ICON_BIG, (LPARAM) hIcon);
    }
#endif
    // the icon in Linux is loaded in CSDLVideo
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



CWindow::~CWindow ()
{
    // If the window exists
#ifdef WIN32
    if (m_hWnd != NULL)
    {
        // Destroy the window
        DestroyWindow (m_hWnd);
        m_hWnd = NULL;
    }
#endif
}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CWindow::SetClientSize (int ClientWidth, int ClientHeight)
{
#ifdef WIN32
    RECT rc;
    SetRect (&rc, 0, 0, ClientWidth, ClientHeight);
    AdjustWindowRectEx (&rc, GetWindowStyle(m_hWnd), (int)GetMenu (m_hWnd), GetWindowExStyle (m_hWnd));
    SetWindowPos (m_hWnd, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);
#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************




#ifdef WIN32
LRESULT CALLBACK CWindow::WinProc (unsigned int msg, WPARAM wParam, LPARAM lParam) 
#else
void CWindow::WinProc (unsigned int msg, WPARAM wParam, LPARAM lParam)
#endif
{
    switch (msg) 
    {
        case WM_CREATE:         OnCreate (m_hWnd, wParam, lParam);  break;
        case WM_INITDIALOG:     OnInitDialog (wParam, lParam);      break;
        case WM_ACTIVATEAPP:    OnActivateApp (wParam, lParam);     break;
        case WM_SIZE:           OnSize (wParam, lParam);            break;
        case WM_MOVE:           OnMove (wParam, lParam);            break;
        case WM_PAINT:          OnPaint (wParam, lParam);           break;
        case WM_KEYDOWN:        OnKeyDown (wParam, lParam);         break;
        case WM_KEYUP:          OnKeyUp (wParam, lParam);           break;
        case WM_COMMAND:        OnCommand (wParam, lParam);         break;
        case WM_SYSCOMMAND:     OnSysCommand (wParam, lParam);      break;
        case WM_CLOSE:          OnClose (wParam, lParam);           break;
        case WM_DESTROY:        OnDestroy (wParam, lParam);         break;
#ifndef WIN32
        case SDL_JOYAXISMOTION: OnJoystickAxis(wParam, lParam);		break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:	OnJoystickButton(wParam, lParam);	break;
#endif
    }

#ifdef WIN32
        return DefWindowProc (m_hWnd, msg, wParam, lParam);
#else
        return;
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CWindow::ShowWindow () 
{ 
#ifdef WIN32
    ::ShowWindow (m_hWnd, SW_SHOW); 
    UpdateWindow (m_hWnd);
#endif
}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// The Message Loop. It pumps the messages, manages them. If the window has some
// idle time, it calls OnWindowActive().

void CWindow::MessagePump ()
{
#ifdef WIN32
    MSG msg;

    while (true)
    {
        // Manage the messages if some are waiting
        if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage (&msg, NULL, 0, 0 ))
            {
                m_hWnd = NULL;
                break;
            }

            TranslateMessage (&msg); 
            DispatchMessage (&msg);
        }
        // No message to manage ?
        else
        {           
            if (m_Active)
            {
                // call the virtual activity method
                OnWindowActive ();
            }
            else
            {
                // make sure we go to sleep if we have nothing else to do
                WaitMessage();
            }
        }
    }
#else
    SDL_Event event;
    bool quit = false;

    WinProc(WM_ACTIVATEAPP, WA_ACTIVE, 0);

    // Start main game loop here
    while (!quit) {
        while(SDL_PollEvent(&event))
        {  
            switch(event.type)
            {  
                case SDL_KEYDOWN:
                    WinProc(WM_KEYDOWN, event.key.keysym.sym, event.key.keysym.mod);
                    break;

                case SDL_KEYUP:
                    WinProc(WM_KEYUP, event.key.keysym.sym, event.key.keysym.mod);
                    break;

                case SDL_JOYAXISMOTION:  // Handle Joystick Motion
                    WinProc(SDL_JOYAXISMOTION, (WPARAM)&event.jaxis, (LPARAM)&event);
                    break;

                case SDL_JOYBUTTONDOWN:  // Handle Joystick buttons
                case SDL_JOYBUTTONUP:
                    WinProc(event.type, (WPARAM)&event.jbutton, (LPARAM)&event);
                    break;

                case SDL_VIDEORESIZE:	 // resize our window
                    WinProc(WM_SIZE, (WPARAM)&event.resize, (LPARAM)&event);
                    break;

                case SDL_ACTIVEEVENT:
                    if (event.active.state != SDL_APPMOUSEFOCUS)
                    {
                        if (event.active.gain)
                            WinProc(WM_ACTIVATEAPP, WA_ACTIVE, 0);
                        else
                            WinProc(WM_ACTIVATEAPP, 0, 0);
                    }
                    break;
                case SDL_QUIT:
                    WinProc(WM_CLOSE, (WPARAM)&event, 0);
                    quit = true;
                    break;
            }
        }

        if (m_Active)
        {
	        // call the virtual activity method
	        OnWindowActive ();
	        SDL_Delay(1); // rest for the cpu
        }
    }
#endif
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



void CWindow::OnWindowActive ()
{
    // Nothing by default
}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_CREATE message (Sent once after window creation)

void CWindow::OnCreate (HWND hwnd, WPARAM wParam, LPARAM lParam) 
{ 
    
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_INITDIALOG message (Sent once before a dialog box is displayed)

void CWindow::OnInitDialog (WPARAM wParam, LPARAM lParam) 
{ 
    
}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Handles the WM_ACTIVATEAPP message  (Sent when a window belonging to a different 
// application than the active window is about to be activated)

void CWindow::OnActivateApp (WPARAM wParam, LPARAM lParam) 
{ 
    // Pause if minimized or not the top window
    m_Active = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Handles the WM_SIZE message (Sent after the window's size has changed)

void CWindow::OnSize (WPARAM wParam, LPARAM lParam) 
{ 
    // Check to see if we are losing our window...
#ifdef WIN32
    m_Active = (wParam != SIZE_MAXHIDE && wParam != SIZE_MINIMIZED);
#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************




// Handles the WM_MOVE message (Sent after the window has been moved)

void CWindow::OnMove (WPARAM wParam, LPARAM lParam) 
{ 

}




//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


// Handles the WM_PAINT message (Sent when Windows or another application makes a 
// request to paint a portion of the application's window)

void CWindow::OnPaint (WPARAM wParam, LPARAM lParam) 
{ 

}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_KEYDOWN message (Posted to the window with the keyboard focus when 
// a nonsystem key is pressed (ALT not pressed)).

void CWindow::OnKeyDown (WPARAM wParam, LPARAM lParam) 
{ 

}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_KEYUP message (Posted to the window with the keyboard focus when 
// a nonsystem key is released (ALT not pressed)).

void CWindow::OnKeyUp (WPARAM wParam, LPARAM lParam) 
{ 

}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_TIMER message (Sent after each interval specified in the SetTimer 
// function used to install a timer).

void CWindow::OnTimer (WPARAM wParam, LPARAM lParam) 
{ 

}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_COMMAND message (Sent when the user selects a command item from a 
// menu, when a control sends a notification message to its parent window, or when an 
// accelerator keystroke is translated)

void CWindow::OnCommand (WPARAM wParam, LPARAM lParam) 
{ 

}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_SYSCOMMAND message (A window receives this message when the user 
// chooses a command from the window menu (also known as the System menu or Control 
// menu) or when the user chooses the Maximize button or Minimize button.)
// Returns whether to call the default window proc or not after handling this message

bool CWindow::OnSysCommand (WPARAM wParam, LPARAM lParam) 
{ 
    return true;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_CLOSE message (Sent as a signal that a window or an application 
// should terminate) 

void CWindow::OnClose (WPARAM wParam, LPARAM lParam) 
{ 
#ifdef WIN32
	DestroyWindow (m_hWnd); // Posts WM_DESTROY
#endif
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the WM_DESTROY message (Sent when a window is being destroyed. It is sent 
// to the window procedure of the window being destroyed after the window is removed 
// from the screen) 

void CWindow::OnDestroy (WPARAM wParam, LPARAM lParam) 
{ 
#ifdef WIN32
	PostQuitMessage (0); // Posts WM_QUIT
#endif
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


#ifndef WIN32
// Handles the SDL_JOYAXISMOTION message (SDL only).

void CWindow::OnJoystickAxis (WPARAM wParam, LPARAM lParam) 
{ 

}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************



// Handles the SDL_JOYBUTTONDOWN/-UP message (SDL only).

void CWindow::OnJoystickButton (WPARAM wParam, LPARAM lParam) 
{ 

}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
#endif // WIN32
