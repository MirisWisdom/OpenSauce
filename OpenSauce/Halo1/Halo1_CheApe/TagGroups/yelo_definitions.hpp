/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	struct tag_group;

	namespace TagGroups
	{
		struct project_yellow_globals;

		extern struct s_yelo_definition_globals {
			bool initialized;
			PAD24;
			tag_group* py_globals_cv_definition;
			tag_group* py_globals_definition;
			tag_group* py_definition;

		private:
			bool VerifyYeloGlobalsCvGroup() const;
			bool VerifyYeloGlobalsGroup() const;
			bool VerifyYeloScenarioGroup() const;
			bool VerifyGroupDefinitions() const;

			// Populate the group references with their function pointers, etc
			void InitializeGroupDefinitions();
			void InitializeGroupReferences();
		public:
			void Initialize();
			void Dispose();
		}_yelo_definition_globals;

		void YeloDefinitionsInitialize();
		void YeloDefinitionsDispose();

#if PLATFORM_ID == PLATFORM_TOOL
		bool YeloToolCheckTagsForGameStateUpgradeRequirements();
#endif
	};
};