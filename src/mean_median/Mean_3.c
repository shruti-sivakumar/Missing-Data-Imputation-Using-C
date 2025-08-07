#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 2000
#define MAX_COLS 100

double calculate_mean(double arr[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

double calculate_median(double arr[], int size) {
    double temp;
    int i, j;
    // Using bubble sort to sort the array
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    // Calculating median
    if (size % 2 != 0) {
        return arr[size / 2];
    } else {
        return (arr[(size - 1) / 2] + arr[size / 2]) / 2.0;
    }
}

int main() {
    FILE *file = fopen("melb_data.csv", "r");
    if (file == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    double data[MAX_ROWS][MAX_COLS];
    int rows = 0;
    int cols = 0;
    char line[1024];

    // Read the CSV file and store the values in a 2D array
    while (fgets(line, 1024, file) && rows < MAX_ROWS) {
        char *token = strtok(line, ",");
        cols = 0;
        while (token != NULL && cols < MAX_COLS) {
            data[rows][cols++] = atof(token);
            token = strtok(NULL, ",");
        }
        rows++;
    }

    fclose(file);

    // Impute missing values with mean or median
    for (int j = 0; j < cols; j++) {
        double column[MAX_ROWS];
        int count = 0;
        for (int i = 0; i < rows; i++) {
            if (data[i][j] != 0) {
                column[count++] = data[i][j];
            }
        }
        double mean = calculate_mean(column, count);
        double median = calculate_median(column, count);

        // Impute missing values with mean
        for (int i = 0; i < rows; i++) {
            if (data[i][j] == -1) {
                // Replace missing values with mean
                data[i][j] = mean;
            }
        }
    }

    // Write the updated data back to the CSV file
    file = fopen("imputed_melb_data_mean.csv", "w");
    if (file == NULL) {
        printf("Could not create the output file.\n");
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, j == cols - 1 ? "%.2f\n" : "%.2f,", data[i][j]);
        }
    }

    fclose(file);

    return 0;
}
