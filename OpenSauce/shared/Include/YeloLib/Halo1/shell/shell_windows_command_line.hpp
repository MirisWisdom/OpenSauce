/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#define CMDLINE_PARAM_NAME(name)BOOST_PP_CAT(cmd_line_param_, name)

// macro for getting a cmd line parameter object (CLP)
#define CMDLINE_GET_PARAM(name) Settings::CMDLINE_PARAM_NAME(name)

namespace Yelo
{
	namespace Settings
	{
		class c_cmd_line_parameter
		{
		protected:
			bool m_parameter_set;
			PAD24;
			cstring m_parameter;

		public:
			virtual void Ctor(cstring parameter);

			bool ParameterSet() const;

			virtual bool ParseValue(cstring) = 0;

			void GetParameter();
		};

		template<typename T>
		class c_cmd_line_argument : public c_cmd_line_parameter
		{
		public:
			virtual T GetValue() = 0;
		};

		template<>
		class c_cmd_line_argument<cstring> : public c_cmd_line_parameter
		{
			cstring m_value;

		public:
			cstring GetValue() { return m_value; }

			// string specialization
			bool c_cmd_line_argument<cstring>::ParseValue(cstring value)
			{
				if(!value) return false;

				m_value = value;
				return true;
			}

			void c_cmd_line_argument<cstring>::Ctor(cstring argument)
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
			bool c_cmd_line_argument<int32>::ParseValue(cstring value)
			{
				if(!value) return false;

				// read the value as an integer
				m_value = atoi(value);
				return true;
			}

			void c_cmd_line_argument<int32>::Ctor(cstring argument)
			{
				c_cmd_line_parameter::Ctor(argument);

				m_value = 0;
			}
		};

		class c_cmd_line_switch : public c_cmd_line_parameter
		{
		public:
			// switch's shouldn't have a value set so it is ignored
			inline bool ParseValue(cstring) { return true; }
		};

		//////////////////////////////////////////////////////////////////////////
		// Declare the params here
#define CMDLINE_DEFINE_ARG(name, type)		extern c_cmd_line_argument<##type##> CMDLINE_PARAM_NAME(name);
#define CMDLINE_DEFINE_SWITCH(name)			extern c_cmd_line_switch CMDLINE_PARAM_NAME(name);
#include "Common/YeloSettings.CmdLineSettings.inl"

		void ReadCmdLineSettings();
	};
};