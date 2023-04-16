#ifndef DOOMSDAY_C_BENCHMARK_H
#define DOOMSDAY_C_BENCHMARK_H


#include <time.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>


#define Errno int


struct benchmark_data{
    uint32_t milliseconds_start;
    uint32_t seconds_start;
    uint32_t milliseconds_end;
    uint32_t seconds_end;
    uint32_t milliseconds_result;
    uint32_t seconds_result;
    char misuration_data[100];
    char function_name[1024];
};
typedef struct benchmark_data Bencmark;


#define get_starting_time_return(value) {return_value = value; goto get_starting_time_return;}
Errno get_starting_time(Bencmark* benchmark_struct){
    int return_value = 0;
    struct timespec tp;
    if((clock_gettime(CLOCK_MONOTONIC_RAW, &tp)) != 0){
        get_starting_time_return(errno);
    }
    benchmark_struct->milliseconds_start = tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
    benchmark_struct->seconds_start = tp.tv_sec;
get_starting_time_return:
    return return_value;
}


#define get_ending_time_return(value) {return_value = value; goto get_ending_time_return;}
Errno get_ending_time(Bencmark* benchmark_struct){
    int return_value = 0;
    struct timespec tp;
    if((clock_gettime(CLOCK_MONOTONIC_RAW, &tp)) != 0){
        get_ending_time_return(errno);
    }
    benchmark_struct->milliseconds_end = tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
    benchmark_struct->seconds_end = tp.tv_sec;
get_ending_time_return:
    return return_value;
}


void set_result_time(Bencmark* benchmark_struct){
    benchmark_struct->milliseconds_result = benchmark_struct->milliseconds_end - benchmark_struct->milliseconds_start;
    benchmark_struct->seconds_result = benchmark_struct->seconds_end - benchmark_struct->seconds_start;
}


#define BENCMARK_INIT \
    Bencmark bencmark; \
    int8_t use_setup = false; \
    void (*setup_function)(void) = NULL; \
    int8_t use_after = false; \
    void (*after_function)(void) = NULL;

#define BENCMARK_SETUP(function) \
    use_setup = true; \
    setup_function = function;

#define BENCMARK_SETUP_FREE \
    use_setup = false \
    setup_function = NULL

#define BENCMARK_AFTER(function) \
    use_after = true; \
    after_function = function;

#define BENCMARK_AFTER_FREE \
    use_after = false \
    after_function = NULL

#define BENCMARK(function) \
    if(use_setup) setup_function(); \
    strcpy(bencmark.function_name, #function); \
    get_starting_time(&bencmark); \
    function(); \
    get_ending_time(&bencmark); \
    set_result_time(&bencmark); \
    if(use_after) after_function(); 


#endif // !DOOMSDAY_C_BENCHMARK_H
