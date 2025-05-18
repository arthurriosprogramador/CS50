#include <math.h>

#include "helpers.h"

const BYTE MAX_BYTE_VALUE = 255;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *imgPixel = &image[i][j];
            BYTE averageRgb =
                round((imgPixel->rgbtBlue + imgPixel->rgbtGreen + imgPixel->rgbtRed) / 3.0);
            imgPixel->rgbtBlue = averageRgb;
            imgPixel->rgbtGreen = averageRgb;
            imgPixel->rgbtRed = averageRgb;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *imgPixel = &image[i][j];
            BYTE originalRed = imgPixel->rgbtRed;
            BYTE originalGreen = imgPixel->rgbtGreen;
            BYTE originalBlue = imgPixel->rgbtBlue;
            int sepiaRed = round(originalRed * .393 + originalGreen * .769 + originalBlue * .189);
            int sepiaGreen = round(originalRed * .349 + originalGreen * .686 + originalBlue * .168);
            int sepiaBlue = round(originalRed * .272 + originalGreen * .534 + originalBlue * .131);
            imgPixel->rgbtRed = (sepiaRed > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : sepiaRed;
            imgPixel->rgbtGreen = (sepiaGreen > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : sepiaGreen;
            imgPixel->rgbtBlue = (sepiaBlue > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE *imgPixel = &image[i][j];
            RGBTRIPLE *reflectedPixel = &image[i][width - 1 - j];
            RGBTRIPLE temp = *reflectedPixel;

            *reflectedPixel = *imgPixel;
            *imgPixel = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imgTemp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imgTemp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float totalRed = 0.0;
            float totalGreen = 0.0;
            float totalBlue = 0.0;
            int count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        totalRed += imgTemp[ni][nj].rgbtRed;
                        totalGreen += imgTemp[ni][nj].rgbtGreen;
                        totalBlue += imgTemp[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            RGBTRIPLE *imgPixel = &image[i][j];

            imgPixel->rgbtRed = round((float) totalRed / count);
            imgPixel->rgbtGreen = round((float) totalGreen / count);
            imgPixel->rgbtBlue = round((float) totalBlue / count);
        }
    }
}
