/*
You must write a program that reads from the user a sequence of 10 positive numbers between 0 and 100 that will be passed to him/her and then offers the following options:
0. Exit the program
1. Update the value at a specific index (provided by the user) of the sequence
2. View sequence
3. Display sequence in reverse order
4. Display the sum of all values ​​in the sequence
5. Display count of duplicate numbers of the sequence
6. Display the unique (non-repeating) values ​​of the sequence
7. Display frequency of all sequence values
8. Display the maximum, minimum and average value (with two decimal places) of the sequence
9. Display the even and odd values ​​of the sequence (without repetition)
10. Display the sequence rotated M times (amount provided by the user) to the left
11. Display the sequence rotated M times (amount provided by the user) to the right

RESTRICTIONS: You cannot use any concept not seen in the classroom (character strings)
 */

#include <stdio.h>

int main() {
    int sequence[10];
    int op, i, j, n, sum=0, acc = 0;
    int max = sequence[0], min = sequence[0];
    float average;

    for (i = 0; i < 10; i++) {
        do {
            scanf("%d", &sequence[i]);
        } while (sequence[i] < 0 || sequence[i] > 100); /*checking if it is within the allowed range*/
    }

    do {
        scanf("%d", &op);

        switch (op) {
            case 0:
                break;

            case 1:
                /*update value at a specific index**/
                scanf("%d", &i);
                if (i >= 0 && i < 10) {
                    scanf("%d", &sequence[i]);
                }
                break;

            case 2:
                /*display sequence*/
                for (i = 0; i < 10; i++) { /*wanders the vector to display the numbers*/
                    printf(" %d", sequence[i]);
                }
                printf("\n");
                break;

            case 3:
                /*display sequence in reverse order*/
                for (i = 9; i >= 0; i--) { /*wanders through the vector backwards to display the numbers*/
                    printf(" %d", sequence[i]);
                }
                printf("\n");
                break;

            case 4:
                /*display summary of values*/
                for (i = 0; i < 10; i++) { /*wanders through the array to sum the numbers*/
                    sum += sequence[i];
                }
                printf("%d\n", sum);
                break;

            case 5:
                /*display duplicated sequence count*/
                for (i = 0; i < 9; i++) {
                    for (j = i + 1; j < 10; j++) {
                        if (sequence[i] == sequence[j]) { /*compare each pair of numbers to detect duplicates*/
                            acc++;
                            break;
                        }
                    }
                }
                printf("%d", acc);
                break;

            case 6:
                /*display unique values*/
                for (i = 0; i < 10; i++) {
                    int repetido = 0;
                    for (j = 0; j < i; j++) {
                        if (sequence[i] == sequence[j]) { /*checks if the number has already been found previously*/
                            repetido = 1;
                            break;
                        }
                    }
                    if (!repetido) { /*if not found, it is unique and is displayed*/
                        printf(" %d", sequence[i]);
                    }
                }
                printf("\n");
                break;

            case 7:
                /*display frequency of values*/
                for (i = 0; i < 10; i++) {
                    int frequencia = 1;
                    for (j = i + 1; j < 10; j++) {
                        if (sequence[i] == sequence[j]) { /*counts how many times the number occurs in the vector*/
                            frequencia++;
                        }
                    }
                    printf("%d: %d\n", sequence[i], frequencia);
                }
                break;

            case 8:
                /*display maximum, minimum and average value*/
                for (i = 0; i < 10; i++) {
                    sum += sequence[i];
                    if (sequence[i] > max) { /*finds the maximum value*/
                        max = sequence[i];
                    }
                    if (sequence[i] < min) { /*finds the minimum value*/
                        min = sequence[i];
                    }
                }
                average = (float)sum / 10; /*calculate the average*/
                printf("%d\n", max);
                printf("%d\n", min);
                printf("%.2f\n", average);
                break;

            case 9:
                /*display even and odd values*/
                for (i = 0; i < 10; i++) {
                    if (sequence[i] % 2 == 0) { /*checks if the number is even*/
                        printf(" %d", sequence[i]);
                    }
                }
                printf("\n");
                for (i = 0; i < 10; i++) {
                    if (sequence[i] % 2 != 0) { /*checks if the number is odd*/
                        printf(" %d", sequence[i]);
                    }
                }
                printf("\n");
                break;

            case 10:
                /*display sequence rotated M times to the left*/
                scanf("%d", &n);
                n = n % 10; /*ensuring that it will not run more than the size of the vector*/
                for (i = n; i < 10; i++) {
                    printf(" %d", sequence[i]);
                }
                for (i = 0; i < n; i++) {
                    printf(" %d", sequence[i]);
                }
                printf("\n");
                break;

            case 11:
                /*display sequence rotated M times to the right*/
                scanf("%d", &n);
                n = n % 10; /*ensuring that it will not run more than the size of the vector*/
                for (i = 10 - n; i < 10; i++) {
                    printf(" %d", sequence[i]);
                }
                for (i = 0; i < 10 - n; i++) {
                    printf(" %d", sequence[i]);
                }
                printf("\n");
                break;

            default:
                printf("op invalida!\n");
        }
    } while (op != 0);

    return 0;
}
