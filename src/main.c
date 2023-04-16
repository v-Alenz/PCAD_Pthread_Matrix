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


#define M 1000
#define N 1500
#define P 1200


Matrix A, B, C, R, Rm;


void setup( void ){
    matrix_init(&A, M, N, 1.0f);
    matrix_init(&B, N, P, 1.0f);
    matrix_init(&C, P, M, 1.0f);
    matrix_create(&R, 0, 0);
    matrix_create(&Rm, 0, 0);
}
void after( void ){
    matrix_delete(&A);
    matrix_delete(&B);
    matrix_delete(&C);
    matrix_delete(&R);
    matrix_delete(&Rm);
}
void multiply_singe_thread( void ){
    matrix_multiply(&A, &B, &Rm);
    matrix_multiply(&C, &Rm, &R);
}
void multiply_multi_pthread_2( void ){
    matrix_multiply_pthread(&A, &B, &Rm, 2);
    matrix_multiply_pthread(&C, &Rm, &R, 2);
}

void multiply_multi_pthread_4( void ){
    matrix_multiply_pthread(&A, &B, &Rm, 4);
    matrix_multiply_pthread(&C, &Rm, &R, 4);
}

void multiply_multi_pthread_8( void ){
    matrix_multiply_pthread(&A, &B, &Rm, 8);
    matrix_multiply_pthread(&C, &Rm, &R, 8);
}

void multiply_multi_pthread_16( void ){
    matrix_multiply_pthread(&A, &B, &Rm, 16);
    matrix_multiply_pthread(&C, &Rm, &R, 16);
}


void print_bencmark_result( Bencmark* bencmark ){
    printf("Function Name: %s\n", bencmark->function_name);
    printf("Execuiton time: %ums\n", bencmark->milliseconds_result/1000); 
    printf("===========================================================\n");
}

int main( void )
{
    BENCMARK_INIT;
    BENCMARK_SETUP(setup);
    BENCMARK_AFTER(after);
    printf("== MATRIX MULTIPLY BENCMARK ===============================\n");
    BENCMARK(multiply_singe_thread);
    print_bencmark_result(&bencmark);
    BENCMARK(multiply_multi_pthread_2);
    print_bencmark_result(&bencmark);
    BENCMARK(multiply_multi_pthread_4);
    print_bencmark_result(&bencmark);
    BENCMARK(multiply_multi_pthread_8);
    print_bencmark_result(&bencmark);
    BENCMARK(multiply_multi_pthread_16);
    print_bencmark_result(&bencmark);

    return EXIT_SUCCESS;
}
