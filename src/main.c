/* PCAD - Pthread Matrix
 * Prima consegna del corso di PCAD dell'Universita' degli Studi di Genova
 * Programma per la manipolazione di matrici in parallelo 
 * creata usando la libreria pthread
 *
 * Copyright (C) 2023 Andrea Valenzano 4548315
 * Copyright (C) 2023 Lorenzo Contino 4832500
 * Copyright (C) 2023 Eugenio Pallestrini 4878184
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */ 

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
