#include <stdio.h>
#include <stdlib.h>

// Function to calculate the mean of a 2D array
double calculateMean(double arr[][9], int rows, int cols) {
    double sum = 0.0;
    int size = rows * cols;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sum += arr[i][j];
        }
    }

    return sum / size;
}

// Function to calculate the median of a 2D array
double calculateMedian(double arr[][9], int rows, int cols) {
    int size = rows * cols;
    double flatArray[size];

    // Flatten the 2D array into a 1D array
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            flatArray[i * cols + j] = arr[i][j];
        }
    }

    // Sort the 1D array
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (flatArray[j] > flatArray[j + 1]) {
                // Swap elements if they are in the wrong order
                double temp = flatArray[j];
                flatArray[j] = flatArray[j + 1];
                flatArray[j + 1] = temp;
            }
        }
    }

    // Calculate median
    if (size % 2 == 0) {
        // If the size is even, take the average of the two middle values
        return (flatArray[size / 2 - 1] + flatArray[size / 2]) / 2.0;
    } else {
        // If the size is odd, return the middle value
        return flatArray[size / 2];
    }
}

int main() {
    // Example array with missing values represented by -1
    double data[10][9] = {
        {1, 85, 66, 29, -1, 26.6, 0.351, 31, -1},
        {8, 183, 64, -1, -1, 23.3, 0.672, 32, 1},
        {1, 89, 66, 23, 94, 28.1, 0.167, 21, -1},
        {-1, 137, 40, 35, 168, 43.1, 2.288, 33, 1},
        {5, 116, 74, -1, -1, 25.6, 0.201, 30, -1},
        {3, 78, 50, 32, 88, 31, 0.248, 26, 1},
        {10, 115, -1, -1, -1, 35.3, 0.134, 29, -1},
        {2, 197, 70, 45, 543, 30.5, 0.158, 53, 1},
        {8, 125, 96, -1, -1, -1, 0.232, 54, 1}
    };

    int rows = sizeof(data) / sizeof(data[0]);
    int cols = sizeof(data[0]) / sizeof(data[0][0]);

    // Mean imputation
    double mean = calculateMean(data, rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (data[i][j] == -1) {
                data[i][j] = mean;
            }
        }
    }

    // Output after mean imputation
    printf("After mean imputation:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%lf ", data[i][j]);
        }
        printf("\n");
    }

    // Reset array for median imputation
    double data2[10][9] = {
        {1, 85, 66, 29, -1, 26.6, 0.351, 31, -1},
        {8, 183, 64, -1, -1, 23.3, 0.672, 32, 1},
        {1, 89, 66, 23, 94, 28.1, 0.167, 21, -1},
        {-1, 137, 40, 35, 168, 43.1, 2.288, 33, 1},
        {5, 116, 74, -1, -1, 25.6, 0.201, 30, -1},
        {3, 78, 50, 32, 88, 31, 0.248, 26, 1},
        {10, 115, -1, -1, -1, 35.3, 0.134, 29, -1},
        {2, 197, 70, 45, 543, 30.5, 0.158, 53, 1},
        {8, 125, 96, -1, -1, -1, 0.232, 54, 1}
    };

    // Median imputation
    double median = calculateMedian(data2, rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (data2[i][j] == -1) {
                data2[i][j] = median;
            }
        }
    }

    // Output after median imputation
    printf("\nAfter median imputation:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%lf ", data2[i][j]);
        }
        printf("\n");
    }

    return 0;
}