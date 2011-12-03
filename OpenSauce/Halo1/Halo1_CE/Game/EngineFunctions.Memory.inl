/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

datum_index DatumNewAtIndex(Yelo::Memory::s_data_array* data, datum_index index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

	if(data == NULL || index.index == NONE) return datum_index::null;

	__asm {
		mov		edx, data
		mov		eax, index
		call	TEMP_CALL_ADDR
	}
}

datum_index DatumNew(Yelo::Memory::s_data_array* data)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEW);

	if(data == NULL) return datum_index::null;

	__asm {
		mov		edx, data
		call	TEMP_CALL_ADDR
	}
}

void DatumDelete(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_DELETE);

	if(data == NULL || datum.index == NONE) return;

	__asm {
		mov		edx, datum
		mov		eax, data
		call	TEMP_CALL_ADDR
	}
}

void* DataIteratorNext(void* iterator)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATA_ITERATOR_NEXT);

	if(iterator == NULL) return NULL;

	__asm {
		push	edi

		mov		edi, iterator
		call	TEMP_CALL_ADDR

		pop		edi
	}
}

datum_index DatumNextIndex(Yelo::Memory::s_data_array* data, datum_index base)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEXT_INDEX);

	if(data == NULL || base.index == NONE) return datum_index::null;

	__asm {
		push	edi
		push	esi

		mov		edi, data
		mov		esi, base
		call	TEMP_CALL_ADDR

		pop		esi
		pop		edi
	}
}

void* DatumGet(Yelo::Memory::s_data_array* data, datum_index datum)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_GET);

	if(data == NULL || datum.index == NONE) return NULL;

	__asm {
		push	esi

		mov		esi, data
		mov		edx, datum
		call	TEMP_CALL_ADDR

		pop		esi
	}
}

void DatumInitialize(Yelo::Memory::s_data_array* data, void* buffer)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_INITIALIZE);

	if (data == NULL || buffer == NULL) return;

	__asm {
		push	esi

		mov		edx, data
		mov		esi, buffer
		call	TEMP_CALL_ADDR

		pop		esi
	}
}