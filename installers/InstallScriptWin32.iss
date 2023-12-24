; Bombermaaan Installation Script
; Written by Billy Araujo, 07-01-2017
;
; https://github.com/bjaraujo/bombermaaan
;

; The APPID is kept as long as the first two numbers in the version are the same
; (the whole 1.5 version uses the same APPID)
; (yes, two opening curly braces and only one closing curly brace)
#define APPID "{{27E65ECC-E43F-4EE1-BDAC-E1CC8EBB4C88}"

#include "..\trunk\src\Bombermaaan.h"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
; SignTool=signtool
AppId={#APPID}
AppName=Bombermaaan
AppVerName=Bombermaaan {#APP_VERSION_INFO}
AppPublisher=The Bombermaaan Team
AppPublisherURL=https://github.com/bjaraujo/bombermaaan
AppSupportURL=https://github.com/bjaraujo/bombermaaan
AppUpdatesURL=https://github.com/bjaraujo/bombermaaan
DefaultDirName={commonpf}\Bombermaaan
DefaultGroupName=Bombermaaan {#APP_VERSION_INFO}
AllowNoIcons=true
LicenseFile=..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\COPYING.txt
OutputDir=.\output
OutputBaseFilename=Bombermaaan_{#APP_VERSION_INFO}_setup
Compression=lzma
SolidCompression=true
VersionInfoVersion={#APP_VERSION_INFO}
VersionInfoDescription=Installer for Bombermaaan
VersionInfoCopyright=Copyright (C) 2000-2002, 2007, 2008, 2016 Thibaut Tollemer, Bernd Arnold, Jerome Bigot, Markus Drescher, Billy Araujo
; WizardImageFile=compiler:wizmodernimage-is.bmp
; This is not the best image since the setup dialog shows only a square, not a rectangle
WizardSmallImageFile=HeaderImage_Innosetup.bmp
WizardImageStretch=false
UninstallDisplayIcon={app}\Bombermaaan.exe

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons};
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\Bombermaaan.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\Bombermaaan32.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\CrashSender1403.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\CrashRpt1403.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\dbghelp.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\crashrpt_lang.ini; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\COPYING.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\README.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\SDL2.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\SDL2_mixer.dll; DestDir: {app}; Flags: ignoreversion
; Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\SDL2_net.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\msvcp140.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\vcruntime140.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\releases\msvc17-win32\Bombermaaan_{#APP_VERSION_INFO}\Levels\*.TXT; DestDir: {app}\Levels\

[Icons]
Name: {commondesktop}\Bombermaaan; Filename: {app}\Bombermaaan.exe; Tasks: desktopicon; Parameters: --use-appdata-dir

[Run]
Filename: {app}\Bombermaaan.exe; Description: {cm:LaunchProgram,Bombermaaan}; Flags: nowait postinstall skipifsilent unchecked; Parameters: --use-appdata-dir
Filename: {app}\README.txt; Description: "View the README file"; Flags: postinstall shellexec skipifsilent

[Dirs]
Name: {app}\Levels

