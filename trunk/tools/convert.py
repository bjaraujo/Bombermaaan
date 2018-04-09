import glob
import configparser

for file in glob.glob("*.TXT"):
    config = configparser.ConfigParser()
    config.optionxform=str
    config.read(file)

    config['Settings']['ItemsInWalls.Bombs'] = '2'
    config['Settings']['ItemsInWalls.Flames'] = '2'
    config['Settings']['ItemsInWalls.Kicks'] = '1'
    config['Settings']['ItemsInWalls.Rollers'] = '1'
    config['Settings']['ItemsInWalls.Skulls'] = '1'
    config['Settings']['ItemsInWalls.Throws'] = '1'
    config['Settings']['ItemsInWalls.Punches'] = '1'
    config['Settings']['ItemsInWalls.Sheilds'] = '1'
    config['Settings']['ItemsInWalls.StrongWeak'] = '0'
    
    config['Settings']['BomberSkillsAtStart.FlameSize'] = '2'
    config['Settings']['BomberSkillsAtStart.MaxBombs'] = '1'
    config['Settings']['BomberSkillsAtStart.BombItems'] = '1'
    config['Settings']['BomberSkillsAtStart.FlameItems'] = '0'
    config['Settings']['BomberSkillsAtStart.RollerItems'] = '0'
    config['Settings']['BomberSkillsAtStart.PunchItems'] = '0'
    config['Settings']['BomberSkillsAtStart.KickItems'] = '0'
    config['Settings']['BomberSkillsAtStart.ThrowItems'] = '0'
    config['Settings']['BomberSkillsAtStart.PunchItems'] = '0'
    config['Settings']['BomberSkillsAtStart.RemoteItems'] = '0'
    config['Settings']['BomberSkillsAtStart.ShieldItems'] = '0'
    config['Settings']['BomberSkillsAtStart.StrongWeakItems'] = '0'
    
    with open(file, 'w') as configfile:    # save
        config.write(configfile)
        
    with open(file, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        f.write('; Bombermaaan level file version=2'.rstrip('\r\n') + '\n' + content)
    
    