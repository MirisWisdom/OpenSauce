/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.HaloReach.Tags
{
	[TI.Definition]
	public sealed class field_block<FieldType> : TI.field_block<FieldType> where FieldType : TI.Field, new() { };
}