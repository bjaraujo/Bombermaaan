# Bombermaaan

## Copyright

This bombermaaan is based on the work of:
- 2000-2002, 2007 Thibaut Tollemer
- 2007, 2008 Bernd Arnold
- 2008 Jerome Bigot
- 2008 Markus Drescher

## License

Bombermaaan is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bombermaaan is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bombermaaan.  If not, see &lt;http://www.gnu.org/licenses/&gt;.
	
## Introduction

This is a version of Thibaut's Bombermaaan game project
which started on July 2000. He didn't develop this
project in order to bring new concepts of gameplay
or to make the best Bomberman game ever but to
increase his programming skills and have a game
project he can show to be hired.

## History

This program is still, and will always be under 
development. You can check the latest history of 
this project by browsing <a href="http://bombermaaan.sourceforge.net/">its website</a>. You'll be 
informed of what was changed, fixed, added, and 
removed in each version.

## System requirements

This program has been tested under Windows 95 OSR2, 98 PE/SE, 2000 and XP.

It requires DirectX 7.0 or later to run. It is
recommended you set your display to at least
16 bit color depth. A sound card is recommended.

## Parameters and switches for Win32

When Bombermaaan is called without a command line parameter (started with mouse click for example),
the game fetches level files from the directory &lt;path-to-exe-file&gt;\Levels. The configuration
file named config.xml and log file named log.txt are read and/or written in the same folder where
the called exe file resides.

Since some Windows systems (Vista for example) don't allow the creation of files in the program's
directory, Bombermaaan was modified so the dynamic files (config.xml and log.txt) are handled
in a subfolder of the user's application data folder.<br />
You can use <kbd>echo %appdata%</kbd> in the command line to see where your application folder is
placed. Bombermaaan creates a subfolder called &quot;Bombermaaan&quot;. You can browse to this folder by
running <kbd>%appdata%\Bombermaaan</kbd> or typing <kbd>start &quot;%appdata%\Bombermaaan&quot;</kbd>
in a command line window.<br />
<strong>This behaviour is NOT enabled by default.</strong><br />
To enable the use of your %appdata% folder, append <kbd>--use-appdata-dir</kbd> when calling the
Bombermaaan exe file.<br />
Users can store their own level files in the %appdata%\Bombermaaan\Levels directory. These files
are also loaded as level files. So there is no need to touch the program files directory. Remember:
this behaviour is enabled when the <kbd>--use-appdata-dir</kbd> parameter is used.

You can call Bombermaaan with the <kbd>--help</kbd> switch to see a message box with copyright and license notice.

## Parameters and switches for Linux

When Bombermaaan is called without a command line parameter (started with mouse click for example),
the game fetches level files from the directory &lt;path-to-executable-file&gt;\Levels. The configuration
file named config.xml and log file named log.txt are read and/or written in your home folder <code>~/.Bombermaaan</code>.

Users can store their own level files in the ~\.Bombermaaan\Levels directory. These files
are also loaded as level files. So there is no need to touch the folder where the executable files are stored.

If you don't like this behavior you can pass the <kbd>--ignore-home-dir</kbd> switch to the executable.<br />
<strong>This behaviour is NOT enabled by default.</strong><br />
To enable the use of the local folder, append <kbd>--ignore-home-dir</kbd> when calling the
Bombermaaan executable file.

You can call Bombermaaan with the <kbd>--help</kbd> switch to see a message box with copyright and license notice.

## Controls

During a match :
- Here are the configurations :
  Keyboard 1 (Default) : 8/5/4/6 + PageUp/Home
  Keyboard 2 (Default) : R/F/D/G + 2/1
  Keyboard 3 (Default) : I/K/J/L + 8/7
  Keyboard 4 (Default) : H/N/B/M + 5/4
  Keyboard 5 (Default) : Up/Down/Left/Right + RightControl/RightShift
  Joystick X (Default) : Up/Down/Left/Right + Button1/Button2
- On your configuration :
  - Up/Down/Left/Right : make your bomber move
  - Action 1 : drop a bomb if possible,
               or lift the bomb you are onto.
               hold Action 1 to hold the bomb.
               release Action 1 to throw the bomb.
  - Action 2 : stop a bomb you kicked that is still rolling,
               or punch a bomb that is right in front of you,
               or remote fuse a bomb.
- Escape : pause the match and make the menu box appear
- P : pause or resume

In the menu screens :
- Up/Down............... : select another field to modify
- Left/Right............ : change value of current field
- Backspace............. : previous screen
- Return/Space bar/Enter : next screen

In the menu box :
- Arrows................ : change between yes or no
- Return/Space bar/Enter : validate choice
- Escape................ : make the menu box disappear and resume the match

Anywhere in the game :
- F1...... : set 320x240 display mode (not available in 32-pixels version)
- F2...... : set 512x384 display mode (not available in 32-pixels version)
- F3...... : set 640x480 display mode
- F4...... : set windowed display mode
- Ctrl+F12 : quickly exit the game without confirmation

Please note that the display mode you chose may
be unavailable on your computer. The program will
not change the display mode if this one is
unavailable.

## Credits

- 2000-2002, 2007 Thibaut Tollemer
- 2007, 2008 Bernd Arnold
- 2008 Jerome Bigot
- 2008 Markus Drescher

Bombermaaan is using TinyXML from <a href="https://sourceforge.net/projects/tinyxml">https://sourceforge.net/projects/tinyxml</a>.

Bombermaaan is using <a href="http://www.doxygen.org/index.html">Doxygen</a> for <a href="http://bombermaaan.sourceforge.net/doxydoc/html/">documentation</a> purposes.

Bombermaaan is using SimpleIni from <a href="http://code.jellycan.com/simpleini/">http://code.jellycan.com/simpleini/</a>.

## Project host

The Bombermaaan project is hosted on SourceForge.net. 

## News

Check <a href="http://bombermaaan.sourceforge.net/">http://bombermaaan.sourceforge.net/</a> for the latest news.

## Compiling

Set SDLDIR and SDLMIXERDIR environment variables and run cmake.



