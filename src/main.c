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
#include "doomBench.h"
#include <stdio.h>


#define HIGH 1000
#define WIDTH 1500


Matrix first, second, result;


void setup( void ){
    matrix_init(&first, HIGH, WIDTH, 1.0f);
    matrix_init(&second, WIDTH, HIGH, 1.0f);
    matrix_create(&result, 0, 0);
}
void after( void ){
    matrix_delete(&first);
    matrix_delete(&second);
    matrix_delete(&result);
}
void multiply_singe_thread( void ){
    matrix_multiply(&first, &second, &result);
}
void multiply_multi_pthread( void ){
    matrix_multiply_pthread(&first, &second, &result, 20);
}

int main( void )
{
    BENCMARK_INIT;
    BENCMARK_SETUP(setup);
    BENCMARK_AFTER(after);
    BENCMARK(multiply_singe_thread);
    BENCMARK(multiply_multi_pthread);

    return EXIT_SUCCESS;
}
