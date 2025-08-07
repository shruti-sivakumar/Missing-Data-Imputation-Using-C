#include <stdio.h>

void main(){
    int arr[10][3]={ 
        {3, 4, 6},
        {9, 0, 2},
        {3, 4, 1},
        {8, 3, 1},
        {0, 5, 1},
        {0, 3, 2},
        {1, 4, 0},
        {3, 5, 7},
        {2, 4, 0},
        {4, 0, 1}
    };

    //number of rows
    int rows = sizeof(arr) / sizeof(arr[0]);
    int cols = sizeof(arr[0]) / sizeof(arr[0][0]);

    printf("array before imputation\n");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d\t",arr[i][j]);
        }
        printf("\n");
    }

    for(int i=0;i<rows;i++){
        
        //check if row i has missing values
        int missing_values = 0;
        for(int j=0;j<cols;j++){
            if(arr[i][j]==0)
                missing_values = 1;    
        }
        //execute this part only if there are missing values in row i
        if(missing_values == 1){
             
        //we now need to decide which row to take donor variablws from for imputation
            // set preferences values for all the other rows.
            int preferences[rows];
            preferences[i] = -1;
            for(int k=0;k<rows;k++){
                if(k!=i){
                    preferences[k]=0;
                    for(int l=0;l<cols;l++){
                        if((arr[k][l]==arr[i][l])&&(arr[k][l]!=0))
                            preferences[k] = preferences[k]+1;
                    }
                }
            }


            //find the highest preference alloted to a row
            int max_pref = 0;
            for(int k=0;k<rows;k++){
                if (preferences[k] > max_pref){
                    max_pref = preferences[k];
                }
            }

            //make a array with only the indexs of the highest preference rows
            int max_pref_arr[rows];
            int no_of_pref = 0;
            for(int k=0;k<rows;k++){
                if(preferences[k]==max_pref){
                    max_pref_arr[no_of_pref] = k;
                    no_of_pref++;
                }
            }
            

            int donor_index = rand() % no_of_pref;
            int donor_row = max_pref_arr[donor_index];

            //putting all the missing values from the donor row to the row with missing indix

            for(int l=0;l<cols;l++){
                if(arr[i][l]==0){
                    int donor_values = arr[donor_row][l];
                    arr[i][l] = donor_values;

                }
            }
            
        }

        missing_values = 0;

    }
    printf("array after imputation\n");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d\t",arr[i][j]);
        }
        printf("\n");
    }
}