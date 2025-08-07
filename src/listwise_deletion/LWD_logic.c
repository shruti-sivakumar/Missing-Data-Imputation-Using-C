#include <stdio.h>

// Function to display the 2D array
void displayArray(int arr[][5], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

// Function to delete specific rows from the 2D array
void deleteRows(int arr[][5], int *rows, int cols, int row1, int row2) {
    if (*rows <= 0 || row1 >= *rows || row2 >= *rows) {
        printf("Invalid row indices\n");
        return;
    }

    // Create a new array with reduced size
    int newArray[*rows - 2][cols];

    // Copy elements from the original array to the new array excluding the specified rows
    for (int i = 0, newRow = 0; i < *rows; i++) {
        if (i != row1 && i != row2) {
            for (int j = 0; j < cols; j++) {
                newArray[newRow][j] = arr[i][j];
            }
            newRow++;
        }
    }

    // Update the original array and the number of rows
    for (int i = 0; i < *rows - 2; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = newArray[i][j];
        }
    }
    *rows -= 2;
}

int main() {
    int dataset[5][5] = {
        {7, 133, 84,23,50},
        {0, 180, 66,0,7},
        {1, 146, 5,77,35},
        {10,125,70,9,41},
        {0,51,0,35,0}
    };

    int rows = 5;
    int cols = 5;

    printf("Original Array:\n");
    displayArray(dataset, rows, cols);

    int rowToDelete1 = 1;  // Index of the first row to delete
    int rowToDelete2 = 4;  // Index of the second row to delete

    deleteRows(dataset, &rows, cols, rowToDelete1, rowToDelete2);

    printf("\nArray after deleting rows %d and %d:\n", rowToDelete1, rowToDelete2);
    displayArray(dataset, rows, cols);

    return 0;
}
