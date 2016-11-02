/************************************

	Charles Bailey

	mario.c

	Returns a pyramid of n #'s high. 

************************************/
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

int height; /*
    made global var as per http://www.tutorialspoint.com/cprogramming/c_scope_rules.htm
*/
int main(void) {
	bool correct = false;
	printf("Please tell me how tall you want your pyramid:");
	do {
		height = GetInt();
		if ((height < 0)||(height > 23)) {
			printf("%i is an incorrect input!\n Try again: ", height);
			continue;
		} else if (height == 0){
			exit(0);
 		} else {
			correct = true; //being super redundant here!
			break;
		}
	} while (correct == false); // ; here?
	for (int i = 2; i <= (height + 1); i++) {
		for (int x = 1; x < (height - i + 2); x++) {
			printf(" ");
		}
		for (int x = 1; x < (i + 1); x++) {
			printf("#");
		}
		printf("\n");
	}
}