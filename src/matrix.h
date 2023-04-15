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

#ifndef PTHREAD_MATRIX_H
#define PTHREAD_MATRIX_H


#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>


struct matrix{
    float** data;
    uint32_t h;
    uint32_t w;
};
typedef struct matrix  Matrix;


#define Errno int
#define EMATRIXISNULL -1
#define EMATRIXSIZEISZERO -2
#define EMATRIXINCONPATIBLESIZE -3


#define matrix_create_return(value) do{return_value=(value);goto matrix_create_to_return;}while(0);
Errno matrix_create(Matrix * const matrix, uint32_t const h, uint32_t const w){
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
    matrix->data = (float**)malloc(sizeof(float*)*h);
    if(matrix->data==NULL){
        matrix_create_return(errno);
    }
    for(uint32_t i=0; i<h; i++){
        matrix->data[i] = (float*)malloc(sizeof(float)*w);
        if(matrix->data[i]==NULL){
            matrix_create_return(errno);
        }
    }
matrix_create_to_return:
    return return_value;
}


void matrix_delete(Matrix * const matrix){
    for(uint32_t i=0; i<matrix->h; i++){
        free(matrix->data[i]);
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->h=0;
    matrix->w=0;
}


Errno matrix_fill(Matrix const * const matrix, float const value){
    if(matrix == NULL){
        return EMATRIXISNULL;
    }
    for (uint32_t i=0; i<matrix->h; i++){
        for(uint32_t j=0; j<matrix->w; j++){
            matrix->data[i][j] = value;
        }
    }
    return 0;
}


#define matrix_init_return_random(value) do{return_value=(value);goto matrix_init_to_return_random;}while(0);
Errno matrix_init(Matrix * const matrix, uint32_t const h, uint32_t const w, float const value){
    Errno return_value = 0;
    return_value = matrix_create(matrix, h, w);
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


Errno matrix_fill_random(Matrix const * const matrix, uint32_t limit){
    if(matrix == NULL){
        return EMATRIXISNULL;
    }
    srand(time(NULL));
    for (uint32_t i=0; i<matrix->h; i++){
        for(uint32_t j=0; j<matrix->w; j++){
            matrix->data[i][j] = (float)(rand()%(limit*1000))/1000;
        }
    }
    return 0;
}


#define matrix_init_return_random(value) do{return_value=(value);goto matrix_init_to_return_random;}while(0);
Errno matrix_init_random(Matrix * const matrix, uint32_t const h, uint32_t const w, uint32_t limit){
    Errno return_value = 0;
    return_value = matrix_create(matrix, h, w);
    if(return_value != 0){
        matrix_init_return_random(return_value);
    }
    return_value = matrix_fill_random(matrix, limit);
    if(return_value != 0){
        matrix_init_return_random(return_value);
    }
matrix_init_to_return_random:
    return return_value;
}


Errno matrix_print(Matrix const * const matrix){
    if(matrix == NULL){
        return EMATRIXISNULL;
    } 
    for (uint32_t i=0; i<matrix->h; i++){
        printf("[");
        for(uint32_t j=0; j<matrix->w; j++){
            printf("%f, ", matrix->data[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
    return 0;
}


#define matrix_transpose_return(value) do{return_value=(value);goto matrix_transpose_to_return;}while(0);
Errno matrix_transpose(Matrix * const matrix){
    Errno return_value = 0;
    Matrix old_data;
    old_data.h = matrix->h;
    old_data.w = matrix->w;
    old_data.data = matrix->data;
    if((return_value=matrix_create(matrix, old_data.w, old_data.h))!=0){
       matrix_transpose_return(return_value); 
    }
    for (uint32_t i=0; i<matrix->h; i++){
        for(uint32_t j=0; j<matrix->w; j++){
            matrix->data[i][j] = old_data.data[j][i];
        }
    }
matrix_transpose_to_return:
    matrix_delete(&old_data);
    return return_value;
}


float vector_multiply(float const * const first, float const * const second, uint32_t size){
    float result = 0.0f;
    for(uint32_t i=0; i<size; i++){
        result += first[i]*second[i];
    }
    return result;
}


#define matrix_multiply_return(value) do{return_value=(value);goto matrix_multiply_to_return;}while(0);
Errno matrix_multiply(Matrix const * const first, Matrix * const second, Matrix * result){
    Errno return_value = 0;
    if(first->w != second->h){ 
        matrix_multiply_return(EMATRIXINCONPATIBLESIZE);
    }
    if(result->data != NULL){
        matrix_delete(result);
    }
    return_value = matrix_create(result, first->h, second->w);
    if(return_value != 0){
        matrix_multiply_return(return_value);
    }
    return_value = matrix_transpose(second);
    if(return_value != 0){
        matrix_multiply_return(return_value);
    }
    for(uint32_t i=0; i<result->h; i++){
        for(uint32_t j=0; j<result->w; j++){
            result->data[i][j] = vector_multiply(first->data[i], second->data[j], first->w);
        }
    } 
matrix_multiply_to_return:
    return return_value;
}


struct matrix_multiply_pthread_worker_params{
    Matrix* first;
    Matrix* second;
    Matrix* result;
    uint32_t block_size;
    uint32_t block_index;
}; 
typedef struct matrix_multiply_pthread_worker_params multiply_args;


pthread_barrier_t block_index_barier;
pthread_barrier_t multiply_barrier;
void * matrix_multiply_pthread_worker(void * matrix_multiply_pthread_args){
    multiply_args* args = (multiply_args*)matrix_multiply_pthread_args;
    uint32_t block_index = args->block_index;
    pthread_barrier_wait(&block_index_barier);
    for(uint32_t i=args->block_size*block_index; i<(args->block_size*block_index)+args->block_size; i++){
        args->result->data[i/args->result->w][i%args->result->w] = vector_multiply(args->first->data[i/args->result->w], args->second->data[i%args->result->w], args->first->w);
    }
    pthread_barrier_wait(&multiply_barrier);
    return NULL;
}


#define matrix_multiply_pthread_return(value) do{return_value=(value);goto matrix_multiply_pthread_to_return;}while(0);
Errno matrix_multiply_pthread(Matrix * const first, Matrix * const second, Matrix * result, uint32_t threads){
    Errno return_value = 0;
    if(first->w != second->h){ 
        matrix_multiply_pthread_return(EMATRIXINCONPATIBLESIZE);
    }
    if(first->h*second->w < threads){ 
        matrix_multiply_pthread_return(EMATRIXINCONPATIBLESIZE);
    }
    if(result->data != NULL){
        matrix_delete(result);
    }
    return_value = matrix_init(result, first->h, second->w, 0.0f);
    if(return_value != 0){
        matrix_multiply_pthread_return(return_value);
    }
    return_value = matrix_transpose(second);
    if(return_value != 0){
        matrix_multiply_pthread_return(return_value);
    }
    
    pthread_t* threads_list;
    uint32_t threads_list_index;
    uint32_t block_size_spare;
    uint32_t block_size;
    multiply_args* params;
    params = (multiply_args*)malloc(sizeof(multiply_args));
    threads_list = (pthread_t*)malloc(sizeof(pthread_t)*threads); 
    block_size_spare = (result->h*result->w)%threads;
    block_size = ((result->h*result->w)-block_size_spare)/threads; 
    params->first = first;
    params->second = second;
    params->result = result;
    params->block_size = block_size;
    threads_list_index = 0;
    pthread_barrier_init(&block_index_barier, NULL, 2);
    pthread_barrier_init(&multiply_barrier, NULL, threads+1);
    for(uint32_t i=0; i<(result->h*result->w)/block_size; i++){
        params->block_index = i; // TODO - Questo non e' thread_safe e va lockato
        pthread_create(&threads_list[threads_list_index], NULL, matrix_multiply_pthread_worker, (void * )params);
        threads_list_index++;
        pthread_barrier_wait(&block_index_barier);
    }  
    for(uint32_t i=block_size*threads;i<(block_size*threads)+block_size_spare; i++){
        result->data[i/result->w][i%result->w] = vector_multiply(first->data[i/result->w], second->data[i%result->w], first->w);
    }
    pthread_barrier_wait(&multiply_barrier);
    pthread_barrier_destroy(&block_index_barier);
    pthread_barrier_destroy(&multiply_barrier);
    free(params);
    for(uint32_t i=0; i<threads; i++){
        pthread_join(threads_list[i], NULL);
    }
    free(threads_list);

matrix_multiply_pthread_to_return:
    return return_value;
}


//TODO

// funzione che moltiplica smazzandosi le dimensioni dei blocchi

// funzione che prende in bocca i blocchi e moltiplica e ritorna i/il valore/i

// TODO FACOLTATIVO

// funzione che legge la matrici da file che qui si fa dura a testare

#endif // !PTHREAD_MATRIX_H
