/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
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