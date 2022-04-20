Client-server echo application

Desctiption

There you can find 3 directories: POSIX, client and server. 

POSIX directory - is a wrapping library that handles all POSIX errors and displays them to the screen as a error. To catch error, use try-catch construction.

client and server directories are main directories of the project. After making project, you will get two executable files: client and server. Also, you can build them independently of each other, because after the cmake ... command, you get a make file for each directory.

Target of this project: learning architecture of client-server.

In future, we will added single message and interactive mode for the client. Also, multi-thread will be included, also.