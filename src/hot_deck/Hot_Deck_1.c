#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 768   // Maximum number of rows in the CSV file
#define MAX_COLS 9    // Maximum number of columns in the CSV file

float arr[MAX_ROWS][MAX_COLS];

// Function to read CSV file and convert it to a 2D array
int readCSV(const char *filename, float array[MAX_ROWS][MAX_COLS]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;  // Return -1 to indicate an error
    }

    int numRows = 0;
    char line[1024];  // Assuming each line in the CSV file is less than 1024 characters

    // Read each line from the file
    while (fgets(line, sizeof(line), file) != NULL && numRows < MAX_ROWS) {
        // Tokenize the line based on the CSV delimiter (assuming ',' here)
        char *token = strtok(line, ",");
        int numCols = 0;

        // Convert and store each token in the array
        while (token != NULL && numCols < MAX_COLS) {
            array[numRows][numCols] = atof(token);
            token = strtok(NULL, ",");
            numCols++;
        }

        numRows++;
    }

    fclose(file);
    return numRows;  // Return the number of rows read
}

void writeCSV(const char *filename, float array[MAX_ROWS][MAX_COLS], int numRows, int numCols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write each element of the array to the file
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            fprintf(file, "%f", array[i][j]);

            // Add a comma after each element (except the last one in a row)
            if (j < numCols - 1) {
                fprintf(file, ",");
            }
        }
        // Move to the next line after each row
        fprintf(file, "\n");
    }

    fclose(file);
}


int main() {
    const char *filename = "pi_diabetes";
    const char *filename1 = "imputed_pi_diabetes_hotdeck";


    int numRows = readCSV(filename, arr);

    if (numRows == -1) {
        // An error occurred while reading the CSV file
        return 1;
    }

    //code 
    int rows = sizeof(arr) / sizeof(arr[0]);
    int cols = sizeof(arr[0]) / sizeof(arr[0][0]);

    

    for(int i=0;i<rows;i++){
        
        //check if row i has missing values
        int missing_values = 0;
        for(int j=0;j<cols;j++){
            if(arr[i][j]==0)
                missing_values = 1;    
        }
        //execute this part only if there are missing values in row i
        if(missing_values == 1){
             
        //we now need to decide which row to take donor variables from for imputation
            // set preferences values for all the other rows.
            int preferences[rows];
            preferences[i] = -1;
            for(int k=0;k<rows;k++){
                if(k!=i){
                    preferences[k]=0;
                    for(int l=0;l<cols;l++){
                        if((arr[k][l]==arr[i][l])&&(arr[k][l]!=0))
                            preferences[k] = preferences[k]+1;
                    }
                }
            }


            //find the highest preference alloted to a row
            int max_pref = 0;
            for(int k=0;k<rows;k++){
                if (preferences[k] > max_pref){
                    max_pref = preferences[k];
                }
            }

            //make a array with only the indexs of the highest preference rows
            int max_pref_arr[rows];
            int no_of_pref = 0;
            for(int k=0;k<rows;k++){
                if(preferences[k]==max_pref){
                    max_pref_arr[no_of_pref] = k;
                    no_of_pref++;
                }
            }
            

            int donor_index = rand() % no_of_pref;
            int donor_row = max_pref_arr[donor_index];

            //putting all the missing values from the donor row to the row with missing index

            for(int l=0;l<cols;l++){
                if(arr[i][l]==0){
                    int donor_values = arr[donor_row][l];
                    arr[i][l] = donor_values;

                }
            }
            
        }

        missing_values = 0;

    }
    return 0;
}
