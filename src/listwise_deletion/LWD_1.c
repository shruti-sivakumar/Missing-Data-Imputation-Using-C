#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 768
#define MAX_COLS 9
#define MISSING_VALUE -1

int main() {
    FILE *file = fopen("pi_diabetes.csv", "r"); 
    if (file == NULL) {
        printf("File not found or unable to open.\n");
        return 1;
    }

    double data[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    char line[1024];

    // Read CSV file and store data in a 2D array
    while (fgets(line, sizeof(line), file) && rows < MAX_ROWS) {
        char *token = strtok(line, ",");
        cols = 0;
        while (token != NULL && cols < MAX_COLS) {
            data[rows][cols++] = atof(token);
            token = strtok(NULL, ",");
        }
        rows++;
    }
    fclose(file);

    // Perform list-wise deletion for missing values
    int valid_rows = 0;
    double cleaned_data[MAX_ROWS][MAX_COLS];

    for (int i = 0; i < rows; i++) {
        int has_missing_value = 0;
        for (int j = 0; j < cols; j++) {
            if (data[i][j] == MISSING_VALUE) { // Check for missing values
                has_missing_value = 1;
                break;
            }
        }

        // If the row doesn't contain missing values, copy it to the cleaned data
        if (!has_missing_value) {
            for (int j = 0; j < cols; j++) {
                cleaned_data[valid_rows][j] = data[i][j];
            }
            valid_rows++;
        }
    }

    // Output the cleaned data to a new CSV file
    FILE *output_file = fopen("imputed_pi_diabetes_lwd.csv", "w");
    if (output_file == NULL) {
        printf("Unable to create or write to file.\n");
        return 1;
    }

    // Write the cleaned data to the file
    for (int i = 0; i < valid_rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(output_file, "%.2f", cleaned_data[i][j]);
            if (j != cols - 1) {
                fprintf(output_file, ",");
            }
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);

    return 0;
}
