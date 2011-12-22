/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <Common/LinkedList.hpp>

namespace Yelo
{
	uint32 GetListLength(ILinkedListObject* list_head)
	{
		ILinkedListObject* current = list_head;
		if(!current)
			return 0;

		uint32 count = 0;
		do { count++; }while(current = current->GetNext());
		return count;
	}

	void AppendLinkedListObject(ILinkedListObject** list_head, ILinkedListObject* object)
	{
		if(!*list_head)
		{
			*list_head = object;
			return;
		}

		ILinkedListObject* current = *list_head;
		while(current->GetNext())
			current = current->GetNext();

		current->SetNext(object);
		object->SetPrevious(current);
	}

	void RemoveLinkedListObject(ILinkedListObject** list_head, ILinkedListObject* object)
	{
		ILinkedListObject* next = object->GetNext();
		ILinkedListObject* previous = object->GetPrevious();

		if(*list_head == object)
		{
			if(next)
				next->SetPrevious(NULL);
			*list_head = next;
			return;
		}

		if(previous)
			previous->SetNext(next);

		if(next)
			next->SetPrevious(previous);
	}

	void InsertAfterLinkedListObject(ILinkedListObject* previous, ILinkedListObject* object)
	{
		ILinkedListObject* next = previous->GetNext();

		previous->SetNext(object);
		object->SetPrevious(previous);

		object->SetNext(next);
		if(next)
			next->SetPrevious(object);
	}

	ILinkedListObject* GetElementByIndex(ILinkedListObject** list_head, int32 index)
	{
		int i = 0;

		ILinkedListObject* current;
		for(current = *list_head; current && current->GetNext() && (i < index); i++)
			current = current->GetNext();

		if(i != index)
			return NULL;
		return current;
	}
};