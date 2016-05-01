import glob
import configparser

for file in glob.glob("*.TXT"):
    config = configparser.ConfigParser()
    config.optionxform=str
    config.read(file)
    config['Settings']['BomberSkillsAtStart.MaxBombs'] = '1'
    config['Settings']['BomberSkillsAtStart.KickItems'] = '0'
    config['Settings']['BomberSkillsAtStart.ThrowItems'] = '0'
    config['Settings']['BomberSkillsAtStart.PunchItems'] = '0'
    
    with open(file, 'w') as configfile:    # save
        config.write(configfile)
        
    with open(file, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        f.write('; Bombermaaan level file version=2'.rstrip('\r\n') + '\n' + content)
    
    