//A fudge of the Unix passwd struct, so we can use getpwuid(), etc.
struct passwd
	{
	char *pw_name;
	};

//Declarations for the functions that must have them
struct passwd *getpwuid();
