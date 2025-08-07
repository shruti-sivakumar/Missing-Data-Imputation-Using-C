// Including header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> 

// Defining constants - dataset size
#define ATTRIBUTES 9
#define RECORDS 768
#define NORMALISE 8
#define MISSING 5

// Create structures
struct Original_Record {
    int pregnancies;
    float glucose;
    float blood_pressure;
    float skin_thickness;
    float insulin;
    float bmi;
    float pedigree;
    int age;
    int class_variable;
};

struct Copy_Record {
    int pregnancies;
    float glucose;
    float blood_pressure;
    float skin_thickness;
    float insulin;
    float bmi;
    float pedigree;
    int age;
    int class_variable;
};

// Function prototypes
void readCSV(const char *filename, struct Original_Record *dataset);;
void copyStruct(struct Original_Record dataset[], struct Copy_Record copy_dataset[]);
void normalise(struct Copy_Record dataset[], float max_vals[], float min_vals[]);
float calculateDistance(struct Copy_Record record_1, struct Copy_Record record_2);
int isMissingValue(float value);
int hasMissingValues(struct Copy_Record record);
void findNearestNeighbours(struct Copy_Record dataset[], int k, int i, int nearest_neighbours_indices[], float nearest_neighbours_distances[]);
void knnImputation(struct Copy_Record dataset[], int k);
void denormalise(struct Copy_Record dataset[], float max_vals[], float min_vals[]);
void writeCSV(const char *filename, struct Original_Record dataset[], struct Copy_Record copy_dataset[]);

// Function to read data from CSV file into the dataset
void readCSV(const char *filename, struct Original_Record *dataset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Skipping the header row
    fscanf(file, "%*[^\n]\n");

    for (int i = 0; i < RECORDS; ++i) {
        fscanf(file, "%d,%f,%f,%f,%f,%f,%f,%d,%d\n", &dataset[i].pregnancies, &dataset[i].glucose, &dataset[i].blood_pressure,
               &dataset[i].skin_thickness, &dataset[i].insulin, &dataset[i].bmi, &dataset[i].pedigree, &dataset[i].age, &dataset[i].class_variable);
    }

    fclose(file);
}

// Function to copy Original_Record to Processed_Record
void copyStruct(struct Original_Record dataset[], struct Copy_Record copy_dataset[]){
        for (int i = 0; i < RECORDS; ++i) {
            // Copying each field from Original_Record to Copy_Record
            copy_dataset[i].pregnancies = dataset[i].pregnancies;
            copy_dataset[i].glucose = dataset[i].glucose;
            copy_dataset[i].blood_pressure = dataset[i].blood_pressure;
            copy_dataset[i].skin_thickness = dataset[i].skin_thickness;
            copy_dataset[i].insulin = dataset[i].insulin;
            copy_dataset[i].bmi = dataset[i].bmi;
            copy_dataset[i].pedigree = dataset[i].pedigree;
            copy_dataset[i].class_variable = dataset[i].class_variable;
    }
}

// Function to normalise values
void normalise(struct Copy_Record dataset[], float max_vals[], float min_vals[]) {

    // Finding maximum and minimum values of each attribute
    for (int i = 0; i < RECORDS; ++i) {
        max_vals[0] = fmax(max_vals[0], dataset[i].pregnancies);
        max_vals[1] = fmax(max_vals[1], dataset[i].glucose);
        max_vals[2] = fmax(max_vals[2], dataset[i].blood_pressure);
        max_vals[3] = fmax(max_vals[3], dataset[i].skin_thickness);
        max_vals[4] = fmax(max_vals[4], dataset[i].insulin);
        max_vals[5] = fmax(max_vals[5], dataset[i].bmi);
        max_vals[6] = fmax(max_vals[6], dataset[i].pedigree);
        max_vals[7] = fmax(max_vals[7], dataset[i].age);

        min_vals[0] = fmin(min_vals[0], dataset[i].pregnancies);
        min_vals[1] = fmin(min_vals[1], dataset[i].glucose);
        min_vals[2] = fmin(min_vals[2], dataset[i].blood_pressure);
        min_vals[3] = fmin(min_vals[3], dataset[i].skin_thickness);
        min_vals[4] = fmin(min_vals[4], dataset[i].insulin);
        min_vals[5] = fmin(min_vals[5], dataset[i].bmi);
        min_vals[6] = fmin(min_vals[6], dataset[i].pedigree);
        min_vals[7] = fmin(min_vals[7], dataset[i].age);
    }

    // Normalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].pregnancies = (dataset[i].pregnancies - min_vals[0]) / (max_vals[0] - min_vals[0]);
        dataset[i].glucose = (dataset[i].glucose - min_vals[1]) / (max_vals[1] - min_vals[1]);
        dataset[i].blood_pressure = (dataset[i].blood_pressure - min_vals[2]) / (max_vals[2] - min_vals[2]);
        dataset[i].skin_thickness = (dataset[i].skin_thickness - min_vals[3]) / (max_vals[3] - min_vals[3]);
        dataset[i].insulin = (dataset[i].insulin - min_vals[4]) / (max_vals[4] - min_vals[4]);
        dataset[i].bmi = (dataset[i].bmi - min_vals[5]) / (max_vals[5] - min_vals[5]);
        dataset[i].pedigree = (dataset[i].pedigree - min_vals[6]) / (max_vals[6] - min_vals[6]);
        dataset[i].age = (dataset[i].age - min_vals[7]) / (max_vals[7] - min_vals[7]);
    }
}

