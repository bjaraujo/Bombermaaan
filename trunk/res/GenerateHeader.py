import sys
import os.path
import re

file_in = 'res.rc'

i = 1

for line in open(file_in):
    
    if 'BITMAP' in line:
        #print line.rstrip()
        
        print '#define ' + line.split()[0].ljust(40) + ' ' + str(1000 + i)
        i = i + 1
        
print ' '
print ' '

i = 1

for line in open(file_in):
    
    if 'SOUND' in line:
        #print line.rstrip()
        
        print '#define ' + line.split()[0].ljust(40) + ' ' + str(2000 + i)
        i = i + 1
        
        