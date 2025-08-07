#include <stdio.h>
#include <math.h>

#define RECORDS 3
#define ATTRIBUTES 5

void linearRegressionImputation(float copy_dataset[][ATTRIBUTES], int num_records) {
    // Iterate through each record
    for (int i = 0; i < num_records; ++i) {
        // Iterate through each attribute
        for (int attr = 0; attr < ATTRIBUTES; ++attr) {
            // Check if the attribute has a missing value
            if (copy_dataset[i][attr] == 0.0) {
                // Perform linear regression using other non-missing attribute
                float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
                int count = 0;

                // Calculate sums for linear regression
                for (int j = 0; j < num_records; ++j) {
                    if (copy_dataset[j][attr] != 0.0) {
                        sum_x += copy_dataset[j][(attr + 1) % ATTRIBUTES]; // Use the next attribute in order
                        sum_y += copy_dataset[j][attr];
                        sum_xy += copy_dataset[j][(attr + 1) % ATTRIBUTES] * copy_dataset[j][attr];
                        sum_x2 += copy_dataset[j][(attr + 1) % ATTRIBUTES] * copy_dataset[j][(attr + 1) % ATTRIBUTES];
                        count++;
                    }
                }

                // Avoid division by zero and handle the case when count is zero
                if (count > 0 && sum_x2 - sum_x * sum_x / count != 0.0) {
                    // Calculate linear regression coefficients
                    float slope = (count * sum_xy - sum_x * sum_y) / (count * sum_x2 - sum_x * sum_x);
                    float intercept = (sum_y - slope * sum_x) / count;

                    // Impute missing value using linear regression equation: y = mx + b
                    copy_dataset[i][attr] = slope * copy_dataset[i][(attr + 1) % ATTRIBUTES] + intercept;
                }
            }
        }
    }
}

int main() {
    // Example 3x5 array with one missing value
    float example_2D_array[RECORDS][ATTRIBUTES] = {
        {1.0, 2.0, 3.0, 4.0, 5.0},
        {0.0, 2.0, 3.0, 4.0, 5.0},  // Missing value represented as 0.0
        {1.0, 0.0, 3.0, 4.0, 5.0}
    };

    printf("Original 2D Array:\n");
    for (int i = 0; i < RECORDS; ++i) {
        for (int j = 0; j < ATTRIBUTES; ++j) {
            printf("%f ", example_2D_array[i][j]);
        }
        printf("\n");
    }

    // Call the linear regression imputation function
    linearRegressionImputation(example_2D_array, RECORDS);

    printf("\n2D Array after Linear Regression Imputation:\n");
    for (int i = 0; i < RECORDS; ++i) {
        for (int j = 0; j < ATTRIBUTES; ++j) {
            printf("%f ", example_2D_array[i][j]);
        }
        printf("\n");
    }

    return 0;
}