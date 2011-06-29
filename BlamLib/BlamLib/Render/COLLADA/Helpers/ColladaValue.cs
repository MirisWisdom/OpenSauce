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
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render
{
	namespace COLLADA
	{
		[SerializableAttribute()]
		[XmlTypeAttribute(AnonymousType = true)]
		public partial class ColladaValue<T>
		{
			public ColladaValue() { }
			public ColladaValue(T value) { Value = value; }

			[XmlTextAttribute()]
			public T Value;
		}
	}
}