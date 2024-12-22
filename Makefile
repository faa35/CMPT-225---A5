all: dtest

dtest: Dictionary.o hashFunctionTestDriver.o ElementDoesNotExistException.o ElementAlreadyExistsException.o EmptyDataCollectionException.o UnableToInsertException.o Profile.o
	g++ -Wall -o dtest Dictionary.o hashFunctionTestDriver.o ElementDoesNotExistException.o ElementAlreadyExistsException.o EmptyDataCollectionException.o UnableToInsertException.o Profile.o

Dictionary.o: Dictionary.h Dictionary.cpp Profile.h
	g++ -Wall -c Dictionary.cpp

hashFunctionTestDriver.o: hashFunctionTestDriver.cpp Profile.h
	g++ -Wall -c hashFunctionTestDriver.cpp 

ElementDoesNotExistException.o: ElementDoesNotExistException.h ElementDoesNotExistException.cpp
	g++ -Wall -c ElementDoesNotExistException.cpp

ElementAlreadyExistsException.o: ElementAlreadyExistsException.h ElementAlreadyExistsException.cpp
	g++ -Wall -c ElementAlreadyExistsException.cpp
		
EmptyDataCollectionException.o: EmptyDataCollectionException.h EmptyDataCollectionException.cpp
	g++ -Wall -c EmptyDataCollectionException.cpp
	
UnableToInsertException.o: UnableToInsertException.h UnableToInsertException.cpp
	g++ -Wall -c UnableToInsertException.cpp

Profile.o: Profile.h Profile.cpp
	g++ -Wall -c Profile.cpp

clean:
	rm -f td *.o
