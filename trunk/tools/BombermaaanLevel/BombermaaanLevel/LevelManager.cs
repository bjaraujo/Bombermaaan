using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using Ini;

namespace BombermaaanLevel
{

    enum EBlockType
    {
        BLOCKTYPE_HARDWALL,			//!< There must be a hard wall here
        BLOCKTYPE_SOFTWALL,			//!< There must be a soft wall here
        BLOCKTYPE_RANDOM,			//!< There must be either a soft wall, or a free place here (random)
        BLOCKTYPE_FREE,             //!< There must be a free place here 
        BLOCKTYPE_WHITEBOMBER,      //!< The white bomber must start here
        BLOCKTYPE_BLACKBOMBER,      //!< The black bomber must start here
        BLOCKTYPE_REDBOMBER,        //!< The red bomber must start here
        BLOCKTYPE_BLUEBOMBER,       //!< The blue bomber must start here
        BLOCKTYPE_GREENBOMBER,      //!< The green bomber must start here
        BLOCKTYPE_MOVEBOMB_RIGHT,   //!< A bomb starts moving right if placed here
        BLOCKTYPE_MOVEBOMB_DOWN,    //!< A bomb starts moving down if placed here
        BLOCKTYPE_MOVEBOMB_LEFT,    //!< A bomb starts moving left if placed here
        BLOCKTYPE_MOVEBOMB_UP,      //!< A bomb starts moving up if placed here
        BLOCKTYPE_ITEM_BOMB,        //!< A bomb item if placed here
        BLOCKTYPE_ITEM_FLAME,       //!< A flame item if placed here
        BLOCKTYPE_ITEM_ROLLER,      //!< A roller item if placed here 
        BLOCKTYPE_ITEM_KICK,        //!< A kick item if placed here
        BLOCKTYPE_ITEM_THROW,       //!< A throw item if placed here
        BLOCKTYPE_ITEM_PUNCH,       //!< A punch item if placed here
        BLOCKTYPE_ITEM_SKULL,       //!< A skull item if placed here
        BLOCKTYPE_ITEM_REMOTE,      //!< A remotes item if placed here
        BLOCKTYPE_ITEM_SHIELD,      //!< A sheild item if placed here
        BLOCKTYPE_ITEM_STRONGWEAK   //!< A strong/weak item if placed here
    };

    class LevelManager
    {

        IniFile anIniFile = null;

        public bool OpenFile(string aFileName)
        {

            anIniFile = new IniFile(aFileName);

            return true;

        }

