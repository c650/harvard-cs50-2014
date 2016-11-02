/************************************

	Charles Bailey

	caesar.c

	Encrypts according to caesar. 

************************************/
#include <cs50.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
// don't forget semicolons.
string encrypt(int key, string phrase); // proto for encrypt func

int main(int argc, string argv[]) {
	if (argc != 2) { // checks for right number of args
		printf("You didn't pass a CL arg.");
		return 1; // returns error code
	} else {
		string input = GetString(); // get user input if args supplied
		if (input != NULL) { // only works if input is received
			int key = strtol(argv[1], NULL, 10); // redundant?
			while (key > 26) { // brings key down to range
				key -= 26;
			}
			printf("%s\n",encrypt(key,input)); // print out new string and call to func
			return 0; // return 0 explicitly
		} else {
			return 1; // returns 1 if input is null
		}
	}
}

string encrypt(int key, string phrase) {
	for (int i = 0, n = strlen(phrase); i < n; i++) { // goes thru each element in phrase
		if (phrase[i] >= 'a' && phrase[i] <= 'z') { // if phrase is lowercase char
			phrase[i] = ((phrase[i] - 'a' + key) % 26) + 'a'; // add key to it and wrap it in range
			// this guarantees wrap-around, explicitly ensure phrase[i] is a char
		} else if (phrase[i] >= 'A' && phrase[i] <= 'Z') { // same as if, but uppercase
			phrase[i] = ((phrase[i] - 'A' + key) % 26) + 'A';
		}
	}
	return phrase; // returns the phrase
}
/************************************

	SOURCES:

	http://www.asciitable.com/

	and for strtol:
		https://stackoverflow.com/
		questions/7021725/conve
		rting-string-to-integer-c
	  as well as:
		https://stackoverflow.com/
		questions/7021725/converti
		ng-string-to-integer-c
	  and:
		https://reference.cs50.net/
		stdlib.h/strtol

************************************/