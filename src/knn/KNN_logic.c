#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> 

// Test Dataset
float test_dataset[15][9]={{6,148,72,35,0,33.6,0.627,50,1},{1,85,66,29,0,26.6,0.351,31,0},{8,183,64,0,0,23.3,0.672,32,1},{1,89,66,23,94,28.1,0.167,21,0},{0,137,40,35,168,43.1,2.288,33,1},{5,116,74,0,0,25.6,0.201,30,0},{3,78,50,32,88,31,0.248,26,1},{10,115,0,0,0,35.3,0.134,29,0},{2,197,70,45,543,30.5,0.158,53,1},{8,125,96,0,0,0,0.232,54,1},{4,110,92,0,0,37.6,0.191,30,0},{10,168,74,0,0,38,0.537,34,1},{10,139,80,0,0,27.1,1.441,57,0},{1,189,60,23,846,30.1,0.398,59,1},{5,166,72,19,175,25.8,0.587,51,1}};

// Function prototypes
void normalise(float test_dataset[][9], float max_vals[], float min_vals[]);
float calculateDistance(float record_1[], float record_2[]);
int isMissingValue(float value);
int hasMissingValues(float record[]);
void findNearestNeighbours(float test_dataset[][9], int k, int nearest_neighbors_indices[], float nearest_neighbors_distances[]);
void knnImputation(float test_dataset[][9], int k);
void denormalise(float test_dataset[][9], float max_vals[], float min_vals[]);

// Function to normalise values
void normalise(float test_dataset[][9], float max_vals[], float min_vals[]) {
    // Finding the maximum and minimum values for each feature

    for (int i = 0; i < 7; ++i) {
        max_vals[i] = test_dataset[0][i];
        min_vals[i] = test_dataset[0][i];

    }

    // Finding the maximum values for each feature
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 7; ++j) {
            max_vals[j] = fmax(max_vals[j], test_dataset[i][j]);
            min_vals[j] = fmin(min_vals[j], test_dataset[i][j]);
        }
    }

    // Normalising each feature
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 7; ++j){
            test_dataset[i][j] = (test_dataset[i][j] - min_vals[j]) / (max_vals[j] - min_vals[j]);
        }
    }
}

// Function to calculate Euclidean distance between two records
float calculateDistance(float record_1[], float record_2[]) {
    float sum=0;
    for (int i = 0; i < 7; ++i){
        sum+=pow(record_1[i] - record_2[i], 2);
    }
    return sqrt(sum);
}

// Function to check if a value is missing
int isMissingValue(float value) {
    return value==0;
}

// Function to check for missing values in a record
int hasMissingValues(float record[]) {
    for (int i = 1; i < 6; ++i){
        if (isMissingValue(record[i]))
            return 1;
    }
    return 0;
}

// Function to find K-nearest neighbours
void findNearestNeighbours(float test_dataset[][9], int k, int nearest_neighbors_indices[], float nearest_neighbors_distances[]){    
    // Initialising nearest neighbour arrays
    for (int j = 0; j < k; ++j) {
    nearest_neighbors_indices[j] = -1;  // Invalid index
    nearest_neighbors_distances[j] = FLT_MAX;  // Maximum float value 
    }

    // Looping through records
    for (int i = 0; i < 15; ++i){
        for (int j = 0; j < 15; ++j) {
            if (i != j) {  // Excluding the current record
                // Calculating Euclidean distance
                float distance = calculateDistance(test_dataset[i], test_dataset[j]);

                // Updating nearest neighbors arrays
                for (int m = 0; m < k; ++m) {
                    if (distance < nearest_neighbors_distances[m]) {
                        // Shifting elements to make room for the new neighbor
                        for (int n = k - 1; n > m; --n) {
                            nearest_neighbors_indices[n] = nearest_neighbors_indices[n - 1];
                            nearest_neighbors_distances[n] = nearest_neighbors_distances[n - 1];
                        }
                        // Insert the new neighbor
                        nearest_neighbors_indices[m] = j;
                        nearest_neighbors_distances[m] = distance;
                        break;
                    }
                }
            }
        }
    }
}

// Function to impute missing values using KNN
void knnImputation(float test_dataset[][9], int k) {
    // Variables for indices and distances of nearest neighbors
    int nearest_neighbors_indices[k];
    float nearest_neighbors_distances[k];
    
    //Finding K-nearest neighbours
    findNearestNeighbours(test_dataset, k, nearest_neighbors_indices, nearest_neighbors_distances);
    
    // Looping through records with missing values
    for (int i = 0; i < 15; ++i) {
        if (hasMissingValues(test_dataset[i])) {

            // Imputing missing values based on the values of nearest neighbors
            float imputed_values[6];
            for (int j = 1; j < 6; ++j){
                imputed_values[j] = 0.0;
            }

            // Summing the values of the attributes from nearest neighbors
            for (int j = 0; j < k; ++j) {
                int neighbor_index = nearest_neighbors_indices[j];
                for (int m = 1; m < 6; ++m){
                    imputed_values[m] += test_dataset[neighbor_index][m];
                }
            }

            // Averaging the values to impute the missing values
            for (int j = 1; j < 6; ++j){
                imputed_values[j] /= k;
            }
            
            // Updating the current record with imputed values
            for (int j = 1; j < 6; ++j){
                if (isMissingValue(test_dataset[i][j])){
                    test_dataset[i][j] = imputed_values[j];
                }
            }
        }
    }
}

// Function to denormalise values
void denormalise(float test_dataset[][9], float max_vals[], float min_vals[]){
    // Denormalising each feature
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 7; ++j){
            test_dataset[i][j] = test_dataset[i][j] * (max_vals[j] - min_vals[j]) + min_vals[j];
        }
    }
}

int main() {
	// Printing Original 2D Array
	printf("Original Array:\n");
		for (int i = 0; i < 15; ++i){
            for (int j = 0; j < 8; ++j){
                printf("%f ", test_dataset[i][j]);
            }
        printf("\n");
    }
		
    // Initialising maximum and minimum values for normalising
    float max_vals[8], min_vals[8];

    // Normalising data
    normalise(test_dataset, max_vals, min_vals);

    // Choosing distance metric (Euclidean distance)

    // Determining the number of neighbors (K)
    int k_neighbors = 3;

    // Perform KNN imputation
    knnImputation(test_dataset, k_neighbors);

    // Denormalising data
    denormalise(test_dataset, max_vals, min_vals);
		
	// Printing Imputed 2D Array
	printf("\nImputed Array:\n");
    for (int i = 0; i < 15; ++i){
        for (int j = 0; j < 8; ++j){
            printf("%f ", test_dataset[i][j]);
        }
        printf("\n");
    }

    return 0;
}