#include <stdio.h>
#include <stdlib.h>

#define INCLUDE_OUTPUT_H

#include "IKBasic.h"
#include "BuchM.h"
#include "Output.h"

// Function to read points from file, dynamically allocating memory
KnotPtr readPointsFromFile(const char *filename, int *iktot)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("File opening failed");
        return NULL;
    }

    int maxPoints = 10000; // Maximum number of points
    int blockSize = 100;   // Initial block size for allocation
    KnotPtr points = malloc(sizeof(Knot) * blockSize);
    if (points == NULL)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    int i = 0;
    while (fscanf(file, "%hd,%f,%f", &points[i].knotType, &points[i].x, &points[i].y) == 3)
    {
        i++;
        if (i >= maxPoints) // Check if maximum limit is reached
        {
            break; // Stop reading if maximum number of points is reached
        }
        if (i >= blockSize) // Check if current block is full
        {
            blockSize += 100; // Increase block size
            if (blockSize > maxPoints)
                blockSize = maxPoints; // Ensure blockSize does not exceed maxPoints
            KnotPtr temp = realloc(points, sizeof(Knot) * blockSize);
            if (temp == NULL)
            {
                free(points);
                fclose(file);
                fprintf(stderr, "Memory reallocation failed\n");
                return NULL;
            }
            points = temp;
        }
    }
    *iktot = i; // Set the total number of digitizations

    // Check if we reached the end of file or encountered a format error
    if (!feof(file))
    {
        free(points);
        fclose(file);
        fprintf(stderr, "CSV format error before reaching end of file\n");
        return NULL;
    }

    fclose(file);
    return points;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input-file-path>\n", argv[0]);
        return 1; // Error due to incorrect usage
    }

    int iktot;
    KnotPtr knotList = readPointsFromFile(argv[1], &iktot);
    if (knotList == NULL)
    {
        return 1; // Error reading file or memory allocation failed
    }

    // Assuming you want to process the entire list
    int ianf = 0;
    int iend = iktot - 1;
    int istart = 0;           // Assuming the first point is a start point
    Float reduceFactor = 1.0; // Example reduce factor

    // Call BuchM to process the points
    printf("[\n");
    BuchM(ianf, iend, istart, iktot, reduceFactor, knotList);
    printf("]\n");

    // Cleanup
    free(knotList);

    return 0;
}
