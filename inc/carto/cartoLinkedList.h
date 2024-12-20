#ifndef CARTOLINKEDLIST
#define CARTOLINKEDLIST
// 10-08-2019 - Ray Bambery - added void to LinkedList_getLinkedList
// 10-09-2019 - Ray Bambery - added LinkedList_removeNode
///////////////////////////////////////////
struct node{
   void *data;
   struct node *next;
   struct node *prev;
   int rank;
};

///////////////////////////////////////////
typedef struct{
   struct node *head;
   struct node *tail;
   int size;
} LINKEDLIST;

/*********************************************/
LINKEDLIST* LinkedList_getLinkedList(void);

/*********************************************/
void LinkedList_setNodeArray(LINKEDLIST *list, struct node** array);

/*********************************************/
void LinkedList_addWithRank(LINKEDLIST *list, void *data, int rank);

/*********************************************/
void LinkedList_add(LINKEDLIST *list, void *data);

/**********************************************/
void LinkedList_removeNode(LINKEDLIST *list, struct node *n);

/*********************************************/
void* LinkedList_remove(LINKEDLIST *list, int index);

/*********************************************/
void LinkedList_free(LINKEDLIST **list);

/*********************************************/
LINKEDLIST* LinkedList_sortAscending(LINKEDLIST **list);

/*********************************************/
LINKEDLIST* LinkedList_bigMemSortAscending(LINKEDLIST **list);

/*********************************************/
struct node* LinkedList_getMinNode(LINKEDLIST *list);

/*********************************************/
//struct node* LinkedList_getNode(LINKEDLIST *list, int index);

#endif
