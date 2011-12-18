/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	interface ILinkedListObject
		abstract
	{
	public:
		virtual ILinkedListObject* GetNext() = 0;
		virtual ILinkedListObject* GetPrevious() = 0;

		virtual void SetNext(ILinkedListObject* next) = 0;
		virtual void SetPrevious(ILinkedListObject* previous) = 0;
	};

	uint32 GetListLength(ILinkedListObject* list_head);

	void AppendLinkedListObject(ILinkedListObject** list_head, ILinkedListObject* object);
	void RemoveLinkedListObject(ILinkedListObject** list_head, ILinkedListObject* object);
	void InsertAfterLinkedListObject(ILinkedListObject* previous, ILinkedListObject* object);

	ILinkedListObject* GetElementByIndex(ILinkedListObject** list_head, int32 index);
};