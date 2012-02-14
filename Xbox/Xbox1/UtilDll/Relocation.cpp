/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "UtilDll.hpp"

#include <stdio.h>
#include <stdlib.h>

#if defined(_DEBUG)
	#define DebugCode( ... ) __VA_ARGS__
#else
	#define DebugCode( ... )
#endif

#pragma warning(push)
#pragma warning(disable : 4200) // for our non-fixed arrays


static const char* k_section_name_modexport = "modxport";
static const char* k_section_name_reloc = ".reloc\0\0";

// Is the given section name valid for xbox-bound module code or data?
// Eg, .reloc *isn't* valid as it's just debug data in this context. It shouldn't be included in the xbox-bound module buffer
static bool section_name_is_usable(const char* section_name)
{
	static const char* k_section_name_code = ".text\0\0\0";
	static const char* k_section_name_const =".rdata\0\0";
	static const char* k_section_name_data = ".data\0\0\0";

	return	memcmp(section_name, k_section_name_code, IMAGE_SIZEOF_SHORT_NAME) == 0 ||
			memcmp(section_name, k_section_name_const, IMAGE_SIZEOF_SHORT_NAME) == 0 || 
			memcmp(section_name, k_section_name_data, IMAGE_SIZEOF_SHORT_NAME) == 0;
}
// Currently, the only other section that is allowed to have relocs is the module export section
static bool section_name_is_usable_or_can_contain_relocs(const char* section_name)
{
	return	section_name_is_usable(section_name) ||
			memcmp(section_name, k_section_name_modexport, IMAGE_SIZEOF_SHORT_NAME) == 0;
}

struct s_reloc_table
{
	IMAGE_BASE_RELOCATION Header;
	struct Address {
		USHORT Offset : 12;
		USHORT Type : 4;
	}Reloc[];

	const Address* Get(int index) const	{ return &this->Reloc[index]; }
	size_t Count() const				{ return (Header.SizeOfBlock - sizeof(Header)) / sizeof(s_reloc_table::Address); }
	bool RelocIsEmpty(int index) const	{ return this->Reloc[index].Offset == 0 && this->Reloc[index].Type == 0; }
};

#pragma warning(pop)

UTILDLL_API HRESULT Util_CalculateModuleCodeSize(LPCWSTR lpFileName, PUINT32 code_size)
{
	struct s_file_struct {
		IMAGE_DOS_HEADER Dos;
		IMAGE_FILE_HEADER File;
		IMAGE_OPTIONAL_HEADER32 Header;
		size_t SectionCount;
		IMAGE_SECTION_HEADER* Sections;

		s_file_struct()	{ memset(this, 0, sizeof(*this)); }
		~s_file_struct()
		{
			if(Sections != NULL)
			{
				delete[] Sections;
				Sections = NULL;
			}
		}

		void AllocateSections(size_t count)
		{
			SectionCount = count;
			Sections = new IMAGE_SECTION_HEADER[count];
		}
	}DllStruct;
	DWORD lpNumberOfBytesRead;

	HANDLE dll_file = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HRESULT result = S_OK;

	*code_size = 0;

	if(dll_file != INVALID_HANDLE_VALUE)
	{
		// !--- Read file headers ---
		if(! ReadFile(dll_file, &DllStruct.Dos, sizeof(DllStruct.Dos), &lpNumberOfBytesRead, NULL) ) result = E_ABORT;
		else
		{
			SetFilePointer(dll_file, DllStruct.Dos.e_lfanew + sizeof(DWORD), NULL, FILE_BEGIN);

			if(! ReadFile(dll_file, &DllStruct.File, sizeof(DllStruct.File), &lpNumberOfBytesRead, NULL) ) result = E_ABORT;
			if(! ReadFile(dll_file, &DllStruct.Header, sizeof(DllStruct.Header), &lpNumberOfBytesRead, NULL) ) result = E_ABORT;
		}
		// !--- Read file headers ---



		// !--- Read file sections ---
		if(result == S_OK)
		{
			DllStruct.AllocateSections(DllStruct.File.NumberOfSections);
			if(! ReadFile(dll_file, DllStruct.Sections, sizeof(DllStruct.Sections[0]) * DllStruct.SectionCount, &lpNumberOfBytesRead, NULL) ) result = E_ABORT;
		}
		// !--- Read file sections ---

		// will close the file even if there were errors above
		CloseHandle(dll_file);

		if(result == S_OK)
		{
//			DWORD sub = 0;
			for(size_t x = 0; x < DllStruct.SectionCount; x++)
			{
				const char* section_name = reinterpret_cast<char*>(DllStruct.Sections[x].Name);
				if(	!section_name_is_usable(section_name) )
					continue;

// 				*code_size = DllStruct.Sections[x].VirtualAddress - sub;
// 				sub = DllStruct.Sections[x].VirtualAddress;
				*code_size += DllStruct.Sections[x].SizeOfRawData;
			}
		}
	}
	else result = E_FAIL;

	return result;
}

