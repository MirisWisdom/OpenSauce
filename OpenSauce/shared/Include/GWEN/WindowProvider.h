/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#pragma once

#if !PLATFORM_IS_DEDI

#ifndef GWEN_WINDOW_PROVIDER_H
#define GWEN_WINDOW_PROVIDER_H

namespace Gwen
{
	class WindowProvider
	{
		public:

			virtual void* GetWindow() = 0;
	};

}

#endif
#endif
