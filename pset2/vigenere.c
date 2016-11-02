/*************************************

	Charles Bailey

	vigenere.c

	It encrypts using a word as the
	key, acting like weaved caesar
	ciphers.

*************************************/
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[]) {
	if (argc != 2) {
		printf("You didn't pass a CL arg.\n");
		return 1;
		// check for wrong # of args
	}

	string key = argv[1]; // make variable for key to make more readable

	// check for non-letter chars
	for (int i = 0, n = strlen(argv[1]); i < n; i++) {
		// checks if key is valid
		if (!(key[i] >= 'a' && key[i] <= 'z') && !(key[i] >= 'A' && key[i] <= 'Z')) {
			printf("INVALID KEY INPUT! Exiting...\n");
			return 1; // return error code
		}
	}

	// make key all lower case
	for (int i = 0, n = strlen(key); i < n; i++) {
		key[i] = tolower(key[i]);
	}
	// get the input
	string input = GetString();
	// make array to store
	char fullKey[strlen(input)]; // unecessary, yet logical step. makes more readable
	int keySize = strlen(key); // makes keysize for later patching (see ln 49)
	int spaceCounter = 0; // makes counter to keep track of skips

	for (int i = 0, n = strlen(input); i < n; i++) {

		if (input[i] >= 'a' && input[i] <= 'z') {
			// will add appropriate key char to a full key
			fullKey[i] = key[(i - spaceCounter) % keySize] - 'a';
			// ^^ sets fK[i] to a zero-based index in alphabet
			// mods input @ i to new, shifted char
			input[i] = ((input[i] + fullKey[i] - 'a') % 26) + 'a';

		} else if (input[i] >= 'A' && input[i] <= 'Z') {
			// see if statement above, ln48-52
			fullKey[i] = toupper(key[(i - spaceCounter) % keySize]) - 'A';
			input[i] = ((input[i] + fullKey[i] - 'A') % 26) + 'A';

		} else {
			// if input[i] isn't letter, put space in key array, 
			// add one to space counter so that the key keeps
			// at its place so as to not skip over any char in key
			fullKey[i] = ' ';
			spaceCounter += 1;
		}
	}
	printf("%s\n", input);
}

/*************************************

	SOURCES: 


	https://stackoverflow.com/questio
	ns/2661766/c-convert-a-mixed-case
	-string-to-all-lower-case	

*************************************/