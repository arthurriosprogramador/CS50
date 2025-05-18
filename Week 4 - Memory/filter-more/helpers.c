#include "helpers.h"
#include <math.h>

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    const int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

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
            int gxRed = 0;
            int gxGreen = 0;
            int gxBlue = 0;
            int gyRed = 0;
            int gyGreen = 0;
            int gyBlue = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        int kernelRow = di + 1;
                        int kernelCol = dj + 1;

                        RGBTRIPLE pixel = imgTemp[ni][nj];

                        gxRed += pixel.rgbtRed * GX[kernelRow][kernelCol];
                        gxGreen += pixel.rgbtGreen * GX[kernelRow][kernelCol];
                        gxBlue += pixel.rgbtBlue * GX[kernelRow][kernelCol];

                        gyRed += pixel.rgbtRed * GY[kernelRow][kernelCol];
                        gyGreen += pixel.rgbtGreen * GY[kernelRow][kernelCol];
                        gyBlue += pixel.rgbtBlue * GY[kernelRow][kernelCol];
                    }
                }
            }

            int roundedRed = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int roundedGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int roundedBlue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            RGBTRIPLE *imgPixel = &image[i][j];

            imgPixel->rgbtRed = (roundedRed > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : roundedRed;
            imgPixel->rgbtGreen = (roundedGreen > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : roundedGreen;
            imgPixel->rgbtBlue = (roundedBlue > MAX_BYTE_VALUE) ? MAX_BYTE_VALUE : roundedBlue;
        }
    }
}
