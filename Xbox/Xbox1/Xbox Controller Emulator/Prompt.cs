/*
    OpenSauceBox: SDK for Xbox User Modding

    Copyright (C)  Kornner Studios (http://kornner.com)

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
using System.Windows.Forms;

namespace Xbox_Controller_Emulator
{
    public partial class Prompt : Form
    {
        public string DebugName;

        public Prompt()
        {
            InitializeComponent();
        }

        void txtDebugName_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && txtDebugName.Text.Length > 0)
            {
                DebugName = txtDebugName.Text;
                Dispose();
            }
        }
    };
}