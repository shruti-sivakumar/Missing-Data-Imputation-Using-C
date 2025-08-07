# Missing Data Imputation Using C

This project represents my **first university project**, developed collaboratively to explore classic missing data imputation techniques using C. The goal was to implement and evaluate five methods on three real-world datasets, with each method coded from scratch and adapted to the specific structure and missingness patterns of each dataset.

Missing data imputation is a critical preprocessing step in any data pipeline. This project systematically compares several popular techniques to highlight their strengths, limitations, and contextual suitability.

---

## Techniques Implemented

Each imputation strategy was implemented from scratch in C, including logic tailored to each dataset:

| Method | Description |
|--------|-------------|
| **Linear Regression Imputation** | Builds a regression model to predict and impute missing values using correlated variables. |
| **k-Nearest Neighbors (k-NN)** | Uses similarity-based voting among nearest neighbors to estimate missing entries. |
| **Mean/Median Imputation** | Fills missing values using column-level statistical estimates (mean or median). |
| **Listwise Deletion** | Removes entire rows with missing entries — easy but causes potential information loss. |
| **Hot Deck Imputation** | Imputes missing values using similar “donor” observations from the same dataset. |

---

## Datasets Used

| Dataset | Description | Size | Key Features with Missing Data |
|--------|-------------|------|--------------------------------|
| **Pima Indian Diabetes** | Medical dataset of diagnostic measures and outcomes | 768 rows × 9 columns | Plasma Glucose, BMI, Blood Pressure |
| **World Happiness Report** | Global socio-economic indicators of happiness | 1,949 rows × 11 columns | Perceptions of Corruption |
| **Melbourne Housing** | Real estate listings from 2016–17 | 13,580 rows × 21 columns | Land Size, Building Area |

---

## Sample Evaluation Summary

| Method | Pima Indians | World Happiness | Melbourne Housing |
|--------|--------------|------------------|-------------------|
| Linear Regression | ✅ Excellent | ✅ Excellent | ✅ Excellent |
| KNN | ✅ Good | ✅ Excellent | ⚠️ Poor |
| Mean/Median | ✅ Accurate but repetitive | ✅ Consistent | ⚠️ Limited uniqueness |
| Listwise Deletion | ⚠️ Significant data loss | ✅ Effective | ✅ High performance |
| Hot Deck | ⚠️ 2.4% missing left | ✅ Excellent | ✅ Good |

> See `/analysis` for full breakdowns.

---

## Repository Structure

```
├── data/
│   ├── raw/            # Original datasets
│   └── imputed/        # Post-imputation outputs by method
├── src/
│   ├── linear_regression/
│   ├── knn/
│   ├── mean_median/
│   ├── listwise_deletion/
│   └── hot_deck/
├── analysis/           # Result interpretations per method
├── LICENSE
└── README.md
```

---

## Key Learnings

- Linear Regression and KNN provide robust, context-aware imputation when feature correlation is strong.
- Mean/Median is effective in reducing missingness but may oversimplify distributions.
- Listwise Deletion, while easy to implement, often discards valuable data and should be used cautiously.
- Hot Deck's performance varies widely depending on dataset density and feature spread.

---

## Contributors

This project was collaboratively developed as a foundational exploration into data preprocessing and algorithmic reasoning using C:

- **Shruti Sivakumar**
- Harshitha Chandrasekar
- Shreya Sriram
- Vida Nadheera
- Manvitha Payyavula

---

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more information.