UTILDLL_API HRESULT Util_RebaseModule(LPCWSTR lpFileName, PBYTE reloc_dll, UINT32 reloc_dll_size, PUINT32 base_address, UINT32 reloc_alloc_count)
{
	if(reloc_alloc_count <= 0)
	{
		// the default size of this table may not be big enough since the xbox dll will always be compiled with max optimization settings
		reloc_alloc_count = 4096;
	}

	struct s_managed_file
	{
		FILE* file;

		s_managed_file(const char* name, const char* mode)
		{
			fopen_s(&file, name, mode);
		}
		~s_managed_file()
		{
			if(file != NULL)
			{
				fflush(file);
				fclose(file);
				file = NULL;
			}
		}

		operator FILE*() { return file; }
	};

	struct s_buffer
	{
		size_t size;
		BYTE* buffer;

		s_buffer() : size(0), buffer(NULL) {}
		~s_buffer()
		{
			if(buffer != NULL)
			{
				delete[] buffer;
				buffer = NULL;

				size = NULL;
			}
		}

		void Allocate(size_t _size)
		{
			size = _size;
			buffer = new BYTE[_size];
		}

		operator BYTE*() { return buffer; }
	};

	struct s_file_struct {
		IMAGE_DOS_HEADER Dos;
		IMAGE_FILE_HEADER File;
		IMAGE_OPTIONAL_HEADER32 Header;
		size_t SectionCount;
		IMAGE_SECTION_HEADER* Sections;
		IMAGE_SECTION_HEADER* RelocSection;
		DWORD RelocationsCount;
		s_reloc_table** Relocations;
		struct _SectionInfo {
			DWORD Start;
			DWORD End;
		}* SectionInfo;

		s_file_struct()	{ memset(this, 0, sizeof(*this)); }
		~s_file_struct()
		{
			if(Sections != NULL)
			{
				delete[] Sections;
				Sections = NULL;

				delete[] SectionInfo;
				SectionInfo = NULL;

				SectionCount = 0;
				RelocSection = NULL;
			}

			if(Relocations != NULL)
			{
				delete[] Relocations;
				Relocations = NULL;

				RelocationsCount = 0;
			}
		}

		void AllocateSections(size_t count)
		{
			SectionCount = count;
			Sections = new IMAGE_SECTION_HEADER[count];
			SectionInfo = new _SectionInfo[count];
		}
		void AllocateRelocations(size_t count)
		{
			Relocations = new s_reloc_table*[count];
		}
	}DllStruct;
	DWORD lpNumberOfBytesRead;

	s_buffer dll_buffer;
	{
		HANDLE dll_file = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(dll_file == INVALID_HANDLE_VALUE) return E_FAIL;

		HRESULT result = S_OK;

		// !--- Read file size and data ---
		dll_buffer.Allocate( GetFileSize(dll_file, NULL) );

		if(! ReadFile(dll_file, dll_buffer, dll_buffer.size, &lpNumberOfBytesRead, NULL) ) result = E_FAIL;
		// !--- Read file size and data ---




		// !--- Read file headers ---
		if(result == S_OK)
		{
			SetFilePointer(dll_file, 0, NULL, FILE_BEGIN);

			if(! ReadFile(dll_file, &DllStruct.Dos, sizeof(DllStruct.Dos), &lpNumberOfBytesRead, NULL) ) result = E_FAIL;
			else
				SetFilePointer(dll_file, DllStruct.Dos.e_lfanew + sizeof(DWORD), NULL, FILE_BEGIN);

			if(! ReadFile(dll_file, &DllStruct.File, sizeof(DllStruct.File), &lpNumberOfBytesRead, NULL) ) result = E_FAIL;

			if(! ReadFile(dll_file, &DllStruct.Header, sizeof(DllStruct.Header), &lpNumberOfBytesRead, NULL) ) result = E_FAIL;
		}
		// !--- Read file headers ---



		// !--- Read file sections ---
		if(result == S_OK)
		{
			DllStruct.AllocateSections(DllStruct.File.NumberOfSections);
			if(! ReadFile(dll_file, DllStruct.Sections, sizeof(DllStruct.Sections[0]) * DllStruct.SectionCount, &lpNumberOfBytesRead, NULL) ) result = E_FAIL;
		}
		// !--- Read file sections ---

		CloseHandle(dll_file);
		if(result != S_OK) return result;
	}

	DebugCode( s_managed_file debug_file = s_managed_file("UtilDll.log", "wb") );
	DebugCode( fprintf_s(debug_file, "length:%d\tsections:%d\n", dll_buffer.size, DllStruct.SectionCount) );



	DWORD x;

	// !--- Get the relocation section data ---
	DWORD modexport_index = MAXDWORD, reloc_index = MAXDWORD;
	for(x = 0; x < DllStruct.SectionCount; x++)
	{
		// calculate ranges
		DllStruct.SectionInfo[x].Start = DllStruct.Sections[x].VirtualAddress;
		DllStruct.SectionInfo[x].End = (DllStruct.Sections[x].VirtualAddress + DllStruct.Sections[x].SizeOfRawData);

		DebugCode( fprintf_s(debug_file, "%X\t%X\n", DllStruct.SectionInfo[x].Start, DllStruct.SectionInfo[x].End) );

		const char* section_name = reinterpret_cast<char*>(DllStruct.Sections[x].Name);
		if(!memcmp(section_name, k_section_name_modexport, 
			sizeof(DllStruct.Sections[x].Name))) // hey, we found the modxport section header!
			modexport_index = x; // record its index in the section header table

		if(!memcmp(section_name, k_section_name_reloc, 
			sizeof(DllStruct.Sections[x].Name))) // hey, we found the .reloc section header!
			reloc_index = x; // record its index in the section header table
	}

	if(modexport_index == MAXDWORD || reloc_index == MAXDWORD)
		return E_ABORT; // we couldn't find a fucking relocation section and the export section isn't present. so sod off, get your own cheese...

	// TODO: we shouldn't need to do this anymore since we check for only valid section names now
#if FALSE
	if( (reloc_index - modexport_index) != 1 )
	{
		DebugCode( fprintf_s(debug_file, "ERROR: modexport doesn't come immediate before reloc! %d %d\n", modexport_index, reloc_index) );
		return E_ABORT;
	}
#endif

	DllStruct.RelocSection = &DllStruct.Sections[reloc_index]; // hey, why not make it a little easier and add a pointer to that reloc section header
	DebugCode( fputs("\n", debug_file) );

	DllStruct.SectionCount--;// the last section should be the reloc, everything else should be what we want
	// !--- Get the relocation section data ---




	// !--- Fill in the relocation section pointers ---
	BYTE* reloc_buffer = &dll_buffer.buffer[DllStruct.RelocSection->PointerToRawData]; // pointer to the relocation table section's *data*
	DWORD reloc_buffer_size = DllStruct.RelocSection->SizeOfRawData; // size of the relocation table section's *data*

	DebugCode( fputs("reloc buffer sizes\n", debug_file) );
	DllStruct.AllocateRelocations(reloc_alloc_count);
	x = 0; // x = offset in the relocations section buffer
	// index = indexer to the relocations table we're building
	for(DWORD index = 0; x <= reloc_buffer_size; index++)
	{
		if(x >= reloc_alloc_count)
		{
			DebugCode( fputs("ERROR: raise 'reloc_alloc_count'!", debug_file) );
			return E_ABORT;
		}

		if( *(reinterpret_cast<DWORD*>(&reloc_buffer[x])) == 0 ) // if we got to a null reloc table we're now in the padding
		{
			DllStruct.RelocationsCount = index;
			reloc_buffer_size = x;
			break; // no more relocation structures, so gtfo and do something else
		}
		DebugCode( fprintf_s(debug_file, "\t%d\n", x) );

		DllStruct.Relocations[index] = reinterpret_cast<s_reloc_table*>(&reloc_buffer[x]);
		x += DllStruct.Relocations[index]->Header.SizeOfBlock;
	}
	DebugCode( fputs("\n", debug_file) );
	// !--- Fill in the relocation section pointers ---




	// !--- Perform relocations ---
	DWORD old = DllStruct.Sections[0].VirtualAddress - DllStruct.Header.ImageBase; // we want our code to be starting at the first segment's address
	DebugCode( fprintf_s(debug_file, "base:%X\tour base:%X\n", DllStruct.Header.ImageBase, *base_address) );

	for(DWORD index = 0; index < DllStruct.RelocationsCount; index++)
	{
		const s_reloc_table* rt = DllStruct.Relocations[index];
		DWORD tmp = rt->Header.VirtualAddress;
		DWORD section_info_index = MAXDWORD;

		for(DWORD y = 0; y < DllStruct.SectionCount; y++)
		{
			if(tmp >= DllStruct.SectionInfo[y].Start && tmp <= DllStruct.SectionInfo[y].End)
			{
				section_info_index = y;

				const char* section_name = reinterpret_cast<char*>(DllStruct.Sections[y].Name);
				if( !section_name_is_usable_or_can_contain_relocs(section_name) )
				{
					DebugCode(
						char name[8+1];
						name[8] = '\0';
						memcpy(name, DllStruct.Sections[y].Name, sizeof(name)-1);

						fprintf_s(debug_file, "ERROR: found relocation to an unusable section: %s", name);
					);
					return E_ABORT;
				}
			}
		}

		if(section_info_index == MAXDWORD)
		{
			DebugCode( fprintf_s(debug_file, "ERROR: couldn't find section for VA=0x%X\n", tmp) );
			return E_ABORT;
		}

		DWORD section_offset = DllStruct.Sections[section_info_index].PointerToRawData + 
			(rt->Header.VirtualAddress - DllStruct.Sections[section_info_index].VirtualAddress);

		DebugCode( fprintf_s(debug_file, "-- 0x%04X\t0x%04X\t0x%08X\t0x%08X\t0x%08X --\n", 
			index, section_info_index,
			rt->Header.VirtualAddress, rt->Count(), section_offset) );
		for(DWORD i = 0; i < rt->Count(); i++)
		{
			if(rt->RelocIsEmpty(i)) break; // there was a padding address added, this is the last one so quit

			if(rt->Get(i)->Type == IMAGE_REL_BASED_HIGHLOW)
			{
				DebugCode( fprintf_s(debug_file, "\t0x%08X\t0x%08X\t\t", rt->Get(i)->Type, rt->Get(i)->Offset) );
				DWORD* ptr = reinterpret_cast<DWORD*>(&dll_buffer.buffer[section_offset + rt->Get(i)->Offset]);
				DebugCode( fprintf_s(debug_file, "\t0x%08X\t", *ptr) );

				tmp = (*ptr - old); // take out the built in base
				DebugCode( fprintf(debug_file, "\t0x%08X", tmp + *base_address) );
				*ptr = tmp + *base_address; // and add our base

				DebugCode( fputs("\n", debug_file) );
			}
			else
			{
				DebugCode( fprintf_s(debug_file, "\tignoring relocation type 0x%08X\n", rt->Get(i)->Type) );
			}
		}
	}
	*base_address = (DllStruct.Header.AddressOfEntryPoint - old) /*+ *base_address*/; // calculate the entry point *offset* in the reloc_dll buffer
	DebugCode( fprintf_s(debug_file, "0x%X\n\n", *base_address) );
	// !--- Perform relocations ---


	// !--- Put the result data in the user buffer ---
	x = 0;
	for(DWORD offset = 0; x < DllStruct.SectionCount; x++)
	{
		const char* section_name = reinterpret_cast<char*>(DllStruct.Sections[x].Name);
		if(	!section_name_is_usable(section_name) )
			continue;

		DebugCode( 
			char name[8+1];
			name[8] = '\0';
			memcpy(name, DllStruct.Sections[x].Name, sizeof(name)-1);

			fprintf_s(debug_file, "%8s\t0x%08X\t0x%08X\n", name, DllStruct.Sections[x].PointerToRawData, DllStruct.Sections[x].SizeOfRawData)
			);
		memcpy(reloc_dll + offset, 
			dll_buffer.buffer + DllStruct.Sections[x].PointerToRawData, 
			DllStruct.Sections[x].SizeOfRawData);
		offset += DllStruct.Sections[x].SizeOfRawData;
	}
	// !--- Put the result data in the user buffer ---

	return S_OK;
}