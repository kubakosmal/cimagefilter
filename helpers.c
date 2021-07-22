#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double gv = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {


            gv = image[i][j].rgbtBlue + image[i][j].rgbtGreen  + image[i][j].rgbtRed;
            gv = round(gv / 3);
            gv = (int) gv;
            image[i][j].rgbtBlue = gv;
            image[i][j].rgbtGreen = gv;
            image[i][j].rgbtRed = gv;
        }
    }

    return;

}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    double rv = 0;
    double gv = 0;
    double bv = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // red value
            rv = (0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue);
            rv = round(rv);
            if (rv > 255)
            {
                rv = 255;
            }

            // green value
            gv = (0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue);
            gv = round(gv);
            if (gv > 255)
            {
                gv = 255;
            }

            // blue value
            bv = (0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue);
            bv = round(bv);
            if (bv > 255)
            {
                bv = 255;
            }

            // updating values
            image[i][j].rgbtRed = rv;
            image[i][j].rgbtGreen = gv;
            image[i][j].rgbtBlue = bv;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        int dec = 1;
        if (width % 2 == 0)
        {
            for (int j = 0; j < (width / 2); j++)
            {
                temp = image[i][j];
                image[i][j] = image[i][width - dec];
                image[i][width - dec] = temp;

                dec++;
            }
        }
        else
        {
            for (int j = 0; j < ((width - 1) / 2); j++)
            {
                temp = image[i][j];
                image[i][j] = image[i][width - dec];
                image[i][width - dec] = temp;

                dec++;
            }
        }

    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy of original image values
    RGBTRIPLE imageO[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            imageO[h][w].rgbtRed = (double) image[h][w].rgbtRed;
            imageO[h][w].rgbtGreen = (double) image[h][w].rgbtGreen;
            imageO[h][w].rgbtBlue = (double) image[h][w].rgbtBlue;
        }
    }


    // x axis loop
    for (int i = 0; i < height; i++)
    {
        // y axis loop
        for (int j = 0; j < width; j++)
        {
            // for every adjacent pixel
            {
                // reset average values
                double averageR = 0;
                double averageG = 0;
                double averageB = 0;

                // count neighbours
                int neighbours = 0;

                // start from upper left, iterate over 3x3 matrix
                // if pos out of matrix - continue
                for (int x = (i - 1); x < (i + 2); x++)
                {
                    for (int y = (j - 1); y < (j + 2); y++)
                    {
                        // if pos < 0 or pos > height - continue
                        if (x < 0 || x > (height - 1))
                        {
                            ;
                        }
                        // if pos < 0 or pos > width - continue
                        else if (y < 0 || y > (width - 1))
                        {
                            ;
                        }
                        // if pos > 0 and pos < height, width - add red, green, blue value
                        else
                        {
                            averageR += imageO[x][y].rgbtRed;
                            averageG += imageO[x][y].rgbtGreen;
                            averageB += imageO[x][y].rgbtBlue;

                            neighbours++;
                        }
                    }
                }
                // divide average values by number of neighbours
                averageR = round(averageR / neighbours);
                averageG = round(averageG / neighbours);
                averageB = round(averageB / neighbours);


                // update pixels
                image[i][j].rgbtRed = averageR;
                image[i][j].rgbtGreen = averageG;
                image[i][j].rgbtBlue = averageB;

            }
        }
    }





}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    
    RGBTRIPLE imageO[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            imageO[h][w].rgbtRed = (double) image[h][w].rgbtRed;
            imageO[h][w].rgbtGreen = (double) image[h][w].rgbtGreen;
            imageO[h][w].rgbtBlue = (double) image[h][w].rgbtBlue;
        }
    }
    
    // gx matrix
    int gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    
    // gy matrix
    int gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    
    // x axis
    for (int h = 0; h < height; h++)
    {
        // y axis loop
        for (int w = 0; w < width; w++)
        {
            int gxred = 0;
            int gxgreen = 0;
            int gxblue = 0;
            
            int gyred = 0;
            int gygreen = 0;
            int gyblue = 0;
            
            
            // calculate gx values
            for (int x = (h - 1), gxx = 0; x < (h + 2); x++, gxx++)
            {
                for (int y = (w - 1), gxy = 0; y < (w + 2); y++, gxy++)
                {
                    // if pos < 0 or pos > height - continue
                    if (x < 0 || x > (height - 1))
                    {
                        ;
                    }
                    // if pos < 0 or pos > width - continue
                    else if (y < 0 || y > (width - 1))
                    {
                        ;
                    }
                    else
                    {
                        gxred += imageO[x][y].rgbtRed * gx[gxx][gxy];
                        gxgreen += imageO[x][y].rgbtGreen * gx[gxx][gxy];
                        gxblue += imageO[x][y].rgbtBlue * gx[gxx][gxy];
                    }
                }
            }
            
            // calculate gy values
            for (int x = (h - 1), gyx = 0; x < (h + 2); x++, gyx++)
            {
                for (int y = (w - 1), gyy = 0; y < (w + 2); y++, gyy++)
                {
                    // if pos < 0 or pos > height - continue
                    if (x < 0 || x > (height - 1))
                    {
                        ;
                    }
                    // if pos < 0 or pos > width - continue
                    else if (y < 0 || y > (width - 1))
                    {
                        ;
                    }
                    else
                    {
                        gyred += imageO[x][y].rgbtRed * gy[gyx][gyy];
                        gygreen += imageO[x][y].rgbtGreen * gy[gyx][gyy];
                        gyblue += imageO[x][y].rgbtBlue * gy[gyx][gyy];
                    }
                }
            }
            
            
            // calculate new values
            long newRed = (gxred * gxred) + (gyred * gyred);
            newRed = round(sqrt(newRed));
            if (newRed > 255)
            {
                newRed = 255;
            }
            
            long newGreen = (gxgreen * gxgreen) + (gygreen * gygreen);
            newGreen = round(sqrt(newGreen));
            if (newGreen > 255)
            {
                newGreen = 255;
            }
            
            long newBlue = (gxblue * gxblue) + (gyblue * gyblue);
            newBlue = round(sqrt(newBlue));
            if (newBlue > 255)
            {
                newBlue = 255;
            }
            
            
            // update values
            image[h][w].rgbtRed = newRed;
            image[h][w].rgbtGreen = newGreen;
            image[h][w].rgbtBlue = newBlue;
        }
    }
}
