/*
	GWEN
	Copyright (c) 2012 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/DX9/DX9.hpp"

#include "Gwen/Macros.h"
#include "Gwen/Platform.h"

#include <time.h>

void Gwen::Platform::Sleep( unsigned int iMS )
{
	YELO_ERROR_CRITICAL("Sleep not applicable in Halo runtime");
}

void Gwen::Platform::SetCursor( unsigned char iCursor )
{
	YELO_ERROR_CRITICAL("SetCursor not applicable in Halo runtime");
}

Gwen::UnicodeString Gwen::Platform::GetClipboardText()
{
	if ( !OpenClipboard( NULL ) ) { return L""; }

	HANDLE hData = GetClipboardData( CF_UNICODETEXT );

	if ( hData == NULL )
	{
		CloseClipboard();
		return L"";
	}

	wchar_t* buffer = ( wchar_t* ) GlobalLock( hData );
	UnicodeString str = buffer;
	GlobalUnlock( hData );
	CloseClipboard();
	return str;
}

bool Gwen::Platform::SetClipboardText( const Gwen::UnicodeString & str )
{
	if ( !OpenClipboard( NULL ) ) { return false; }

	EmptyClipboard();
	// Create a buffer to hold the string
	size_t iDataSize = ( str.length() + 1 ) * sizeof( wchar_t );
	HGLOBAL clipbuffer = GlobalAlloc( GMEM_DDESHARE, iDataSize );
	// Copy the string into the buffer
	wchar_t* buffer = ( wchar_t* ) GlobalLock( clipbuffer );
	wcscpy( buffer, str.c_str() );
	GlobalUnlock( clipbuffer );
	// Place it on the clipboard
	SetClipboardData( CF_UNICODETEXT, clipbuffer );
	CloseClipboard();
	return true;
}

double GetPerformanceFrequency()
{
	static double Frequency = 0.0f;

	if ( Frequency == 0.0f )
	{
		__int64 perfFreq;
		QueryPerformanceFrequency( ( LARGE_INTEGER* ) &perfFreq );
		Frequency = 1.0 / ( double ) perfFreq;
	}

	return Frequency;
}

float Gwen::Platform::GetTimeInSeconds()
{
	static float fCurrentTime = 0.0f;
	static __int64 iLastTime = 0;
	__int64 thistime;
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &thistime );
	float fSecondsDifference = ( double )( thistime - iLastTime ) * GetPerformanceFrequency();

	if ( fSecondsDifference > 0.1f ) { fSecondsDifference = 0.1f; }

	fCurrentTime += fSecondsDifference;
	iLastTime = thistime;
	return fCurrentTime;
}

bool Gwen::Platform::FileOpen( const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback )
{
	YELO_ERROR_CRITICAL("FileOpen not applicable in Halo runtime");
	return false;
}

bool Gwen::Platform::FileSave( const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Gwen::Event::Handler::FunctionWithInformation fnCallback )
{
	YELO_ERROR_CRITICAL("FileSave not applicable in Halo runtime");
	return false;
}

bool Gwen::Platform::FolderOpen( const String & Name, const String & StartPath, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback )
{
	YELO_ERROR_CRITICAL("FolderOpen not applicable in Halo runtime");
	return false;
}

void* Gwen::Platform::CreatePlatformWindow( int x, int y, int w, int h, const Gwen::String & strWindowTitle )
{
	YELO_ERROR_CRITICAL("CreatePlatformWindow not applicable in Halo runtime");
	return nullptr;
}

void Gwen::Platform::DestroyPlatformWindow( void* pPtr )
{
	YELO_ERROR_CRITICAL("DestroyPlatformWindow not applicable in Halo runtime");
}

void Gwen::Platform::MessagePump( void* pWindow, Gwen::Controls::Canvas* ptarget )
{
	YELO_ERROR_CRITICAL("MessagePump not applicable in Halo runtime");
}

void Gwen::Platform::SetBoundsPlatformWindow( void* pPtr, int x, int y, int w, int h )
{
	YELO_ERROR_CRITICAL("SetBoundsPlatformWindow not applicable in Halo runtime");
}

void Gwen::Platform::SetWindowMaximized( void* pPtr, bool bMax, Gwen::Point & pNewPos, Gwen::Point & pNewSize )
{
	YELO_ERROR_CRITICAL("SetWindowMaximized not applicable in Halo runtime");
}

void Gwen::Platform::SetWindowMinimized( void* pPtr, bool bMinimized )
{
	YELO_ERROR_CRITICAL("SetWindowMinimized not applicable in Halo runtime");
}

bool Gwen::Platform::HasFocusPlatformWindow( void* pPtr )
{
	YELO_ERROR_CRITICAL("HasFocusPlatformWindow not applicable in Halo runtime");
	return true;
}

void Gwen::Platform::GetDesktopSize( int & w, int & h )
{
	w = Yelo::DX9::D3DPresetParams()->BackBufferWidth;
	h = Yelo::DX9::D3DPresetParams()->BackBufferHeight;
}

void Gwen::Platform::GetCursorPos( Gwen::Point & po )
{
	YELO_ERROR_CRITICAL("GetCursorPos not applicable in Halo runtime");
}
#endif