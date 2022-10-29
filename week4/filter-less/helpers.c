#include "helpers.h"
#include <math.h>

int min(int a, int b);
int getAverage(int row, int column, int height, int width, int colorID, RGBTRIPLE copy[height][width]);

#define BLUE 0
#define GREEN 1
#define RED 2

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gray = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = min(255, round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue));
            int sepiaGreen = min(255, round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue));
            int sepiaBlue = min(255, round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue));

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Returns the lesser between two integers
int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < floor(width / 2); j++)
        {
            // Create a temporary variable to allow the swapping of two values between two positions in the array
            int tempBlue = image[i][j].rgbtBlue;
            int tempGreen = image[i][j].rgbtGreen;
            int tempRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = tempBlue;
            image[i][width - j - 1].rgbtGreen = tempGreen;
            image[i][width - j - 1].rgbtRed = tempRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of the image, pixel by pixel
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // For each pixel, apply the blur effect for every color, considering every valid neighboring pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = getAverage(i, j, height, width, BLUE, copy);
            image[i][j].rgbtGreen = getAverage(i, j, height, width, GREEN, copy);
            image[i][j].rgbtRed = getAverage(i, j, height, width, RED, copy);
        }
    }
    return;
}

// Gets the average value for blue, red, or green, of all valid pixels surrounding the current pixel
int getAverage(int row, int column, int height, int width, int colorID, RGBTRIPLE copy[height][width])
{
    int sum = 0;
    float count = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            // Check whether the neighboring pixel in question is valid or not
            if (j < 0 || j >= width || i < 0 || i >= height)
            {
                continue;
            }

            // Accumulate the values of the valid neighboring pixels, for the color being worked in at the moment
            if (colorID == 0)
            {
                sum += copy[i][j].rgbtBlue;
            }
            else if (colorID == 1)
            {
                sum += copy[i][j].rgbtGreen;
            }
            else
            {
                sum += copy[i][j].rgbtRed;
            }

            // Count how many values are being accumulated / how many valid pixels there are for the current pixel
            count++;
        }
    }

    // Get average
    return round(sum / count);
}