        public bool NewFile(string aFileName)
        {

            File.WriteAllText(aFileName, "; Bombermaaan level file version=2");

            anIniFile = new IniFile(aFileName);

            anIniFile.IniWriteValue("General", "Width", "15");
            anIniFile.IniWriteValue("General", "Height", "13");
            anIniFile.IniWriteValue("General", "MaxPlayers", "5");
            anIniFile.IniWriteValue("General", "MinPlayers", "1");
            anIniFile.IniWriteValue("General", "Creator", "");
            anIniFile.IniWriteValue("General", "Priority", "0");
            anIniFile.IniWriteValue("General", "Comment", "");
            anIniFile.IniWriteValue("General", "Description", "");

            anIniFile.IniWriteValue("Map", "Line.00", "***************");
            anIniFile.IniWriteValue("Map", "Line.01", "*             *");
            anIniFile.IniWriteValue("Map", "Line.02", "*             *");
            anIniFile.IniWriteValue("Map", "Line.03", "*             *");
            anIniFile.IniWriteValue("Map", "Line.04", "*             *");
            anIniFile.IniWriteValue("Map", "Line.05", "*             *");
            anIniFile.IniWriteValue("Map", "Line.06", "*             *");
            anIniFile.IniWriteValue("Map", "Line.07", "*             *");
            anIniFile.IniWriteValue("Map", "Line.08", "*             *");
            anIniFile.IniWriteValue("Map", "Line.09", "*             *");
            anIniFile.IniWriteValue("Map", "Line.10", "*             *");
            anIniFile.IniWriteValue("Map", "Line.11", "*             *");
            anIniFile.IniWriteValue("Map", "Line.12", "***************");

            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Bombs", "5");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Flames", "3");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Kicks", "3");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Rollers", "5");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Skulls", "1");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Throws", "2");
            anIniFile.IniWriteValue("Settings", "ItemsInWalls.Punches", "2");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.FlameSize", "2");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.MaxBombs", "1");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.BombItems", "0");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.FlameItems", "0");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.RollerItems", "0");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.KickItems", "0");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.ThrowItems", "0");
            anIniFile.IniWriteValue("Settings", "BomberSkillsAtStart.PunchItems", "0");
            anIniFile.IniWriteValue("Settings", "ContaminationsNotUsed", "None");

            return true;

        }

        public int MaxPlayers()
        {
            if (anIniFile == null)
                return 0;

            return Int32.Parse(anIniFile.IniReadValue("General", "MaxPlayers"));
        }

        public int MinPlayers()
        {
            if (anIniFile == null)
                return 0;

            return Int32.Parse(anIniFile.IniReadValue("General", "MinPlayers"));
        }

        public string Creator()
        {
            if (anIniFile == null)
                return "";

            return anIniFile.IniReadValue("General", "Creator");
        }

        public string Comment()
        {
            if (anIniFile == null)
                return "";

            return anIniFile.IniReadValue("General", "Comment");
        }

        public string Description()
        {
            if (anIniFile == null)
                return "";

            return anIniFile.IniReadValue("General", "Description");
        }

        public void SetBlockType(int i, int j, EBlockType aBlockType)
        {

            if (anIniFile == null)
                return;

            string aLine = anIniFile.IniReadValue("Map", String.Format("Line.{0:00}", j));

            char aNewChar = ' ';

            switch (aBlockType)
            {
                case EBlockType.BLOCKTYPE_HARDWALL:
                    aNewChar = '*';
                    break;
                case EBlockType.BLOCKTYPE_SOFTWALL:
                    aNewChar = '-';
                    break;
                case EBlockType.BLOCKTYPE_RANDOM:
                    aNewChar = '?';
                    break;
                case EBlockType.BLOCKTYPE_FREE:
                    aNewChar = ' ';
                    break;
                case EBlockType.BLOCKTYPE_WHITEBOMBER:
                    aNewChar = '1';
                    break;
                case EBlockType.BLOCKTYPE_BLACKBOMBER:
                    aNewChar = '2';
                    break;
                case EBlockType.BLOCKTYPE_REDBOMBER:
                    aNewChar = '3';
                    break;
                case EBlockType.BLOCKTYPE_BLUEBOMBER:
                    aNewChar = '4';
                    break;
                case EBlockType.BLOCKTYPE_GREENBOMBER:
                    aNewChar = '5';
                    break;
                case EBlockType.BLOCKTYPE_MOVEBOMB_RIGHT:
                    aNewChar = 'R';
                    break;
                case EBlockType.BLOCKTYPE_MOVEBOMB_DOWN:
                    aNewChar = 'D';
                    break;
                case EBlockType.BLOCKTYPE_MOVEBOMB_LEFT:
                    aNewChar = 'L';
                    break;
                case EBlockType.BLOCKTYPE_MOVEBOMB_UP:
                    aNewChar = 'U';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_BOMB:
                    aNewChar = 'B';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_FLAME:
                    aNewChar = 'F';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_ROLLER:
                    aNewChar = 'S';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_KICK:
                    aNewChar = 'K';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_THROW:
                    aNewChar = 'T';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_PUNCH:
                    aNewChar = 'P';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_SKULL:
                    aNewChar = 'C';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_REMOTE:
                    aNewChar = 'Z';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_SHIELD:
                    aNewChar = 'V';
                    break;
                case EBlockType.BLOCKTYPE_ITEM_STRONGWEAK:
                    aNewChar = 'I';
                    break;
                default:
                    break;
            }

            StringBuilder sbLine = new StringBuilder(aLine);
            sbLine[i] = aNewChar;

            aLine = sbLine.ToString();

            anIniFile.IniWriteValue("Map", String.Format("Line.{0:00}", j), aLine);

        }

        public EBlockType BlockType(int i, int j)
        {

            if (anIniFile == null)
                return EBlockType.BLOCKTYPE_FREE;

            string aLine = anIniFile.IniReadValue("Map", String.Format("Line.{0:00}", j));
            char aChar = aLine.Substring(i, 1)[0];

            switch (aChar)
            {
                case '*': // HARDWALL
                    return EBlockType.BLOCKTYPE_HARDWALL;
                case '-': // SOFTWALL 
                    return EBlockType.BLOCKTYPE_SOFTWALL;
                case '?': // RANDOM; 
                    return EBlockType.BLOCKTYPE_RANDOM;
                case ' ': // FREE; 
                    return EBlockType.BLOCKTYPE_FREE;
                case '1': // WHITEBOMBER; 
                    return EBlockType.BLOCKTYPE_WHITEBOMBER;
                case '2': // BLACKBOMBER; 
                    return EBlockType.BLOCKTYPE_BLACKBOMBER;
                case '3': // REDBOMBER; 
                    return EBlockType.BLOCKTYPE_REDBOMBER;
                case '4': // BLUEBOMBER; 
                    return EBlockType.BLOCKTYPE_BLUEBOMBER;
                case '5': // GREENBOMBER; 
                    return EBlockType.BLOCKTYPE_GREENBOMBER;
                case 'R': // MOVEBOMB_RIGHT; 
                    return EBlockType.BLOCKTYPE_MOVEBOMB_RIGHT;
                case 'D': // MOVEBOMB_DOWN; 
                    return EBlockType.BLOCKTYPE_MOVEBOMB_DOWN;
                case 'L': // MOVEBOMB_LEFT; 
                    return EBlockType.BLOCKTYPE_MOVEBOMB_LEFT;
                case 'U': // MOVEBOMB_UP; 
                    return EBlockType.BLOCKTYPE_MOVEBOMB_UP;
                case 'B': // ITEM_BOMB; 
                    return EBlockType.BLOCKTYPE_ITEM_BOMB;
                case 'K': // ITEM_KICK; 
                    return EBlockType.BLOCKTYPE_ITEM_KICK;
                case 'F': // ITEM_FLAME; 
                    return EBlockType.BLOCKTYPE_ITEM_FLAME;
                case 'S': // ITEM_ROLLER; 
                    return EBlockType.BLOCKTYPE_ITEM_ROLLER;
                case 'P': // ITEM_PUNCH; 
                    return EBlockType.BLOCKTYPE_ITEM_PUNCH;
                case 'T': // ITEM_THROW; 
                    return EBlockType.BLOCKTYPE_ITEM_THROW;
                case 'Z': // ITEM_REMOTES; 
                    return EBlockType.BLOCKTYPE_ITEM_REMOTE;
                case 'C': // ITEM_SKULL; 
                    return EBlockType.BLOCKTYPE_ITEM_SKULL;
                case 'V': // ITEM_SHIELD; 
                    return EBlockType.BLOCKTYPE_ITEM_SHIELD;
                case 'I': // ITEM_STRONGWEAK; 
                    return EBlockType.BLOCKTYPE_ITEM_STRONGWEAK;
                default:
                    return EBlockType.BLOCKTYPE_FREE;
            }

        }

    }
}
