#include "LinkedList.h"
#include <stdlib.h>

int LinkedList__Internal_InsertBefore(LinkedList* _List, LinkedList_Node* _RefNode, LinkedList_Node* _NewNode);

int LinkedList_Initialize(LinkedList* _List)
{
	_List->length = 0;
	_List->head = NULL;
	_List->tail = NULL;

	return 0;
}

int LinkedList_MoveToStart(LinkedList* _List, void* _Item)
{
	void* item = LinkedList_Remove(_List, _Item);
	if(item == NULL)
		return 1;

	return LinkedList_AddFirst(_List, item);
}

int LinkedList_MoveToEnd(LinkedList* _List, void* _Item)
{
	void* item = LinkedList_Remove(_List, _Item);
	if(item == NULL)
		return 1;

	return LinkedList_AddLast(_List, item);
}

LinkedList_Node* LinkedList_Create(void* _Item)
{
	LinkedList_Node* node = (LinkedList_Node*)malloc(sizeof(LinkedList_Node));
	if(node == NULL)
		return NULL;

	node->item = _Item;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

int LinkedList_IndexOf(LinkedList* _List, void* _Item)
{
	if(_List->head == NULL || _List->length == 0)
		return -1;

	LinkedList_Node* node = _List->head;

	int i;
	for (i = 0; i < _List->length; i++)
	{
		if(node->item == _Item)
			return i;

		node = node->next;
	}

	return -1;
}

int LinkedList_Add(LinkedList* _List, void* _Item, int _Index)
{
	if (_Index > _List->length)
		return -2;

	if (_Index == 0)
	{
		LinkedList_AddFirst(_List, _Item);
	}
	else if(_Index == _List->length)
	{
		LinkedList_AddLast(_List, _Item);
	}
	else
	{
		LinkedList_Node* node = _List->head;
		int i = 0;
		while (i < _Index)
		{
			node = node->next;
			i++;
		}

		LinkedList_Node* newNode = LinkedList_Create(_Item);
		if(newNode == NULL)
			return -1;

		LinkedList__Internal_InsertBefore(_List, node, newNode);
		_List->length++;
	}

	return 0;
}

int LinkedList_AddFirst(LinkedList* _List, void* _Item)
{
	LinkedList_Node* newNode = LinkedList_Create(_Item);
	if(newNode == NULL)
		return -1;

	LinkedList_Node* head = _List->head;

	if (head == NULL)
	{
		_List->head = newNode;
	}
	else
	{
		LinkedList_Node* last = _List->tail;

		if (last == NULL)
			last = head;

		newNode->next = head;
		head->prev = newNode;
		_List->head = newNode;
		_List->tail = last;
	}

	_List->length++;

	return 0;
}

int LinkedList_AddLast(LinkedList* _List, void* _Item)
{
	LinkedList_Node* newNode = LinkedList_Create(_Item);
	if(newNode == NULL)
		return -1;

	LinkedList_Node* head = _List->head;
	LinkedList_Node* tail = _List->tail;

	if (head == NULL)
	{
		_List->head = newNode;
	}
	else
	{
		LinkedList_Node* lastNode = tail;
		if (tail == NULL)
			lastNode = head;

		lastNode->next = newNode;
		newNode->prev = lastNode;
		_List->tail = newNode;
	}
	_List->length++;

	return 0;
}

int LinkedList__Internal_InsertBefore(LinkedList* _List, LinkedList_Node* _RefNode, LinkedList_Node* _NewNode)
{
	LinkedList_Node* prev = _RefNode->prev;
	if(prev == NULL)
	{
		_RefNode->prev = _NewNode;
		_NewNode->next = _RefNode;
		_NewNode->prev = NULL;

		_List->head = _NewNode;
		_List->tail = _RefNode;
	}
	else
	{
		_RefNode->prev = _NewNode;
		_NewNode->next = _RefNode;
		prev->next = _NewNode;
		_NewNode->prev = prev;
	}

	return 0;
}

int LinkedList_InsertBefore(LinkedList* _List, LinkedList_Node* _RefNode, void* _Item)
{
	LinkedList_Node* newNode = LinkedList_Create(_Item);
	if(newNode == NULL)
		return -1;

	LinkedList__Internal_InsertBefore(_List, _RefNode, newNode);
	_List->length++;

	return 0;
}

void* LinkedList_Get(LinkedList* _List, int _Index)
{
	if (_List->length == 0 || _Index >= _List->length)
		return NULL;

	if(_Index == 0)
		return LinkedList_GetFirst(_List);

	if(_Index+1 == _List->length)
		return LinkedList_GetLast(_List);


	LinkedList_Node* node = _List->head;

	int i = 0;
	while (i < _Index)
	{
		node = node->next;
		i++;
	}
	return node->item;
}

void* LinkedList_GetFirst(LinkedList* _List)
{
	if (_List->length == 0)
		return NULL;

	return _List->head->item;
}

LinkedList_Node* LinkedList_GetFirstNode(LinkedList* _List)
{
	if (_List->length == 0)
		return NULL;

	return _List->head;
}

void* LinkedList_GetLast(LinkedList* _List)
{
	if (_List->length == 0)
		return NULL;

	if (_List->length == 1)
		return LinkedList_GetFirst(_List);

	return _List->tail->item;
}

LinkedList_Node* LinkedList_GetLastNode(LinkedList* _List)
{
	if (_List->length == 0)
		return NULL;

	if (_List->length == 1)
		return LinkedList_GetFirstNode(_List);

	return _List->tail;
}

void* LinkedList_Remove(LinkedList* _List, void* _Item)
{
	int index = LinkedList_IndexOf(_List, _Item);
	if(index < 0)
		return NULL;

	return LinkedList_RemoveAt(_List, index);
}

void* LinkedList_RemoveAt(LinkedList* _List, int _Index)
{
	if (_List->length == 0)
		return NULL;

	else if (_Index >= _List->length)
		return NULL;



	if (_Index == 0)
	{
		return LinkedList_RemoveFirst(_List);
	}
	else if (_Index + 1 == _List->length)
	{
		return LinkedList_RemoveLast(_List);
	}
	else
	{
		LinkedList_Node* node = _List->head;
		LinkedList_Node* prev;
		LinkedList_Node* next;
		int i = 0;
		void* item;
		while (i < _Index)
		{
			node = node->next;
			i++;
		}
		item = node->item;

		prev = node->prev;
		next = node->next;
		prev->next = next;
		next->prev = prev;

		free(node);
		_List->length--;

		return item;
	}
}

void* LinkedList_RemoveFirst(LinkedList* _List)
{
	LinkedList_Node* head = _List->head;
	void* item = NULL;

	if (head == NULL)
		return NULL;

	item = head->item;
	LinkedList_Node* next = head->next;
	_List->head = next;

	if (next != NULL)
		next->prev = NULL;

	free(head);
	_List->length--;

	if (_List->length <= 1)
		_List->tail = NULL;

	return item;
}

void* LinkedList_RemoveLast(LinkedList* _List)
{
	if (_List->length == 0)
		return NULL;

	if (_List->length == 1)
		return LinkedList_RemoveFirst(_List);

	LinkedList_Node* tail = _List->tail;
	LinkedList_Node* prev = tail->prev;

	void* item = tail->item;
	prev->next = NULL;

	if (_List->length > 1)
		_List->tail = prev;

	free(tail);
	_List->length--;

	if (_List->length <= 1)
		_List->tail = NULL;

	return item;
}

void LinkedList_Clear(LinkedList* _List)
{
	while(_List->length > 0)
		LinkedList_RemoveFirst(_List);

}

void LinkedList_Dispose(LinkedList* _List)
{
	LinkedList_Clear(_List);

	_List->length = 0;
	_List->head = NULL;
	_List->tail = NULL;
}

