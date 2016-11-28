/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#pragma once

class c_basic_log_writer
{
private:
	HANDLE m_handle;

	void Open();
	void Close();

public:
	void Initialize(LPCSTR log_name);

	void Write(LPCSTR Format, ...);
};