#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//define new type to store a byte of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open user image file
    FILE *card = fopen(argv[1], "r");

    //if file cannot open return error
    if (&fopen == NULL)
    {
        printf("File cannot be opened for reading.");
        return 1;
    }

    //define array of bytes to use as buffer when reading file
    BYTE buffer[512];

    //define string for filename
    char filename[8];

    //create new file to store jpeg images recovered
    FILE *img = NULL;

    //define integer img_count to count how many inages recovered
    //used to name recovered jpeg files
    int img_count = 0;

    //define bool flage to false
    //used to determine the start and end of jpeg files
    bool flag = false;

    //continue to loop through each 512 byte block of data until reaches the end
    while (fread(buffer, 512, 1, card) == 1)
    {
        //checks the first four bytes of the block to see if it is the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //closes previous jpeg file if needed
            if (flag == true)
            {
                fclose(img);
            }

            //else sets flag to true when first image found
            else
            {
                flag = true;
            }

            //creates name of new image file
            sprintf(filename, "%03i.jpg", img_count);

            //assigns new open file
            img = fopen(filename, "w");

            //adds to image count
            img_count++;
        }

        //if 512 byte block is not the start of the jpeg, add to open img file
        if (flag == true)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

    //close both files when you reach the end of the original file
    fclose(card);
    fclose(img);

    return 0;
}
