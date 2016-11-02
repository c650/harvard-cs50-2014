/************************************

	Charles Bailey

	greedy.c

	Returns number of coins when
	a certain amount of change is
	due.

************************************/
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h> 
float changeDue;
int main(void) {
	printf("Please input change due: ");
	while (true) {
		changeDue = GetFloat();
		if (changeDue < 0.0) {
			printf("%f is negative!", changeDue);
		} else if (changeDue == 0.0) {
			printf("No change is due.\n");
			exit(EXIT_SUCCESS); //exits loop
			// ^^ from: https://stackoverflow.com/questions/2425167/use-of-exit-function
		} else {
			break; //breaks loop
		}
		printf("Try again:"); //prompts again
	}
	int coins = 0;
	int newChange = (int)((changeDue + 0.001) * 100);
	// added fudge val .001 as per http://cboard.cprogramming.com/c-programming/107750-converting-float-integer-without-rounding.html
	while (newChange > 0) {
		if (newChange >= 25) {
			newChange -= 25;
			coins++;
		} else if (newChange >= 10 ) {
			newChange -= 10;
			coins++;
		} else if (newChange >= 5) {
			newChange -= 5;
			coins++;
		} else {
			newChange -= 1;
			coins++;
		}
	}
	printf("%i\n", coins);
}