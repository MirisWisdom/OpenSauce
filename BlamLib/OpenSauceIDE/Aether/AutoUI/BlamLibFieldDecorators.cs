using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenSauceIDE.Aether.AutoUI.TypeDescriptor;
using OpenSauceIDE.Aether.AutoUI.Attributes;

namespace OpenSauceIDE.Aether.AutoUI
{
	#region String
	public class AutoUITypeDescriptorBlamString
		: AutoUITypeDecorator<BlamLib.TagInterface.String>
	{
		public AutoUITypeDescriptorBlamString()
		{
			AddMember("Value");
		}
	}
	#endregion String

	#region Real
	public class AutoUITypeDescriptorBlamReal
		: AutoUITypeDecorator<BlamLib.TagInterface.Real>
	{
		public AutoUITypeDescriptorBlamReal()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamRealPoint2D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealPoint2D>
	{
		public AutoUITypeDescriptorBlamRealPoint2D()
		{
			AddMember("X");
			AddMember("Y");
		}
	}

	public class AutoUITypeDescriptorBlamRealPoint3D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealPoint3D>
	{
		public AutoUITypeDescriptorBlamRealPoint3D()
		{
			AddMember("X");
			AddMember("Y");
			AddMember("Z");
		}
	}

	public class AutoUITypeDescriptorBlamRealVector2D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealVector2D>
	{
		public AutoUITypeDescriptorBlamRealVector2D()
		{
			AddMember("I");
			AddMember("J");
		}
	}

	public class AutoUITypeDescriptorBlamRealVector3D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealVector3D>
	{
		public AutoUITypeDescriptorBlamRealVector3D()
		{
			AddMember("I");
			AddMember("J");
			AddMember("K");
		}
	}

	public class AutoUITypeDescriptorBlamRealQuaternion
		: AutoUITypeDecorator<BlamLib.TagInterface.RealQuaternion>
	{
		public AutoUITypeDescriptorBlamRealQuaternion()
		{
			AddMember("I");
			AddMember("J");
			AddMember("K");
			AddMember("W");
		}
	}

	public class AutoUITypeDescriptorBlamRealEulerAngles2D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealEulerAngles2D>
	{
		public AutoUITypeDescriptorBlamRealEulerAngles2D()
		{
			AddMember("Y");
			AddMember("P");
		}
	}

	public class AutoUITypeDescriptorBlamRealEulerAngles3D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealEulerAngles3D>
	{
		public AutoUITypeDescriptorBlamRealEulerAngles3D()
		{
			AddMember("Y");
			AddMember("P");
			AddMember("R");
		}
	}

	public class AutoUITypeDescriptorBlamRealPlane2D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealPlane2D>
	{
		public AutoUITypeDescriptorBlamRealPlane2D()
		{
			AddMember("I");
			AddMember("J");
			AddMember("D");
		}
	}

	public class AutoUITypeDescriptorBlamRealPlane3D
		: AutoUITypeDecorator<BlamLib.TagInterface.RealPlane3D>
	{
		public AutoUITypeDescriptorBlamRealPlane3D()
		{
			AddMember("I");
			AddMember("J");
			AddMember("D");
			AddMember("K");
		}
	}

	public class AutoUITypeDescriptorBlamRealColor
		: AutoUITypeDecorator<BlamLib.TagInterface.RealColor>
	{
		public AutoUITypeDescriptorBlamRealColor()
		{
			AddMember("A", new Attribute[] { new AutoUINameAttribute("Alpha") });
			AddMember("R", new Attribute[] { new AutoUINameAttribute("Red") });
			AddMember("G", new Attribute[] { new AutoUINameAttribute("Green") });
			AddMember("B", new Attribute[] { new AutoUINameAttribute("Blue") });
		}
	}

	public class AutoUITypeDescriptorBlamRealBounds
		: AutoUITypeDecorator<BlamLib.TagInterface.RealBounds>
	{
		public AutoUITypeDescriptorBlamRealBounds()
		{
			AddMember("Upper");
			AddMember("Lower");
		}
	}
	#endregion Real

	#region Integers
	public class AutoUITypeDescriptorBlamByteInteger
		: AutoUITypeDecorator<BlamLib.TagInterface.ByteInteger>
	{
		public AutoUITypeDescriptorBlamByteInteger()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamShortInteger
		: AutoUITypeDecorator<BlamLib.TagInterface.ShortInteger>
	{
		public AutoUITypeDescriptorBlamShortInteger()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamLongInteger
		: AutoUITypeDecorator<BlamLib.TagInterface.LongInteger>
	{
		public AutoUITypeDescriptorBlamLongInteger()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamTag
		: AutoUITypeDecorator<BlamLib.TagInterface.Tag>
	{
		public AutoUITypeDescriptorBlamTag()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamEnum
		: AutoUITypeDecorator<BlamLib.TagInterface.Enum>
	{
		public AutoUITypeDescriptorBlamEnum()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamFlags
		: AutoUITypeDecorator<BlamLib.TagInterface.Flags>
	{
		public AutoUITypeDescriptorBlamFlags()
		{
			AddMember("Value");
		}
	}

	public class AutoUITypeDescriptorBlamPoint2D
		: AutoUITypeDecorator<BlamLib.TagInterface.Point2D>
	{
		public AutoUITypeDescriptorBlamPoint2D()
		{
			AddMember("X");
			AddMember("Y");
		}
	}

	public class AutoUITypeDescriptorBlamRectangle2D
		: AutoUITypeDecorator<BlamLib.TagInterface.Rectangle2D>
	{
		public AutoUITypeDescriptorBlamRectangle2D()
		{
			AddMember("T", new Attribute[] { new AutoUINameAttribute("Top") });
			AddMember("L", new Attribute[] { new AutoUINameAttribute("Left") });
			AddMember("B", new Attribute[] { new AutoUINameAttribute("Bottom") });
			AddMember("R", new Attribute[] { new AutoUINameAttribute("Right") });
		}
	}

	public class AutoUITypeDescriptorBlamColor
		: AutoUITypeDecorator<BlamLib.TagInterface.Color>
	{
		public AutoUITypeDescriptorBlamColor()
		{
			AddMember("A", new Attribute[] { new AutoUINameAttribute("Alpha") });
			AddMember("R", new Attribute[] { new AutoUINameAttribute("Red") });
			AddMember("G", new Attribute[] { new AutoUINameAttribute("Green") });
			AddMember("B", new Attribute[] { new AutoUINameAttribute("Blue") });
		}
	}

	public class AutoUITypeDescriptorBlamShortIntegerBounds
		: AutoUITypeDecorator<BlamLib.TagInterface.ShortIntegerBounds>
	{
		public AutoUITypeDescriptorBlamShortIntegerBounds()
		{
			AddMember("Lower");
			AddMember("Upper");
		}
	}

	public class AutoUITypeDescriptorBlamBlockIndex
		: AutoUITypeDecorator<BlamLib.TagInterface.BlockIndex>
	{
		public AutoUITypeDescriptorBlamBlockIndex()
		{
			AddMember("Value");
		}
	}
	#endregion Integers
}