// Function to calculate Euclidean distance between two records
float calculateDistance(struct Copy_Record record_1, struct Copy_Record record_2) {
    float sum = 0;
    // Assuming all attributes contribute to the distance calculation
    sum += pow(record_1.pregnancies - record_2.pregnancies, 2);
    sum += pow(record_1.glucose - record_2.glucose, 2);
    sum += pow(record_1.blood_pressure - record_2.blood_pressure, 2);
    sum += pow(record_1.skin_thickness - record_2.skin_thickness, 2);
    sum += pow(record_1.insulin - record_2.insulin, 2);
    sum += pow(record_1.bmi - record_2.bmi, 2);
    sum += pow(record_1.pedigree - record_2.pedigree, 2);
    sum += pow(record_1.age - record_2.age, 2);
    return sqrt(sum);
}

// Function to check if a value is missing
int isMissingValue(float value) {
    return value==0;
}

// Function to check for missing values in a record
int hasMissingValues(struct Copy_Record record) {
    return (isMissingValue(record.glucose) || isMissingValue(record.blood_pressure) || isMissingValue(record.skin_thickness) || isMissingValue(record.insulin) || isMissingValue(record.bmi));
}

// Function to find K-nearest neighbours
void findNearestNeighbours(struct Copy_Record dataset[], int k, int i, int nearest_neighbours_indices[], float nearest_neighbours_distances[]) {    
    // Initialising nearest neighbour arrays
    for (int j = 0; j < k; ++j) {
    nearest_neighbours_indices[j] = -1;  // Invalid index
    nearest_neighbours_distances[j] = FLT_MAX;  // Maximum float value 
    }

    // Looping through records
    for (int j = 0; j < RECORDS; ++j) {
        if (i != j) {  // Excluding the current record
            // Calculating Euclidean distance
            float distance = calculateDistance(dataset[i], dataset[j]);

            // Updating nearest neighbours arrays
            for (int m = 0; m < k; ++m) {
                if (distance < nearest_neighbours_distances[m]) {
                    // Shifting elements to make room for the new neighbour
                    for (int n = k - 1; n > m; --n) {
                        nearest_neighbours_indices[n] = nearest_neighbours_indices[n - 1];
                        nearest_neighbours_distances[n] = nearest_neighbours_distances[n - 1];
                    }
                    // Insert the new neighbour
                    nearest_neighbours_indices[m] = j;
                    nearest_neighbours_distances[m] = distance;
                    break;
                }
            }
        }
    }
}

