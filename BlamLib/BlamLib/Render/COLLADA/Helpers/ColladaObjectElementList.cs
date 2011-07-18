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

namespace BlamLib.Render.COLLADA
{
	public class ColladaObjectElementList<T> : ColladaObject
		where T : new()
	{
		public List<T> Value { get; set; }

		public override object GetValue() { return Value; }
		public override Type GetObjectType() { return typeof(T); }
		public override string GetTypeName() { return typeof(T).Name; }

		public override void ValidateField(Enums.ColladaElementType parent_type)
		{
			if (Value != null && typeof(T).IsSubclassOf(typeof(ColladaElement)))
				foreach (var element in Value)
					(element as ColladaElement).ValidateElement(parent_type);
		}
	};
}