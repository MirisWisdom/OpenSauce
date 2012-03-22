/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	// Use as a base class only
	template<typename T>
	class LinkedListNode
	{
		T* m_next;
		T* m_prev;

	protected:
		void ClearNodeData()
		{
			m_next = NULL;
			m_prev = NULL;
		}

	public:
		T* GetNext() const { return m_next; }
		T* GetPrevious() const { return m_prev; }

		void SetNext(T* next) { m_next = next; }
		void SetPrevious(T* previous) { m_prev = previous; }
	};

	// TNode's are LinkedListNode's

	template<typename TNode>
	size_t GetListLength(const TNode* list_head)
	{
		size_t count = 0;
		for(TNode* cur = list_head; cur != NULL; cur = cur->GetNext(), count++)
			;

		return count;
	}

	template<typename TNode>
	void DeleteLinkedList(TNode*& list_head)
	{
		for(TNode* cur = list_head->GetNext(); cur != NULL; cur = cur->GetNext())
			delete cur;

		delete list_head;
		list_head = NULL;
	}

	template<typename TNode>
	void AppendLinkedListNode(TNode*& list_head, TNode* object)
	{
		if(list_head == NULL)
		{
			list_head = object;
			return;
		}

		TNode* cur, * next;
		for(cur = list_head, next = cur->GetNext(); next != NULL; cur = next, next = cur->GetNext())
			;

		cur->SetNext(object);
		object->SetPrevious(cur);
	}

	template<typename TNode>
	void RemoveLinkedListNode(TNode*& list_head, const TNode* object)
	{
		TNode* next = object->GetNext();
		TNode* prev = object->GetPrevious();

		if(list_head == object)
		{
			if(next != NULL)
				next->SetPrevious(NULL);
			list_head = next;
			return;
		}

		if(prev != NULL) prev->SetNext(next);
		if(next != NULL) next->SetPrevious(prev);
	}

	template<typename TNode>
	void InsertAfterLinkedListNode(TNode* previous, TNode* object)
	{
		TNode* next = previous->GetNext();

		previous->SetNext(object);
		object->SetPrevious(previous);

		object->SetNext(next);
		if(next != NULL)
			next->SetPrevious(object);
	}

	template<typename TNode>
	TNode* GetNodeByIndex(TNode* list_head, int32 index)
	{
		int32 i = 0;

		TNode* cur, * next;
		for(cur = list_head; cur != NULL && (next = cur->GetNext()) != NULL && i < index; cur = next, i++)
			;

		if(i != index)
			return NULL;

		return cur;
	}
};