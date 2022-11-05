// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


/*
Student Name: Ali Rizvi
Date: November 4th, 2022
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
    e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
    e.g. "_HashTable", "_HashTableEntry"

    Recall that in C, we have to type "struct" together with the name of the struct
    in order to initialize a new variable. To avoid this, in hash_table.h
    we use typedef to provide new "nicknames" for "struct _HashTable" and
    "struct _HashTableEntry". As a result, we can create new struct variables
    by just using:
        - "HashTable myNewTable;"
    or
        - "HashTableEntry myNewHashTableEntry;"
    
    The preceding underscore "_" simply provides a distinction between the names
    of the actual struct defition and the "nicknames" that we use to initialize
    new structs.
    [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
    the first letter is lower-case.
    e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
        It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
    as a distinction between function local variables and struct members.
    e.g. "num_buckets" is a member of "HashTable".
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // Allocate memory for the HashTableEntry 
    HashTableEntry* entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));

    // Initialize struct members 
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    //return the pointer to the HashTableEntry
    return entry;
}




/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    //In order to get to an item, must get the bucket index using the hash function in hashtable
    HashTableEntry* newPtr;
    unsigned int bucketLocation = hashTable->hash(key);
    newPtr = hashTable->buckets[bucketLocation];
    if (!newPtr) {
        return NULL;
    }
    if (newPtr->key == key) {
        // returns the pointer to the entry 
        return newPtr;
    }

    HashTableEntry* currentNode = newPtr->next;
    while (currentNode) {
        if (currentNode->key == key) {
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;

}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable) {
    unsigned int numBucs = hashTable->num_buckets;
    for (unsigned int i = 0; i < numBucs; i++) { //will loop through each bucket of the hashtable 
        if (hashTable->buckets[i] == NULL) { //if the hashtable bucket points to null already nothing needed to be done
            continue;
        } else {
            HashTableEntry* current = hashTable->buckets[i]; //traverses through each entry and frees up the space of each entry 
            if (current == NULL) { //if current is null then can continue to the next bucket since this bucket has no entries 
                continue;
            } else { //this else statement contains code that will traverse through the linked list of hashtable entrys and frees the space for each one. 
                HashTableEntry* nextNode;
                void* val;
                
                while (current) {  //while loop that traverses through code and removes each node by freeing up space from the value and node itself until null hit
                    nextNode = current->next;
                    val = current->value;
                    if (val) {
                        free(current->value);
                    }
                    free(current);
                    current = nextNode;
                }
            }
        }
    }
    free(hashTable->buckets); //will free all of the buckest of the hashtable
    free(hashTable);        // will free up the hashtable in memory 
   
    return;
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    HashTableEntry* entr = findItem(hashTable, key); //to check if the entry exists for that key 

    if (entr) {                 // to check if an entry does exist already 
        void* oldval = entr->value; //this will keep track of the old value 
        entr->value = value; // this will replace the entry's value with the new value
        return oldval;
    } else {
        unsigned int bucketNum = hashTable->hash(key);     //this will retreieve the bucket id 
        HashTableEntry* bucketEntrys = hashTable->buckets[bucketNum]; //get pointer to the bucket 
        HashTableEntry* newEntry = createHashTableEntry(key, value);
        hashTable->buckets[bucketNum] = newEntry; //adds the new entry to the start of the bucket 

        if (bucketEntrys) {                // if bucket is not empty 
            newEntry->next = bucketEntrys;   // connect the original linked list to the new head
        }                                       // otherwise do nothing else
        return NULL;
    }

}

void *getItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry* entry = findItem(hashTable, key); //gets entry with key and hashtable information

    if (entry) {
        return entry->value; //returns the value of the entry previously created 
    }
    return NULL;
}

void *removeItem(HashTable *hashTable, unsigned int key)
{

    HashTableEntry* entry = findItem(hashTable, key);
    
    if (entry) { //if the entry exists and is not null we can continue to preform the remove operations
        void* val = entry->value;
        unsigned int bucketNum = hashTable->hash(key);
        HashTableEntry* currentNode = hashTable->buckets[bucketNum];

        if (!currentNode) {
            return val;
        }
        //if the node that we want to remove is located within the first node 
        if (currentNode->key == key) {
            hashTable->buckets[bucketNum] = currentNode->next;  // this will make the bucket point to the next value in the list
            free(currentNode);
            return val;
        }
        //if the node that we want to remove is not within the first node 
        while(currentNode) {
            if (currentNode->next->key == key) {
                // the next node needs to be removed
                HashTableEntry* temp = currentNode->next->next;
                free(currentNode->next);
                currentNode->next = temp;
                return val;
            }
            currentNode = currentNode->next;
        }

        return val;
    }

    return NULL; //will return NULL if no entries exist of if the key is not present in the list 

}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry* entry = findItem(hashTable, key); //this will check if the entry exists or not will save a lot of code

    if (entry) { //if the entry exists and is not nulll we can continue to preform the operations to remove it 
        unsigned int bucketNum = hashTable->hash(key);
        HashTableEntry* currentNode = hashTable->buckets[bucketNum];

        if (!currentNode) { //if the currentNode is null then we do not need to do anything and we just return the function technically this scenario should not happen
            return;
        }
        void* val; //will hold the value of the currentNode to be removed 

        //if the node that we want to remove is located within the first node 
        if (currentNode->key == key) {
            hashTable->buckets[bucketNum] = currentNode->next;  // this will make the bucket point to the next value in the list
            val = currentNode->value;
            if (val) {
                free(val); //frees up the value space
            }
            free(currentNode); //will free up the currentNode space 
        }
        //if the node that we want to remove is not within the first node 
        while(currentNode) { //will terminate when the currentNode is equal to NULL
            if (currentNode->next->key == key) { //this will check ahead to see if the next entry's key matches the input key 
                // the next node needs to be removed
                HashTableEntry* temp = currentNode->next->next;
                val = currentNode->next->value;
                if (val) {
                    free(val);
                }
                free(currentNode->next);
                currentNode->next = temp;
            }
            currentNode = currentNode->next;
        }
    }

}
