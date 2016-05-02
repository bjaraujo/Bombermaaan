using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Ini;

namespace BomberMaaanLevel
{

    enum EBlockType
    {
        BLOCKTYPE_HARDWALL,			//!< There must be a hard wall here
        BLOCKTYPE_SOFTWALL,			//!< There must be a soft wall here
        BLOCKTYPE_RANDOM,			//!< There must be either a soft wall, or a free place here (random)
        BLOCKTYPE_FREE,				//!< There must be a free place here 
        BLOCKTYPE_WHITEBOMBER,		//!< The white bomber must start here
        BLOCKTYPE_BLACKBOMBER,		//!< The black bomber must start here
        BLOCKTYPE_REDBOMBER,		//!< The red bomber must start here
        BLOCKTYPE_BLUEBOMBER,		//!< The blue bomber must start here
        BLOCKTYPE_GREENBOMBER,		//!< The green bomber must start here
        BLOCKTYPE_MOVEBOMB_RIGHT,   //!< A bomb starts moving right if placed here
        BLOCKTYPE_MOVEBOMB_DOWN,    //!< A bomb starts moving down if placed here
        BLOCKTYPE_MOVEBOMB_LEFT,    //!< A bomb starts moving left if placed here
        BLOCKTYPE_MOVEBOMB_UP,      //!< A bomb starts moving up if placed here
        BLOCKTYPE_ITEM_BOMB,		//!< A bomb item if placed here
        BLOCKTYPE_ITEM_FLAME,		//!< A flame item if placed here
        BLOCKTYPE_ITEM_ROLLER,		//!< A roller item if placed here 
        BLOCKTYPE_ITEM_KICK,		//!< A kick item if placed here
        BLOCKTYPE_ITEM_THROW,		//!< A throw item if placed here
        BLOCKTYPE_ITEM_PUNCH,		//!< A punch item if placed here
        BLOCKTYPE_ITEM_SKULL,		//!< A skull item if placed here : TODO
        BLOCKTYPE_ITEM_REMOTES		//!< A remotes item if placed here : TODO
    };

    class LevelManager
    {

        IniFile anIniFile = null;

        public bool OpenFile(string aFileName)
        {

            anIniFile = new IniFile(aFileName);

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
                    aNewChar = ' '; // TODO
                    break;
                case EBlockType.BLOCKTYPE_ITEM_REMOTES:
                    aNewChar = ' '; // TODO
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
                default:
                    return EBlockType.BLOCKTYPE_FREE;
            }

        }

    }
}
