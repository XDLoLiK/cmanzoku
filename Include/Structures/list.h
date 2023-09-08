#ifndef Man_LIST_H
#define Man_LIST_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct List {
	void *value;
	struct List *next;
};

struct List *List_New(void *elem, size_t elemSize);
struct List *List_Insert(struct List *head, void *elem, size_t elemSize);
struct List *List_Find(struct List *head, void *elem, size_t elemSize);
struct List *List_Erase(struct List *head, void *elem, size_t elemSize);
struct List *List_InsertAfter(struct List *head, struct List *where, struct List *what);
struct List *List_InsertBefore(struct List *head, struct List *where, struct List *what);
struct List *List_Delete(struct List *head);
struct List *List_Next(struct List *current);

#endif // Man_LIST_H
