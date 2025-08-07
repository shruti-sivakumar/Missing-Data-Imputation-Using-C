// Including header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Defining constants - dataset size
#define ATTRIBUTES 10
#define RECORDS 1949
#define NORMALISE 9
#define MISSING 8

// Create structures
struct Record {
    int year;
    float life_ladder;
    float log_gdp;
    float social_support;
    float life_expectancy;
    float life_choices;
    float generosity;
    float corruption;
    float positive_affect;
    float negative_affect;
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

        int result = fscanf(file, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", &dataset[i].year, &dataset[i].life_ladder, &dataset[i].log_gdp, &dataset[i].social_support, &dataset[i].life_expectancy, &dataset[i].life_choices, &dataset[i].generosity, &dataset[i].corruption, &dataset[i].positive_affect, &dataset[i].negative_affect);

        if (result != ATTRIBUTES) {
            moveBackToStartOfLine(file);
            fscanf(file, "%d,", &dataset[i].year);

            float* fields[] = {&dataset[i].life_ladder, &dataset[i].log_gdp, &dataset[i].social_support, &dataset[i].life_expectancy, &dataset[i].life_choices, &dataset[i].generosity, &dataset[i].corruption, &dataset[i].positive_affect, &dataset[i].negative_affect};

            for (int j = 0; j <= MISSING; ++j) {
                result = fscanf(file, "%f,", fields[j]);
                if (result != 1) {
                    *fields[j] = NAN;
                    if (j == MISSING) fscanf(file, "\n");
                    else fscanf(file, ",");
                }
            }
        }
    }

    fclose(file);
}

// Function to normalise values
void normalise(struct Record dataset[], float max_vals[], float min_vals[]) {

    // Finding maximum and minimum values of each attribute
    for (int i = 0; i < RECORDS; ++i) {
        max_vals[0] = fmax(max_vals[0], dataset[i].life_ladder);
        max_vals[1] = fmax(max_vals[1], dataset[i].log_gdp);
        max_vals[2] = fmax(max_vals[2], dataset[i].social_support);
        max_vals[3] = fmax(max_vals[3], dataset[i].life_expectancy);
        max_vals[4] = fmax(max_vals[4], dataset[i].life_choices);
        max_vals[5] = fmax(max_vals[5], dataset[i].generosity);
        max_vals[6] = fmax(max_vals[6], dataset[i].corruption);
        max_vals[7] = fmax(max_vals[7], dataset[i].positive_affect);
        max_vals[8] = fmax(max_vals[8], dataset[i].negative_affect);

        min_vals[0] = fmin(min_vals[0], dataset[i].life_ladder);
        min_vals[1] = fmin(min_vals[1], dataset[i].log_gdp);
        min_vals[2] = fmin(min_vals[2], dataset[i].social_support);
        min_vals[3] = fmin(min_vals[3], dataset[i].life_expectancy);
        min_vals[4] = fmin(min_vals[4], dataset[i].life_choices);
        min_vals[5] = fmin(min_vals[5], dataset[i].generosity);
        min_vals[6] = fmin(min_vals[6], dataset[i].corruption);
        min_vals[7] = fmin(min_vals[7], dataset[i].positive_affect);
        min_vals[8] = fmin(min_vals[8], dataset[i].negative_affect);
    }

    // Normalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].life_ladder = (dataset[i].life_ladder - min_vals[0]) / (max_vals[0] - min_vals[0]);
        dataset[i].log_gdp = (dataset[i].log_gdp - min_vals[1]) / (max_vals[1] - min_vals[1]);
        dataset[i].social_support = (dataset[i].social_support - min_vals[2]) / (max_vals[2] - min_vals[2]);
        dataset[i].life_expectancy = (dataset[i].life_expectancy - min_vals[3]) / (max_vals[3] - min_vals[3]);
        dataset[i].life_choices = (dataset[i].life_choices - min_vals[4]) / (max_vals[4] - min_vals[4]);
        dataset[i].generosity = (dataset[i].generosity - min_vals[5]) / (max_vals[5] - min_vals[5]);
        dataset[i].corruption = (dataset[i].corruption - min_vals[6]) / (max_vals[6] - min_vals[6]);
        dataset[i].positive_affect = (dataset[i].positive_affect - min_vals[7]) / (max_vals[7] - min_vals[7]);
        dataset[i].negative_affect = (dataset[i].negative_affect - min_vals[8]) / (max_vals[8] - min_vals[8]);
    }
}

// Function to calculate Euclidean distance between two records
float calculateDistance(struct Record record_1, struct Record record_2) {
    float sum = 0;
    // Assuming all attributes contribute to the distance calculation
    sum += pow(record_1.life_ladder - record_2.life_ladder, 2);
    sum += pow(record_1.log_gdp - record_2.log_gdp, 2);
    sum += pow(record_1.social_support - record_2.social_support, 2);
    sum += pow(record_1.life_expectancy - record_2.life_expectancy, 2);
    sum += pow(record_1.life_choices - record_2.life_choices, 2);
    sum += pow(record_1.generosity - record_2.generosity, 2);
    sum += pow(record_1.corruption - record_2.corruption, 2);
    sum += pow(record_1.positive_affect - record_2.positive_affect, 2);
    sum += pow(record_1.negative_affect - record_2.negative_affect, 2);
    return sqrt(sum);
}

