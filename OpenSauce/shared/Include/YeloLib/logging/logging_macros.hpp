/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/logging/i_logger.hpp>
#include <YeloLib/logging/c_log_collection.hpp>

#define LOG(verbosity, message) Yelo::Logging::c_log_singleton::Get().Log(Yelo::Logging::LogVerbosity::verbosity, message);
#else
#define LOG(verbosity, message)
#endif