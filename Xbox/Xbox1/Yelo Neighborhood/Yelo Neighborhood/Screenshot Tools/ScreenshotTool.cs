using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;
using YeloDebug;
using System.IO;
using System.Drawing;

namespace Yelo_Neighborhood
{
    public partial class ScreenshotTool : Form
    {
		List<Image> Images = new List<Image>();
        ImageFormatSelector IFS = new ImageFormatSelector();

        public ScreenshotTool()
        {
            InitializeComponent();
            cboViewStyle.Items.Add(ImageLayout.Center);
            cboViewStyle.Items.Add(ImageLayout.Stretch);
            cboViewStyle.SelectedIndex = 0;
        }

        public void TakeScreenshot()
        {
            if (!LiveStreamRunning && !Program.XBox.Ping()) new Settings().ShowDialog();
            DateTime now = DateTime.Now;
            listImages.Items.Add(new ListViewItem(now.ToString() + " " + now.Second.ToString() + "." + now.Millisecond.ToString(), imageList.Images.Count, listImages.Groups[0]));
            Pauser.Reset();
            if(LiveStreamRunning) Waiter.WaitOne();
			Image screenshot = Program.XBox.Screenshot();
            Pauser.Set();
            Images.Add(screenshot);
            imageList.Images.Add(screenshot);
            if (!checkLiveStream.Checked)
            {
                listImages.SelectedItems.Clear();
                listImages.SelectedIndices.Clear();
                listImages.Items[listImages.Items.Count - 1].Selected = true;
            }
            cmdSaveChecked.Enabled = true;
            cmdSaveSelected.Enabled = true;
        }

        void ScreenshotTool_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            { 
                case Keys.F5:
                    TakeScreenshot();
                    break;
            }
        }

        void captureF5ToolStripMenuItem_Click(object sender, EventArgs e)
        { TakeScreenshot(); }

        void listImages_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        { imageBox.BackgroundImage = Images[e.Item.ImageIndex]; }

        void checkLiveStream_CheckedChanged(object sender, EventArgs e)
        {
            if (checkLiveStream.Checked)
            {
                if (!Program.XBox.Ping()) new Settings().ShowDialog();
                checkLiveStream.Text = "Live Stream (On)";
                new Thread(new ParameterizedThreadStart(LiveStream)).Start(imageBox);
            }
            else
            {
                LiveStreamRunning = false;
                checkLiveStream.Text = "Live Stream (Off)";
            }
        }

        static bool LiveStreamRunning = false;
        static ManualResetEvent Pauser = new ManualResetEvent(true);
        static ManualResetEvent Waiter = new ManualResetEvent(false);
        static void LiveStream(object pictureBox)
        {
            PictureBox imageBox = (PictureBox)pictureBox;
            Program.XBox.SetFileCacheSize(1);
            XboxVideoStream xvs = new XboxVideoStream(Program.XBox, XboxVideoStream.VideoSize.Medium, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.Immediate);
            LiveStreamRunning = true;
            xvs.Begin();
            while (LiveStreamRunning)
            {
                Waiter.Reset();
                imageBox.BackgroundImage = xvs.NextFrame();//imageBox.BackgroundImage = Program.XBox.Screenshot();
                Waiter.Set();
                Pauser.WaitOne();
            }
            xvs.End();
            Program.ScreenshotTool.checkLiveStream.Checked = false;
        }

        void ScreenshotTool_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                Hide();
            }
            LiveStreamRunning = false;
        }

        void cmdSaveChecked_Click(object sender, EventArgs e)
        {
            if (listImages.CheckedItems.Count == 0) return;
            if (FBD.ShowDialog() != DialogResult.OK) return;
            if (IFS.ShowDialog() != DialogResult.OK) return;

            foreach (ListViewItem lvt in listImages.CheckedItems)
            {
                string name = lvt.Text;
                foreach (char c in Path.GetInvalidFileNameChars()) name = name.Replace(c, '_');

                Image outImage = new Bitmap(Images[lvt.ImageIndex], Images[0].Width, Images[0].Height);
                using (var fs = new FileStream(Path.Combine(FBD.SelectedPath, name) + "." + IFS.ImageFormat.ToString().ToLower(), FileMode.Create))
                {
                    outImage.Save(fs, IFS.ImageFormat);
                }
            }
        }

        void cmdSaveSelected_Click(object sender, EventArgs e)
        {
            if (listImages.SelectedItems.Count == 0) return;
            if (FBD.ShowDialog() != DialogResult.OK) return;
            if (IFS.ShowDialog() != DialogResult.OK) return;

            foreach (ListViewItem lvt in listImages.SelectedItems)
            {
                string name = lvt.Text;
                foreach (char c in Path.GetInvalidFileNameChars()) name = name.Replace(c, '_');

                Image outImage = new Bitmap(Images[lvt.ImageIndex], Images[0].Width, Images[0].Height);
                using (var fs = new FileStream(Path.Combine(FBD.SelectedPath, name) + "." + IFS.ImageFormat.ToString().ToLower(), FileMode.Create))
				{
                    outImage.Save(fs, IFS.ImageFormat);
                }
            }
        }

        void cboViewStyle_SelectedIndexChanged(object sender, EventArgs e)
        {
            imageBox.BackgroundImageLayout = (ImageLayout)cboViewStyle.SelectedItem;
        }
    };
}