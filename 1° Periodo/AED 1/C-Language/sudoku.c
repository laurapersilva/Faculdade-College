/*
You will have to create a program that reads a 9x9 matrix of numbers from 1 to 9 and then perform the operation desired by the user until the value 0, indicating the end of the program, is presented. The options regarding the program’s functionality are:

1. Update index: Along with option 1, in the same line, the row index (between 0 to 8), the column index (0 to 8), and the new digit (1 to 9) that will replace the old digit in the indicated position.

2. Check line: Along with option 2, on the same line, the index of the line (between 0 to 8) to be checked will be displayed. If there is no repetition of numbers, the character 'N' must be printed; otherwise print 'Y'.

3. Check column: Along with option 3, on the same line, the column index (between 0 to 8) to be checked will be displayed. If there is no repetition of numbers, the character 'N' must be printed; otherwise print 'Y'.

4. Check quadrant: Along with option 4, on the same line, the index of the quadrant (0 to 8) to be checked will be displayed. If there is no repetition of numbers, the character 'N' must be printed; otherwise print 'Y'. The quadrants are listed as follows:

0 1 2
3 4 5
6 7 8

5. Check matrix: If there is no repetition of numbers, the character 'N' must be printed; otherwise print 'Y'

Any other value that is not in the range [0,5], it is assumed that the user wants to print the matrix in a formatted way, making clear the separation of the quadrants via spaces and blank lines.

RESTRICTIONS: You cannot use any concept not (yet) seen in the classroom
*/


#include <stdio.h>
int main()
{
    int matrix[9][9], op, ro=0, col=0, alg, cont=0, rep=0, quad, i, l, c, li, co;

    for(l=0; l<9; l++){
        for(c=0; c<9; c++){
            scanf("%d%*c",&matrix[l][c]);
        }
    }
    do{
        scanf("%d%*c",&op);
        switch(op){

        case 0:
            /*ends the program*/
            break;
        case 1:
            /*will replace the old digit matrix in the indicated position*/

            scanf("%d%*c",&ro); /*chooses rows from 0 to 8*/
            scanf("%d%*c",&col); /*chooses columns from 1 to 9*/
            scanf("%d%*c",&alg); /*chooses the new number from 1 to 9*/
            matrix[ro][col]=alg;
            break;

        case 2:
            /*If there is no repetition of numbers in the rows (between 0 and 8), the character 'N' is printed; otherwise print 'Y'*/
            ro=0, rep=0, col=0;
            scanf("%d%*c",&ro);

            for(l=0; l<9; l++){
                cont=0;
                for (c=0; c<9; c++){
                    if(matrix[ro][col]==matrix[ro][c]){
                        cont++;
                    }
                    if(cont>1){
                        rep++;
                    }
                }
                col++;
            }

            if(rep>1){
                printf("Y\n");
            }else{
                printf("N\n");
            }

            break;

        case 3:
            /*If there is no repetition of numbers in the columns (between 0 and 8), the character 'N' must be printed; otherwise print 'Y'*/
            ro=0, rep=0, col=0;
            scanf("%d%*c",&col);
            for(l=0; l<9; l++){
                cont=0;
                for(c=0; c<9; c++){
                    if( matrix[ro][col] == matrix[c][col]){
                        cont++;
                    }
                    if(cont>1){
                        rep++;
                    }
                }
                ro++;
            }

            if(rep>0){
                printf("Y\n");
            }else{
                printf("N\n");
            }
            break;

        case 4:
              /*If there is no repetition of numbers within the quadrants, the character 'N' must be printed; otherwise print 'Y'. The quadrants are listed as follows:*/

            scanf("%d%*c",&quad);
            if(quad<3){
                ro=0;
            }
            else if(quad>=3 && quad<=5){
                ro=3;
            }
            else if(quad>=6 && quad<=8){
                ro=6;
            }

            if(quad==0 || quad==3 || quad==6){
                col=0;
            }
            else if(quad==1 || quad==4 || quad==7){
                col=3;
            }
            else if(quad==2 || quad==5 || quad==8){
                col=6;
            }

            rep=0;
            for(l=ro; l<ro+3; l++){
                for(c=col; c<col+3; c++){
                    cont=0;
                    for(li=ro; li<ro+3; li++){
                        for(co=col; co<col+3; co++){
                            if(matrix[l][c]==matrix[li][co] && (l!=li || c!=co)) /*comparando as matrizes*/{
                                cont++;
                            }
                        }
                    }
                    if(cont>0){
                        rep++;
                    }
                }
            }
            if(rep>0){
                printf("Y\n");
            }else{
                printf("N\n");
            }

            break;

        case 5:
            /*5 if there is no repetition of numbers, the character 'N' must be printed, otherwise, print 'S'*/
            rep=0;
            for(l=0; l<9; l++)  /*reading the rows to see if it repeats*/{
                for (c=0; c<9; c++){
                    for(co=c+1; co<9; co++){
                        if(matrix[l][c]==matrix[l][co]){
                            rep=1;
                        }
                    }
                }
            }

            int rep1=0;
            for(l=0; l<9; l++)  /*reading the column to see if it repeats*/{
                for (c=0; c<9; c++){
                    for(ro=l+1; ro<9; ro++){
                        if(matrix[l][c]==matrix[ro][c]){
                            rep1=1;
                        }
                    }
                }
            }

            int rep2=0;
            int rows=0;
            int columns=0;
             do {
        for (l = rows; l < rows + 3; l++) {
            for (c = 0; c < 9; c++) {
                for (ro = rows; ro < rows + 3; ro++) {
                    for (col = 0; col < 9; col++) {
                        if (l != ro || c != col) { /*avoid comparing the same element*/
                            if (matrix[l][c] != 0 && matrix[l][c] == matrix[ro][col]) {
                                rep2 = 1;
                            }
                        }
                    }
                }
            }
        }
        rows = rows + 3;
    } while (rows < 9);

            if(rep>0 && rep1>0 && rep2>0){
                printf("Y\n");
            }else{
                printf("N\n");
            }
            break;


        default:
            for(l=0; l<9; l++){
                if(l==3 || l==6){
                    printf("\n");
                }
                for(c=0; c<9; c++){
                    if(c==3 || c==6){
                        printf(" ");
                    }
                    printf("%d ", matrix[l][c]);
                }
                printf("\n");
            }
            break;
        }
    }
    while(op!=0);

    return 0;
