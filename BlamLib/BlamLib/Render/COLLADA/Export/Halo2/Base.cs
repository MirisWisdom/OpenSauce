using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib.Blam;
using BlamLib.Render;

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaExporterH2 : ColladaExporter
	{
		#region Class Fields
		private IHalo2ShaderDatumList shaderInfo;

		protected Managers.TagIndexBase tagIndex;
		protected Managers.TagManager tagManager;
		protected string tagName;

		protected List<DatumIndex> bitmapDatums = new List<DatumIndex>();

		protected List<Fx.ColladaImage> listImage = new List<Fx.ColladaImage>();
		protected List<Fx.ColladaEffect> listEffect = new List<Fx.ColladaEffect>();
		protected List<Fx.ColladaMaterial> listMaterial = new List<Fx.ColladaMaterial>();
		#endregion

		#region Constructor
		protected ColladaExporterH2() { }
		public ColladaExporterH2(IHalo2ShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base()
		{
			shaderInfo = info;
			tagIndex = tag_index;
			tagManager = tag_manager;
			tagName = Path.GetFileNameWithoutExtension(tagManager.Name);
		}
		#endregion

		#region Helpers
		/// <summary>
		/// Returns true if the DatumIndex is neither null, nor sentinel
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		protected bool IsDatumValid(DatumIndex index)
		{
			return (!index.Equals(DatumIndex.Null) && !Managers.TagIndex.IsSentinel(index));
		}

		/// <summary>
		/// Gets a tag definition instance from the tag index
		/// </summary>
		/// <typeparam name="T">Tag definition type to return</typeparam>
		/// <param name="tag_datum">Datum index of the tag</param>
		/// <param name="cast_from_group">The tag group of the tag</param>
		/// <param name="cast_to_group">The tag group of the definition we are casting to</param>
		/// <returns></returns>
		protected T GetTagDefinition<T>(DatumIndex tag_datum,
			TagInterface.TagGroup cast_from_group,
			TagInterface.TagGroup cast_to_group)
			where T : TagInterface.Definition
		{
			// report if the datum index is invalid
			if (!IsDatumValid(tag_datum))
			{
				OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex,
					tag_datum.ToString()));
				return null;
			}

			// attempt to cast the definition to the specified type
			T tag = tagIndex[tag_datum].TagDefinition as T;
			// if the cast failed, throw an exception
			if (tag == null)
				throw new ColladaException(String.Format(
					ColladaExceptionStrings.InvalidDefinitionCast,
					tagIndex[tag_datum].Name,
					cast_from_group.ToString(), cast_to_group.ToString()));

			return tag;
		}
		#endregion
	}
}