// Function to impute missing values using KNN
void knnImputation(struct Copy_Record dataset[], int k) {
    // Looping through records with missing values
    for (int i = 0; i < RECORDS; ++i) {
        if (hasMissingValues(dataset[i])) {
            // Variables for indices and distances of nearest neighbours
            int nearest_neighbours_indices[k];
            float nearest_neighbours_distances[k];

            // Finding K-nearest neighbours
            findNearestNeighbours(dataset, k, i, nearest_neighbours_indices, nearest_neighbours_distances);

            // Imputing missing values based on the values of nearest neighbours
            float imputed_values[MISSING];
            for (int j = 0; j < MISSING; ++j) {
                imputed_values[j] = 0.0;
            }

            // Summing the values of the attributes from nearest neighbours (excluding pregnancies, age, class_variable)
            for (int j = 0; j < k; ++j) {
                int neighbour_index = nearest_neighbours_indices[j];
                imputed_values[0] += dataset[neighbour_index].glucose;
                imputed_values[1] += dataset[neighbour_index].blood_pressure;
                imputed_values[2] += dataset[neighbour_index].skin_thickness;
                imputed_values[3] += dataset[neighbour_index].insulin;
                imputed_values[4] += dataset[neighbour_index].bmi;
            }

            // Averaging the values to impute the missing values
            for (int j = 0; j < MISSING; ++j) {
                imputed_values[j] /= k;
            }

            // Updating the current record with imputed values
            if (isMissingValue(dataset[i].glucose)) dataset[i].glucose = imputed_values[0];
            if (isMissingValue(dataset[i].blood_pressure)) dataset[i].blood_pressure = imputed_values[1];
            if (isMissingValue(dataset[i].skin_thickness)) dataset[i].skin_thickness = imputed_values[2];
            if (isMissingValue(dataset[i].insulin)) dataset[i].insulin = imputed_values[3];
            if (isMissingValue(dataset[i].bmi)) dataset[i].bmi = imputed_values[4];

        }
    }
}

// Function to denormalise values
void denormalise(struct Copy_Record dataset[], float max_vals[], float min_vals[]) {
    // Denormalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].pregnancies = dataset[i].pregnancies * (max_vals[0] - min_vals[0]) + min_vals[0];
        dataset[i].glucose = dataset[i].glucose * (max_vals[1] - min_vals[1]) + min_vals[1];
        dataset[i].blood_pressure = dataset[i].blood_pressure * (max_vals[2] - min_vals[2]) + min_vals[2];
        dataset[i].skin_thickness = dataset[i].skin_thickness * (max_vals[3] - min_vals[3]) + min_vals[3];
        dataset[i].insulin = dataset[i].insulin * (max_vals[4] - min_vals[4]) + min_vals[4];
        dataset[i].bmi = dataset[i].bmi * (max_vals[5] - min_vals[5]) + min_vals[5];
        dataset[i].pedigree = dataset[i].pedigree * (max_vals[6] - min_vals[6]) + min_vals[6];
        dataset[i].age = dataset[i].age * (max_vals[7] - min_vals[7]) + min_vals[7];
    }
}

// Function to write data from dataset into CSV file
void writeCSV(const char *filename, struct Original_Record dataset[], struct Copy_Record copy_dataset[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "pregnancies,glucose,blood_pressure,skin_thickness,insulin,bmi,pedigree,age,class_variable\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%d,%f,%f,%f,%f,%f,%f,%d,%d\n", dataset[i].pregnancies, copy_dataset[i].glucose, copy_dataset[i].blood_pressure, copy_dataset[i].skin_thickness, copy_dataset[i].insulin, copy_dataset[i].bmi, dataset[i].pedigree, dataset[i].age, dataset[i].class_variable);
    }

    fclose(file);
}

int main() {
    // Initialize maximum and minimum values for normalizing
    float max_vals[NORMALISE], min_vals[NORMALISE];
    for (int i = 0; i < NORMALISE; ++i){
        max_vals[i] = 0.0;
        min_vals[i] = FLT_MAX;
    }

    // Reading data from CSV file
    struct Original_Record dataset[RECORDS];
    readCSV("pi_diabetes.csv", dataset);

    // Copying to Copy_Record
    struct Copy_Record copy_dataset[RECORDS];
    copyStruct(dataset, copy_dataset);

    // Normalising data
    normalise(copy_dataset, max_vals, min_vals);

    // Choosing distance metric (Euclidean distance)

    // Determining the number of neighbours (K)
    int k_neighbours = 27; // Rule of thumb : sqrt(RECORDS) approx.

    // Perform k-NN imputation
    knnImputation(copy_dataset, k_neighbours);

    // Denormalising data
    denormalise(copy_dataset, max_vals, min_vals);

    // Write imputed data to CSV file
    writeCSV("imputed_pi_diabetes_knn.csv", dataset, copy_dataset);

    return 0;
}