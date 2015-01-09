# Windows-Programming
Exercises on Windows programming mainly based on multithreading concurrency and Win APIs

The purpose and functionalities of the varoius laboratories are presented in the txt files that can be found in the root of the repository. Navigate through labs' folders in order to reach the Source.cpp files. 

The folders are projects exported from Visual Studio for Desktop 2013.

The various programs' specifications can be also found in the following parts of the README:

##LAB 11
System and Device Programming
Laboratory Week 11

Laboratory target:
- manipulate file systems
- write recursive programs
- run threads with proper synchronization when necessary



Note:
All students are requested to *up-load* on the class "portal" web page
the solution of Exercise 04.
Each student should submit a *single* C file with name
s<registerNumber>.cpp"
(e.g., s123456.cpp).
Comments and notes should just duplicate the name of the file.
Deadline: May, Monday the 26th, 12.00 p.m.



Exercise 01:

Copy a directory tree.

A C program is run with two parameters
name1 name2
where name1 and name2 are C strings which indicates relative
or absolute paths of two directory trees.

The program has to copy the entire name1 directory (with its files and
eventually sub-directories and other files) to the name2 location.
In other words, the program makes a *recursive* copy of a directory
named name1 into a directory named name2 with the same topological
structure.



Exercise 02:

Modify a directory tree while copying it.

A C program is run with two parameters
name1 name2
where name1 and name2 are C strings which indicates relative
or absolute paths of two directory trees.

The program has to copy the directory name1 into a isomorphic
directory name2 but for each file add two rows at the beginning of the
file specifying:
- the first one, the name of the file 
- the second one specifies the size of the original) files.



Exercise 03:

Visit N directory trees with N threads.

A C program is run with N parameters.
Each parameter indicates a relative or an absolute path to
a file system directory.

The program has to run N threads (one for each argument).
Each thread recursively visits one of the directory and
for each directory entry it prints-out its identifier and
the directory entry name.
The main thread awaits for the termination off all threads
and finally it ends the entire program.

Notice that to avoid merging the outputs of all threads
on standard output, each thread can store the entry list
in a file whose name is generated based on the thread identifier.



Exercise 04:

Compare directory.

A C program is run with N parameters.
Each parameter indicates a relative or an absolute path to
a file system directory tree.

The program has to compare the content of all directories
to decide whether all directories have the same content or not.

Two directories have the same content *if and only if* all directory
entries (files and sub-directories) have the same name (excluding the
path leading to the root, which differ, in general).
Notice that, it can be supposed that, in case of equivalent directory
tree, all visits proceed in the same order, i.e., they deliver the
same entry order.

Upon termination the program has to state whether all directory have
the same content or not.

Suggestions:
- The main program run one "reading" thread for each directory tree
  plus one "comparison" thread 
- Each "reading" thread visits one of the directory tree
- For each file/directory found "reading" threads synchronize
  themselves by activating the "comparison" thread
- The "comparison" thread compares the name of all entries found.
  It stops all other threads (and the program) in case the entries are
  not the same.
  Otherwise, it returns the control to the "reading" threads.



## LAB 12

System and Device Programming
Laboratory Week 12

Laboratory target:
- Synchronizing threads with semaphores and events
- Error handling
- Concurrent programming with processes and synchronization
  through file locking


Exercise 01 (versions A and B):

A concurrent program runs 4 threads.
Each thread iterates through a cycle waiting for 32-bit integer
values.
Moreover:
- the first thread computes the sum of those values and for each number
  received it prints-out (on standard output) the partial sum of the
  numbers received until that moment
- the second thread computes and prints-out (on standard output)
  the partial product of the numbers
- the third thread computes and prints-out (on standard output)
  the factorial of the numbers
- the fourth thread prints-out (on standard output) a number
  of '#' symbols equal to that number on the same standard
  output line.

The four threads have to synchronize with a main thread which is in
charge of reading the numbers from a file.
The file name is received on the command line and it stores the
sequence of integer 32-bit numbers.
When the sequence ends the main thread has to end the four "secondary"
threads and the main program.

Implement two versions of the program:

- Version A:
  Use semaphores (only) to solve all synchrnonization problems

- Version B:
  Using Windows-32 events (and semaphore if necessary).

Suggestions:
1) notice that the 4 secondary threads have to wait numbers coming
   from the main thread, but the main thread has to wait those
   threads to do their job before reading the next value from
   file
2) are 2 semaphores sufficient to synchronize them all?



Exercise 02:

A file contains a sequence of 32-bit values in binary form.
A program receives a file name on the command line and runs
two threads:

* The first thread:
  - Reads from the file pairs of value n1, n2
  - Computes their ratios n1/n2
  - Prints-out on standard output n1, n2 and n1/n2

  The thread has to handle exceptions for:
  - Trying to opening a non-existing file, file handle exception
  - Performing a division by zero, mathematical exception
  - Reading file with an odd number of value (the n2 value for
    the last pair does not exist)

* The second thread
  - Considers the first integer stored in the file as an array size
  - Dynamically allocates an array of that size
  - Stores all subsequent values within the file into the array
  - Orders the array and prints it out on standard output.

  The thread has to handle exceptions for:
  - Wrong (array) memory allocation
  - Wrong memory access, during array element read or write
  - Reading a number of values from file larger than the array
    size, i.e., array bound exception.

Notice that when one thread handles an exception it has to force
to a clean termination the other thread, i.e., memory has to be
free (no memory leaks), files have to be closed, etc.



