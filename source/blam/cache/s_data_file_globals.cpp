#include <blam/blam.h>
#include <blam/cache/s_data_file_globals.h>

namespace Yelo { namespace Cache
{
	s_data_file& s_data_file_globals::get(
		const Enums::data_file_reference_type data_file)
	{
		switch (data_file)
		{
			case Enums::_data_file_reference_bitmaps:
				return this->bitmaps;
			case Enums::_data_file_reference_sounds:
				return this->sounds;
			case Enums::_data_file_reference_locale:
				return this->locale;

			YELO_ASSERT_CASE_UNREACHABLE();
		}
	}

	#if PLATFORM_TYPE == PLATFORM_TOOL
	void s_data_file_globals::save()
	{
		sounds.Save();
		locale.Save();
		bitmaps.Save();
	}

	void s_data_file_globals::preprocess_for_save()
	{
		sounds.PreprocessForSave();
		locale.PreprocessForSave();
		bitmaps.PreprocessForSave();
	}
	#endif
}}
