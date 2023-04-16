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

#define matrix_create_return(value) do{return_value=(value);goto matrix_create_to_return;}while(0);
Errno CUDA_matrix_create(Matrix * const matrix, uint32_t const h, uint32_t const w){
    Errno return_value = 0;
    if (h==0 && w==0) {
        matrix->w = 0;
        matrix->h = 0;
        matrix->data = NULL; 
        matrix_create_return(return_value);
    }
    if (h==0 || w==0) {
        matrix_create_return(EMATRIXSIZEISZERO)
    }
    matrix->h = h;
    matrix->w = w;
    cudaMallocManaged(&matrix->data, sizeof(float*)*h);
    if(matrix->data==NULL){
        matrix_create_return(errno);
    }
    for(uint32_t i=0; i<h; i++){
        cudaMallocManaged(&matrix->data[i], sizeof(float)*w);
        if(matrix->data[i]==NULL){
            matrix_create_return(errno);
        }
    }
matrix_create_to_return:
    return return_value;
}


#define matrix_init_return_random(value) do{return_value=(value);goto matrix_init_to_return_random;}while(0);
Errno CUDA_matrix_init(Matrix * const matrix, uint32_t const h, uint32_t const w, float const value){
    Errno return_value = 0;
    return_value = CUDA_matrix_create(matrix, h, w);
    if(return_value != 0){
        matrix_init_return_random(return_value);
    }
    return_value = matrix_fill(matrix, value);
    if(return_value != 0){
        matrix_init_return_random(return_value);
    }
matrix_init_to_return_random:
    return return_value;
}


void CUDA_matrix_delete(Matrix * const matrix){
    for(uint32_t i=0; i<matrix->h; i++){
        cudaFree(&matrix->data[i]);
    }
    cudaFree(&matrix->data);
    matrix->data = NULL;
    matrix->h=0;
    matrix->w=0;
}


#define matrix_transpose_return(value) do{return_value=(value);goto matrix_transpose_to_return;}while(0);
Errno CUDA_matrix_transpose(Matrix * const matrix){
    Errno return_value = 0;
    Matrix old_data;
    old_data.h = matrix->h;
    old_data.w = matrix->w;
    old_data.data = matrix->data;
    if((return_value=CUDA_matrix_create(matrix, old_data.w, old_data.h))!=0){
       matrix_transpose_return(return_value); 
    }
    for (uint32_t i=0; i<matrix->h; i++){
        for(uint32_t j=0; j<matrix->w; j++){
            matrix->data[i][j] = old_data.data[j][i];
        }
    }
matrix_transpose_to_return:
    CUDA_matrix_delete(&old_data);
    return return_value;
}


__global__ void CUDA_matrix_multiply_worker(Matrix const * const first, Matrix * const second, Matrix * result){
    float result_number = 0.0f;
    for(uint32_t i=0; i<result->w; i++){
        result_number += first->data[threadIdx.x/result->w][i] * second->data[threadIdx.x%result->w][i];
    }
    result->data[threadIdx.x/result->w][threadIdx.x%result->w] = result_number;
}

#define matrix_multiply_return(value) do{return_value=(value);goto matrix_multiply_to_return;}while(0);
Errno CUDA_matrix_multiply(Matrix const * const first, Matrix * const second, Matrix * result){
    Errno return_value = 0;
    if(first->w != second->h){ 
        matrix_multiply_return(EMATRIXINCONPATIBLESIZE);
    }
    if(result->data != NULL){
        CUDA_matrix_delete(result);
    }
    return_value = CUDA_matrix_create(result, first->h, second->w);
    if(return_value != 0){
        matrix_multiply_return(return_value);
    } 
    return_value = CUDA_matrix_transpose(second);
    if(return_value != 0){
        matrix_multiply_return(return_value);
    }
    CUDA_matrix_multiply_worker<<<(result->h*result->w)/256, 256>>>(first, second, result); 
matrix_multiply_to_return:
    return return_value;
}


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

void CUDA_setup( void ){
    CUDA_matrix_init(&A, M, N, 1.0f);
    CUDA_matrix_init(&B, N, P, 1.0f);
    CUDA_matrix_init(&C, P, M, 1.0f);
    CUDA_matrix_create(&R, 0, 0);
    CUDA_matrix_create(&Rm, 0, 0);
}
void CUDA_after( void ){
    CUDA_matrix_delete(&A);
    CUDA_matrix_delete(&B);
    CUDA_matrix_delete(&C);
    CUDA_matrix_delete(&R);
    CUDA_matrix_delete(&Rm);
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


void cuda_multiply( void ){
    CUDA_matrix_multiply(&A, &B, &Rm);
    CUDA_matrix_multiply(&C, &Rm, &R);
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
    /* BENCMARK(multiply_multi_pthread_2); */
    /* print_bencmark_result(&bencmark); */
    /* BENCMARK(multiply_multi_pthread_4); */
    /* print_bencmark_result(&bencmark); */
    /* BENCMARK(multiply_multi_pthread_8); */
    /* print_bencmark_result(&bencmark); */
    BENCMARK(multiply_multi_pthread_16);
    print_bencmark_result(&bencmark);
    printf("== MATRIX MULTIPLY CUDA BENCMARK ==========================\n");
    BENCMARK_SETUP(CUDA_setup);
    BENCMARK_AFTER(CUDA_after);
    BENCMARK(cuda_multiply);
    print_bencmark_result(&bencmark);

    return EXIT_SUCCESS;
}