Exercise 03:

Write a client-server application, based on two *processes*
which communicate using an intermediate file.

Process A (the client) works as follow:
- It opens a temporary file for writing (use GetTempFileName to
  obtain a unique file name).
- It starts Process B passing as argument the name of the file.
- It reads from standard input strings of characters, and it writes
  those strings on the intermediate file.
  Use string of constant length (e.g., #define MAX_LEN 100).
- It stops reading strings from standard input when it receives an
  end-of-file (ctrl-Z).
- Before terminating, it writes the string
  ".end"
  as last line of the file.

Process B (the server) works as follows:
- It opens the intermediate file for reading.
- It reads lines as soon as they are available, and it prints them
  out on standard output.

Suggestions:
- Process A keeps the file open, and it always locks the next part of
  the file before unlocking the previous part.
- Process B runs in parallel, and it reads a shared file, waiting to
  have access to single lines to read them.



##LAB 13

A “product” file contains information for each car model marketed in a country by the car industry.
For each car model the file includes a single line. Each line reports the manufacturer name, the car name, and a file name. This file stores the selling results of all seller branches for that car model. The following is a correct example a the “product” file:
FIAT 500 file500.txt BMW serie1 bmwS1.txt ...
Which indicates that for the FIAT 500 car the selling data are stored in file500.txt, etc. Notice that the file is stored in binary form and records have a fixed-length.
For each car, a “trade” file stores, on each single line, the branch identifier, the selling date, and the selling price. The following is a correct example of the file named file500.txt:
fiatIt001 04.01.2014 15131 fiatIt006 04.01.2014 17000 dddXYZ 21.01.2014 13245 ...
Which indicates that for the FIAT 500, branch fiatIt001 sold a car the 4th of January at 15132 euros, etc. As for the “product” file, also the “trade” file is stored in binary form with records of fixed-length.
Write a C program in the MS Visual Studio environment satisfying the following specification:
• The program receives on the command line three parameters: The name of the input “product” file, an integer
value N, and an output file name.
• The program runs N threads. Each thread is in charge of:
1. Reading a single line from the “product” file, and the entire corresponding “trade” file.
2. Evaluating some statistics on the sales figures, such as: The number of cars sold for that model, the total amount of money made for that model, the branch name which sold more models, and the date in which more
models have been sold.
3. Storing those data on the file whose name has been specified as third parameter on the command line. Each
set of statistics has to be stored together with the thread identifier, the car producer and car name.
4. Moving back to step 1, until all rows in the “product” file have been processed.
• When the entire “product” file has been processed and all N threads stopped running, the main thread has to collect their termination, closing the “product” file, and terminate itself.
Notice that all threads read lines from the same “product” file and write their results on the same output file. Outputs from different threads do not have to be interleaved (i.e., having one line from one thread and another from another thread is not allowed).

##LAB 14

System and Device Programming
Laboratory Week 14

Laboratory target:
- Building and using DLLs


Exercise 01 (simple DLL: implicit and explicit linking):

Write a simple program that exploits a sorting function (e.g. bubble, 
insertion or selection sort), implemented in a DLL module.

A DLL contains compiled functions that can be reused by many different applications.
The rules to write and build a DLL module can be found in
http://msdn.microsoft.com/en-us/library/aa294382(v=vs.60).aspx

The __declspec(dllexport) / __declspec(dllimport) technique is suggested.

The DLL file (sortDll.c) should include a function

void sort (int *v, int n);

that implements a sort algorithm. The file has to be properly compiled.
To generate the DLL outputs, select the Build option from the Build menu on the menu bar. 
Note that there two significant output files created when building a DLL: a .dll file and a .lib file. 
The .dll file contains the compiled functions that were implemented in the project source code. 
The .lib file is known as an import library. 
An import library is used when linking a program that makes calls to the functions exported by a DLL. 
The import library provides information to the linker about the function signatures and function 
locations within the compiled DLL code.
In order for a client application to use the DLL that has been created, the following files must be 
supplied to the client application programmer:
	•	header files that declare the functions exported by the DLL

	•	the .dll file generated by the linker

	•	the .lib file generated by the linker

The header files, the .lib file, and the .dll file should be copied into the project directory of the client application. 
Add the .h and .lib files to the client application project.
In order to use the functions defined in the DLL, the source code in the client application should include the 
header file(s) copied from the DLL project. After the client project is compiled and linked, it can be executed. 
When the application is loaded by the operating system, the operating system will automatically load the necessary DLL file.
Try implicit linking and explicit linking.
For explicit linking, use functions:
HINSTANCE LoadLibrary (LPCTSTR lpLibFileName);
FARPROC GetProcAddress (HMODULE hModule,LPCSTR lpProcName);


Exercise 02 (file buffer):

Build a DLL that works as a buffer to a file:
the DLL should provide functions

LPVOID CreateBuffer(HANDLE f, DWORD bufSize); // called for an open file
BOOL ReadFileBuffered(HANDLE f, LPVOID buf, 
              DWORD nBytesToRead, LPDWORD nBytesRead);
FreeBuffer(LPVOID buf);

The buffer is allocated of a given size. File reads are done in the buffer, for a size “bufSize” 
(unless end-of-file is reached).
ReadFileBuffered reads data from buffer, if available. If not the empty buffer is filled by another read from file.

The DLL has to be tested by an application file that:

- opens the file
- creates a buffer
- reads the entire file (supposed to be a text file) by single characters (counting lines).
- frees the buffer and closes the file.




