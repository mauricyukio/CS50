#include "helpers.h"
#include <math.h>

#define BLUE 0
#define GREEN 1
#define RED 2

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

// Returns the average value for blue, red, or green, of all valid pixels surrounding the current pixel
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

// Returns the weighted value for blue, red, or green, of all pixels surrounding the current pixel
double getValue(int row, int column, int height, int width, int colorID, RGBTRIPLE copy[height][width],
                int GxM[3][3], int GyM[3][3])
{
    double Gx = 0;
    double Gy = 0;

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
                Gx += copy[i][j].rgbtBlue * GxM[i - row + 1][j - column + 1];
                Gy += copy[i][j].rgbtBlue * GyM[i - row + 1][j - column + 1];
            }
            else if (colorID == 1)
            {
                Gx += copy[i][j].rgbtGreen * GxM[i - row + 1][j - column + 1];
                Gy += copy[i][j].rgbtGreen * GyM[i - row + 1][j - column + 1];
            }
            else
            {
                Gx += copy[i][j].rgbtRed * GxM[i - row + 1][j - column + 1];
                Gy += copy[i][j].rgbtRed * GyM[i - row + 1][j - column + 1];
            }
        }
    }

    double result = sqrt(pow(Gx, 2.0) + pow(Gy, 2.0));
    return result;

}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize the two matrixes Gx and Gy
    int GxM[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GyM[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

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
            image[i][j].rgbtBlue = min(255, round(getValue(i, j, height, width, BLUE, copy, GxM, GyM)));
            image[i][j].rgbtGreen = min(255, round(getValue(i, j, height, width, GREEN, copy, GxM, GyM)));
            image[i][j].rgbtRed = min(255, round(getValue(i, j, height, width, RED, copy, GxM, GyM)));
        }
    }
    return;
}
