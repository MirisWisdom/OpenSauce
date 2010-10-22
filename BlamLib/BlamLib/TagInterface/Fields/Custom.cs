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
﻿using System;
using System.Collections.Generic;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Base class for a special field which contains data which can't
	/// be described by our standard tag field types. CustomData fields 
	/// should never be used for fields which are meant to be interfaced 
	/// with in a UI
	/// </summary>
	public abstract class CustomDataField : Field
	{
		protected CustomDataField() : base(FieldType.Custom) { }
		protected CustomDataField(CustomDataField from) : base(FieldType.Custom) { }

		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }

		public abstract int Sizeof(BlamVersion game, bool cache);

		public virtual bool ByteSwap(Definition owner) { return true; }
	};
}