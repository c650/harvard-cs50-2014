/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(void)
{
	FILE* cardptr = fopen("./card.raw","r");
	// open up file ^, check if null
	if (cardptr == NULL) {
		printf("Something went wrong!\n");
		return 1;
	}
	// init counter for file name
    int counter = 0;
    char name[10];

    BYTE* piece = malloc(sizeof(BYTE)*512);
    if (piece == NULL) {
    	printf("Couldn't allocate memory for piece.\n");
    	return 2;
    }
    //read the first block from the file
    while (fread(piece, sizeof(BYTE), 512, cardptr) && piece != NULL) {
    	// check for jpeg header
    	if (piece[0] == 0xff && piece[1] == 0xd8 && piece[2] == 0xff && (piece[3] >= 0xe0 && piece[3] <= 0xef)) {
    		// update counter and name
    		sprintf(name, "./%03d.jpg", counter); 
    		counter++; //inc counter

    		// open a new file with the name ###.jpg
    		FILE* jpeg = fopen(name, "w"); // dont forget w instead of r, silly goose!
    		if (jpeg == NULL) {
    			printf("Failed to open file %s\n", name);
    			return 2;
    		}
     		// write the block to jpeg
     		fwrite(piece, sizeof(BYTE), 512, jpeg); // write the jpg header to it

    		// read piece again
     		fread(piece, sizeof(BYTE), 512, cardptr);
			// while the jpg is still there, keep reading and writing 1 block at a time
			// this works because each jpg starts on a new block
			while (piece != NULL) {

				// write to file
				fwrite(piece, sizeof(BYTE), 512, jpeg);
				// check for end of file, read simult
				if (fread(piece, sizeof(BYTE), 512, cardptr) < 512) {
					fclose(jpeg); // close and free everything
					free(piece); // ^
					fclose(cardptr); // ^^
					return 0;
				}
				// check if next block is header
				if (piece[0] == 0xff && piece[1] == 0xd8 && piece[2] == 0xff && (piece[3] >= 0xe0 && piece[3] <= 0xef) ) {
					fseek(cardptr, -512, SEEK_CUR); // roll back one, while loop reverses
					break; // break out of inner loop
				}
			}
			fclose(jpeg); // close the file
    	}
    }
    // memo frees
    free(piece);

    fclose(cardptr);
}
/**
`	SOURCES:


 */
