//write a code that transposes a matrix given by the user

#include <stdio.h>


void transposed(int rows, int columns, int matrix[rows][columns]){
    int transpost[columns][rows];

    for(int i = 0; i < rows; i++){
      for(int j = 0; j < columns; j++){
        transpost[j][i] = matrix[i][j];
      }
    }

    for(int i = 0; i < columns; i++){
      for(int j = 0; j < rows; j++){
      printf("%d", transpost[i][j]);
      }
      printf("\n");
    }
}

int main() {
    int rows, columns;

    scanf("%d %d", &rows, &columns);

    int matrix[rows][columns];

    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            scanf(" %d", &matrix[i][j]);
        }
    }

    transposed(rows, columns, matrix); 
}
