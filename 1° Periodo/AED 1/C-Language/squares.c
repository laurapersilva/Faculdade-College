/*Write a program that reads any positive integer and a character that indicates the type of square (with side l) to be printed on the screen using the character '*'. If the character is:

'c', print a fully filled square;
'b', print only its edges;
'p', print its edges and main diagonal;
's', print its edges and secondary diagonal;
'h', print its edges and its horizontal division (in the middle);
'v' prints its edges and its vertical division (in the middle);
otherwise, print the error message: "No such option."
RESTRICTIONS: You cannot use any concept not seen in the classroom (strings)
*/
#include <stdio.h>

int main(){
  int side;
  char op;
   int i;
   int j;

  scanf("%d%*c", &side);

  /*give the user the option of which type of square to print*/
  scanf("%c%*c", &op);



  switch (op){
    case 'c':
      /*thinking as a matrix, i represents the rows and j the columns in which the asterisks that will form the square will be printed in both*/

    for(i=0; i<side; i++){
      for(j=0; j<side; j++){
        printf("*");
      }
      printf("\n");
    } break;
      case 'b':
            for (i = 0; i < side; i++) {
                for (j = 0; j < side; j++) {
                  /*when one of these conditions is true, the asterisk is printed, otherwise, spaces are printed, allowing the construction of the square's edges*/
                    if (i == 0 || i == side - 1 || j == 0 || j == side - 1) {
                        printf("*");
                    } else {
                        printf("  ");
                    }
                }
                printf("\n");
            }
            break;
        case 'p':
            for (i = 0; i < side; i++) {
                for (j = 0; j < side; j++) {
                  /*In this case, with these conditions, specifically i==j, it is possible to construct the square filled with the main diagonal*/
                    if (i == j || i == 0 || i == side - 1 || j == 0 || j == side - 1) {
                        printf("*");
                    } else {
                        printf("  ");
                    }
                }
                printf("\n");
            }
            break;
        case 's':
            for (i = 0; i < side; i++) {
                for (j = 0; j < side; j++) {
                  /*keeping in mind that it works like a matrix, the condition i+j==side-1 allows you to construct a secondary diagonal, based on the values ​​of i and j*/
                    if (i + j == side - 1 || i == 0 || i == side - 1 || j == 0 || j == side - 1) {
                        printf("*");
                    } else {
                        printf("  ");
                    }
                }
                printf("\n");
            }
            break;
        case 'h':
            for (i = 0; i < side; i++) {
                for (j = 0; j < side; j++) {
                  /*since here we are aiming for a line that divides the square in half, the condition i==side/2 allows the asterisks to be printed on a horizontal line in the middle*/
                    if (i == side/2 || i == 0 || i == side - 1 || j == 0 || j == side - 1) {
                        printf("*");
                    } else {
                        printf("  ");
                    }
                }
                printf("\n");
            }
            break;
        case 'v':
            for (i = 0; i < side; i++) {
                for (j = 0; j < side; j++) {
                  /*Same idea as the previous case, but the line is now vertical, making it necessary to divide the columns as in j== side/2*/
                    if (j == side/2 || i == 0 || i == side - 1 || j == 0 || j == side - 1) {
                        printf("*");
                    } else {
                        printf("  ");
                    }
                }
                printf("\n");
            }
            break;
        default:
            printf("that option doesn't exist\n");
    }

    return 0;
}
