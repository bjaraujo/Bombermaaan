; Bombermaaan Installation Script
; Written by Billy Araujo, 02-05-2016
;
; https://github.com/billyaraujo/bombermaaan
;

; The APPID is kept as long as the first two numbers in the version are the same
; (the whole 1.5 version uses the same APPID)
; (yes, two opening curly braces and only one closing curly brace)
#define APPID "{{B8F62E9A-8AAE-4124-828F-28A826424986}"

#include "..\trunk\src\Bombermaaan.h"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={#APPID}
AppName=Bombermaaan
AppVerName=Bombermaaan {#APP_VERSION_INFO}
AppPublisher=The Bombermaaan team
AppPublisherURL=https://github.com/billyaraujo/bombermaaan
AppSupportURL=http://bombermaaan.sourceforge.net/
AppUpdatesURL=http://bombermaaan.sourceforge.net/
DefaultDirName={pf}\Bombermaaan {#APP_VERSION_INFO}
DefaultGroupName=Bombermaaan {#APP_VERSION_INFO}
AllowNoIcons=true
LicenseFile=..\releases\Bombermaaan_{#APP_VERSION_INFO}\COPYING.txt
OutputDir=.\output
OutputBaseFilename=Bombermaaan_{#APP_VERSION_INFO}_setup
Compression=lzma
SolidCompression=true
VersionInfoVersion={#APP_VERSION_INFO}
VersionInfoDescription=Installer for Bombermaaan
VersionInfoCopyright=Copyright (C) 2000-2002, 2007, 2008 Thibaut Tollemer, Bernd Arnold, Jerome Bigot, Markus Drescher
WizardImageFile=compiler:wizmodernimage-is.bmp
; This is not the best image since the setup dialog shows only a square, not a rectangle
WizardSmallImageFile=HeaderImage_Innosetup.bmp
WizardImageStretch=false

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\Bombermaaan.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\Bombermaaan32.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\COPYING.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\libFLAC-8.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\libmikmod-2.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\libogg-0.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\libvorbis-0.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\libvorbisfile-3.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\SDL.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\SDL_mixer.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\smpeg.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\msvcp120.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\msvcr120.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\Bombermaaan_{#APP_VERSION_INFO}\Levels\*.TXT; DestDir: {app}\Levels\

[Icons]
Name: {commondesktop}\Bombermaaan; Filename: {app}\Bombermaaan.exe; Tasks: desktopicon; Parameters: --use-appdata-dir

[Run]
Filename: {app}\Bombermaaan.exe; Description: {cm:LaunchProgram,Bombermaaan}; Flags: nowait postinstall skipifsilent unchecked; Parameters: --use-appdata-dir

[Dirs]
Name: {app}\Levels

