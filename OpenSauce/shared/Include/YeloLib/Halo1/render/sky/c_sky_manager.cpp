/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/sky/c_sky_manager.hpp>

#ifdef API_DEBUG
#include <blamlib/Halo1/scenario/sky_definitions.hpp>
#endif

namespace Yelo
{
    namespace Render
    {
        namespace Sky
        {
#pragma region s_sky_entry
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Initializes a new instance of the s_sky_entry class that overrides another sky entry. </summary>
            ///
            /// <param name="sky_index">	Datum index of the sky tag. </param>
            /// <param name="sky_entry">	The sky entry being overridden. </param>
            c_sky_manager::s_sky_entry::s_sky_entry(const datum_index sky_tag_index, const sky_entry_ptr_t sky_entry)
                : m_sky_tag_index(sky_tag_index),
                  m_is_override(true),
                  m_original_sky_entry(sky_entry) { }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Initializes a new instance of the s_sky_entry class. </summary>
            ///
            /// <param name="sky_index">	Datum index of the sky tag. </param>
            c_sky_manager::s_sky_entry::s_sky_entry(const datum_index sky_tag_index)
                : s_sky_entry(sky_tag_index, nullptr)
            {
                m_is_override = false;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Query if this object is a sky override. </summary>
            ///
            /// <returns>	true if overriding another sky, false if not. </returns>
            bool c_sky_manager::s_sky_entry::IsOverride() const
            {
                return m_is_override;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets the sky datum index. </summary>
            ///
            /// <returns>	A datum_index. </returns>
            datum_index c_sky_manager::s_sky_entry::GetTagIndex() const
            {
                return m_sky_tag_index;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets the original sky pointer if set. </summary>
            ///
            /// <returns>	A c_sky_manager::sky_entry_ptr_t. </returns>
            c_sky_manager::sky_entry_ptr_t c_sky_manager::s_sky_entry::OriginalSky() const
            {
                return m_original_sky_entry;
            }
#pragma endregion s_sky_entry

#pragma region c_sky_manager
            /// <summary>	Resets the sky list, reverting all overrides. </summary>
            void c_sky_manager::Reset()
            {
                for (auto& sky : m_sky_list)
                {
                    // If the sky entry is an override, replace it with the sky stored within
                    if (sky->IsOverride())
                    {
                        sky = sky->OriginalSky();
                    }
                }
            }

            /// <summary>	Clears the sky list. </summary>
            void c_sky_manager::Clear()
            {
                m_sky_list.clear();
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Add's entries to the sky list for the skies listed in the scenario tag. </summary>
            ///
            /// <param name="skies">	The scenario's skies block. </param>
            void c_sky_manager::SetScenarioSkies(const TagBlock<tag_reference>& skies)
            {
                for (auto& sky : skies)
                {
                    m_sky_list.emplace_back(std::make_shared<s_sky_entry>(sky.tag_index));
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Sets a sky index to a different sky tag. </summary>
            ///
            /// <param name="sky_index">	Zero-based index of the sky. </param>
            /// <param name="sky_datum">	The sky tag datum index. </param>
            void c_sky_manager::SetSkyIndex(const byte sky_index, const datum_index sky_tag_index)
            {
                if (sky_index == static_cast<byte>(NONE))
                {
                    return;
                }

                // If the chosen sky is an override, get the original sky entry
                // Otherwise use the sky entry in the list
                sky_entry_ptr_t original_sky;
                if (m_sky_list[sky_index]->IsOverride())
                {
                    original_sky = m_sky_list[sky_index]->OriginalSky();
                }
                else
                {
                    original_sky = m_sky_list[sky_index];
                }

                // Create a new sky entry to override the stock one
                m_sky_list[sky_index] = std::make_shared<s_sky_entry>(sky_tag_index, original_sky);
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets a sky's datum index. </summary>
            ///
            /// <param name="sky_index">	Zero-based index of the sky. </param>
            ///
            /// <returns>	The sky tag's datum index. </returns>
            datum_index c_sky_manager::GetSkyTagIndex(const byte sky_index) const
            {
                if (sky_index == static_cast<byte>(NONE))
                {
                    return datum_index::null;
                }
                if (sky_index >= m_sky_list.size())
                {
                    return datum_index::null;
                }
                return m_sky_list[sky_index]->GetTagIndex();
            }
#pragma endregion c_sky_manager

#ifdef API_DEBUG

            class c_sky_manager_test_fixture : public Automation::i_automated_test
            {
            protected:
                cstring m_any_tag_name = "anyTagName";
                cstring m_any_other_tag_name = "anyOtherTagName";
                TagBlock<tag_reference> m_any_skies;
                tag_reference m_tag_references[2];

            public:
                c_sky_manager_test_fixture()
                    : m_any_skies()
                {
                    m_any_skies.Address = &m_tag_references[0];
                    m_any_skies.Count = 2;

                    m_tag_references[0].group_tag = TagGroups::s_sky_definition::k_group_tag;
                    m_tag_references[0].name = m_any_tag_name;
                    m_tag_references[0].tag_index = datum_index::Create(0x1111, 0x2222);

                    m_tag_references[1].group_tag = TagGroups::s_sky_definition::k_group_tag;
                    m_tag_references[1].name = m_any_other_tag_name;
                    m_tag_references[1].tag_index = datum_index::Create(0x3333, 0x4444);
                }
            };

#define YELO_UNIT_TEST_SKY_MANAGER(name) YELO_UNIT_TEST_WITH_FIXTURE(name, Yelo.Render.Sky.c_sky_manager, c_sky_manager_test_fixture)

            YELO_UNIT_TEST_SKY_MANAGER(Reset_WithOverriddenSky_ResetsSkyTagIndex)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);
                auto expected = manager.GetSkyTagIndex(1);
                auto any_datum = datum_index::Create(0x5555, 0x6666);

                manager.SetSkyIndex(1, any_datum);
                manager.Reset();
                auto result = manager.GetSkyTagIndex(1);

                AssertThat(result, Equals(expected));
            }

            YELO_UNIT_TEST_SKY_MANAGER(Clear_WithSkiesAdded_RemovesAllSkies)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);

                manager.Clear();
                auto result = manager.GetSkyTagIndex(1);

                AssertThat(result, Equals(datum_index::null));
            }

            YELO_UNIT_TEST_SKY_MANAGER(SetSkyIndex_WithNone_DoesNotChangeSkyIndex)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);
                auto expected = manager.GetSkyTagIndex(1);
                auto any_datum = datum_index::Create(0x5555, 0x6666);

                manager.SetSkyIndex(NONE, any_datum);
                auto result = manager.GetSkyTagIndex(1);

                AssertThat(result, Equals(expected));
            }

            YELO_UNIT_TEST_SKY_MANAGER(SetSkyIndex_WithIndex_ChangesSkyIndex)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);
                auto expected = datum_index::Create(0x5555, 0x6666);

                manager.SetSkyIndex(1, expected);
                auto result = manager.GetSkyTagIndex(1);

                AssertThat(result, Equals(expected));
            }

            YELO_UNIT_TEST_SKY_MANAGER(GetSkyTagIndex_WithNone_ReturnsNullDatum)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);

                auto result = manager.GetSkyTagIndex(NONE);

                AssertThat(result, Equals(datum_index::null));
            }

            YELO_UNIT_TEST_SKY_MANAGER(GetSkyTagIndex_WithOutOfBoundsIndex_ReturnsNullDatum)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);

                auto result = manager.GetSkyTagIndex(3);

                AssertThat(result, Equals(datum_index::null));
            }

            YELO_UNIT_TEST_SKY_MANAGER(GetSkyTagIndex_WithIndex_ReturnsTagIndex)
            {
                c_sky_manager manager;
                manager.SetScenarioSkies(m_any_skies);

                auto result = manager.GetSkyTagIndex(1);

                AssertThat(result.index, Equals(0x3333));
            }

#undef YELO_UNIT_TEST_SKY_MANAGER
#endif
        };
    };
};
