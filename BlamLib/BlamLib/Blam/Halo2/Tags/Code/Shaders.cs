/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

/*
6 value
10 color
13 anims

animated parameters - 
0000FF00 - value type (bitmap, color etc)
000000FF - index
*/
namespace BlamLib.Blam.Halo2.Tags
{
	internal interface IShaderParameter
	{
		TI.TagReference ParameterBitmap();
		TI.Real ParameterValue();
		TI.RealColor ParameterColor();
	};

	#region global_shader_parameter_block
	partial class global_shader_parameter_block
	{
		public TI.TagReference ParameterBitmap() { return Bitmap; }

		public TI.Real ParameterValue() { return ConstValue; }

		public TI.RealColor ParameterColor() { return ConstColor; }
	};
	#endregion


	#region shader_postprocess_definition_new_block
	partial class shader_postprocess_definition_new_block
	{
		#region shader_postprocess_bitmap_new_block
		partial class shader_postprocess_bitmap_new_block
		{
			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				//int index = BitmapGroup.Value & 0xFFFF;
				//if (index != -1) System.Diagnostics.Debug.WriteLine(string.Format(
				//	 "BitmapGroup: {0} {1}", index, c.Index.Tags[index].FileName));
				return true;
			}
		};
		#endregion
	};
	#endregion


	#region shader
	partial class shader_group
	{
		#region shader_properties_block
		partial class shader_properties_block
		{
			internal override bool Upgrade()
			{
				TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
				if(attr.Major == 1)
				{
					switch (attr.Minor)
					{
						case 108:
							this.Add(LightmapFoliageScale);
							break;
					}
				}
				return true;
			}
		};
		#endregion

		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			//if(attr.Major == 1)
			{
				switch(attr.Minor)
				{
					case 120:
						if (attr.Major == 0)
							this.Add(PostprocessProperties);

						this.Add(AddedDepthBiasOffset);
						this.Add(AddedDepthBiasSlopeScale);
						break;
				}
			}
			return true;
		}

		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			PredictedResources.DeleteAll();

			return true;
		}
	};
	#endregion


	#region shader_template_category_block
	partial class shader_template_category_block
	{
		#region shader_template_parameter_block
		partial class shader_template_parameter_block
		{
			#region IShaderParameter Members
			public TI.TagReference ParameterBitmap() { return DefaultBitmap; }

			public TI.Real ParameterValue() { return DefaultConstValue; }

			public TI.RealColor ParameterColor() { return DefaultConstColor; }
			#endregion
		};
		#endregion
	};
	#endregion


	#region shader_template
	partial class shader_template_group
	{
		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			if(attr.Major == 1)
			{
				switch(attr.Minor)
				{
					case 144:
						this.Add(PostprocessDefinition);
						break;
				}
			}
			return true;
		}
	};
	#endregion
}