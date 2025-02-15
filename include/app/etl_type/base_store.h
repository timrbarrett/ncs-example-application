
#pragma once

#include <math.h>
#include "app/etl/deque.h"            // etl::deque
#include "app/etl_type/message_enum.h"     // etl_type enum definitions
#include "app/etl_type/ulisp.h" // cons
#include "app/etl_type/etl_base.h"

/*
  intention: this class can be a base class to any store created that is a deque too.
*/
const int CHARACTER_IN_ULISP_LABEL =7;

extern     /* static*/  etl_base* references[NUMBER_OF_ETL_TYPES];

/*
static void floatToString(char* buffer, double value, int decimalPlaces) {
  int intPart = (int)value;
  int fracPart = abs((double)((value - intPart) 
    * pow((double)10.0, (double)decimalPlaces)));

  // Handle negative values
  if (value < 0) {
    sprintf(buffer, "-%d.%0*d", intPart,
    decimalPlaces, fracPart);
  } else {
    sprintf(buffer, "%d.%0*d", intPart, decimalPlaces, fracPart);
  }
} 
*/
class base_store {
  public:
    base_store(void) {}
    etl_base* etl_lookup(int etl_type);

    object* generic_mock(int etl_type, int32_t int1);                             // most integer types
    object* generic_mock(int etl_type, float int1, float int2, float int3); // devacc, devmean
    object* generic_mock(int etl_type, float firstinput);                         // c1op, c1of, c1tp, c1si

    object* generic_create(int etl_type);

    object* generic_clear(int etl_type);

    int generic_count(int etl_type);

    // returns any valid ulisp return type.
    object* generic_output(int etl_type, int record);
    object* generic_float_output(int etl_type, int record);

    // must be same order as message_enum.h

    static /*constexpr*/ char names[NUMBER_OF_ETL_TYPES][CHARACTER_IN_ULISP_LABEL]; 
};

/* 
  intention: this exists to allow the SpecificDequeType to be seamlessly declared 
  exactly like etl::deque would have been, but with an additional inheritance of base_store

  https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c
*/
template<typename T, const size_t MAX_SIZE_> 
class SpecificDeque:
  public base_store,
  public etl::deque<T, MAX_SIZE_>
{  
};

