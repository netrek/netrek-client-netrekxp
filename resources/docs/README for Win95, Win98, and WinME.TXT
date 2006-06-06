As Windows 95/98/ME user, please use the executable labeled netrek9598ME, rather than
netrek.  The reason for two executables is due to a font resource problem, that requires
the client to be compiled different to work on older vs newer operating systems.
I acknowledge this is not the best solution, I will try to have it fixed in a future
version.

The following options will NOT work with Windows 95/98/ME, due to the use of a native
Windows transform function that was not added until Windows 2000:

newClock: on
colorClient: 4

So in conclusion, don't have any of those above options in your netrekrc, and the client 
should work fine!