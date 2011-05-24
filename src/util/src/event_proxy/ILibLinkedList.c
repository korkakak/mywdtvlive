/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2002, 2003 Intel Corporation.  All rights reserved.
 * 
 * 'bothway linked list utility'
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "ILibLinkedList.h"

struct XLinkedListNode
{
    void *Data;
    struct XLinkedListNode_Root *Root;
    struct XLinkedListNode *Next;
    struct XLinkedListNode *Previous;
};
struct XLinkedListNode_Root
{
    sem_t LOCK;
    long count;
    struct XLinkedListNode *Head;
    struct XLinkedListNode *Tail;
};
struct XStackNode
{
    void *Data;
    struct XStackNode *Next;
};

void *XQueue_Create()
{
    return(XLinkedList_Create());
}

void XQueue_Lock(void *q)
{
    XLinkedList_Lock(q);
}

void XQueue_UnLock(void *q)
{
    XLinkedList_UnLock(q);
}

void XQueue_Destroy(void *q)
{
    XLinkedList_Destroy(q);
}

int XQueue_IsEmpty(void *q)
{
    return(XLinkedList_GetNode_Head(q)==NULL?1:0);
}

void XQueue_EnQueue(void *q, void *data)
{
    XLinkedList_AddTail(q,data);
}
void *XQueue_DeQueue(void *q)
{
    void *RetVal = NULL;
    void *node;

    node = XLinkedList_GetNode_Head(q);
    if(node!=NULL)
    {
        RetVal = XLinkedList_GetDataFromNode(node);
        XLinkedList_Remove(node);
    }
    return(RetVal);
}

void *XQueue_PeekQueue(void *q)
{
    void *RetVal = NULL;
    void *node;

    node = XLinkedList_GetNode_Head(q);
    if(node!=NULL)
    {
        RetVal = XLinkedList_GetDataFromNode(node);
    }
    return(RetVal);
}

void XCreateStack(void **TheStack)
{
    *TheStack = NULL;
}

void XPushStack(void **TheStack, void *data)
{
    struct XStackNode *RetVal = (struct XStackNode*)malloc(sizeof(struct XStackNode));
    RetVal->Data = data;
    RetVal->Next = *TheStack;
    *TheStack = RetVal;
}

void *XPopStack(void **TheStack)
{
    void *RetVal = NULL;
    void *Temp;
    if(*TheStack!=NULL)
    {
        RetVal = ((struct XStackNode*)*TheStack)->Data;
        Temp = *TheStack;
        *TheStack = ((struct XStackNode*)*TheStack)->Next;
        free(Temp);
    }
    return(RetVal);
}

void *XPeekStack(void **TheStack)
{
    void *RetVal = NULL;
    if(*TheStack!=NULL)
    {
        RetVal = ((struct XStackNode*)*TheStack)->Data;
    }
    return(RetVal);
}

void XClearStack(void **TheStack)
{
    void *Temp = *TheStack;
    do
    {
        XPopStack(&Temp);
    }while(Temp!=NULL);
    *TheStack = NULL;
}

void* XLinkedList_Create()
{
    struct XLinkedListNode_Root *root = (struct XLinkedListNode_Root*)malloc(sizeof(struct XLinkedListNode_Root));
    root->Head = NULL;
    root->Tail = NULL;
    root->count=0;
    sem_init(&(root->LOCK),0,1);
    return(root);
}
void* XLinkedList_ShallowCopy(void *LinkedList)
{
    void *RetVal = XLinkedList_Create();
    void *node = XLinkedList_GetNode_Head(LinkedList);
    while(node!=NULL)
    {
        XLinkedList_AddTail(RetVal,XLinkedList_GetDataFromNode(node));
        node = XLinkedList_GetNextNode(node);
    }
    return(RetVal);
}
void* XLinkedList_GetNode_Head(void *LinkedList)
{
    return(((struct XLinkedListNode_Root*)LinkedList)->Head);
}
void* XLinkedList_GetNode_Tail(void *LinkedList)
{
    return(((struct XLinkedListNode_Root*)LinkedList)->Tail);
}
void* XLinkedList_GetNextNode(void *LinkedList_Node)
{
    return(((struct XLinkedListNode*)LinkedList_Node)->Next);
}
void* XLinkedList_GetPreviousNode(void *LinkedList_Node)
{
    return(((struct XLinkedListNode*)LinkedList_Node)->Previous);
}
void* XLinkedList_GetDataFromNode(void *LinkedList_Node)
{
    return(((struct XLinkedListNode*)LinkedList_Node)->Data);
}
void XLinkedList_InsertBefore(void *LinkedList_Node, void *data)
{
    struct XLinkedListNode_Root *r = ((struct XLinkedListNode*)LinkedList_Node)->Root;
    struct XLinkedListNode *n = (struct XLinkedListNode*) LinkedList_Node;

    struct XLinkedListNode *newNode = (struct XLinkedListNode*)malloc(sizeof(struct XLinkedListNode));
    newNode->Data = data;
    newNode->Root = r;
    
    newNode->Next = n;
    newNode->Previous = n->Previous;
    if(newNode->Previous!=NULL)
    {
        newNode->Previous->Next = newNode;
    }
    n->Previous = newNode;
    if(r->Head==n)
    {
        r->Head = newNode;
    }
    ++r->count;
}

