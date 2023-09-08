#include "Structures/list.h"

struct List *List_New(void *elem, size_t elemSize)
{
	struct List *newList = calloc(1, sizeof (struct List));
	if (!newList) {
		return NULL;
	}

	newList->value = calloc(1, elemSize);
	if (newList->value) {
		free(newList);
		return NULL;
	}
	memcpy(newList->value, elem, elemSize);
	newList->next  = NULL;

	return newList;
}


struct List *List_Insert(struct List *head, void *elem, size_t elemSize)
{
	if (head == NULL) {
		return List_New(elem, elemSize);
	}

	struct List *currentList = head;
	while (currentList->next) {
		currentList = currentList->next;
	}
	currentList->next = List_New(elem, elemSize);

	return head;
}


struct List *List_Find(struct List *head, void *elem, size_t elemSize)
{
	struct List *currentList = head;
	while (currentList) {
		if (memcmp(currentList->value, elem, elemSize) == 0) {
			return currentList;
		}
		currentList = currentList->next;
	}

	return NULL;
}


struct List *List_Erase(struct List *head, void *elem, size_t elemSize)
{
	if (memcmp(head->value, elem, elemSize) == 0) {
		struct List *newHead = head->next;
		free(head);
		return newHead;
	}

	struct List *currentList = head;
	while (currentList->next) {
		if  (memcmp(currentList->next->value, elem, elemSize) == 0) {
			struct List* newNext = currentList->next->next;
			free(currentList->next);
			currentList->next = newNext;
			break;
		}
		currentList = currentList->next;
	}

	return head;
}


struct List *List_InsertAfter(struct List *head, struct List *where, struct List *what)
{
	if (where == NULL || what == NULL) {
		return head;
	}

	if (head  == NULL) {
		return what;
	}

	struct List *temp = where->next;
	where->next = what;
	what->next  = temp;

	return head;
}


struct List *List_InsertBefore(struct List *head, struct List *where, struct List *what)
{
	if (where == NULL || what == NULL) {
		return head;
	}

	if (head  == NULL) {
		return what;
	}

	if (where == head) {
		what->next = where;
		return what;
	}

	struct List *currentList = head;
	while (currentList->next) {
		if  (currentList->next == where) {
			struct List* temp = currentList->next;
			currentList->next = what;
			what->next = temp;
			break;
		}
		currentList = currentList->next;
	}

	return head;
}


struct List *List_Delete(struct List *head)
{
	struct List *currentList = head;
	while(currentList) {
		struct List *nextList = currentList->next;
		free(currentList);
		currentList = nextList;
	}

	return NULL;
}


struct List *List_Next(struct List *current)
{
	if (current == NULL) {
		return NULL;
	}
	return current->next;
}
