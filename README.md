# SWECW2-AudioLibrary

CST2550 Coursework 2

Introduction:
The aim of this coursework project is to design and implement an efficient track search functionality for a new audio streaming service. The program must store track details in a suitable data structure and provide functionalities to add tracks from a file, save tracks to a file, search tracks by artist/band name, and remove specific tracks. The program should work with any file of the provided format and allow users to specify the input file name, during or after the program invocation.

Selection Of Data Structure and Design: 
To store the track details, I chose a hash table as the primary data structure. The hash table is a suitable choice due to its average-case constant time complexity for insertion, deletion, and search operations. Two separate hash tables will be used to store track information, one using the track's title as the key and the other using the artist's name as the key. This design choice ensures efficient searches by either track title or artist name.

The Track class will be implemented to store the necessary details of an audio track, including its title, artist, and duration. The HashTable class will be created to manage the insertion, deletion, and search operations on the hash table. The FileLoader class is responsible for loading and saving tracks to and from files.

Pseudo code:

Data Structure: HashTable

Attributes:
- size: an integer representing the number of buckets
- buckets: a vector of lists, where each list stores Track objects with the same hash value

Algorithm: insertTrack(track, useTitleAsKey)
1. if useTitleAsKey:
      key = track.title
  else:
      key = track.artist
2. hashValue = calculateHash(key)
3. bucketIndex = hashValue % size
4. for trackInBucket in buckets[bucketIndex]:
      if trackInBucket.key == key:
          return False // track already exists
5. buckets[bucketIndex].append(track)
6. return True // track inserted

Algorithm: removeTrack(key, useTitleAsKey)
1. hashValue = calculateHash(key)
2. bucketIndex = hashValue % size
3. for trackInBucket in buckets[bucketIndex]:
      if (useTitleAsKey and trackInBucket.title == key) or
         (not useTitleAsKey and trackInBucket.artist == key):
          buckets[bucketIndex].remove(trackInBucket)
          return True // track removed
4. return False // matching track not found

Algorithm: searchTrack(key, useTitleAsKey)
1. hashValue = calculateHash(key)
2. bucketIndex = hashValue % size
3. matchingTracks = []
4. for trackInBucket in buckets[bucketIndex]:
      if (useTitleAsKey and trackInBucket.title == key) or
         (not useTitleAsKey and trackInBucket.artist == key):
          matchingTracks.append(trackInBucket)
5. return matchingTracks // vector of matching tracks




Analysis of data structure and algorithms:

A hash table offers an average-case time complexity of O(1) for insertion, deletion, and search operations. However, in the worst-case scenario, the time complexity can degrade to O(n), where n is the number of elements in the hash table. To minimise this, I implemented a custom hash function that distributes keys uniformly across the table.

The hash function multiplies the current hash value by a prime number (31) and adds the ASCII value of the current character. This approach ensures a uniform distribution of keys, reducing collisions and maintaining average-case time complexity.(1)*


Time complexity analysis:

insertTrack: In the average case, the time complexity is O(1), as the hash function distributes keys uniformly, and the bucket index is computed in constant time. However, in the worst-case scenario, the time complexity can degrade to O(n), where n is the number of elements in the hash table. This is because all keys could end up in the same bucket, and the function would have to iterate through all the elements to check for duplicates.

removeTrack: The average-case time complexity is O(1) because the hash function distributes keys uniformly, and the bucket index is computed in constant time. In the worst-case scenario, the time complexity can degrade to O(n) if all keys end up in the same bucket and the function needs to iterate through all the elements to find the matching track.

searchTrack: The average-case time complexity is O(1) due to the uniform distribution of keys provided by the hash function. In the worst-case scenario, the time complexity can degrade to O(n) if all keys end up in the same bucket and the function needs to iterate through all the elements to find the matching tracks.

hashFunction: The time complexity of this function is O(k), where k is the length of the key string. The function iterates through each character in the key and computes the hash value.
Collision resolution:

The given implementation uses separate chaining for collision resolution. In separate chaining, each bucket in the hash table is a list that stores all the elements with the same hash value. When a collision occurs, the new element is simply added to the end of the list in the corresponding bucket.
This approach allows for efficient handling of collisions and reduces the chances of performance degradation due to collisions.

Overall, the HashTable class implementation provides an efficient track search functionality with an average-case constant time complexity for insertion, deletion, and search operations. The use of a custom hash function and separate chaining for collision resolution ensures the uniform distribution of keys and minimises the likelihood of collisions.



Summary of work done:
I designed and implemented a track search functionality using a hash table data structure. The program is capable of adding tracks from a file, saving tracks to a file, searching tracks by artist/band name, and removing specific tracks. Users can specify the input file name during executable invocation or after. If no file is provided, one will be used as a default option. Users can also specify the path of the file along with the file name during the invocation of the executable.

The program's structure includes classes for managing track details (Track), hash table operations (HashTable), and file loading/saving (FileLoader). The main.cpp file contains the user interface and command loop for processing user inputs.

Testing:
I used the Catch2 testing framework to verify the functionality of the implemented classes and methods. The test cases cover the hash function, insertion, searching, and removal of tracks from the hash table. The 'setup_test_hash_table()' function sets up the hash table to be used in several test cases, creating two hash tables and populating them with tracks from "tracks.txt."

The test cases ensure the correctness and functionality of the implemented classes and methods, including the hash function, inserting, searching, and removing tracks from the hash table.

In order to start tests, use “make tests”, then use “tests.exe”
In order to use the program, use “make”, then use “music_library.exe” or “music_library.exe filename.txt” or “music_library.exe ./path filename.txt” (where “,/path” and “filename.txt” are the path and the file name respectively).



Limitations and critical reflection:
Some of the limitations are: 
Inability to search for all tracks that share same title (but may be from different artists or have different lengths)
“Remove” function currently doesn’t work with terminal input
It is difficult to select/search for tracks as there is function that lists the contents of the library
Input through terminal feels clunky and at times it behaves in unexpected ways (though this doesn’t break the program)

Adding the ability for users to select and export tracks from a list of "searched" tracks would improve the program's usability. Overall, I'm proud of the work I've done on this project, and I believe it achieves its primary goals while leaving room for further improvements.

References:

(1)* Cormen, T.H., Leiserson, C.E., Rivest, R.L. and Stein, C., 2009. Introduction to Algorithms, 3rd ed. Cambridge, MA: The MIT Press.

The mentioned concept can be found in the "Introduction to Algorithms" textbook by Cormen et al. (2009). This book is a widely known and comprehensive resource on algorithms and data structures, and it includes a detailed explanation of hash functions and their properties, including the use of prime numbers and the distribution of keys.

