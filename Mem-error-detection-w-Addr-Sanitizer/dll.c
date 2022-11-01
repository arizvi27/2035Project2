#include <stdio.h>
#include <stdlib.h>

typedef struct ll_node {
  int key;
  int data;
  struct ll_node * next;
} ll_node;

typedef struct {
  ll_node * head;
  ll_node * tail;
} LL;

// Create a new empty list.
LL* create_list(){
  LL* NewList = (LL *) malloc(sizeof(LL));
  if (!NewList) return NULL;
  NewList->head = NULL;
  NewList->tail = NULL;
  return NewList;
}

// if key is in list return a ptr to node.  // Otherwise return NULL
ll_node * find(int key, LL* list) {
  ll_node * this_node = list->head;
  while(this_node){
    if (this_node->key == key)
      return this_node;
    this_node = this_node->next;
  }
 return NULL;  // not found
}

// insert a node in the list, returning a ptr to new node or NULL
ll_node * insert(int key, int data, LL* list) {
  ll_node * this_node;
  ll_node * head = list->head;

  if ((this_node = find(key, list))){ 
      this_node->data = data;
      return this_node;
  }

  // key is not in list
  this_node=(ll_node *)malloc(sizeof(ll_node));
  if (!this_node) return NULL;

  this_node->data = data;
  this_node->key = key;
  this_node->next = head;
	head = this_node;

  return this_node;
}

int main() {
  LL* MyL;
  MyL = create_list();
  insert(7, 63, MyL);
  insert(4, 98, MyL);
  insert(2, 17, MyL);
  return(0);
}
