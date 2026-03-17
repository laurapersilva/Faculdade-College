/*
Make a program that allows you to perform bizarre calculations between two numbers x and y. First, you must read the user's x and y numbers (which may not have the same number of digits) and then read the user's desired option (an integer) which may be one of the following:

1. Removing the y-th digit from x, counted from right to left (so y = 0 is the first digit)

2. Construction of the number whose digits alternate between those in y and x (so, the first digit on the left is the last digit of y, then the second digit of the answer is the last of x)

3. Construction of the number whose digits are those in x and y (so, from right to left, we have the digits of x and then those of y)

RESTRICTIONS: You cannot use any concept not seen in the classroom (math.h library or vectors).
*/


#include <stdio.h>

int main() {
    int x, y, op, result, pow, multiplier, temp, i;

    /*solicitar e ler os números x e y fornecidos pelo usuário*/
    scanf("%d", &x);
    scanf("%d", &y);
    scanf("%d", &op);

    switch (op) {
         /*removing the y-th digit from x*/
        case 1:
            pow = 1;
            for (i = 0; i < y; i++) {
                pow *= 10; /*increases the power of 10 to the digit in the 'y' position*/
            }
            temp = x / (pow * 10); /*divides 'x' to remove the digit at position 'y'*/
            result = temp * pow + (x % pow); /*reconstructs 'x' without the digit in position 'y'*/
             printf("%d\n", result);
            break;

            /*construction of the number whose digits alternate between x and y*/
        case 2:
            result = 0;
            multiplier = 1;
            /*alternately adds digits of x and y to the result*/
            while (x > 0 || y > 0) {
                if (y > 0) {
                    result += (y % 10) * multiplier; /*adds the last digit of 'y' to the result*/
                    y /= 10; /*removes the last digit of 'y'*/
                    multiplier *= 10; /*updates the multiplier to the next position of the result*/
                }
                if (x > 0) {
                    result += (x % 10) * multiplier; /*adds the last digit of 'x' to the result*/
                    x /= 10; /*removes the last digit of 'x'*/
                    multiplier *= 10; /*updates the multiplier to the next position of the result*/
                }
            }
             printf("%d\n", result);
            break;


            /*construction of the number whose digits are the digits of x followed by those of y*/
        case 3:
            pow = 1;
            /*finds the power needed to concatenate x and y*/
            while (y >= pow) {
                pow *= 10;
            }
            /*concatenates x and y*/
            result = x * pow + y;
             printf("%d\n", result);
            break;
    }


    return 0;
}
