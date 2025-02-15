#pragma once

#include "app/etl_type/ulisp.h"

// see https://stackoverflow.com/questions/4272432/c-undefined-reference-to-vtable
// for why virtual and =0;
// needed to enable rtti for the project too.
struct etl_base {
  virtual void on_clear(void)=0;
  virtual void on_create(void)=0;
  virtual int on_count(void)=0;
  virtual object* on_output(int record)=0;
  virtual void set_mock(int value)=0;
  //virtual etl_base* etl_lookup(int etl_type)=0;
};