// Function to check if a value is missing
int isMissingValue(float value) {
    return isnan(value);
}

// Function to check for missing values in a record
int hasMissingValues(struct Record record) {
    return (isMissingValue(record.log_gdp) || isMissingValue(record.social_support) || isMissingValue(record.life_expectancy) || isMissingValue(record.life_choices) || isMissingValue(record.generosity) || isMissingValue(record.corruption) || isMissingValue(record.positive_affect) || isMissingValue(record.negative_affect));
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
                imputed_values[0] += dataset[neighbour_index].log_gdp;
                imputed_values[1] += dataset[neighbour_index].social_support;
                imputed_values[2] += dataset[neighbour_index].life_expectancy;
                imputed_values[3] += dataset[neighbour_index].life_choices;
                imputed_values[4] += dataset[neighbour_index].generosity;
                imputed_values[5] += dataset[neighbour_index].corruption;
                imputed_values[6] += dataset[neighbour_index].positive_affect;
                imputed_values[7] += dataset[neighbour_index].negative_affect;
            }

            // Averaging the values to impute the missing values
            for (int j = 0; j < MISSING; ++j) {
                imputed_values[j] /= k;
            }

            // Updating the current record with imputed values
            if (isMissingValue(dataset[i].log_gdp)) dataset[i].log_gdp = imputed_values[0];
            if (isMissingValue(dataset[i].social_support)) dataset[i].social_support = imputed_values[1];
            if (isMissingValue(dataset[i].life_expectancy)) dataset[i].life_expectancy = imputed_values[2];
            if (isMissingValue(dataset[i].life_choices)) dataset[i].life_choices = imputed_values[3];
            if (isMissingValue(dataset[i].generosity)) dataset[i].generosity = imputed_values[4];
            if (isMissingValue(dataset[i].corruption)) dataset[i].corruption = imputed_values[5];
            if (isMissingValue(dataset[i].positive_affect)) dataset[i].positive_affect = imputed_values[6];
            if (isMissingValue(dataset[i].negative_affect)) dataset[i].negative_affect = imputed_values[7];

        }
    }
}

// Function to denormalise values
void denormalise(struct Record dataset[], float max_vals[], float min_vals[]) {
    // Denormalising each feature
    for (int i = 0; i < RECORDS; ++i) {
        dataset[i].life_ladder = dataset[i].life_ladder * (max_vals[0] - min_vals[0]) + min_vals[0];
        dataset[i].log_gdp = dataset[i].log_gdp * (max_vals[1] - min_vals[1]) + min_vals[1];
        dataset[i].social_support = dataset[i].social_support * (max_vals[2] - min_vals[2]) + min_vals[2];
        dataset[i].life_expectancy = dataset[i].life_expectancy * (max_vals[3] - min_vals[3]) + min_vals[3];
        dataset[i].life_choices = dataset[i].life_choices * (max_vals[4] - min_vals[4]) + min_vals[4];
        dataset[i].generosity = dataset[i].generosity * (max_vals[5] - min_vals[5]) + min_vals[5];
        dataset[i].corruption = dataset[i].corruption * (max_vals[6] - min_vals[6]) + min_vals[6];
        dataset[i].positive_affect = dataset[i].positive_affect * (max_vals[7] - min_vals[7]) + min_vals[7];
        dataset[i].negative_affect = dataset[i].negative_affect * (max_vals[8] - min_vals[8]) + min_vals[8];
    }
}

// Function to write data from dataset into CSV file
void writeCSV(const char *filename, struct Record dataset[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "year,life_ladder,log_gdp,social_support,life_expectancy,life_choices,generosity,corruption,positive_affect,negative_affect\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", dataset[i].year, dataset[i].life_ladder, dataset[i].log_gdp, dataset[i].social_support, dataset[i].life_expectancy, dataset[i].life_choices, dataset[i].generosity, dataset[i].corruption, dataset[i].positive_affect, dataset[i].negative_affect);
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
    readCSV("world_happiness_report.csv", dataset);
    
    // Normalising data
    normalise(dataset, max_vals, min_vals);

    // Choosing distance metric (Euclidean distance)

    // Determining the number of neighbours (K)
    int k_neighbours = 44;

    // Perform k-NN imputation
    knnImputation(dataset, k_neighbours);

    // Denormalising data
    denormalise(dataset, max_vals, min_vals);

    // Write imputed data to CSV file
    writeCSV("imputed_world_happiness_report_knn.csv", dataset);

    return 0;
}