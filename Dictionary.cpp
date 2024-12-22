/*
 * Dictionary.cpp
*
 * Class Description: Dictionary data collection ADT class.
 *                    The implementation of this class is based on 
 *                    the hashing strategy and the open addressing 
 *                    collision resolution strategy called linear probing hashing. 
 *            
 * Class Invariant: Each element stored in this Dictionary is unique (no duplications allowed).
 *
 *
 * Author: Macklin Tsang and Fardin Abdulla
 * Date : April 12, 2024
 * 
 */

// You can add more #include statements if you wish.

#include <iostream>
#include <cstring>
#include "Dictionary.h"  
#include "Profile.h"
#include "UnableToInsertException.h"
#include "ElementAlreadyExistsException.h"
#include "EmptyDataCollectionException.h"
#include "ElementDoesNotExistException.h"

using std::cout;
using std::endl;
using std::__cxx11::stoul;

// Constructor
Dictionary::Dictionary() {
  // Dynamically allocates an array of Profile pointers
  hashTable = new Profile *[CAPACITY];
  for (unsigned int i = 0; i < CAPACITY; ++i)
  {
    // Initializes all elements to nullptr
    hashTable[i] = nullptr;
  }
}

// Destructor
// Description: Destruct a Dictionary object, releasing heap-allocated memory.
Dictionary::~Dictionary() {
  // Release all heap memory
  if ( hashTable != nullptr ) { 
    for ( unsigned int i = 0; i < CAPACITY; i++ ) 
      if ( hashTable[i] != nullptr ) {      
        delete hashTable[i]; // Delete object of Profile class on heap
        hashTable[i] = nullptr;
      }
    delete [] hashTable;
    hashTable = nullptr;	
  }
}

// Description: Returns the total element count currently stored in Dictionary.
// Postcondition: Dictionary remains unchanged.
unsigned int Dictionary::getElementCount() const {
   return elementCount;
}

// Description: Returns the CAPACITY (size) of the Dictionary.
// Postcondition: Dictionary remains unchanged.
unsigned int Dictionary::getCapacity() const{
   return CAPACITY;
}

// Hash Function
// Description: Hashes the given indexingKey producing a "hash table index".
// Time Efficiency: O(1)
// Space Efficiency: O(1) 
unsigned int Dictionary::hashFunction( string indexingKey ) {
  try
  {
    // Hash a numeric key
    // First convert string to unsigned long
    uint64_t indexKeyInt = std::stoul(indexingKey); 
    // then hash the key
    indexKeyInt = (indexKeyInt ^ (indexKeyInt >> 16)) * 0x45d9f3b; 
    return indexKeyInt % CAPACITY;
  }
  catch (std::invalid_argument &e)
  {
    // Fallback for non-numeric keys
    unsigned long hash = 5381;
    for (char c : indexingKey)
    {
      hash = ((hash << 5) + hash) + c;
    }
    return hash % CAPACITY; // O(1)
  }
}

// Description: Inserts an element into the Dictionary and increments "elementCount".
//              For this Assignment 5, you do not have to expand the hashTable when it is full. 
// Precondition: newElement must not already be in in the Dictionary.  
// Exception: Throws UnableToInsertException if we cannot insert newElement in the Dictionary.
//            For example, if the operator "new" fails, or Dictionary is full (temporary solution).
// Exception: Throws ElementAlreadyExistsException if newElement is already in the Dictionary. 
void Dictionary::insert( Profile * newElement )  {



  // Check for full dictionary
  if (elementCount == CAPACITY)
  {
    throw UnableToInsertException("Dictionary is full.");
  }

  // Hashing to find index
  unsigned int index = hashFunction(newElement->getUserName());
  // Save original index for loop termination check
  unsigned int ogIndex = index;
  do
  {
    // Check for empty cell
    if (hashTable[index] == nullptr)
    {
      // When found a cell: insert newElement in hashTable at hashIndex
      hashTable[index] = newElement;
      // One more element inserted!
      elementCount++;
      return;
    }
    // Checking for duplicate element
    if (hashTable[index]->getUserName() == newElement->getUserName())
    {
      throw ElementAlreadyExistsException("Element already exists.");
    }
    // Linear probing for collision resolution
    index = (index + 1) % CAPACITY;
  } while (index != ogIndex);

  throw UnableToInsertException("Cannot insert new element.");
}


// Description: Returns a pointer to the target element if found.
// Postcondition: Dictionary remains unchanged.
// Exception: Throws EmptyDataCollectionException if the Dictionary is empty.
// Exception: Throws ElementDoesNotExistException if target is not found in the Dictionary.
Profile * Dictionary::get( Profile & target )  {

// Put your code here!
  if (elementCount == 0)
  {
    // Check for empty dictionary
    throw EmptyDataCollectionException("Dictionary is empty.");
  }

  // Hashing to find index
  unsigned int index = hashFunction(target.getUserName());
  // Save original index for loop termination check
  unsigned int ogIndex = index;
  do
  {
    if (hashTable[index] != nullptr)
    {
      if (hashTable[index]->getUserName() == target.getUserName())
      {
        // Return the element if found
        return hashTable[index];
      }
    }
    else
    {
      // Element does not exist
      throw ElementDoesNotExistException("Element not found.");
    }
    index = (index + 1) % CAPACITY;
  } while (index != ogIndex);
  throw ElementDoesNotExistException("Element not found.");
 
}
   
// Description: Prints all elements stored in the Dictionary (unsorted).
// Postcondition: Dictionary remains unchanged.
// Exception: Throws EmptyDataCollectionException if the Dictionary is empty.
void Dictionary::printDictionary()  const {
  if ( elementCount == 0 ) 
     throw EmptyDataCollectionException("In printDictionary(): hashTable is empty.");

  cout << endl << "Printing the Dictionary with " << this->elementCount << " elements: " << endl;
  for (unsigned int index = 0; index < CAPACITY; index++) {
    if ( hashTable[index] != nullptr ) 
		cout << "HashTable[" << index << "] = " << *(hashTable[index]) << endl; 
  }   
  return;
}
