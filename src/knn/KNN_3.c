// Including header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Defining constants - dataset size
#define ATTRIBUTES 13
#define RECORDS 13580
#define NORMALISE 13
#define MISSING 3

// Create structures
struct Record {
    float rooms;
    float price;
    float distance;
    float postcode;
    float bedroom;
    float bathroom;
    float car;
    float landsize;
    float building_area;
    float year_built;
    float lattitute;
    float longitude;
    float property_count;
};

// Function prototypes
void moveBackToStartOfLine(FILE *file);
void readCSV(const char *filename, struct Record *dataset);
void normalise(struct Record dataset[], float max_vals[], float min_vals[]);
float calculateDistance(struct Record record_1, struct Record record_2);
int isMissingValue(float value);
int hasMissingValues(struct Record record);
void findNearestNeighbours(struct Record dataset[], int k, int i, int nearest_neighbours_indices[], float nearest_neighbours_distances[]);
void knnImputation(struct Record dataset[], int k);
void denormalise(struct Record dataset[], float max_vals[], float min_vals[]);
void writeCSV(const char *filename, struct Record dataset[]);

// Function to move back to the start of the current line
void moveBackToStartOfLine(FILE *file) {
    int c;
    do {
        fseek(file, -2, SEEK_CUR);  // Move back two positions (one for the current character and one for the newline)
        c = fgetc(file);
    } while (c != '\n');
}

// Function to read data from CSV file into the dataset
void readCSV(const char *filename, struct Record *dataset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Skipping the header row
    fscanf(file, "%*[^\n]\n");

    for (int i = 0; i < RECORDS; ++i) {

        int result = fscanf(file, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", &dataset[i].rooms, &dataset[i].price, &dataset[i].distance, &dataset[i].postcode, &dataset[i].bedroom, &dataset[i].bathroom, &dataset[i].car, &dataset[i].landsize, &dataset[i].building_area, &dataset[i].year_built, &dataset[i].lattitute, &dataset[i].longitude, &dataset[i].property_count);

        if (result != ATTRIBUTES) {
            moveBackToStartOfLine(file);
            fscanf(file, "%f,%f,%f,%f,%f,%f,", &dataset[i].rooms, &dataset[i].price, &dataset[i].distance, &dataset[i].postcode, &dataset[i].bedroom, &dataset[i].bathroom);

            result = fscanf(file, "%f,", &dataset[i].car);
            if (result != 1) {
                *&dataset[i].car = NAN;
                fscanf(file, ",");
            }

            fscanf(file, "%f,", &dataset[i].landsize);

            float* fields[] = {&dataset[i].building_area, &dataset[i].year_built};
            for (int j = 0; j <= 1; ++j) {
                result = fscanf(file, "%f,", fields[j]);
                if (result != 1) {
                    *fields[j] = NAN;
                    fscanf(file, ",");
                }
            }

            fscanf(file, "%f,%f,%f\n", &dataset[i].lattitute, &dataset[i].longitude, &dataset[i].property_count);
        }
    }

    fclose(file);
}

// Function to normalise values
void normalise(struct Record dataset[], float max_vals[], float min_vals[]) {

    // Finding maximum and minimum values of each attribute
    for (int i = 0; i < RECORDS; ++i) {
        max_vals[0] = fmax(max_vals[0], dataset[i].rooms);
        max_vals[1] = fmax(max_vals[1], dataset[i].price);
        max_vals[2] = fmax(max_vals[2], dataset[i].distance);
        max_vals[3] = fmax(max_vals[3], dataset[i].postcode);
        max_vals[4] = fmax(max_vals[4], dataset[i].bedroom);
        max_vals[5] = fmax(max_vals[5], dataset[i].bathroom);
        max_vals[6] = fmax(max_vals[6], dataset[i].car);
        max_vals[7] = fmax(max_vals[7], dataset[i].landsize);
        max_vals[8] = fmax(max_vals[8], dataset[i].building_area);
        max_vals[9] = fmax(max_vals[9], dataset[i].year_built);
        max_vals[10] = fmax(max_vals[10], dataset[i].lattitute);
        max_vals[11] = fmax(max_vals[11], dataset[i].longitude);
        max_vals[12] = fmax(max_vals[12], dataset[i].property_count);

        min_vals[0] = fmin(min_vals[0], dataset[i].rooms);
        min_vals[1] = fmin(min_vals[1], dataset[i].price);
        min_vals[2] = fmin(min_vals[2], dataset[i].distance);
        min_vals[3] = fmin(min_vals[3], dataset[i].postcode);
        min_vals[4] = fmin(min_vals[4], dataset[i].bedroom);
        min_vals[5] = fmin(min_vals[5], dataset[i].bathroom);
        min_vals[6] = fmin(min_vals[6], dataset[i].car);
        min_vals[7] = fmin(min_vals[7], dataset[i].landsize);
        min_vals[8] = fmin(min_vals[8], dataset[i].building_area);
        min_vals[9] = fmin(min_vals[9], dataset[i].year_built);
        min_vals[10] = fmin(min_vals[10], dataset[i].lattitute);
        min_vals[11] = fmin(min_vals[11], dataset[i].longitude);
        min_vals[12] = fmin(min_vals[12], dataset[i].property_count);
    }

    // Normalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].rooms = (dataset[i].rooms - min_vals[0]) / (max_vals[0] - min_vals[0]);
        dataset[i].price = (dataset[i].price - min_vals[1]) / (max_vals[1] - min_vals[1]);
        dataset[i].distance = (dataset[i].distance - min_vals[2]) / (max_vals[2] - min_vals[2]);
        dataset[i].postcode = (dataset[i].postcode - min_vals[3]) / (max_vals[3] - min_vals[3]);
        dataset[i].bedroom = (dataset[i].bedroom - min_vals[4]) / (max_vals[4] - min_vals[4]);
        dataset[i].bathroom = (dataset[i].bathroom - min_vals[5]) / (max_vals[5] - min_vals[5]);
        dataset[i].car = (dataset[i].car - min_vals[6]) / (max_vals[6] - min_vals[6]);
        dataset[i].landsize = (dataset[i].landsize - min_vals[7]) / (max_vals[7] - min_vals[7]);
        dataset[i].building_area = (dataset[i].building_area - min_vals[8]) / (max_vals[8] - min_vals[8]);
        dataset[i].year_built = (dataset[i].year_built - min_vals[9]) / (max_vals[9] - min_vals[9]);
        dataset[i].lattitute = (dataset[i].lattitute - min_vals[10]) / (max_vals[10] - min_vals[10]);
        dataset[i].longitude = (dataset[i].longitude - min_vals[11]) / (max_vals[11] - min_vals[11]);
        dataset[i].property_count = (dataset[i].property_count - min_vals[12]) / (max_vals[12] - min_vals[12]);
    }
}

