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

	if(data == nullptr || index.IsNull()) return datum_index::null;

	__asm {
		mov		edx, data
		mov		eax, index
		call	CALL_ADDR
	}
}

datum_index DatumNew(Yelo::Memory::s_data_array* data)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_NEW);

	if(data == nullptr) return datum_index::null;

	__asm {
		mov		edx, data
		call	CALL_ADDR
	}
}

void DatumDelete(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_DELETE);

	if(data == nullptr || datum.IsNull()) return;

	__asm {
		mov		edx, datum
		mov		eax, data
		call	CALL_ADDR
	}
}

API_FUNC_NAKED void DataDeleteAll(Yelo::Memory::s_data_array* data)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_DELETE_ALL);

	API_FUNC_NAKED_START()
		push	esi

		mov		esi, data
		call	CALL_ADDR

		pop		esi
	API_FUNC_NAKED_END(1)
}

/*void* DataIteratorNext(void* iterator)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_ITERATOR_NEXT);

	if(iterator == nullptr) return nullptr;

	__asm {
		push	edi

		mov		edi, iterator
		call	CALL_ADDR

		pop		edi
	}
}*/

datum_index DataNextIndex(Yelo::Memory::s_data_array* data, datum_index cursor)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATA_NEXT_INDEX);

	if(data == nullptr || cursor.IsNull()) return datum_index::null;

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

void* DatumTryAndGet(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(DATUM_TRY_AND_GET);

	if(data == nullptr) return nullptr;

	__asm {
		push	esi

		mov		esi, data
		mov		edx, datum
		call	CALL_ADDR

		pop		esi
	}
}

Yelo::Memory::s_data_array* DataNewAndMakeValid(cstring name, int32 maximum_count, size_t datum_size)
{
	Yelo::Memory::s_data_array* data = DataNew(name, maximum_count, datum_size);

	if(data != nullptr)
	{
		data->is_valid = true;
		DataDeleteAll(data);
	}

	return data;
}