using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;
using YeloDebug;
using System.IO;

namespace Yelo_Neighborhood
{
    public partial class ScreenshotTool : Form
    {
		List<System.Drawing.Image> Images = new List<System.Drawing.Image>();
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
            listImages.Items.Add(new ListViewItem(DateTime.Now.ToString(), imageList.Images.Count, listImages.Groups[0]));
            Pauser.Reset();
            if(LiveStreamRunning) Waiter.WaitOne();
			System.Drawing.Image screenshot = Program.XBox.Screenshot();
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
            if (FBD.ShowDialog() != DialogResult.OK) return;
            if (IFS.ShowDialog() != DialogResult.OK) return;

            foreach (ListViewItem lvt in listImages.CheckedItems)
            {
                string name = lvt.Text;
                foreach (char c in Path.GetInvalidFileNameChars()) name = name.Replace(c, '_');
                MemoryStream ms = new MemoryStream();
                Images[lvt.ImageIndex].Save(ms, IFS.ImageFormat);
                byte[] data = new byte[(int)ms.Length];
                ms.Read(data, 0, data.Length);
                ms.Close();
                FileStream fs = new FileStream(Path.Combine(FBD.SelectedPath, name) + "." + IFS.ImageFormat.ToString().ToLower(), FileMode.Create);
                fs.Write(data, 0, data.Length);
                fs.Flush();
                fs.Close();
            }
        }

        void cmdSaveSelected_Click(object sender, EventArgs e)
        {
            if (FBD.ShowDialog() != DialogResult.OK) return;
            if (IFS.ShowDialog() != DialogResult.OK) return;

            foreach (ListViewItem lvt in listImages.SelectedItems)
            {
                string name = lvt.Text;
                foreach (char c in Path.GetInvalidFileNameChars()) name = name.Replace(c, '_');

				byte[] data;
				using (var ms = new MemoryStream())
				{
					Images[lvt.ImageIndex].Save(ms, IFS.ImageFormat);
					data = new byte[(int)ms.Length];
					ms.Read(data, 0, data.Length);
				}

                using (var fs = new FileStream(Path.Combine(FBD.SelectedPath, name) + "." + IFS.ImageFormat.ToString().ToLower(), FileMode.Create))
				{
					fs.Write(data, 0, data.Length);
                }
            }
        }

        void cboViewStyle_SelectedIndexChanged(object sender, EventArgs e)
        {
            imageBox.BackgroundImageLayout = (ImageLayout)cboViewStyle.SelectedItem;
        }
    };
}