// Function to calculate Euclidean distance between two records
float calculateDistance(struct Record record_1, struct Record record_2) {
    float sum = 0;
    // Assuming all attributes contribute to the distance calculation
    sum += pow(record_1.rooms - record_2.rooms, 2);
    sum += pow(record_1.price - record_2.price, 2);
    sum += pow(record_1.distance - record_2.distance, 2);
    sum += pow(record_1.postcode - record_2.postcode, 2);
    sum += pow(record_1.bedroom - record_2.bedroom, 2);
    sum += pow(record_1.bathroom - record_2.bathroom, 2);
    sum += pow(record_1.car - record_2.car, 2);
    sum += pow(record_1.landsize - record_2.landsize, 2);
    sum += pow(record_1.building_area - record_2.building_area, 2);
    sum += pow(record_1.year_built - record_2.year_built, 2);
    sum += pow(record_1.lattitute - record_2.lattitute, 2);
    sum += pow(record_1.longitude - record_2.longitude, 2);
    sum += pow(record_1.property_count - record_2.property_count, 2);
    return sqrt(sum);
}

// Function to check if a value is missing
int isMissingValue(float value) {
    return isnan(value);
}

// Function to check for missing values in a record
int hasMissingValues(struct Record record) {
    return (isMissingValue(record.car) || isMissingValue(record.building_area) || isMissingValue(record.year_built));
}

