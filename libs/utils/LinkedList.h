#ifndef LinkedList_h__
#define LinkedList_h__


struct T_LinkedList;
typedef struct T_LinkedList LinkedList;

struct T_LinkedList_Node;
typedef struct T_LinkedList_Node LinkedList_Node;

struct T_LinkedList_Node
{
	void* item;
	LinkedList_Node* prev;
	LinkedList_Node* next;

};

struct T_LinkedList
{
	LinkedList_Node* head;
	LinkedList_Node* tail;
	int length;
};


int LinkedList_Initialize(LinkedList* _List);

int LinkedList_MoveToStart(LinkedList* _List, void* _Item);
int LinkedList_MoveToEnd(LinkedList* _List, void* _Item);

LinkedList_Node* LinkedList_Create(void* _Item);

int LinkedList_IndexOf(LinkedList* _List, void* _Item);

int LinkedList_Add(LinkedList* _List, void* _Item, int _Index);
int LinkedList_AddFirst(LinkedList* _List, void* _Item);
int LinkedList_AddLast(LinkedList* _List, void* _Item);
#define LinkedList_Push(a, b) ((int)LinkedList_AddLast(a, b))

int LinkedList_InsertBefore(LinkedList* _List, LinkedList_Node* _RefNode, void* _Item);


void* LinkedList_Get(LinkedList* _List, int _Index);
void* LinkedList_GetFirst(LinkedList* _List);
LinkedList_Node* LinkedList_GetFirstNode(LinkedList* _List);
void* LinkedList_GetLast(LinkedList* _List);
LinkedList_Node* LinkedList_GetLastNode(LinkedList* _List);

void* LinkedList_Remove(LinkedList* _List, void* _Item);
void* LinkedList_RemoveAt(LinkedList* _List, int _Index);
void* LinkedList_RemoveFirst(LinkedList* _List);
void* LinkedList_RemoveLast(LinkedList* _List);

void LinkedList_Clear(LinkedList* _List);
void LinkedList_Dispose(LinkedList* _List);

//#define LinkedList_ForEach(list, item) for (LinkedList_Node* _node = (list)->head; _node != NULL && ((*(item) = _node->item) || 1); _node = _node->next)

#define LinkedList_ForEach(_List, _Item) \
    LinkedList_Node* _node = (_List)->head; \
	unsigned int _index; \
    for (_index = 0; _node != NULL && ((*(_Item) = _node->item) || 1); _node = _node->next, _index++)






#endif // LinkedList_h__
