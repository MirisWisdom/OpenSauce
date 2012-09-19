/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Game/EngineFunctions.hpp"

// macros for adding cmd line parameters
#define CMDLINE_DEFINE_ARG(name, type)		extern c_cmd_line_argument<##type##> cmd_line_param_##name
#define CMDLINE_DEFINE_SWITCH(name)			extern c_cmd_line_switch cmd_line_param_##name

// macro for getting a cmd line parameter object (CLP)
#define CMDLINE_GET_PARAM(name) Settings::cmd_line_param_##name

namespace Yelo
{
	namespace Settings
	{
		class c_cmd_line_parameter
		{
		protected:
			bool m_parameter_set;
			PAD24;
			const char* m_parameter;

		public:
			virtual void Ctor(const char* parameter)
			{
				m_parameter_set = false;
				m_parameter = parameter;
			}

			bool ParameterSet()
			{
				return m_parameter_set;
			}

			virtual bool ParseValue(char*) = 0;

			void GetParameter()
			{
				char* value = NULL;
				m_parameter_set = Engine::GetCmdLineParameter(m_parameter, &value);

				// if the parameter has been set, parse the parameters value
				if(m_parameter_set && value)
					m_parameter_set = ParseValue(value);
			}
		};

		template<typename T>
		class c_cmd_line_argument : public c_cmd_line_parameter
		{
		public:
			virtual T GetValue() = 0;
		};

		template<>
		class c_cmd_line_argument<char*> : public c_cmd_line_parameter
		{
			char* m_value;

		public:
			char* GetValue() { return m_value; }

			// string specialization
			bool c_cmd_line_argument<char*>::ParseValue(char* value)
			{
				if(!value) return false;

				m_value = value;
				return true;
			}

			void c_cmd_line_argument<char*>::Ctor(const char* argument)
			{
				c_cmd_line_parameter::Ctor(argument);

				m_value = "";
			}
		};

		template<>
		class c_cmd_line_argument<int32> : public c_cmd_line_parameter
		{
			int32 m_value;

		public:
			int32 GetValue() { return m_value; }

			// int32 specialization
			bool c_cmd_line_argument<int32>::ParseValue(char* value)
			{
				if(!value) return false;

				// read the value as an integer
				m_value = atoi(value);
				return true;
			}

			void c_cmd_line_argument<int32>::Ctor(const char* argument)
			{
				c_cmd_line_parameter::Ctor(argument);

				m_value = 0;
			}
		};

		class c_cmd_line_switch : public c_cmd_line_parameter
		{
		public:
			// switch's shouldn't have a value set so it is ignored
			bool ParseValue(char* value) { return true; }
		};

#include "YeloSettings.CmdLineSettings.inl"

		void ReadCmdLineSettings();
	};
};