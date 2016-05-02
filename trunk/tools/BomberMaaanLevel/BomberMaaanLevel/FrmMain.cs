using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BomberMaaanLevel
{
    public partial class FrmMain : Form
    {

        private LevelManager levelManager = new LevelManager();

        const int imageWidth = 50;
        const int imageHeight = 50;

        const int nbTilesX = 15;
        const int nbTilesY = 13;

        PictureBox[,] sTiles = new PictureBox[nbTilesX, nbTilesY];

        public FrmMain()
        {
            InitializeComponent();
        }

        private void FrmMain_Load(object sender, EventArgs e)
        {

            this.Text = "BomberMaaan Level Editor";

            for (int i = 0; i < nbTilesX; i++)
            {
                for (int j = 0; j < nbTilesY; j++)
                {

                    PictureBox aTile = new PictureBox();
                    this.Controls.Add(aTile);

                    sTiles[i, j] = aTile;
                }
            }

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

            OpenFileDialog theDialog = new OpenFileDialog();

            theDialog.Title = "Open Text File";
            theDialog.Filter = "TXT files|*.txt";

            if (theDialog.ShowDialog() == DialogResult.OK)
            {

                levelManager.OpenFile(theDialog.FileName);

                UpdateMap();

            }

        }

        private void UpdateMap()
        {

            for (int i = 0; i < nbTilesX; i++)
            {
                for (int j = 0; j < nbTilesY; j++)
                {
                    PictureBox pic = sTiles[i, j];

                    pic.Left = i * imageWidth + 10;
                    pic.Top = j * imageHeight + 30;
                    pic.Width = imageWidth;
                    pic.Height = imageHeight;
                    pic.SizeMode = PictureBoxSizeMode.StretchImage;

                    EBlockType blockType = levelManager.BlockType(i, j);

                    pic.Image = Resource1.Free;

                    switch (blockType)
                    {
                        case EBlockType.BLOCKTYPE_UNKNOWN:
                            break;
                        case EBlockType.BLOCKTYPE_HARDWALL:
                            pic.Image = Resource1.HardWall;
                            break;
                        case EBlockType.BLOCKTYPE_SOFTWALL:
                            pic.Image = Resource1.SoftWall;
                            break;
                        case EBlockType.BLOCKTYPE_RANDOM:
                            break;
                        case EBlockType.BLOCKTYPE_FREE:
                            pic.Image = Resource1.Free;
                            break;
                        case EBlockType.BLOCKTYPE_WHITEBOMBER:
                            break;
                        case EBlockType.BLOCKTYPE_BLACKBOMBER:
                            break;
                        case EBlockType.BLOCKTYPE_REDBOMBER:
                            break;
                        case EBlockType.BLOCKTYPE_BLUEBOMBER:
                            break;
                        case EBlockType.BLOCKTYPE_GREENBOMBER:
                            break;
                        case EBlockType.BLOCKTYPE_MOVEBOMB_RIGHT:
                            break;
                        case EBlockType.BLOCKTYPE_MOVEBOMB_DOWN:
                            break;
                        case EBlockType.BLOCKTYPE_MOVEBOMB_LEFT:
                            break;
                        case EBlockType.BLOCKTYPE_MOVEBOMB_UP:
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_BOMB:
                            pic.Image = Resource1.ItemBomb;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_FLAME:
                            pic.Image = Resource1.ItemFlame;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_ROLLER:
                            pic.Image = Resource1.ItemRoller;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_KICK:
                            pic.Image = Resource1.ItemKick;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_THROW:
                            pic.Image = Resource1.ItemThrow;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_PUNCH:
                            pic.Image = Resource1.ItemPunch;
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_SKULL:
                            break;
                        case EBlockType.BLOCKTYPE_ITEM_REMOTES:
                            break;
                        default:
                            break;
                    }

                }
            }

            this.Width = nbTilesX * (imageWidth + 2);
            this.Height = nbTilesY * (imageHeight + 2) + 60;

        }

    }
}