void XLinkedList_InsertAfter(void *LinkedList_Node, void *data)
{
    struct XLinkedListNode_Root *r = ((struct XLinkedListNode*)LinkedList_Node)->Root;
    struct XLinkedListNode *n = (struct XLinkedListNode*) LinkedList_Node;

    struct XLinkedListNode *newNode = (struct XLinkedListNode*)malloc(sizeof(struct XLinkedListNode));
    newNode->Data = data;
    newNode->Root = r;


    newNode->Next = n->Next;
    n->Next = newNode;
    newNode->Previous = n;
    if(newNode->Next!=NULL)
    {
        newNode->Next->Previous = newNode;
    }

    if(r->Tail==n)
    {
        r->Tail = newNode;
    }

    ++r->count;
}

void XLinkedList_Remove(void *LinkedList_Node)
{
    struct XLinkedListNode_Root *r = ((struct XLinkedListNode*)LinkedList_Node)->Root;
    struct XLinkedListNode *n = (struct XLinkedListNode*) LinkedList_Node;

    if(n->Previous!=NULL)
    {
        n->Previous->Next = n->Next;
    }
    if(n->Next!=NULL)
    {
        n->Next->Previous = n->Previous;
    }
    if(r->Head==n)
    {
        r->Head = n->Next;
    }
    if(r->Tail==n)
    {
        r->Tail = n->Previous;
    }
    --r->count;
    free(n);
}
void XLinkedList_Remove_ByData(void *LinkedList, void *data)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;
    struct XLinkedListNode *n,*t;

    n = r->Head;
    while(n!=NULL)
    {
        if(n->Data==data)
        {
            --r->count;
            if(n->Previous!=NULL)
            {
                n->Previous->Next = n->Next;
            }
            if(n->Next!=NULL)
            {
                n->Next->Previous = n->Previous;
            }
            if(r->Head==n)
            {
                r->Head = n->Next;
            }
            if(r->Tail==n)
            {
                r->Tail = n->Previous;
            }
            t = n->Next;
            free(n);
            n = t;
        }
        else
        {
            n = n->Next;
        }
    }
}
void XLinkedList_AddHead(void *LinkedList, void *data)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;
    struct XLinkedListNode *newNode = (struct XLinkedListNode*)malloc(sizeof(struct XLinkedListNode));
    newNode->Data = data;
    newNode->Root = r;
    newNode->Previous = NULL;

    newNode->Next = r->Head;
    if(r->Head!=NULL)
    {
        r->Head->Previous = newNode;
    }
    r->Head = newNode;
    if(r->Tail==NULL)
    {
        r->Tail = newNode;
    }

    ++r->count;
}
void XLinkedList_AddTail(void *LinkedList, void *data)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;
    struct XLinkedListNode *newNode = (struct XLinkedListNode*)malloc(sizeof(struct XLinkedListNode));
    newNode->Data = data;
    newNode->Root = r;
    newNode->Next = NULL;

    newNode->Previous = r->Tail;
    if(r->Tail!=NULL)
    {
        r->Tail->Next = newNode;
    }
    r->Tail = newNode;
    if(r->Head==NULL)
    {
        r->Head = newNode;
    }

    ++r->count;
}

void XLinkedList_Lock(void *LinkedList)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;

    sem_wait(&(r->LOCK));
}
void XLinkedList_UnLock(void *LinkedList)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;
    
    sem_post(&(r->LOCK));
}
void XLinkedList_Destroy(void *LinkedList)
{
    struct XLinkedListNode_Root *r = (struct XLinkedListNode_Root*)LinkedList;

    while(r->Head!=NULL)
    {
        XLinkedList_Remove(XLinkedList_GetNode_Head(LinkedList));
    }
    sem_destroy(&(r->LOCK));
    free(r);
}
long XLinkedList_GetCount(void *LinkedList)
{
    return(((struct XLinkedListNode_Root*)LinkedList)->count);
}
