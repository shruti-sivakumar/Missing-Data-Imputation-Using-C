#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define ATTRIBUTES 11
#define RECORDS 1950

struct Original_Record {
    int year;
    float LifeLadder;
    float GDP;
    float Price;
    float socialsupport;
    float healthylifestyle;
    float Freedom;
    float Generosity;
    float corruption;
    float Positiveaffect;
    float Negativeaffect;
};

struct Copy_Record {
    int year;
    float LifeLadder;
    float GDP;
    float Price;
    float socialsupport;
    float healthylifestyle;
    float Freedom;
    float Generosity;
    float corruption;
    float Positiveaffect;
    float Negativeaffect;
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
        fscanf(file, "%*[^,],%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
               &dataset[i].year,&dataset[i].LifeLadder,&dataset[i].GDP,&dataset[i].Price,&dataset[i].socialsupport,&dataset[i].healthylifestyle,&dataset[i].Freedom,&dataset[i].Generosity,&dataset[i].corruption,&dataset[i].Positiveaffect,&dataset[i].Negativeaffect);
    }

    fclose(file);
}
void copyStruct(struct Original_Record dataset[], struct Copy_Record copy_dataset[]){
        for (int i = 0; i < RECORDS; ++i) {
        // Copy each field from Original_Record to Copy_Record
        copy_dataset[i].year = dataset[i].year;
        copy_dataset[i].LifeLadder = dataset[i].LifeLadder;
        copy_dataset[i].GDP = dataset[i].GDP;
        copy_dataset[i].Price = dataset[i].Price;
        copy_dataset[i].socialsupport = dataset[i].socialsupport;
        copy_dataset[i].healthylifestyle= dataset[i].healthylifestyle;
        copy_dataset[i].Freedom = dataset[i].Freedom;
        copy_dataset[i].Generosity = dataset[i].Generosity;
        copy_dataset[i].corruption = dataset[i].corruption;
        copy_dataset[i].Positiveaffect = dataset[i].Positiveaffect;
        copy_dataset[i].Negativeaffect = dataset[i].Negativeaffect;
    }
}
void linearregressionImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].socialsupport== 0.0) {
            float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].socialsupport!= 0.0) {
                    sum_x += copy_dataset[j].LifeLadder;
                    sum_y += copy_dataset[j].socialsupport;
                    sum_xy += copy_dataset[j].LifeLadder * copy_dataset[j].socialsupport;
                    sum_x2 += copy_dataset[j].LifeLadder * copy_dataset[j].LifeLadder;
                    count++;
                }
            }

            // Check if all values for the attribute used in regression are the same
            if (count > 1) {
                float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                float intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].socialsupport = slope * copy_dataset[i].LifeLadder + intercept;
            } else {
                // Handle the case where all values for the attribute are the same
                copy_dataset[i].socialsupport = copy_dataset[i].LifeLadder;
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
    
    fprintf(file, "year,LifeLadder,GDP,Price,socialsupport,healthylifestyle,Freedom,Generosity,corruption,Positiveaffect,Negativeaffect\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",dataset[i].year,dataset[i].LifeLadder,dataset[i].GDP,dataset[i].Price,dataset[i].socialsupport, dataset[i].healthylifestyle,dataset[i].Freedom,dataset[i].Generosity,dataset[i].corruption,dataset[i].Positiveaffect,dataset[i].Negativeaffect);
    }

    fclose(file);
}
int main() {
    struct Original_Record dataset[RECORDS];
    struct Copy_Record copy_dataset[RECORDS];

    // Read data from CSV file
    readCSV("world_happiness_report.csv", dataset);

    // Copy Original_Record to Copy_Record
    copyStruct(dataset, copy_dataset);

    // Perform linear regression imputation
    linearregressionImputation(copy_dataset);

    // Write updated dataset to a new CSV file
    writeCSV("imputed_world_happiness_report_lr.csv", dataset, copy_dataset);

    return 0;
}
