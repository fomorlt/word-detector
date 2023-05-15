# Assignment 7 - The Great Firewall of Santa Cruz
This program uses a hash table and a bloom filter to act as a filter to censor words at the user's discretion. 
It adds a list of forbidden words and translated words to both the hash table (which is made of Node ADTs) and
the bloom filter (which is made of bit vectors). If an inputted word is found in the bloom filter, then it may
be part of the list of banned words. If that banned word does not have a translation, then it is considered 
'badspeak' and added to the binary search tree holding a list of those forbidden words. If the word does have
a translation, then it is considered a minor transgression, and is presented to the user as such (while also 
held in a binary search tree). The user may also customize the size of the hash table and bloom filter independently.

### Building
Type "make clean" to clean all residual binary files, then "make format" to ensure clang formatting of
code, then "make all" to create the executable "banhammer". Alternatively, you can type "make cfa"
which stands for "clean format all" to execute all three at once.

### Running
After compiling, type "./banhammer" without quotes, followed by the various different parameters you would like. 
-h shows help on how to use the program, 
-s tells the program to only print the statistics of the program: average branches traversed, hash table load, and bloom filter load.
[-t size] allows you to specify the number of entries in the hash table to 'size' (default is 2^16, or 65536)
[-f size] allows you to specify the number of entries in the bloom filter to 'size' (default is 2^20, or 1048576)

### Known Bugs
If the user inputs the size as a negative number, the program will segmentation fault.
If the user inputs numbers larger than unsigned integer max into the seed or size array, the program will segment fault.
