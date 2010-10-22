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