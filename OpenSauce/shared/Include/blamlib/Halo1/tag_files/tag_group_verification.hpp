/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		void CheckForUnreferencedGroups();

		// Verify a field that is a child of [block_definition]
		void VerifyTagField(const tag_block_definition* block_definition, const tag_field& field);

		// Verify [block] and its child fields
		void VerifyBlockFieldDefinitions(tag_block_definition* block);

		void VerifyTagGroupTags();
		void VerifyGroupFieldDefinitions();

		// Run any checks on the tag groups, after they have been fully initialized (parents linked, byte swap codes, etc).
		// However, runtime info won't yet be generated at the time of this call
		void VerifyTagGroupsFinalChecks();
	};
};