/************************************

	Charles Bailey

	initials.c

	Returns initials.

************************************/
#include <cs50.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
// don't forget semicolons.
int main(void) {
	string name = GetString();

	if (name == NULL) {
		return 1;
	} // checks for invalid input
/*****************************************
	lines 13-22 from: 
	https://stackoverflow.com/questions/
	15472299/split-string-into-tokens-and
	-save-in-an-array
	 
	and by MOHAMED
*****************************************/
	int i = 0;
	char *arr[5]; // guessing no more than 5 parts to name, not scalable

	arr[i] = strtok(name," "); // get first name

	while(arr[i] != NULL) {
		i++; // wasn't working when this was put in for i in l15
		arr[i] = strtok(NULL," "); // continue splitting string and appending to
		// the array that holds each part of name
	}

	for (int x = 0; x < i; x++) {
		printf("%c",toupper(arr[x][0]));
		// prints the first char in each name
	}
	printf("\n");
}