
import fileinput

print('Setting version info...')

#print('Number of arguments: ' + str(len(sys.argv)))
#print('Argument list: ' + str(sys.argv))

# Get project name
strProjectName = ''

fi = open('../CMakeLists.txt', 'r')

strLines = fi.readlines()

for strLine in strLines:
    if 'PROJECT' in strLine.upper(): 
        strProjectName = strLine.split('(')[-1].replace(')', '').rstrip()

print('Project Name = ' + strProjectName)

fi.close()

# Read version info and 
fi = open('../src/' + strProjectName + '.h', 'r')
strLines = fi.readlines()
fi.close()

intMajorNumber = 0
intMinorNumber = 0
intReleaseNumber = 0
intBuildNumber = 0

strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)

fo = open('../src/' + strProjectName + '.h', 'w')

for strLine in strLines:

    if 'APP_VERSION_INFO' in strLine.upper(): 
        strPrevVersion = strLine.split(' ')[-1].rstrip().replace('"', '')
                
        intMajorNumber = int(strPrevVersion.split('.')[0])
        intMinorNumber = int(strPrevVersion.split('.')[1])
        intReleaseNumber = int(strPrevVersion.split('.')[2])
        intBuildNumber = int(strPrevVersion.split('.')[3])
                
        intBuildNumber = intBuildNumber + 1
        
        strNewVersion = str(intMajorNumber) + '.' + str(intMinorNumber) + '.' + str(intReleaseNumber) + '.' + str(intBuildNumber)
        
        strLine = strLine.replace(strPrevVersion, strNewVersion)
                
    fo.write(strLine)
    
fo.close()

print(strNewVersion)

fio = fileinput.FileInput('../src/' + strProjectName + '.rc', inplace=True, backup='.bak')

for line in fio:

    if 'FILEVERSION' in line:
        print('  FILEVERSION ' + strNewVersion.replace('.', ','))
    elif 'PRODUCTVERSION' in line:
        print('  PRODUCTVERSION ' + strNewVersion.replace('.', ','))    
    elif '"FileVersion"' in line:
        print('      VALUE "FileVersion", "' + strNewVersion + '"')
    elif '"ProductVersion"' in line:
        print('      VALUE "ProductVersion", "' + strNewVersion + '"')             
    else:
        print(line.rstrip())
    
fio.close()





