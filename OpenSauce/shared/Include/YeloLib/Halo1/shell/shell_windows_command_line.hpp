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
			friend struct s_cmd_line_initializer;

			void GetParameter();
		protected:
			cstring m_name;
			bool m_parameter_set;
			PAD24;

			virtual void Initialize(cstring parameter);

			virtual bool ParseValue(cstring) PURE;
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Returns true if the parameter was set on the command line, false if it wasn't. </summary>
			bool ParameterSet() const { return m_parameter_set; }
		};

		template<typename T>
		class c_cmd_line_argument : public c_cmd_line_parameter
		{
		public:
			T GetValue();
		};

		template<>
		class c_cmd_line_argument<cstring> final : public c_cmd_line_parameter
		{
			friend struct s_cmd_line_initializer;

			cstring m_value;

		public:
			cstring GetValue() { return m_value; }

		protected:
			// string specialization
			bool ParseValue(cstring value) override
			{
				if(!value) return false;

				m_value = value;
				return true;
			}

			void Initialize(cstring argument) override
			{
				c_cmd_line_parameter::Initialize(argument);

				m_value = "";
			}
		};

		template<>
		class c_cmd_line_argument<int32> final : public c_cmd_line_parameter
		{
			friend struct s_cmd_line_initializer;

			int32 m_value;

		public:
			int32 GetValue() { return m_value; }

		protected:
			// int32 specialization
			bool ParseValue(cstring value) override
			{
				if(!value) return false;

				// read the value as an integer
				m_value = atoi(value);
				return true;
			}

			void Initialize(cstring argument) override
			{
				c_cmd_line_parameter::Initialize(argument);

				m_value = 0;
			}
		};

		template<>
		class c_cmd_line_argument<real> final : public c_cmd_line_parameter
		{
			friend struct s_cmd_line_initializer;

			real m_value;

		public:
			real GetValue() { return m_value; }

		protected:
			// real specialization
			bool ParseValue(cstring value) override
			{
				if(!value) return false;

				// read the value as a real
				m_value = (real)atof(value);
				return true;
			}

			void Initialize(cstring argument) override
			{
				c_cmd_line_parameter::Initialize(argument);

				m_value = 0.0f;
			}
		};

		class c_cmd_line_switch final : public c_cmd_line_parameter
		{
			friend struct s_cmd_line_initializer;

		protected:
			// switches shouldn't have a value set so it is ignored
			bool ParseValue(cstring) override { return true; }
		};

		//////////////////////////////////////////////////////////////////////////
		// Declare the params here
#define CMDLINE_DEFINE_ARG(name, type)		extern c_cmd_line_argument<##type##> CMDLINE_PARAM_NAME(name);
#define CMDLINE_DEFINE_SWITCH(name)			extern c_cmd_line_switch CMDLINE_PARAM_NAME(name);
#include <YeloLib/Halo1/shell/shell_windows_command_line.inl>

		void ReadCmdLineSettings();
	};
};