// Function to find K-nearest neighbours
void findNearestNeighbours(struct Record dataset[], int k, int i, int nearest_neighbours_indices[], float nearest_neighbours_distances[]) {    
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
void knnImputation(struct Record dataset[], int k) {
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

            // Summing the values of the attributes from nearest neighbours (excluding year)
            for (int j = 0; j < k; ++j) {
                int neighbour_index = nearest_neighbours_indices[j];
                imputed_values[0] += dataset[neighbour_index].car;
                imputed_values[1] += dataset[neighbour_index].building_area;
                imputed_values[2] += dataset[neighbour_index].year_built;
            }

            // Averaging the values to impute the missing values
            for (int j = 0; j < MISSING; ++j) {
                imputed_values[j] /= k;
            }

            // Updating the current record with imputed values
            if (isMissingValue(dataset[i].car)) dataset[i].car = imputed_values[0];
            if (isMissingValue(dataset[i].building_area)) dataset[i].building_area = imputed_values[1];
            if (isMissingValue(dataset[i].year_built)) dataset[i].year_built = imputed_values[2];

        }
    }
}

// Function to denormalise values
void denormalise(struct Record dataset[], float max_vals[], float min_vals[]) {
    // Denormalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].rooms = dataset[i].rooms * (max_vals[0] - min_vals[0]) + min_vals[0];
        dataset[i].price = dataset[i].price * (max_vals[1] - min_vals[1]) + min_vals[1];
        dataset[i].distance = dataset[i].distance * (max_vals[2] - min_vals[2]) + min_vals[2];
        dataset[i].postcode = dataset[i].postcode * (max_vals[3] - min_vals[3]) + min_vals[3];
        dataset[i].bedroom = dataset[i].bedroom * (max_vals[4] - min_vals[4]) + min_vals[4];
        dataset[i].bathroom = dataset[i].bathroom * (max_vals[5] - min_vals[5]) + min_vals[5];
        dataset[i].car = dataset[i].car * (max_vals[6] - min_vals[6]) + min_vals[6];
        dataset[i].landsize = dataset[i].landsize * (max_vals[7] - min_vals[7]) + min_vals[7];
        dataset[i].building_area = dataset[i].building_area * (max_vals[8] - min_vals[8]) + min_vals[8];
        dataset[i].year_built = dataset[i].year_built * (max_vals[9] - min_vals[9]) + min_vals[9];
        dataset[i].lattitute = dataset[i].lattitute * (max_vals[10] - min_vals[10]) + min_vals[10];
        dataset[i].longitude = dataset[i].longitude * (max_vals[11] - min_vals[11]) + min_vals[11];
        dataset[i].property_count = dataset[i].property_count * (max_vals[12] - min_vals[12]) + min_vals[12];
    }
}

// Function to write data from dataset into CSV file
void writeCSV(const char *filename, struct Record dataset[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "rooms,price,distance,postcode,bedroom,bathroom,car,landsize,building_area,year_built,lattitude,longitude,property_count\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", round(dataset[i].rooms), round(dataset[i].price), dataset[i].distance, round(dataset[i].postcode), round(dataset[i].bedroom), round(dataset[i].bathroom), round(dataset[i].car), round(dataset[i].landsize), round(dataset[i].building_area), round(dataset[i].year_built), dataset[i].lattitute, dataset[i].longitude, round(dataset[i].property_count));
    }

    fclose(file);
}

int main() {
    // Initialize maximum and minimum values for normalizing
    float max_vals[NORMALISE], min_vals[NORMALISE];
    for (int i = 0; i < NORMALISE; ++i){
        max_vals[i] = -FLT_MAX;
        min_vals[i] = FLT_MAX;
    }

    // Reading data from CSV file
    struct Record dataset[RECORDS];
    readCSV("melb_data.csv", dataset);
    
    // Normalising data
    normalise(dataset, max_vals, min_vals);

    // Choosing distance metric (Euclidean distance)

    // Determining the number of neighbours (K)
    int k_neighbours = 116;

    // Perform k-NN imputation
    knnImputation(dataset, k_neighbours);

    // Denormalising data
    denormalise(dataset, max_vals, min_vals);

    // Write imputed data to CSV file
    writeCSV("imputed_melb_data_knn.csv", dataset);

    return 0;
}