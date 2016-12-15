/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using System.Linq;
using System.Text;

namespace BlamLib.TagInterface
{
	/// <summary>   A helper class to process paths used in blam. </summary>
	public class BlamPath
	{
		public string AbsoluteFolder { get; private set; }
		public string EndFolder { get; private set; }
		public string Root { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="rootPath"> Full pathname of the tags file. </param>
		public BlamPath(string rootPath)
		{
			AbsoluteFolder = Path.GetFullPath(rootPath);

			var splitPath = AbsoluteFolder.Split(new char[] { Path.DirectorySeparatorChar }, StringSplitOptions.RemoveEmptyEntries);

			EndFolder = splitPath.Last();
			Root = System.String.Join(Path.DirectorySeparatorChar.ToString(), splitPath.Take(splitPath.Length - 1));
		}
	}

	/// <summary>   A helper class to process tag paths used in blam. </summary>
	public class BlamTagPath
		: BlamPath
	{
		public string AbsoluteTagPath { get; private set; }
		public string TagPath { get; private set; }
		public string TagName { get; private set; }
		public string TagExtension { get; private set; }
		public string TagNameWithExtension { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="tagsPath"> Full pathname of the tags file. </param>
		public BlamTagPath(string tagsPath)
			: base(tagsPath)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Sets the path. </summary>
		///
		/// <param name="tagPath">  Full pathname of the tag file. </param>
		public void SetPath(string tagPath)
		{
			AbsoluteTagPath = Path.GetFullPath(tagPath);

			TagName = Path.GetFileNameWithoutExtension(AbsoluteTagPath);
			TagExtension = Path.GetExtension(AbsoluteTagPath);
			TagExtension = TagExtension.TrimStart('.');
			TagNameWithExtension = Path.GetFileName(AbsoluteTagPath);

			var tagsRelative = Path.GetDirectoryName(AbsoluteTagPath);
			tagsRelative = tagsRelative.Replace(AbsoluteFolder, "");

			var tagsRelativeSplit = tagsRelative.Split(new char[] { Path.DirectorySeparatorChar }, StringSplitOptions.RemoveEmptyEntries).ToList();
			tagsRelativeSplit.Add(TagName);

			TagPath = System.String.Join(Path.DirectorySeparatorChar.ToString(), tagsRelativeSplit);
		}
	}
}
