using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BomberMaanLevel
{
    public partial class FrmMain : Form
    {

        private LevelManager levelManager = new LevelManager();

        public FrmMain()
        {
            InitializeComponent();
        }

        private void FrmMain_Load(object sender, EventArgs e)
        {

            this.Text = "BomberMaan Level Editor";

            this.Width = levelManager.NumberOfTilesX() * 64;
            this.Height = levelManager.NumberOfTilesY() * 64;

        }
    }
}
