using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlamLib.Render.COLLADA
{
	public class ColladaNCName
	{
		protected string mValue = "";

		public ColladaNCName(string name)
		{
			mValue = name;
		}

		public override string ToString()
		{
			return ColladaUtilities.FormatName(mValue, " ", "_");
		}

		public static implicit operator string(ColladaNCName name)
		{
			return name.ToString();
		}

		public static implicit operator ColladaNCName(string name)
		{
			return new ColladaNCName(name);
		}
	}

	public class ColladaID<T> : ColladaNCName
	{
		public ColladaID(string id)
			: base(id)
		{ }

		public override string ToString()
		{
			var baseString = base.ToString();

			if (String.IsNullOrEmpty(baseString))
			{
				return "";
			}

			if (ColladaElement.TestFormat<T>(baseString))
			{
				return baseString;
			}
			else
			{
				return ColladaElement.FormatID<T>(base.ToString());
			}
		}

		public static implicit operator string(ColladaID<T> name)
		{
			return name.ToString();
		}

		public static implicit operator ColladaID<T>(string name)
		{
			return new ColladaID<T>(name);
		}
	}
}
