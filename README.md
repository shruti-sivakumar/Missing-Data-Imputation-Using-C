# Missing Data Imputation Using C

This repository contains the first semester project for the course "22AIE101: Problem Solving & C Programming." The project focuses on addressing the critical issue of missing data imputation using C programming across three diverse datasets. Five imputation methods—Linear Regression, K-nearest Neighbours, Mean and Median Imputation, List-wise Deletion, and Hot Deck Imputation—are employed to comprehensively evaluate their strengths and limitations. Each team member focused on the implementation of one of the methods, with the logic code tailored for each dataset. Through systematic assessment, the aim is to identify the best-performing imputation method for each dataset.

## Methods Used

### 1. Linear Regression Imputation
  - Utilizes linear regression to estimate missing values.
  - Treats the variable with missing data as the dependent variable.
  - Constructs a regression model using other relevant variables as predictors.
  - Provides a statistically grounded approach with the incorporation of multiple predictors.

### 2. KNN Imputation
  - Non-parametric technique using values from nearest neighbours.
  - Calculates separation between observation with missing values and its neighbours.
  - Averages or weights values of nearby neighbours for imputation.
  - Requires careful selection of parameters like number of neighbours (k) and distance metric.

### 3. Mean and Median Imputation
  - Replaces missing values with mean or median of observed values for the variable.
  - Simple and easy to implement.
  - Assumes missing values occur entirely at random.
  - May introduce bias and doesn't account for changes in data distribution.

### 4. Listwise Deletion
  - Involves removing cases with any missing values from analysis.
  - Not an imputation method but leads to data loss.
  - Ease of analysis comes at potential cost of excluding valuable information.
  - Can introduce bias if missing data is linked to outcome variable or critical factors.

### 5. Hot Deck Imputation
  - Imputes missing entries by comparing features of observations.
  - Relies on predetermined criteria to identify suitable donor observations.
  - Fills missing values using values from identified donors.
  - Leverages characteristics of comparable observations to preserve coherence and context of the dataset.

## Datasets Used

### 1. Pima Indians
- **Description**: This dataset includes 768 occurrences and 9 attributes, relating to the medical data of a group of people called Pima Indians. The 'class variable' attribute indicates whether the patient demonstrates traits of interest for each instance, and it is binary.
- **Missing Data Patterns**: The dataset was purposefully created to teach imputation techniques, with a larger percentage of missing data. 
- **Features**: Numerical values representing various facets of the patients’ medical problems are included, such as the number of pregnancies, plasma glucose concentration, diastolic blood pressure, and more.
  
### 2. World Happiness Report
- **Description**: This dataset offers an extensive examination of happiness worldwide, with 1,949 occurrences and 11 characteristics.
- **Missing Data Patterns**: Most missing values are concentrated in the attribute "Perceptions of Corruption."
- **Features**: The dataset examines factors affecting happiness, including economic productivity, social support, life expectancy, and more.

### 3. Melbourne Housing Data
- **Description**: This dataset contains 13,580 cases with 21 variables, providing insights into Melbourne housing patterns in 2016 and 2017.
- **Missing Data Patterns**: Notably, 'Land Size' binaries missing values, and a substantial fraction of data have missing values for the 'Building Area' characteristic.
- **Features**: Various features like 'Rooms,' 'Price,' 'Distance,' 'Bedroom2,' 'Bathroom,' and more, offer a comprehensive understanding of Melbourne's housing scene.

## Purpose
The purpose of this project is to explore and compare different missing data imputation techniques using C programming, with the ultimate goal of finding the most effective method for each dataset.

## Implementation
The project consists of C programs implementing each imputation technique across the three datasets. Each program includes comments for clarity and understanding.

## Method-Specific Evaluation

### Linear Regression
- Perfect results across all datasets, reducing the percentage of missing data to zero while maintaining data quality.

### KNN
- Effective for the World Happiness Index dataset, reducing missing data to 0. Quantity of imputation is lower compared to Linear Regression.
- Good imputation quality for the Pima Indians dataset but not sufficient quantity.
- Poor imputation quality for the Melbourne Housing Data.

### Mean & Median Imputation
- Works perfectly for the Pima Indian and World Happiness Index datasets, reducing missing values to 0.0.
- Performance in other datasets is comparable to other methods, but lacks uniqueness in imputed values.

### Listwise Deletion
- Reduces all missing data percentages to 0, including string values.
- Less preferred due to loss of data, best suited for datasets with very few instances of missing data.

### Hot Deck
- Yielded perfect results only for the World Happiness Index dataset.
- Poor imputation quality for Pima Indians, with 2.4% of data still missing.
- Good results for the Melbourne Housing Data.

## Dataset Analysis

### Dataset 1 - Pima Indians
- Linear Regression and KNN are most effective.
- Mean and Median methods reduce missing data to 0.0 but lack imputation quality.
- Listwise deletion results in the loss of valuable data.

### Dataset 2 - World Happiness Report
- All methods consistently yield optimal results by reducing missing data to zero.
- Dataset robustness accommodates different methodologies effectively.

### Dataset 3 - Melbourne Housing Data
- Listwise deletion delivers flawless results.
- Mean and Median imputation lacks uniqueness in imputed values.

## Conclusion
The investigation into various missing data imputation methods across diverse datasets has provided valuable insights into their strengths and limitations. Linear Regression and KNN stand out as robust methods, offering considerable imputation success. However, the choice of method should align with dataset characteristics and desired imputation quality. This study contributes to the discourse on missing data imputation, emphasizing the need for a nuanced approach tailored to dataset intricacies.

## Contributors
- Shruti Sivakumar
- Harshitha Chandrasekar
- Shreya Sriram
- Vida Nadheera
- Manvitha Payyavula

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
