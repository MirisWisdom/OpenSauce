/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	public interface ITagImportInfo
	{
		global_tag_import_info_block GetImportInfo();

		global_error_report_categories_block GetErrors();
	};


	#region globals
	partial class globals_group : ICacheLanguagePackContainer
	{
		public s_cache_language_pack LanguagePackGet(LanguageType lang)
		{
			switch(lang)
			{
				case LanguageType.English:		return English;
				case LanguageType.Japanese:		return Japanese;
				case LanguageType.German:		return German;
				case LanguageType.French:		return French;
				case LanguageType.Spanish:		return Spanish;
				case LanguageType.Italian:		return Italian;
				case LanguageType.Korean:		return Korean;
				case LanguageType.Chinese:		return Chinese;
				case LanguageType.Portuguese:	return Portuguese;
				default:						throw new Debug.Exceptions.UnreachableException(lang);
			}
		}

		internal void LanguagePacksReadFromCache(Halo2.CacheFile c)
		{
			if (!English.IsLoaded)		English.ReadFromCache(c);
			if (!Japanese.IsLoaded)		Japanese.ReadFromCache(c);
			if (!German.IsLoaded)		German.ReadFromCache(c);
			if (!French.IsLoaded)		French.ReadFromCache(c);
			if (!Spanish.IsLoaded)		Spanish.ReadFromCache(c);
			if (!Italian.IsLoaded)		Italian.ReadFromCache(c);
			if (!Korean.IsLoaded)		Korean.ReadFromCache(c);
			if (!Chinese.IsLoaded)		Chinese.ReadFromCache(c);
			if (!Portuguese.IsLoaded)	Portuguese.ReadFromCache(c);
		}
	};
	#endregion
}