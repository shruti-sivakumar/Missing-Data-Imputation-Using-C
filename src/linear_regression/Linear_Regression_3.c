#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define ATTRIBUTES 13
#define RECORDS 13581

struct Original_Record {
    int room;
    int Price;
    float Distance;
    int Postcode;
    int Bedroom2;
    int Bathroom;
    int car;
    int Landsize;
    int buildingarea;
    int yearbuilt;
    float latitude;
    float Longtitude;
    int propertycount;
};

struct Copy_Record {
    int room;
    int Price;
    float Distance;
    int Postcode;
    int Bedroom2;
    int Bathroom;
    int car;
    int Landsize;
    int buildingarea;
    int yearbuilt;
    float latitude;
    float Longtitude;
    int propertycount;
};

// Function to read data from CSV file into the dataset
void readCSV(struct Original_Record *dataset, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Skip the header row
    fscanf(file, "%*[^\n]\n");

    for (int i = 0; i < RECORDS; ++i) {
        fscanf(file, "%d,%d,%f,%d,%d,%d,%d,%d,%d,%d,%f,%f,%d\n",
               &dataset[i].room, &dataset[i].Price, &dataset[i].Distance,
               &dataset[i].Postcode, &dataset[i].Bedroom2, &dataset[i].Bathroom,
               &dataset[i].car, &dataset[i].Landsize, &dataset[i].buildingarea,
               &dataset[i].yearbuilt, &dataset[i].latitude, &dataset[i].Longtitude,
               &dataset[i].propertycount);
    }

    fclose(file);
}

// Function to write updated dataset to a new CSV file
void writeCSV(struct Original_Record dataset[], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Rooms,Price,Distance,Postcode,Bedroom2,Bathroom,Car,Landsize,BuildingArea,YearBuilt,Latitude,Longitude,Propertycount\n");

    for (int i = 0; i < RECORDS; ++i) {
        fprintf(file, "%d,%d,%f,%d,%d,%d,%d,%d,%d,%d,%f,%f,%d\n",
               dataset[i].room, dataset[i].Price, dataset[i].Distance,
               dataset[i].Postcode, dataset[i].Bedroom2, dataset[i].Bathroom,
               dataset[i].car, dataset[i].Landsize, dataset[i].buildingarea,
               dataset[i].yearbuilt, dataset[i].latitude, dataset[i].Longtitude,
               dataset[i].propertycount);
    }

    fclose(file);
}

void copyStruct(struct Original_Record dataset[], struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        copy_dataset[i].room = dataset[i].room ;
        copy_dataset[i].Price = dataset[i].Price;
        copy_dataset[i].Distance = dataset[i].Distance;
        copy_dataset[i].Postcode = dataset[i].Postcode;
        copy_dataset[i].Bedroom2 = dataset[i].Bedroom2;
        copy_dataset[i].Bathroom = dataset[i].Bathroom;
        copy_dataset[i].car = dataset[i].car;
        copy_dataset[i].Landsize = dataset[i].Landsize;
        copy_dataset[i].buildingarea = dataset[i].buildingarea;
        copy_dataset[i].yearbuilt = dataset[i].yearbuilt;
        copy_dataset[i].latitude = dataset[i].latitude;
        copy_dataset[i].Longtitude = dataset[i].Longtitude;
        copy_dataset[i].propertycount = dataset[i].propertycount;
    }
}
void LandsizeImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].Landsize == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].Landsize != 0) {
                    sum_x += copy_dataset[j].Postcode;
                    sum_y += copy_dataset[j].Landsize;
                    sum_xy += copy_dataset[j].Postcode * copy_dataset[j].Landsize;
                    sum_x2 += copy_dataset[j].Postcode * copy_dataset[j].Postcode;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].Landsize = slope * copy_dataset[i].Postcode + intercept;
            } else {
                copy_dataset[i].Landsize = 0;
            }
        }
    }
}

void BuildingAreaImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].buildingarea == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].buildingarea != 0 ) {
                    sum_x += copy_dataset[j].room;
                    sum_y += copy_dataset[j].buildingarea;
                    sum_xy += copy_dataset[j].room * copy_dataset[j].buildingarea;
                    sum_x2 += copy_dataset[j].room* copy_dataset[j].room;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].buildingarea = slope * copy_dataset[i].room + intercept;
            } else {
                copy_dataset[i].buildingarea = 0;
            }
        }
    }
}

void yearbuiltImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].yearbuilt == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].yearbuilt != 0) {
                    sum_x += copy_dataset[j].room;
                    sum_y += copy_dataset[j].yearbuilt;
                    sum_xy += copy_dataset[j].room* copy_dataset[j].yearbuilt;
                    sum_x2 += copy_dataset[j].room * copy_dataset[j].room;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].yearbuilt = slope * copy_dataset[i].room + intercept;
            } else {
                copy_dataset[i].yearbuilt = 0;
            }
        }
    }
}
void latitudeImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].latitude == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].latitude != 0) {
                    sum_x += copy_dataset[j].Postcode;
                    sum_y += copy_dataset[j].latitude;
                    sum_xy += copy_dataset[j].Postcode * copy_dataset[j].latitude;
                    sum_x2 += copy_dataset[j].Postcode * copy_dataset[j].Postcode;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].latitude = slope * copy_dataset[i].Postcode + intercept;
            } else {
                copy_dataset[i].latitude = 0;
            }
        }
    }
}
void LongtitudeImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].Longtitude== 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].Longtitude != 0) {
                    sum_x += copy_dataset[j].room;
                    sum_y += copy_dataset[j].Longtitude;
                    sum_xy += copy_dataset[j].room * copy_dataset[j].Longtitude;
                    sum_x2 += copy_dataset[j].room * copy_dataset[j].room;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].Longtitude = slope * copy_dataset[i].room + intercept;
            } else {
                copy_dataset[i].Longtitude = 0;
            }
        }
    }
}

void PostcodeImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].Postcode == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].Postcode != 0 ) {
                    sum_x += copy_dataset[j].room;
                    sum_y += copy_dataset[j].Postcode;
                    sum_xy += copy_dataset[j].room * copy_dataset[j].Postcode;
                    sum_x2 += copy_dataset[j].room* copy_dataset[j].room;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].Postcode = slope * copy_dataset[i].room + intercept;
            } else {
                copy_dataset[i].Postcode = 0;
            }
        }
    }
}
void propertycountImputation(struct Copy_Record copy_dataset[]) {
    for (int i = 0; i < RECORDS; ++i) {
        if (copy_dataset[i].propertycount == 0) {
            double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
            int count = 0;

            for (int j = 0; j < RECORDS; ++j) {
                if (copy_dataset[j].propertycount!= 0 ) {
                    sum_x += copy_dataset[j].room;
                    sum_y += copy_dataset[j].propertycount ;
                    sum_xy += copy_dataset[j].room * copy_dataset[j].propertycount ;
                    sum_x2 += copy_dataset[j].room* copy_dataset[j].room;
                    count++;
                }
            }

            if (count > 1) {
                double slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                double intercept = (sum_y - slope * sum_x) / count;
                copy_dataset[i].propertycount  = slope * copy_dataset[i].room + intercept;
            } else {
                copy_dataset[i].propertycount = 0;
            }
        }
    }
}

int main() {
    struct Original_Record dataset[RECORDS];
    struct Copy_Record copy_dataset[RECORDS];

    // Read data from CSV file
    readCSV(dataset, "melb_data.csv");

    // Copy Original_Record to Copy_Record
    copyStruct(dataset, copy_dataset);

    // Perform linear regression imputation
    LandsizeImputation(copy_dataset);
    BuildingAreaImputation(copy_dataset);
    yearbuiltImputation(copy_dataset);
    latitudeImputation(copy_dataset);
    LongtitudeImputation(copy_dataset);
    PostcodeImputation(copy_dataset);
    propertycountImputation(copy_dataset);
    // Write updated dataset to a new CSV file
    writeCSV(dataset, "imputed_melb_data_lr.csv");

    return 0;
}
