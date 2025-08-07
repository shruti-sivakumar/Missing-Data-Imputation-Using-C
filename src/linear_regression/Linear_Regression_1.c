#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define ATTRIBUTES 9
#define RECORDS 768

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

// Function to read data from CSV file into the dataset
void readCSV(const char *filename, struct Original_Record *dataset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Skip the header row
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
        // Copy each field from Original_Record to Copy_Record
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

void blood_pressureImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].blood_pressure == 0.0) {
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].blood_pressure != 0.0) {
                    sum_x += copy_dataset[j].pregnancies;
                    sum_y += copy_dataset[j].blood_pressure;
                    sum_xy += copy_dataset[j].pregnancies * copy_dataset[j].blood_pressure;
                    sum_x2 += copy_dataset[j].pregnancies * copy_dataset[j].pregnancies;
                    count++;
                }
            }

            // Check if all values for the attribute used in regression are the same
            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].blood_pressure = slope * copy_dataset[i].pregnancies + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].blood_pressure = copy_dataset[i].pregnancies;
            }
        }
    }
}
void insulinImputation(struct Copy_Record copy_dataset[]) {
    // Identify missing values (assuming missing values are 0)
    for (int i = 0; i < RECORDS; ++i) {
        // Check if the attribute has a missing value
        if (copy_dataset[i].insulin == 0.0) {
            // Perform linear regression using other non-missing attributes
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            // Calculate sums for linear regression
            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].insulin != 0.0) {
                    sum_x += copy_dataset[j].pregnancies; // Use appropriate attribute
                    sum_y += copy_dataset[j].insulin;
                    sum_xy += copy_dataset[j].pregnancies * copy_dataset[j].insulin;
                    sum_x2 += copy_dataset[j].pregnancies* copy_dataset[j].pregnancies;
                    count++;
                }
            }

            // Check if all values for the attribute used in regression are the same
            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].insulin = slope * copy_dataset[i].pregnancies + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].insulin = copy_dataset[i].pregnancies;
            }
        }
    }
}
void bmiImputation(struct Copy_Record copy_dataset[]) {
    // Identify missing values (assuming missing values are 0)
    for (int i = 0; i < RECORDS; ++i) {
        // Check if the attribute has a missing value
        if (copy_dataset[i].bmi == 0.0) {
            // Perform linear regression using other non-missing attributes
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            // Calculate sums for linear regression
            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].bmi!= 0.0) {
                    sum_x += copy_dataset[j].pregnancies; // Use appropriate attribute
                    sum_y += copy_dataset[j].bmi;
                    sum_xy += copy_dataset[j].pregnancies* copy_dataset[j].bmi;
                    sum_x2 += copy_dataset[j].pregnancies* copy_dataset[j].pregnancies;
                    count++;
                }
            }

            // Check if all values for the attribute used in regression are the same
            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].bmi= slope * copy_dataset[i].pregnancies + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].bmi = copy_dataset[i].pregnancies;
            }
        }
    }
}
void skin_thicknessImputation(struct Copy_Record copy_dataset[]) {
    // Identify missing values (assuming missing values are 0)
    for (int i = 0; i < RECORDS; ++i) {
        // Check if the attribute has a missing value
        if (copy_dataset[i].skin_thickness== 0.0) {
            // Perform linear regression using other non-missing attributes
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            // Calculate sums for linear regression
            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].skin_thickness!= 0.0) {
                    sum_x += copy_dataset[j].pregnancies; // Use appropriate attribute
                    sum_y += copy_dataset[j].skin_thickness;
                    sum_xy += copy_dataset[j].pregnancies* copy_dataset[j].skin_thickness;
                    sum_x2 += copy_dataset[j].pregnancies* copy_dataset[j].pregnancies;
                    count++;
                }
            }

            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].skin_thickness= slope * copy_dataset[i].pregnancies + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].skin_thickness = copy_dataset[i].pregnancies;
            }
        }
    }
}
void glucoseImputation(struct Copy_Record copy_dataset[]) {
    // Identify missing values (assuming missing values are 0)
    for (int i = 0; i < RECORDS; ++i) {
        // Check if the attribute has a missing value
        if (copy_dataset[i].glucose== 0.0) {
            // Perform linear regression using other non-missing attributes
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            // Calculate sums for linear regression
            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].glucose!= 0.0) {
                    sum_x += copy_dataset[j].pregnancies; // Use appropriate attribute
                    sum_y += copy_dataset[j].glucose;
                    sum_xy += copy_dataset[j].pregnancies * copy_dataset[j].glucose;
                    sum_x2 += copy_dataset[j].pregnancies* copy_dataset[j].pregnancies;
                    count++;
                }
            }

            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].glucose= slope * copy_dataset[i].pregnancies + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].glucose = copy_dataset[i].pregnancies;
            }
        }
    }
}

void writeCSV(const char *filename, struct Original_Record dataset[], struct Copy_Record copy_dataset[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "pregnancies,glucose,blood_pressure,skin_thickness,insulin,bmi,pedigree,age,class_variable\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%d,%f,%f,%f,%f,%f,%f,%d,%d\n", dataset[i].pregnancies, copy_dataset[i].glucose, copy_dataset[i].blood_pressure, copy_dataset[i].skin_thickness, copy_dataset[i].insulin, copy_dataset[i].bmi, copy_dataset[i].pedigree, dataset[i].age, copy_dataset[i].class_variable);
    }

    fclose(file);
}

int main() {
    struct Original_Record dataset[RECORDS];
    struct Copy_Record copy_dataset[RECORDS];

    // Read data from CSV file
    readCSV("pi_diabetes.csv", dataset);

    // Copy Original_Record to Copy_Record
    copyStruct(dataset, copy_dataset);

    // Perform linear regression imputation
    blood_pressureImputation(copy_dataset);
    insulinImputation(copy_dataset);
    bmiImputation(copy_dataset);
    skin_thicknessImputation(copy_dataset);
    glucoseImputation(copy_dataset);

    // Write updated dataset to a new CSV file
    writeCSV("imputed_pi_diabetes_lr.csv", dataset, copy_dataset);

    return 0;
}