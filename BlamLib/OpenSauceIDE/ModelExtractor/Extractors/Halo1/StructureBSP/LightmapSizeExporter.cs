/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using BlamLib.Managers;
using BlamLib.Messaging;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
	/// <summary>   A bsp lightmap size exporter. </summary>
	public class LightmapSizeExporter
		: IMessageSource
	{
		#region Messaging
		private IMessageHandler mMessageHandler = new MessageHandler();

		/// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
		public event EventHandler<MessageArgs> MessageSent
		{
			add { mMessageHandler.MessageSent += value; }
			remove { mMessageHandler.MessageSent -= value; }
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Exports the provided bsp's lightmap sizes with a multiplier. </summary>
		///
		/// <param name="tagIndex">         Tag index the bsp belongs to. </param>
		/// <param name="tagManager">       The bsp tag's manager. </param>
		/// <param name="sizeMultiplier">   The lightmap size multiplier. </param>
		/// <param name="outputFile">       The bsp's output file. </param>
		public void Export(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager, int sizeMultiplier, string outputFile)
		{
			// Collect the necessary data
			var structureBSP = tagManager.TagDefinition as BlamLib.Blam.Halo1.Tags.structure_bsp_group;
			if(!TagIndex.IsValid(structureBSP.LightmapBitmaps.Datum))
			{
				mMessageHandler.SendMessage("Failed to open lightmap bitmap {0}", structureBSP.LightmapBitmaps.ToString());
				return;
			}

			var bitmapManager = tagIndex[structureBSP.LightmapBitmaps.Datum];
			if (bitmapManager == null)
			{
				mMessageHandler.SendMessage("Failed to open lightmap bitmap {0}", structureBSP.LightmapBitmaps.ToString());
				return;
			}

			if (structureBSP.Lightmaps.Count == 0)
			{
				mMessageHandler.SendMessage("BSP has no lightmaps {0}", tagManager.Name);
				return;
			}

			var bitmap = bitmapManager.TagDefinition as BlamLib.Blam.Halo1.Tags.bitmap_group;
			if (bitmap == null)
			{
				mMessageHandler.SendMessage("Failed to open lightmap bitmap {0}", structureBSP.LightmapBitmaps.ToString());
				return;
			}

			// Build the lightmap size list
			var lightmapSizeList = new List<string>();
			var bspName = Path.GetFileNameWithoutExtension(tagManager.Name);
			foreach(var lightmap in structureBSP.Lightmaps)
			{
				if(lightmap.Bitmap.Value == -1)
				{
					continue;
				}

				if(lightmap.Bitmap.Value >= bitmap.Bitmaps.Count)
				{
					mMessageHandler.SendMessage("Failed to export lightmap sizes. A lightmap bitmap and bsp do not have matching lightmap counts {0}", tagManager.Name);
					return;
				}

				var bitmapInfo = bitmap.Bitmaps[lightmap.Bitmap.Value];

				lightmapSizeList.Add(String.Format("{0}_{1}\t[{2},{3}]"
					, bspName
					, lightmap.Bitmap.Value
					, bitmapInfo.Width * sizeMultiplier
					, bitmapInfo.Height * sizeMultiplier));
			}

			// Save the sizes to file
			var sizeFileName = Path.ChangeExtension(outputFile, "lmsz");
			try
			{
				using (var outputStream = File.OpenWrite(sizeFileName))
				{
					var writer = new StreamWriter(outputStream);
					writer.Write(String.Join("\r\n", lightmapSizeList));
					writer.Flush();
				}
				mMessageHandler.SendMessage("BSP lightmap sizes saved");
			}
			catch (IOException)
			{
				mMessageHandler.SendMessage("Failed to open the lightmap sizes file");
			}
		}
	}
}
