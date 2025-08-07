// Including header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> 

// Defining constants - dataset size
#define ATTRIBUTES 8
#define RECORDS 13589


// Create structures
struct Original_Record {
    float distance;
    int postcode;
    int bedroom;
    int bathroom;
    int car;
    int landsize;
    int Building_area;
    int yearbuilt;
};

// Function to read data from CSV file into the dataset
void readCSV(const char *filename, struct Original_Record *dataset) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Skipping the header row
    fscanf(file, "%*[^\n]\n");

    for (int i = 0; i < RECORDS; ++i) {
        fscanf(file, "%f,%d,%d,%d,%d,%d,%d,%d\n", &dataset[i].distance, &dataset[i].postcode, &dataset[i].bedroom,
               &dataset[i].bathroom, &dataset[i].car, &dataset[i].landsize, &dataset[i].Building_area, &dataset[i].yearbuilt);
    }

    fclose(file);
}

// Function to write data from dataset into CSV file
void writeCSV(const char *filename, struct Original_Record dataset[]) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "distance,postcode,bedroom,bathroom,car,landsize,building area,year built\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%f,%d,%d,%d,%d,%d,%d,%d\n", dataset[i].distance, dataset[i].postcode, dataset[i].bedroom,
               dataset[i].bathroom, dataset[i].car, dataset[i].landsize, dataset[i].Building_area, dataset[i].yearbuilt);
    }

    fclose(file);
}

void main(){

    struct Original_Record dataset[RECORDS];
    readCSV("melb_data.csv", dataset);

    for(int i=0;i<RECORDS;i++){
        int preference[RECORDS];
        //impute only for records with missin cvalues
        if(dataset[i].distance==0|| dataset[i].postcode==0|| dataset[i].bedroom==0|| dataset[i].bathroom==0|| 
        dataset[i].car==0|| dataset[i].landsize==0|| dataset[i].Building_area==0|| dataset[i].yearbuilt==0){
            preference[i] = -1;
            for(int k=0;k<RECORDS;k++){
                if(k!=i){
                    preference[k] = 0;
                    if(dataset[i].distance == dataset[k].distance)
                        preference[k]=preference[k]+1;
                    if(dataset[i].postcode == dataset[k].postcode)
                        preference[k]=preference[k]+1;
                    if(dataset[i].bedroom == dataset[k].bedroom)
                        preference[k]=preference[k]+1;
                    if(dataset[i].bathroom == dataset[k].bathroom)
                        preference[k]=preference[k]+1;
                    if(dataset[i].car == dataset[k].car)
                        preference[k]=preference[k]+1;
                    if(dataset[i].landsize == dataset[k].landsize)
                        preference[k]=preference[k]+1;
                    if((dataset[i].Building_area - dataset[k].Building_area < 0.005) &&(dataset[i].Building_area - dataset[i].Building_area > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].yearbuilt - dataset[k].yearbuilt < 0.005) &&(dataset[i].yearbuilt - dataset[i].yearbuilt > -0.005))
                        preference[k]=preference[k]+1;
                }
            }
            //finding highest preferene alloted to  row
            int max_pref = 0;
            for(int k=0;k<RECORDS;k++){
                if (preference[k] > max_pref){
                    max_pref = preference[k];
                }
            }

            //make an array with only the index of the higheest preference rows
             int max_pref_arr[RECORDS];
            int no_of_pref = 0;
            for(int k=0;k<RECORDS;k++){
                if(preference[k]==max_pref){
                    max_pref_arr[no_of_pref] = k;
                    no_of_pref++;
                }
            }

            int donor_index = rand() % no_of_pref;
            int donor_row = max_pref_arr[donor_index];

            //put all the missing values in from the donor row to the row target row
            if(dataset[i].distance==0)
                dataset[i].distance = dataset[donor_row].distance;
            if(dataset[i].postcode==0)
                dataset[i].postcode = dataset[donor_row].postcode;
            if(dataset[i].bedroom==0)
                dataset[i].bedroom = dataset[donor_row].bedroom;
            if(dataset[i].bathroom==0)
                dataset[i].bathroom = dataset[donor_row].bathroom;
            if(dataset[i].car==0)
                dataset[i].car = dataset[donor_row].car;
            if(dataset[i].landsize==0)
                dataset[i].landsize = dataset[donor_row].landsize;
            if(dataset[i].Building_area==0)
                dataset[i].Building_area = dataset[donor_row].Building_area;
            if(dataset[i].yearbuilt==0)
                dataset[i].yearbuilt = dataset[donor_row].yearbuilt;
        }   
    }

    writeCSV("imputed_melb_data_hotdeck.csv", dataset);
}