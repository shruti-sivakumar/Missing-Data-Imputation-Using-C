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
    float life_ladder;
    float log_gdp_per_capita;
    float social_support;
    float healthy_life_expectancy;
    float freedom;
    float generosity;
    float corruption;
    float positive_affect;
    float negative_affect;
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
        fscanf(file, "%f,%f,%f,%f,%f,%f,%f,%f,%f\n", &dataset[i].life_ladder, &dataset[i].log_gdp_per_capita, &dataset[i].social_support,
               &dataset[i].healthy_life_expectancy, &dataset[i].freedom, &dataset[i].generosity, &dataset[i].corruption, &dataset[i].positive_affect,
                &dataset[i].negative_affect);
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
    
    fprintf(file, "life_ladder,log_gdp_per_capita,social_support,healthy_life_on_birth,freedom,generosity,perception_of_corroption,positive_affect,negative_affect\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%f,%f,%f,%f,%f,%f,%f,%f,%f\n", dataset[i].life_ladder, dataset[i].log_gdp_per_capita, dataset[i].social_support, dataset[i].healthy_life_expectancy, 
        dataset[i].freedom, dataset[i].generosity, dataset[i].corruption, dataset[i].positive_affect, dataset[i].negative_affect);
    }

    fclose(file);
}

void main(){

    struct Original_Record dataset[RECORDS];
    readCSV("world_happiness_report.csv", dataset);

    /*for (int i = 0; i < 1950; i++) {
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",dataset[i].life_ladder, dataset[i].log_gdp_per_capita, dataset[i].social_support, dataset[i].healthy_life_expectancy, 
        dataset[i].freedom, dataset[i].generosity, dataset[i].corruption, dataset[i].positive_affect, dataset[i].negative_affect);
    }*/

    for(int i=0;i<RECORDS;i++){
        int preference[RECORDS];
        //impute only for records with missin cvalues
        if(dataset[i].life_ladder==0|| dataset[i].log_gdp_per_capita==0|| dataset[i].social_support==0|| dataset[i].healthy_life_expectancy==0|| 
        dataset[i].freedom==0|| dataset[i].generosity==0|| dataset[i].corruption==0|| dataset[i].positive_affect==0|| dataset[i].negative_affect==0){
            preference[i] = -1;
            for(int k=0;k<RECORDS;k++){
                if(k!=i){
                    preference[k] = 0;
                    if((dataset[i].life_ladder - dataset[k].life_ladder < 0.005) && (dataset[i].life_ladder - dataset[k].life_ladder > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].log_gdp_per_capita - dataset[k].log_gdp_per_capita < 0.005) &&(dataset[i].log_gdp_per_capita - dataset[i].log_gdp_per_capita > -0.005) )
                        preference[k]=preference[k]+1;
                    if((dataset[i].social_support - dataset[k].social_support < 0.005) &&(dataset[i].social_support - dataset[i].social_support > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].healthy_life_expectancy - dataset[k].healthy_life_expectancy < 0.005) &&(dataset[i].healthy_life_expectancy - dataset[i].healthy_life_expectancy > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].freedom - dataset[k].freedom < 0.005) &&(dataset[i].freedom - dataset[i].freedom > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].generosity - dataset[k].generosity < 0.005) &&(dataset[i].generosity - dataset[i].generosity > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].corruption - dataset[k].corruption < 0.005) &&(dataset[i].corruption - dataset[i].corruption > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].positive_affect - dataset[k].positive_affect < 0.005) &&(dataset[i].positive_affect - dataset[i].positive_affect > -0.005))
                        preference[k]=preference[k]+1;
                    if((dataset[i].negative_affect - dataset[k].negative_affect < 0.005) &&(dataset[i].negative_affect - dataset[i].negative_affect > -0.005))
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
            if(dataset[i].life_ladder==0)
                dataset[i].life_ladder = dataset[donor_row].life_ladder;
            if(dataset[i].log_gdp_per_capita==0)
                dataset[i].log_gdp_per_capita = dataset[donor_row].log_gdp_per_capita;
            if(dataset[i].social_support==0)
                dataset[i].social_support = dataset[donor_row].social_support;
            if(dataset[i].healthy_life_expectancy==0)
                dataset[i].healthy_life_expectancy = dataset[donor_row].healthy_life_expectancy;
            if(dataset[i].freedom==0)
                dataset[i].freedom = dataset[donor_row].freedom;
            if(dataset[i].generosity==0)
                dataset[i].generosity = dataset[donor_row].generosity;
            if(dataset[i].corruption==0)
                dataset[i].corruption = dataset[donor_row].corruption;
            if(dataset[i].positive_affect==0)
                dataset[i].positive_affect = dataset[donor_row].positive_affect;
            if(dataset[i].negative_affect==0)
                dataset[i].negative_affect = dataset[donor_row].negative_affect;
        }   
    }

    writeCSV("imputed_world_happiness_report_hotdeck.csv", dataset);

}