using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace OpenSauceIDE.ModelExtractor.UI
{
    public partial class PathControl : UserControl
        , INotifyPropertyChanged
    {
        private string mTitle;
        private string mSelectedPath;

        [Category("Path Control")]
        public string Title
        {
            get { return mTitle; }
            set
            {
                mTitle = value;
                OnPropertyChanged("Title");
            }
        }

        [Category("Path Control")]
        public string SelectedPath
        {
            get { return mSelectedPath; }
            set
            {
                mSelectedPath = value;
                OnPropertyChanged("SelectedPath");
            }
        }

        [Category("Path Control")]
        public string Description { get; set; }

        [Category("Path Control")]
        public Color BackColorExists { get; set; }

        [Category("Path Control")]
        public Color BackColorMissing { get; set; }

        [Category("Path Control")]
        public Color ForeColorExists { get; set; }

        [Category("Path Control")]
        public Color ForeColorMissing { get; set; }
        
        #region Property Changed
        /// <summary>   Event queue for all listeners interested in PropertyChanged events. </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Executes the property changed action. </summary>
        ///
        /// <param name="propertyName"> Name of the property. </param>
        private void OnPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));
            }
        }
        #endregion

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets a value indicating whether the current path exists. </summary>
        ///
        /// <value> true if the path exists, false if not. </value>
        public bool Exists
        {
            get { return Directory.Exists(SelectedPath); }
        }

        /// <summary>   Default constructor. </summary>
        public PathControl()
        {
            InitializeComponent();

            mTitleLabel.DataBindings.Add(new Binding("Text", this, "Title"));
            mPathTextBox.DataBindings.Add(new Binding("Text", this, "SelectedPath"));
            mPathTextBox.TextChanged += (sender, e) => { UpdateColors(); };

            BackColorExists = Color.White;
            BackColorMissing = Color.White;

            ForeColorExists = Color.Black;
            ForeColorMissing = Color.Red;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Opens the folder browser to get a directory from the user. </summary>
        ///
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        Event information. </param>
        private void Browse(object sender, EventArgs e)
        {
            var browser = new FolderBrowserDialog();

            browser.Description = Description;
            browser.ShowNewFolderButton = false;

            if (browser.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                SelectedPath = browser.SelectedPath;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        ///     Updates the color of the text box according to whether the current path exists.
        /// </summary>
        private void UpdateColors()
        {
            if (Directory.Exists(mPathTextBox.Text))
            {
                mPathTextBox.BackColor = BackColorExists;
                mPathTextBox.ForeColor = ForeColorExists;
            }
            else
            {
                mPathTextBox.BackColor = BackColorMissing;
                mPathTextBox.ForeColor = ForeColorMissing;
            }
        }
    }
}
