using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace BomberMaaanLevel
{
    public partial class FrmMain : Form
    {

        private LevelManager levelManager = new LevelManager();

        private const int imageWidth = 50;
        private const int imageHeight = 50;

        private const int nbTilesX = 15;
        private const int nbTilesY = 13;

        private int ci = -1;
        private int cj = -1;

        PictureBox[,] sTiles = new PictureBox[nbTilesX, nbTilesY];

        public struct TileId
        {
            public int i, j;

            public TileId(int i, int j)
            {
                this.i = i;
                this.j = j;
            }

        };

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

                    TileId aTileId = new TileId(i, j);

                    aTile.Top = -100;
                    aTile.Left = -100;
                    aTile.Width = imageWidth;
                    aTile.Height = imageHeight;

                    aTile.Tag = aTileId; 
                    aTile.Click += aTile_Click;

                    aTile.BackColor = Color.Gray;
                    aTile.Padding = new System.Windows.Forms.Padding(1);

                    this.Controls.Add(aTile);

                    sTiles[i, j] = aTile;
                }
            }

            int k = 0;
            int l = 0;

            int [] spacing = new int[4]  { 4,  5, 4, 8};

            foreach (EBlockType val in Enum.GetValues(typeof(EBlockType)))
            {
                PictureBox aBlockType = new PictureBox();

                aBlockType.Left = 800 + l * imageWidth;
                aBlockType.Top = k * imageHeight + 30;
                aBlockType.Width = imageWidth;
                aBlockType.Height = imageHeight;
                aBlockType.SizeMode = PictureBoxSizeMode.StretchImage;

                k++;

                if (k >= spacing[l])
                {
                    k = 0;
                    l++;
                }

                aBlockType.Tag = val;
                aBlockType.Image = GetImage(val);

                aBlockType.Click += aBlockType_Click;

                this.Controls.Add(aBlockType);
            }

            this.Width = 1100;
            this.Height = 736;

        }

        void aBlockType_Click(object sender, EventArgs e)
        {

            if (ci == -1)
                return;

            if (cj == -1)
                return;

            PictureBox aPicBox = (PictureBox)sender;

            EBlockType aBlockType = (EBlockType)aPicBox.Tag;

            Select(aBlockType);

        }

        void aTile_Click(object sender, EventArgs e)
        {

            PictureBox aPicBox = (PictureBox)sender;

            TileId aTileId = (TileId)(aPicBox).Tag;

            ci = aTileId.i;
            cj = aTileId.j;

            ShowSelection(aPicBox);

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

            OpenFileDialog theDialog = new OpenFileDialog();

            theDialog.Title = "Open Text File";
            theDialog.Filter = "TXT files|*.TXT";

            if (theDialog.ShowDialog() == DialogResult.OK)
            {

                levelManager.OpenFile(theDialog.FileName);

                UpdateMap();

                this.Text = "BomberMaaan Level Editor" + " - " + Path.GetFileName(theDialog.FileName);

            }

        }

        private Bitmap GetImage(EBlockType aBlockType)
        {

            switch (aBlockType)
            {
                case EBlockType.BLOCKTYPE_HARDWALL:
                    return Resource1.HardWall;
                case EBlockType.BLOCKTYPE_SOFTWALL:
                    return Resource1.SoftWall;
                case EBlockType.BLOCKTYPE_RANDOM:
                    return Resource1.Random;
                case EBlockType.BLOCKTYPE_FREE:
                    return Resource1.Free;
                case EBlockType.BLOCKTYPE_WHITEBOMBER:
                    return Resource1.WhiteBomber;
                case EBlockType.BLOCKTYPE_BLACKBOMBER:
                    return Resource1.BlackBomber;
                case EBlockType.BLOCKTYPE_REDBOMBER:
                    return Resource1.RedBomber;
                case EBlockType.BLOCKTYPE_BLUEBOMBER:
                    return Resource1.BlueBomber;
                case EBlockType.BLOCKTYPE_GREENBOMBER:
                    return Resource1.GreenBomber;
                case EBlockType.BLOCKTYPE_MOVEBOMB_RIGHT:
                    return Resource1.MoveRight;
                case EBlockType.BLOCKTYPE_MOVEBOMB_DOWN:
                    return Resource1.MoveDown;
                case EBlockType.BLOCKTYPE_MOVEBOMB_LEFT:
                    return Resource1.MoveLeft;
                case EBlockType.BLOCKTYPE_MOVEBOMB_UP:
                    return Resource1.MoveUp;
                case EBlockType.BLOCKTYPE_ITEM_BOMB:
                    return Resource1.ItemBomb;
                case EBlockType.BLOCKTYPE_ITEM_FLAME:
                    return Resource1.ItemFlame;
                case EBlockType.BLOCKTYPE_ITEM_ROLLER:
                    return Resource1.ItemRoller;
                case EBlockType.BLOCKTYPE_ITEM_KICK:
                    return Resource1.ItemKick;
                case EBlockType.BLOCKTYPE_ITEM_THROW:
                    return Resource1.ItemThrow;
                case EBlockType.BLOCKTYPE_ITEM_PUNCH:
                    return Resource1.ItemPunch;
                case EBlockType.BLOCKTYPE_ITEM_SKULL:
                case EBlockType.BLOCKTYPE_ITEM_REMOTES:
                default:
                    break;
            }

            return Resource1.Free;

        }

        private void UpdateTile(int i, int j)
        {

            PictureBox aPicBox = sTiles[i, j];

            aPicBox.Left = i * imageWidth + 10;
            aPicBox.Top = j * imageHeight + 30;
            aPicBox.Width = imageWidth;
            aPicBox.Height = imageHeight;
            aPicBox.SizeMode = PictureBoxSizeMode.StretchImage;

            EBlockType aBlockType = levelManager.BlockType(i, j);
            aPicBox.Image = GetImage(aBlockType);

        }

        private void UpdateMap()
        {

            for (int i = 0; i < nbTilesX; i++)
            {
                for (int j = 0; j < nbTilesY; j++)
                {

                    this.UpdateTile(i, j);

                }
            }

            this.Width = nbTilesX * (imageWidth + 2) + 20 + 300;
            this.Height = nbTilesY * (imageHeight + 2) + 60;

        }

        private void ClearSelection()
        {

            for (int i = 0; i < nbTilesX; i++)
            {
                for (int j = 0; j < nbTilesY; j++)
                {
                    PictureBox aPicBox = sTiles[i, j];

                    aPicBox.BackColor = Color.Gray;
                    aPicBox.Padding = new System.Windows.Forms.Padding(1);

                }
            }

        }

        private void ShowSelection(PictureBox aPicBox)
        {

            this.ClearSelection();

            aPicBox.BackColor = Color.Red;
            aPicBox.Padding = new System.Windows.Forms.Padding(1);

            EBlockType aBlockType = levelManager.BlockType(ci, cj);
            //aPicBox.Image = GetImage(aBlockType);
            
        }

        private void Select(EBlockType aBlockType)
        {

            levelManager.SetBlockType(ci, cj, aBlockType);

            this.UpdateTile(ci, cj);

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {

            Application.Exit();

        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {

            SaveFileDialog theDialog = new SaveFileDialog();

            theDialog.Title = "Open Text File";
            theDialog.Filter = "TXT files|*.TXT";

            if (theDialog.ShowDialog() == DialogResult.OK)
            {

                levelManager.NewFile(theDialog.FileName);

                UpdateMap();

                this.Text = "BomberMaaan Level Editor" + " - " + Path.GetFileName(theDialog.FileName);

            }

        }

    }
}
