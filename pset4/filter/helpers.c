#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop through each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculate the average of the three color values
            int gryscl = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.);

            //reassign each color value to the new grayscale value
            image[i][j].rgbtBlue = gryscl;
            image[i][j].rgbtGreen = gryscl;
            image[i][j].rgbtRed = gryscl;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //loop through the first half of pixels in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2 ; j++)
        {
            //create a temporary array of image pixels
            RGBTRIPLE temp;

            //swap the values of one column with its "rainbow" opposite
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a temporary array of image pixels to store the newly calculated values
    RGBTRIPLE temp[height][width];

    //loop through each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            //initialize the values of the color sums and the counter
            //counter is used to keep track of how many pixels are being averaged
            float sum_Blue = 0;
            float sum_Red = 0;
            float sum_Green = 0;
            float counter = 0;

            //loop through the surrounding 3 x 3 array for each pixel
            for (int a = -1; a < 2; a++)
            {

                for (int b = -1; b < 2; b++)
                {
                    //identifies pixels beyond the image border and skips over them
                    if (i + a < 0 || i + a > height - 1)
                    {
                        continue;
                    }

                    if (j + b < 0 || j + b > width - 1)
                    {
                        continue;
                    }

                    //adds the pixel color values to their respective sums
                    sum_Blue += image[i + a][j + b].rgbtBlue;
                    sum_Red += image[i + a][j + b].rgbtRed;
                    sum_Green += image[i + a][j + b].rgbtGreen;
                    //increments counter by 1
                    counter++;
                }
            }

            //sets the rounded color sum average to the corresponding temporary pixel array value
            temp[i][j].rgbtBlue = round(sum_Blue / counter);
            temp[i][j].rgbtRed = round(sum_Red / counter);
            temp[i][j].rgbtGreen = round(sum_Green / counter);
        }
    }

    //loop through each pixel in the image
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            //sets the original image values to the newly calculated values
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    //loop through each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //initializes varaibles used for kernel
            int rGx, gGx, bGx, rGy, gGy, bGy;

            //sets values to 0
            rGx = gGx = bGx = rGy = gGy = bGy = 0;

            //calculate value for upper center when within image boundary
            if (i > 0)
            {
                rGy += -2 * image[i - 1][j].rgbtRed;
                gGy += -2 * image[i - 1][j].rgbtGreen;
                bGy += -2 * image[i - 1][j].rgbtBlue;
            }

            //calculate value for lower center when within image boundary
            if (i < height - 1)
            {
                rGy += 2 * image[i + 1][j].rgbtRed;
                gGy += 2 * image[i + 1][j].rgbtGreen;
                bGy += 2 * image[i + 1][j].rgbtBlue;
            }

            //calculate value for left center when withing image boundary
            if (j > 0)
            {
                rGx += -2 * image[i][j - 1].rgbtRed;
                gGx += -2 * image[i][j - 1].rgbtGreen;
                bGx += -2 * image[i][j - 1].rgbtBlue;
            }

            //calculate value for right center when within image boundary
            if (j < width - 1)
            {
                rGx += 2 * image[i][j + 1].rgbtRed;
                gGx += 2 * image[i][j + 1].rgbtGreen;
                bGx += 2 * image[i][j + 1].rgbtBlue;
            }

            //calculate value for upper left corner within image boundary
            if (i > 0 && j > 0)
            {
                rGx += -1 * image[i - 1][j - 1].rgbtRed;
                gGx += -1 * image[i - 1][j - 1].rgbtGreen;
                bGx += -1 * image[i - 1][j - 1].rgbtBlue;
                rGy += -1 * image[i - 1][j - 1].rgbtRed;
                gGy += -1 * image[i - 1][j - 1].rgbtGreen;
                bGy += -1 * image[i - 1][j - 1].rgbtBlue;
            }

            //clalculate value for upper right corner when within image boundary
            if (i > 0 && j < width - 1)
            {
                rGx += 1 * image[i - 1][j + 1].rgbtRed;
                gGx += 1 * image[i - 1][j + 1].rgbtGreen;
                bGx += 1 * image[i - 1][j + 1].rgbtBlue;
                rGy += -1 * image[i - 1][j + 1].rgbtRed;
                gGy += -1 * image[i - 1][j + 1].rgbtGreen;
                bGy += -1 * image[i - 1][j + 1].rgbtBlue;
            }

            //clalculate value for lower right corner when within image boundary
            if (i < height - 1  && j < width - 1)
            {
                rGx += 1 * image[i + 1][j + 1].rgbtRed;
                gGx += 1 * image[i + 1][j + 1].rgbtGreen;
                bGx += 1 * image[i + 1][j + 1].rgbtBlue;
                rGy += 1 * image[i + 1][j + 1].rgbtRed;
                gGy += 1 * image[i + 1][j + 1].rgbtGreen;
                bGy += 1 * image[i + 1][j + 1].rgbtBlue;
            }

            //clalculate value for lower left corner when within image boundary
            if (i < height - 1  && j > 0)
            {
                rGx += -1 * image[i + 1][j - 1].rgbtRed;
                gGx += -1 * image[i + 1][j - 1].rgbtGreen;
                bGx += -1 * image[i + 1][j - 1].rgbtBlue;
                rGy += 1 * image[i + 1][j - 1].rgbtRed;
                gGy += 1 * image[i + 1][j - 1].rgbtGreen;
                bGy += 1 * image[i + 1][j - 1].rgbtBlue;
            }

            //uses Sobel operartor to detect edges
            //if more than 255, set temporary value to 255
            //if less than or equal to 255 set temporary value to value calculated
            if ((sqrt((rGx * rGx) + (rGy * rGy))) > 255)
            {
                temp[i][j].rgbtRed = 255.;
            }

            else
            {
                temp[i][j].rgbtRed = round((sqrt((rGx * rGx) + (rGy * rGy))));
            }

            if ((sqrt((gGx * gGx) + (gGy * gGy))) > 255)
            {
                temp[i][j].rgbtGreen = 255.;
            }

            else
            {
                temp[i][j].rgbtGreen = round((sqrt((gGx * gGx) + (gGy * gGy))));
            }

            if ((sqrt((bGx * bGx) + (bGy * bGy))) > 255)
            {
                temp[i][j].rgbtBlue = 255.;
            }

            else
            {
                temp[i][j].rgbtBlue = round((sqrt((bGx * bGx) + (bGy * bGy))));
            }

        }
    }

    //loop through each pixel in the image
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            //sets the original image values to the newly calculated values
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }

    return;
}
