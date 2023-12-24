
import os
import platform
import time
import shutil
import glob

# Build platform
if platform.system().lower() == 'windows':
    build = 'msvc17-win32'
elif platform.system().lower() == 'linux':
    build = 'gxx-linux-x86'

# Crash report
bCrashReport = True

if bCrashReport:
    configuration = 'RelWithDebInfo'

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
incVersionInput = input('Increment version number [Y/n]?')

incVersion = True 
if incVersionInput == 'n':
    incVersion = False

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
print('------------ Building release ------------')
print('version: ' + strNewVersion)
print('build: ' + build)
time.sleep(3)
if platform.system().lower() == 'windows':
    os.system('cmake -S trunk -B build/' + build + ' -G "Visual Studio 17 2022" -A Win32 -DLOAD_RESOURCES_FROM_FILES:BOOL=OFF')
elif platform.system().lower() == 'linux':
    os.system('cmake -S trunk -B build/' + build + ' -G "Ninja" -DLOAD_RESOURCES_FROM_FILES:BOOL=ON')

os.system('cmake --build build/' + build + ' --config ' + configuration)

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

strNewFolder = 'releases/' + build
if not os.path.isdir(strNewFolder):
	os.mkdir(strNewFolder)

strNewFolder = 'releases/' + build + '/Bombermaaan_' + strNewVersion
if not os.path.isdir(strNewFolder):
	os.mkdir(strNewFolder)

# Copy files
if platform.system().lower() == 'windows':
    shutil.copy2('build/' + build + '/bin/Bombermaaan.exe', strNewFolder + '/Bombermaaan.exe')
    shutil.copy2('build/' + build + '/bin/Bombermaaan32.dll', strNewFolder + '/Bombermaaan32.dll')
        
    shutil.copy2(os.environ.get('SDL2_DIR') + '/lib/x86/SDL2.dll', strNewFolder + '/SDL2.dll')
    shutil.copy2(os.environ.get('SDL2_MIXER_DIR') + '/lib/x86/SDL2_mixer.dll', strNewFolder + '/SDL2_mixer.dll')
    #shutil.copy2(os.environ.get('SDL2_NET_DIR')   + '/lib/x86/SDL2_net.dll', strNewFolder + '/SDL2_net.dll')
         
    shutil.copy2(os.environ.get('SystemRoot') + '/SysWOW64/msvcp140.dll', strNewFolder + '/msvcp140.dll')
    shutil.copy2(os.environ.get('SystemRoot') + '/SysWOW64/vcruntime140.dll', strNewFolder + '/vcruntime140.dll')

    if bCrashReport:
        shutil.copy2(os.environ.get('CRASHRPT_DIR') + '/bin/CrashSender1403.exe', strNewFolder + '/CrashSender1403.exe')
        shutil.copy2(os.environ.get('CRASHRPT_DIR') + '/bin/CrashRpt1403.dll', strNewFolder + '/CrashRpt1403.dll')
        shutil.copy2(os.environ.get('CRASHRPT_DIR') + '/bin/dbghelp.dll', strNewFolder + '/dbghelp.dll')
        shutil.copy2(os.environ.get('CRASHRPT_DIR') + '/bin/crashrpt_lang.ini', strNewFolder + '/crashrpt_lang.ini')
        shutil.copy2('build/' + build + '/bin/Bombermaaan.pdb', strNewFolder + '/Bombermaaan.pdb')

    if not os.path.isdir(strNewFolder + '/levels'):
        os.mkdir(strNewFolder + '/levels')
    for file in glob.glob('trunk/levels/*.TXT'):
        shutil.copy2(file, os.path.join(strNewFolder, 'levels', os.path.basename(file)))
        
elif platform.system().lower() == 'linux':
    shutil.copy2('build/' + build + '/bin/Bombermaaan', strNewFolder + '/Bombermaaan')    

    if not os.path.isdir(strNewFolder + '/images'):
        os.mkdir(strNewFolder + '/images')
    for file in glob.glob('trunk/res/images/*.bmp'):
        shutil.copy2(file, os.path.join(strNewFolder, 'images', os.path.basename(file)))

    if not os.path.isdir(strNewFolder + '/sounds'):
        os.mkdir(strNewFolder + '/sounds')
    for file in glob.glob('trunk/res/sound/*.ogg'):
        shutil.copy2(file, os.path.join(strNewFolder, 'sounds', os.path.basename(file)))
    for file in glob.glob('trunk/res/sound/*.mod'):
        shutil.copy2(file, os.path.join(strNewFolder, 'sounds', os.path.basename(file)))
    for file in glob.glob('trunk/res/sound/*.s3m'):
        shutil.copy2(file, os.path.join(strNewFolder, 'sounds', os.path.basename(file)))

    if not os.path.isdir(strNewFolder + '/levels'):
        os.mkdir(strNewFolder + '/levels')
    for file in glob.glob('trunk/levels/*.TXT'):
        shutil.copy2(file, os.path.join(strNewFolder, 'levels', os.path.basename(file)))

# Copy license
shutil.copy2('COPYING.txt', strNewFolder + '/COPYING.txt')

# Copy read me
shutil.copy2('README.md', strNewFolder + '/README.txt')

# Create tag
if incVersion:
    os.system('git commit -a -m v' + strNewVersion)
    os.system('git tag v' + strNewVersion)
    os.system('git push --tags')

# Create package
if platform.system().lower() == 'windows':
    setup = '"D:/Programs/Inno Setup 6/iscc.exe"'
    os.system(setup + ' installers\InstallScriptWin32.iss')

elif platform.system().lower() == 'linux':
    os.system('fpm -s dir -t deb -C ' + strNewFolder \
    + ' --prefix /usr/games/bombermaaan ' \
    + ' -n bombermaaan ' \
    + ' -v ' + strNewVersion \
    + ' --license \'GPLv3\' ' \
    + ' --category Game ' \
    + ' -d \'libsdl1.2debian\' -d \'libsdl-mixer1.2\' ' \
    + ' -m \'bjaraujo <bjaraujo@github.com>\'' \
    + ' --url \'https://github.com/bjaraujo/Bombermaaan\'' \
    + ' --description \'Bombermaaan is a classic bomberman game\'' \
    + ' -p Bombermaaan_' + strNewVersion + '_i386.deb ' \
    + ' -f --verbose .')

