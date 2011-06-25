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
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaCommonFloatOrParamType : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaSIDValue<float>> _float;
		ColladaObjectElement<ColladaParam> _param;
		#endregion

		#region Children
		[XmlElement("float")]
		public ColladaSIDValue<float> Float
		{ get { return _float.Value; } set { _float.Value = value; } }

		[XmlElement("param")]
		public ColladaParam Param
		{ get { return _param.Value; } set { _param.Value = value; } }
		#endregion

		public ColladaCommonFloatOrParamType() : base(Enums.ColladaElementType.Fx_CommonFloatOrParamType)
		{
			Fields.Add(_float = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_param = new ColladaObjectElement<ColladaParam>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_float);
			field_list.Add(_param);

			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
		}

		//public override void ValidateElement(Enums.ColladaElementType parent_type)
		//{
		//    #region Validate Self
		//    bool has_float = (Float != null);
		//    bool has_param = (Param != null);

		//    if (!(has_float || has_param))
		//        throw new ColladaValidationException(
		//            String.Format(ColladaValidationException.Required, "float, param"));
		//    else if (has_float && has_param)
		//        throw new ColladaValidationException(
		//            String.Format(ColladaValidationException.MutuallyExclusive, "float, param"));
		//    #endregion

		//    #region Validate Children
		//    try
		//    {
		//        if (Param != null) Param.ValidateElement(m_element_type);
		//    }
		//    catch (Exception e)
		//    {
		//        throw new ColladaValidationException(e);
		//    }
		//    #endregion
		//}
	}
}