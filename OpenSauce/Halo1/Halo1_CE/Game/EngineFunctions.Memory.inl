/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

API_FUNC_NAKED Yelo::Memory::s_data_array* DataNew(cstring name, int32 maximum_count, size_t datum_size)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_NEW);

	API_FUNC_NAKED_START()
		push	ebx

		mov		ebx, datum_size
		push	maximum_count
		push	name
		call	CALL_ADDR
		add		esp, 4 * 2

		pop		ebx
	API_FUNC_NAKED_END(3)
}

datum_index DatumNewAtIndex(Yelo::Memory::s_data_array* data, datum_index index)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

	if(data == NULL || index.IsNull()) return datum_index::null;

	__asm {
		mov		edx, data
		mov		eax, index
		call	CALL_ADDR
	}
}

datum_index DatumNew(Yelo::Memory::s_data_array* data)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_NEW);

	if(data == NULL) return datum_index::null;

	__asm {
		mov		edx, data
		call	CALL_ADDR
	}
}

void DatumDelete(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_DELETE);

	if(data == NULL || datum.IsNull()) return;

	__asm {
		mov		edx, datum
		mov		eax, data
		call	CALL_ADDR
	}
}

API_FUNC_NAKED void DataMakeValid(Yelo::Memory::s_data_array* data)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_MAKE_VALID);

	API_FUNC_NAKED_START()
		push	esi

		mov		esi, data
		call	CALL_ADDR

		pop		esi
	API_FUNC_NAKED_END(1)
}

void* DataIteratorNext(void* iterator)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_ITERATOR_NEXT);

	if(iterator == NULL) return NULL;

	__asm {
		push	edi

		mov		edi, iterator
		call	CALL_ADDR

		pop		edi
	}
}

datum_index DatumNextIndex(Yelo::Memory::s_data_array* data, datum_index cursor)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_NEXT_INDEX);

	if(data == NULL || cursor.IsNull()) return datum_index::null;

	__asm {
		push	edi
		push	esi

		mov		edi, data
		mov		esi, cursor
		call	CALL_ADDR

		pop		esi
		pop		edi
	}
}

void* DatumGet(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_GET);

	if(data == NULL || datum.IsNull()) return NULL;

	__asm {
		push	esi

		mov		esi, data
		mov		edx, datum
		call	CALL_ADDR

		pop		esi
	}
}

void DatumInitialize(Yelo::Memory::s_data_array* data, void* buffer)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_INITIALIZE);

	if (data == NULL || buffer == NULL) return;

	__asm {
		push	esi

		mov		edx, data
		mov		esi, buffer
		call	CALL_ADDR

		pop		esi
	}
}

Yelo::Memory::s_data_array* DataNewAndMakeValid(cstring name, int32 maximum_count, size_t datum_size)
{
	Yelo::Memory::s_data_array* data = DataNew(name, maximum_count, datum_size);

	if(data != NULL)
	{
		data->is_valid = true;
		DataMakeValid(data);
	}

	return data;
}