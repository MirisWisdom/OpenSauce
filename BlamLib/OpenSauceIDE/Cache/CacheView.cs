/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Windows.Forms;

namespace OpenSauceIDE.Cache
{
	public partial class CacheView : Form
	{
		/// <summary>The valid engines which CheApe can be used with</summary>
		public const BlamLib.BlamVersion kAllowedPlatforms = 
			BlamLib.BlamVersion.Halo1 | BlamLib.BlamVersion.Stubbs |
			BlamLib.BlamVersion.Halo2 |
			BlamLib.BlamVersion.Halo3 | BlamLib.BlamVersion.HaloOdst | BlamLib.BlamVersion.HaloReach;
		const string kDlgName = "Cache Viewer";

		BlamLib.BlamVersion m_engine;
		BlamLib.Blam.CacheFile m_cache;

		public CacheView(BlamLib.BlamVersion engine)
		{
			InitializeComponent();

			m_engine = engine;
			m_cache = null;
		}

		void SetCacheViewerPath(string cache_path)
		{
			this.Text = string.Format("{0} - [{1}]", kDlgName, cache_path);
		}
		void OnCacheLoaded(string cache_path)
		{
			FileClose.Enabled = true;

			SetCacheViewerPath(cache_path);
		}
		void OnCacheClosed()
		{
			m_cache = null;

			FileClose.Enabled = false;
			SetCacheViewerPath("No File Loaded");
		}

		object OnFileFinishedOpening(Exception ex)
		{
			if (ex != null)
			{
				OnCacheClosed();

				if (ex is BlamLib.Blam.InvalidCacheFileException)
					MessageBox.Show(this, string.Format("Selected cache file has an invalid format. Refer to debug logs for more information"),
						"Error opening cache", MessageBoxButtons.OK, MessageBoxIcon.Error);
				else
					MessageBox.Show(this, string.Format("Exception:{0}{1}", BlamLib.Program.NewLine, ex),
						"Error opening cache", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}

			if (m_cache != null)
				OnCacheLoaded(OpenFileDlg.FileName);

			return null; // dummy
		}
		void OnFileOpen(object sender, EventArgs e)
		{
			if (m_cache == null && OpenFileDlg.ShowDialog(this) == DialogResult.OK)
			{
				var thread = new System.Threading.Thread(() =>
				{
					Exception except = null;
					var gd = BlamLib.Program.GetManager(m_engine);
					var cid = BlamLib.Blam.DatumIndex.Null;
					try
					{
						cid = gd.OpenCacheFile(m_engine, OpenFileDlg.FileName);
						m_cache = gd.GetCacheFile(cid);
						m_cache.Read();
					}
					catch (Exception ex)
					{
						except = ex;

						if (!cid.IsNull && m_cache != null)
							gd.CloseCacheFile(m_cache.CacheId);
						m_cache = null;
					}
					this.BeginInvoke(new Func<Exception,object>(OnFileFinishedOpening), except);
				});
				SetCacheViewerPath("Loading...");
				thread.Start();
			}
		}

		void OnFileClose(object sender, EventArgs e)
		{
			if (m_cache != null)
			{
				var gd = BlamLib.Program.GetManager(m_engine);
				gd.CloseCacheFile(m_cache.CacheId);

				OnCacheClosed();
			}
		}

		void OnFormShown(object sender, EventArgs e)
		{
			OnFileOpen(this, null);
		}
		void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			OnFileClose(this, null);
		}
	};
}