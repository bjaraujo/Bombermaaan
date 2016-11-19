
import os
import platform
import time
import sys
import shutil
import glob

# Build platform
if platform.system().lower() == 'windows':
    build = 'msvc12-win32'
elif platform.system().lower() == 'linux':
    build = 'gxx-linux'

# Read version info
fi = open('trunk/src/Bombermaaan.h', 'r')
strLines = fi.readlines()
fi.close()

intMajorNumber = 0
intMinorNumber = 0
intReleaseNumber = 0
intBuildNumber = 0

strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)

# Increment version
incVersionInput = raw_input('Increment version number [Y/n]?')

incVersion = False 
if incVersionInput == 'Y' or incVersionInput == 'y':
    incVersion = True

fo = open('trunk/src/Bombermaaan.h', 'w')

for strLine in strLines:

    if 'APP_VERSION_INFO' in strLine.upper(): 
        strPrevVersion = strLine.split(' ')[-1].rstrip().replace('"', '')
                
        intMajorNumber = int(strPrevVersion.split('.')[0])
        intMinorNumber = int(strPrevVersion.split('.')[1])
        intReleaseNumber = int(strPrevVersion.split('.')[2])
        intBuildNumber = int(strPrevVersion.split('.')[3])
        
        if incVersion:
            intReleaseNumber = intReleaseNumber + 1
            
            if intReleaseNumber > 9:
                intMinorNumber = intMinorNumber + 1
                intReleaseNumber = 0

            if intMinorNumber > 9:
                intMajorNumber = intMajorNumber + 1
                intMinorNumber = 0
            
        strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)
        
        strLine = strLine.replace(strPrevVersion, strNewVersion)
                
    fo.write(strLine)
    
fo.close()

# Build
print 'Building release: ' + strNewVersion
time.sleep(4)

if platform.system().lower() == 'windows':
    os.system('"C:\Program Files (x86)/MSBuild/12.0/Bin/MSBuild.exe" build/' + build + '/src/Bombermaaan.vcxproj /p:Configuration=Release /t:Rebuild')
elif platform.system().lower() == 'linux':
    os.system('build/' + build + '/make clean');
    os.system('build/' + build + '/make');

# Read version info
fi = open('trunk/src/Bombermaaan.h', 'r')
strLines = fi.readlines()
fi.close()

intMajorNumber = 0
intMinorNumber = 0
intReleaseNumber = 0
intBuildNumber = 0

strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)

fi = open('trunk/src/Bombermaaan.h', 'r')

for strLine in strLines:

    if 'APP_VERSION_INFO' in strLine.upper(): 
        strPrevVersion = strLine.split(' ')[-1].rstrip().replace('"', '')
                
        intMajorNumber = int(strPrevVersion.split('.')[0])
        intMinorNumber = int(strPrevVersion.split('.')[1])
        intReleaseNumber = int(strPrevVersion.split('.')[2])
        intBuildNumber = int(strPrevVersion.split('.')[3])
                        
        strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)
                            
fi.close()

if not os.path.exists('releases'):
    os.mkdir('releases')

strNewFolder = 'releases/' + platform
os.mkdir(strNewFolder)
strNewFolder = 'releases/' + platform + '/Bombermaaan_' + strNewVersion
os.mkdir(strNewFolder)

# Copy files
if platform.system().lower() == 'windows':

    shutil.copy2('build/' + build + '/src/Release/Bombermaaan.exe', strNewFolder + '/Bombermaaan.exe')
    shutil.copy2('build/' + build + '/src/Release/Bombermaaan32.dll', strNewFolder + '/Bombermaaan32.dll')

    shutil.copy2(os.environ.get('SDLDIR') + '/lib/x86/SDL.dll', strNewFolder + '/SDL.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/SDL_mixer.dll', strNewFolder + '/SDL_mixer.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/smpeg.dll', strNewFolder + '/smpeg.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/libFLAC-8.dll', strNewFolder + '/libFLAC-8.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/libmikmod-2.dll', strNewFolder + '/libmikmod-2.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/libvorbis-0.dll', strNewFolder + '/libvorbis-0.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/libvorbisfile-3.dll', strNewFolder + '/libvorbisfile-3.dll')
    shutil.copy2(os.environ.get('SDLMIXERDIR') + '/lib/x86/libogg-0.dll', strNewFolder + '/libogg-0.dll')
    shutil.copy2(os.environ.get('SDLNETDIR') + '/lib/x86/SDL_net.dll', strNewFolder + '/SDL_net.dll')

    shutil.copy2(os.environ.get('SystemRoot') + '/System32/msvcr120.dll', strNewFolder + '/msvcr120.dll')
    shutil.copy2(os.environ.get('SystemRoot') + '/System32/msvcp120.dll', strNewFolder + '/msvcp120.dll')

elif platform.system().lower() == 'linux':

    shutil.copy2('build/' + build + '/Bombermaaan', strNewFolder + '/Bombermaaan')
    
# Copy license
shutil.copy2('COPYING.txt', strNewFolder + '/COPYING.txt')

# Copy levels
os.mkdir(strNewFolder + '/Levels')
for file in glob.glob('trunk/levels/*.TXT'):
    shutil.copy2(file, os.path.join(strNewFolder, 'Levels', os.path.basename(file)))

if incVersion:        
    os.system('git commit -a -m v' + strNewVersion)
    os.system('git tag v' + strNewVersion)
    os.system("git push --tags")
