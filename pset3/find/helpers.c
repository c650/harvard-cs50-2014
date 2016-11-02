/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
	if (n > 0 ) { //check if array has elements
		if ((values[0] == value) || (values[1] == value) || (values[n-1] == value)){

			return true;

		} else if (values[n/2] == value) { // checks if middle element == value

			return true; // returns true if so

		} else if (values[n/2] > value){ 

			// go to left side
			int arr[n/2 + 1];

			for (int i = 0, x = (n/2); i <= x; i++) {
				arr[i] = values[i];
			}

			search(value, arr, (n/2));

		} else if (values[n/2] < value){

			// go to right side
			int arr[n/2 + 1];

			for (int i = 0, x = (n/2); i <= x; i++) {
				arr[i] = values[i + x];
			}

			search(value, arr, (n/2));


		} else {
	
			// give up
			return false;

		}

	} 
	return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
	int changesMade;
    
    for (int i = 0; i < (n - 1); i++) {
    	
    	changesMade = 0;
    	
    	for (int j = 0; j < (n - 1); j++) {

    		int value1 = values[j]; // assign temp value holders
    		// checks if the left value is bigger than the right
    		if (value1 > values[j + 1]) {
    			// swaps values if condition passes
    			values[j] = values[j + 1];
    			values[j + 1] = value1;
    			changesMade++;
    		
    		}
    	}

    	if (changesMade == 0 ) {
    		break; //breaks the loop if no changes were made on last cycle
    	} else {
    		// continues the loop if changes were made
    		continue;
    	}
    }
    
    return; 
}