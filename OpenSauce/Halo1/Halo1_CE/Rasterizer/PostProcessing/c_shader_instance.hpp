/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <Common/LinkedList.hpp>

#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_shader_instance : public ILinkedListObject
		{
			/////////////////////////////////////////////////
			// ILinkedListObject
		private:
			struct
			{
				c_shader_instance* previous;
				c_shader_instance* next;
			}m_list_tree;

		public:
			ILinkedListObject* GetPrevious() { return m_list_tree.previous; }
			ILinkedListObject* GetNext() { return m_list_tree.next; }
			void SetPrevious(ILinkedListObject* object) { m_list_tree.previous = CAST_PTR(c_shader_instance*, object); }
			void SetNext(ILinkedListObject* object) { m_list_tree.next = CAST_PTR(c_shader_instance*, object); }

			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				struct
				{
					bool is_valid;
					bool is_active;
					PAD16;
				}m_flags;

				c_shader_postprocess* definition;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			virtual void SetShader(c_shader_postprocess* definition);
			c_shader_postprocess* GetShader();
			bool IsValid();
			bool& IsActive();
			bool UsesGBuffer();

			/////////////////////////////////////////////////
			// initializers
		public:
			virtual void Ctor()
			{
				m_list_tree.previous = NULL;
				m_list_tree.next = NULL;

				m_members.m_flags.is_valid = false;
				m_members.m_flags.is_active = true;
				m_members.definition = NULL;
			}

			virtual void Dtor()
			{
				m_list_tree.next = NULL;
				m_list_tree.previous = NULL;

				m_members.m_flags.is_valid = false;
				m_members.m_flags.is_active = true;
				m_members.definition = NULL;
			}

			/////////////////////////////////////////////////
			// shader instance setup
		public:
			virtual void SetupShaderInstance() {}
			void Validate();
		protected:
			virtual bool ValidateImpl();

			/////////////////////////////////////////////////
			// shader instance application
		public:
			virtual void UpdateShaderInstance(real delta_time) { }
			virtual void SetShaderInstanceVariables() {}
		};
	};};
};
#endif