/**************************************************************
*
*	Charles Bailey
*
*	resize.c
*
*	It resizes a BMP
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy size infile outfile\n");
        return 1;
    } 

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    int scaleFactor = strtol(argv[1], NULL, 10);

    if (scaleFactor > 100 || scaleFactor < 1) {
        printf("%i\n is out of scaling range", scaleFactor);
        return 5;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    printf("File is being checked for BMP\n");
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    printf("File size is being updated...\n");
    
    // change this value to reflect increasing size
    bf.bfSize = (bf.bfSize - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER)) * scaleFactor 
    			+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    printf("%i is the new size of the file.\n", bf.bfSize);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    printf("File header has been written.\n");

    // save original dimensions first
    int* biWidth = malloc(sizeof(bi.biWidth));
    int* biHeight = malloc(sizeof(bi.biHeight));
    *biWidth = bi.biWidth;
    *biHeight = bi.biHeight;

    printf("Old biSizeImage: %i\n", bi.biSizeImage);
    // update width & height
    bi.biWidth *= scaleFactor;
    bi.biHeight *= scaleFactor; 
    bi.biSizeImage *= scaleFactor; 

    printf("Old Width: %i, New: %i, Old Height: %i, New: %i\n", *biWidth, bi.biWidth, *biHeight, bi.biHeight);
    printf("biSizeImage: %i\n", bi.biSizeImage);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    printf("INFO header has been written successfully\n");
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("Padding determined to be: %i\n", padding);

    // iterate over infile's scanlines
    // this repeats a height 'scaleFactor' times
	    
	for (int i = 0; i < abs(*biHeight); i++) {
        printf("Height # %i\n", i);
	    for (int n = 0; n < scaleFactor; n++) {
    
            // iterate over pixels in scanline
    		for (int j = 0; j < (*biWidth); j++) {
    		    
                for (int m = 0; m < scaleFactor; m++) {    

                    // temporary storage
        		    RGBTRIPLE triple;

                    // read RGB triple from infile
        	        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

        		    // write RGB triple to outfile
        		    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                    // go back to the beginning of the row, unless on last iteration
                    if (scaleFactor - m > 1) {
                        fseek(inptr,(*biWidth) * -1, SEEK_CUR);
                    }
    		    }
    	    }    
    	    
            // skip over padding, if any
    	    // fseek(inptr, padding, SEEK_CUR);

    	    // // then add  it back (to demonstrate how)
    	    // for (int k = 0; k < padding; k++)
    	    // {
    	    //     fputc(0x00, outptr);
    	    // }

            // go back to the beginning of the row, unless on last iteration
            if(scaleFactor - 1 == n) {
                fseek(inptr, abs(*biHeight) * -1, SEEK_CUR);
            }
            printf("Scale Factor: %i\n", n);     
	    }
    }

	free(biWidth);
	free(biHeight);
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}







/**************************************************************
*
*	SOURCES: 
*
*
*
**************************************************************/