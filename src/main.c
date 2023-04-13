#include "matrix.h"


#define HIGH 4
#define WIDTH 5


int main( void )
{

    Matrix matrix_1, matrix_2, matrix_res;
    matrix_init(&matrix_1, HIGH, WIDTH, 2.5f);
    matrix_init(&matrix_2, WIDTH, WIDTH, 1.5f);
    matrix_create(&matrix_res, 0, 0);
    /* matrix_print(&matrix_1); */
    /* matrix_print(&matrix_2); */
    matrix_multiply_pthread(&matrix_1, &matrix_2, &matrix_res, 5);
    /* matrix_print(&matrix_res); */


    matrix_delete(&matrix_1);
    matrix_delete(&matrix_2);
    matrix_delete(&matrix_res);


    return EXIT_SUCCESS;
}
