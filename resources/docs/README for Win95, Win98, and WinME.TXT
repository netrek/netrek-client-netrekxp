As Windows 95/98/ME user, please use the executable labeled netrek9598ME, rather than
netrek.  The reason for two executables is due to a font resource problem, that requires
the client to be compiled different to work on older vs newer operating systems.
I acknowledge this is not the best solution, I will try to have it fixed in a future
version.

The following options will NOT work with Windows 95/98/ME.  In fact, they are very
likely to crash the client, so do not use them.

newClock: on
colorClient: 4
planetBitmap: 3
planetBitmapGalaxy: 3

If you use your old netrekrc, please add newClock: off, as the default for the new
clock is "on".

So in conclusion, don't have any of those above options in your netrekrc, and the client 
should work fine!  Also, I would recommend not switching between ship or planet bitmaps
in game via options menu, as it may also break your client.  Just choose a bitmap set
you like and stick with it.
