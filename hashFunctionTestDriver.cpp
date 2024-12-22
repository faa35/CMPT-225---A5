/*
 * hashFunctionTestDriver.cpp
 * 
 * Description: Testing hash functions.
 *
 * Note: dataFile_100_16.txt does not contain duplicates, i.e.,
 *       all the indexing keys it contains are distint.
 * 
 * Author: AL
 * Modification Date: April 2024
 */


#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include "UnableToInsertException.h"
#include "ElementAlreadyExistsException.h"
#include "EmptyDataCollectionException.h"
#include "ElementDoesNotExistException.h"
#include "Dictionary.h"
#include "Profile.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::setprecision;
using std::string;



// Description: Prints an histogram showing distribution of hash indices over the hash table.
// Postcondition: Dictionary remains unchanged.	
void histogram(unsigned int * initialProbes, unsigned int htCapacity) {

  cout << endl << "Histogram showing distribution of hash indices over the hash table: " << endl;
  for ( unsigned int i = 0; i < htCapacity; i++ ) {
    cout << "At hashTable[" << i << "]: ";
    for ( unsigned int j = 0; j < initialProbes[i]; j++ ) 
       cout << "*";
    cout << endl;
  } 
  return;
}

// Description: Prints various stats.
// Postcondition: Dictionary remains unchanged.	
void printStats(unsigned int * initialProbes, unsigned int htCapacity) {
  
  unsigned int results[10];

  for ( unsigned int j = 0; j < 10; j++ ) results[j] = 0;
  
  for ( unsigned int i = 0; i < htCapacity; i++ ) {
    // cout << "initialProbes[" << i << "] = " << initialProbes[i] << endl;
    results[initialProbes[i]]++;
  }
    
  cout << endl << "There are " << results[0] << " empty cells in this Dictionary (hash table) -> \t" 
       << setprecision(4) << results[0]/((float)htCapacity)*100 << "% of the hash table is empty." << endl;
  cout << "There are " << results[1] << " elements inserted that have no synonyms -> \t" 
       << setprecision(4) << results[1]/((float)htCapacity)*100
       << "% of the hash table has been probed only once (no collisions)." << endl;
  
  for ( unsigned int k = 2; k < 10; k++ ) {
    if ( results[k] > 0 ) 
      cout << "There were " << results[k] << " indexing keys that hashed into cells " << 
      k << " times -> \t" << setprecision(4) << results[k]/((float)htCapacity)*100
	  << "% of the hash table has been probed " << k << " times (" << k-1 << " collisions)." << endl;
  }
  
  return;
} 
  
  
int main(int argc, char *argv[]) {
 
  Dictionary * testing = new Dictionary();
  if (testing != nullptr ) {
    unsigned int htCapacity = testing->getCapacity();
    unsigned int initialProbes[htCapacity];      // Records the number of times distint indexing keys (synonyms)
                                                 // have hashed (probed) into to the same location (cell) of 
                                                 // the hash table (i.e., records the number of synonyms).
												 // We are only recording the initial (first) probe.
												 // The content of this array will show whether the 
												 // hash function evenly distribute the indexing keys across 
												 // the range of hash table (array) indices.
    for ( unsigned int i = 0; i < htCapacity; i++ ) initialProbes[i] = 0;
		  
    string anIndexingKey = "";
    Profile * tempP = nullptr;   
	
    // Enter he command: ./hftd dataFile_100_16.txt
    // Each line (element) of dataFile_100_16.txt is read as 
    // an indexing key (a string), which once converted into an 
    // unsigned int is then perceived as a userName and use to 
    // instantiate an object of Profile class. 
    // This object is then inserted into "testing",
    // which is an object of the Dictionary class.

    // As part of inserting this newElement,
    // anIndexingKey is hashed and the produced hash index is
    // used to insert anIndexingKey into the hashTable. 
    // See the implementation of insert(...) in hashDataCollection.cpp.
    if ( argc == 2 ) {
      ifstream myfile( argv[1] );
      if ( myfile.is_open() ) {
	    while ( getline( myfile, anIndexingKey ) ) {    
      	  // cout << "The indexing key read is " << anIndexingKey << endl; 
      	  //anIndexingKey = stoul(anIndexingKeyStr);
		  cout << "in hashFunctionTestDriver: indexingKey = " << anIndexingKey << endl;
		  
      	  tempP = new Profile(anIndexingKey);
		  // cout << "in hashFunctionTestDriver: Profile userName = " << tempP << endl;

		  // Testing for duplication
		  
      	  try { testing->insert(tempP); }
		  catch ( ElementAlreadyExistsException & anException ) {
			cout << "in hashFunctionTestDriver: Profile userName = " << *tempP << " already exists!" << endl;  
		  }
					
      	  unsigned int hashIndex = testing->hashFunction(anIndexingKey);
/*
          cout << "indexingKey = " << anIndexingKey << " h(indexingKey) = " << 
          hashIndex << " and this indexingKey has now probed hashTable[" << hashIndex << "] " << 
          initialProbes[hashIndex]+1 << " times." << endl;
*/  
		  
          initialProbes[hashIndex]++;
	    } 
	    myfile.close();
      	
	    // Print the content of the hashTable 
	    // Print an histogram showing how well (or poorly) the hash indices
	    // are distributed over the whole hashTable.
	    try {
			testing->printDictionary();
		}
		catch( EmptyDataCollectionException & anException) {
			// Do nothing!
			cout << "***Hash Table is empty!***" << endl;
		}
	    histogram(initialProbes, htCapacity);
	    printStats(initialProbes, htCapacity);        
      }
      else 
	    cout << "Unable to open file"; 
    }
    else 
      cout << "Forgot the file name? Try again!" << endl;

    delete testing;
    testing = nullptr;
  }
  
  return 0;
}
