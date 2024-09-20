#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <customheaders.h>

#define MAX_LINE_LENGTH 1024
//#define POINT_MAIN

// Define the point structure

// Function prototype

#ifdef POINT_MAIN
int main() {
    const char *filename = "Data/trial1output.csv";  // Hardcoded filename
    int numPoints;
    Point *points = createPointsArray(filename, &numPoints);

    if (!points) {
        fprintf(stderr, "Failed to create points array.\n");
        return 1;
    }

    // Print the points (optional)
    for (int i = 0; i < numPoints; i++) {
        printf("Point %d: x = %.2f, y = %.2f\n", i, points[i].x, points[i].y);
    }

    // Clean up
    free(points);
    return 0;
}
#endif /*POINT_MAIN*/

Point* createPointsArray(const char *filename, int *numPoints) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    *numPoints = 0;

    // First pass: count valid data points
    // Skip the header
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        fclose(file);
        return NULL;
    }

    // Count valid data points
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token;
        float magX, magZ;
        int hasMagX = 0, hasMagZ = 0;

        token = strtok(line, ",");
        if (token && sscanf(token, "%lf", &magX) == 1) hasMagX = 1;
        token = strtok(NULL, ","); // Skip Mag Y
        token = strtok(NULL, ",");
        if (token && sscanf(token, "%lf", &magZ) == 1) hasMagZ = 1;

        if (hasMagX && hasMagZ) {
            (*numPoints)++;
        }
    }

    if (*numPoints == 0) {
        fclose(file);
        return NULL;
    }

    // Allocate memory for points
    Point *points = malloc(*numPoints * sizeof(Point));
    if (!points) {
        fclose(file);
        return NULL;
    }

    // Second pass: populate the points array
    rewind(file);
    // Skip the header
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        free(points);
        fclose(file);
        return NULL;
    }

    int index = 0;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token;
        float magX, magZ;
        int hasMagX = 0, hasMagZ = 0;

        token = strtok(line, ",");
        if (token && sscanf(token, "%lf", &magX) == 1) hasMagX = 1;
        token = strtok(NULL, ","); // Skip Mag Y
        token = strtok(NULL, ",");
        if (token && sscanf(token, "%lf", &magZ) == 1) hasMagZ = 1;

        if (hasMagX && hasMagZ) {
            points[index].x = magX;
            points[index].y = magZ;
            index++;
        }
    }

    fclose(file);
    return points;
}