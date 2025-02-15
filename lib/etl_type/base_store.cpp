#include "app/etl_type/base_store.h"
//#include "accelerometer.h"
//#include "meanacc.h"
#ifdef __cplusplus 
extern "C" { 
#endif
#include "app/etl_type/ulisp.h" // makefloat
#ifdef __cplusplus 
} 
#endif
#include "app/etl_type/etl_type_setup.h"
//#include "create_pulses.h"

// from https://stackoverflow.com/questions/45503329/cannot-declare-static-constexpr-char
char base_store::names[NUMBER_OF_ETL_TYPES][CHARACTER_IN_ULISP_LABEL];
etl_base* references[NUMBER_OF_ETL_TYPES];

// the function that translates the number passed to uLisp into an actual object that can have a method called on it.
etl_base* base_store::etl_lookup(int etl_type) {

  if (references [etl_type] == nullptr) { 
    //NRF_LOG_INFO(" **** null etl_lookup: type %i",etl_type); 
    return nullptr;
  } else {
    //NRF_LOG_INFO("etl_lookup returns %i %i %s", etl_type, references[etl_type], names[etl_type]);
    return references[etl_type];
  }
  
  //NRF_LOG_INFO("We shouldn't be able to get here!");
}

object*  base_store::generic_clear(int etl_type) {
  
  // deal with those that can be automated
  if (etl_lookup(etl_type)) { 
    etl_lookup(etl_type)->on_clear(); 
    //NRF_LOG_INFO("*** on_clear! ***");
  };
  
  /*
  // triples cannot *yet* be automated
  if (etl_type == MESSAGE_CHAIN_LINK) static_cast<chain::ReadingStore*>(all_stores[etl_type])->clear();
  if (etl_type == MESSAGE_ACCELEROMETER) static_cast<accelerometer::ReadingStore*>(all_stores[etl_type])->clear();
  if (etl_type == MESSAGE_MEANACC) { 
    static_cast<meanacc::ReadingStore*>(all_stores[etl_type])->clear();
    meanacc::clearAverages();
  }
  if (etl_type == MESSAGE_DEVFUN) static_cast<devfun::ReadingStore*>(all_stores[etl_type])->clear();
  if (etl_type == MESSAGE_C2FU) static_cast<c2fu::ReadingStore*>(all_stores[etl_type])->clear();
  */
  return number(-1);

}

void create_centiHzPulse(); // forwards declaration - hopefully

object * base_store::generic_create(int etl_type) {

  bool handled=false;

  // deal with those that can be automated
  if (etl_lookup(etl_type)) { 
    etl_lookup(etl_type)->on_create(); 
    handled = true;
    //NRF_LOG_INFO("*** on_create! ***");
  };

  /*
  // standard etl types
  if (etl_type == MESSAGE_CHAIN_LINK)       { chain::on_create(chain::mock);                  handled = true; }
  if (etl_type == MESSAGE_ACCELEROMETER)    { accelerometer::on_create(accelerometer::mock);  handled = true; }
  if (etl_type == MESSAGE_MEANACC)          { meanacc::on_create(meanacc::mock);              handled = true; }
  if (etl_type == MESSAGE_DEVFUN)           { devfun::on_create(devfun::mock);                handled = true; }
  if (etl_type == MESSAGE_C2FU)             { c2fu::on_create(c2fu::mock);                handled = true; }
  // instantaneous etl types
  if (etl_type == MESSAGE_CENTIHZPULSE)     { create_centiHzPulse();                          handled = true; } // defined in pulse.h
  if (etl_type == MESSAGE_BLEHEX)           { blehex.on_create();            handled = true; }
  */
  if (handled == false) {
    //NRF_LOG_INFO("base_store.cpp unhandled generic create %i",etl_type);
    return number(-1);
  }
  return number (0);
}

int base_store::generic_count(int etl_type) {

  // deal with those that can be automated
  if (etl_lookup(etl_type)) { 
    //NRF_LOG_INFO("*** on_count! ***");
    return etl_lookup(etl_type)->on_count(); 
  };

  /*
  if (etl_type == MESSAGE_CHAIN_LINK) return static_cast<chain::ReadingStore*>(all_stores[etl_type])->size();
  if (etl_type == MESSAGE_ACCELEROMETER) return static_cast<accelerometer::ReadingStore*>(all_stores[etl_type])->size();
  if (etl_type == MESSAGE_MEANACC) return static_cast<meanacc::ReadingStore*>(all_stores[etl_type])->size();
  if (etl_type == MESSAGE_DEVFUN) return static_cast<devfun::ReadingStore*>(all_stores[etl_type])->size();
  if (etl_type == MESSAGE_C2FU) return static_cast<c2fu::ReadingStore*>(all_stores[etl_type])->size();
  if (etl_type == MESSAGE_BLEHEX) return blehex.messageStore.size();
  */
  return -1;
}

object* base_store::generic_float_output(int etl_type, int record) {

  // immediate return if asked for a generation not stored
  if (record >= generic_count(etl_type)) return number (-1); 

  //                    asked for an etl_type not templated
  if (!etl_lookup(etl_type)) return number (-2);  
    
  //NRF_LOG_INFO("*** on_output! ***");
  return etl_lookup(etl_type)->on_output(record); 

}

object* base_store::generic_output(int etl_type, int record) {

  if (record < generic_count(etl_type)) {
    
    if (etl_lookup(etl_type)) { 
      //NRF_LOG_INFO("*** on_output! ***");
      return etl_lookup(etl_type)->on_output(record); 
    };

/*
    if (etl_type == MESSAGE_CHAIN_LINK) {
      int link = static_cast<chain::ReadingStore*>(all_stores[etl_type])->at(record).data1;
      int startAt = static_cast<chain::ReadingStore*>(all_stores[etl_type])->at(record).data2;
      int endAt = static_cast<chain::ReadingStore*>(all_stores[etl_type])->at(record).data3;

      return cons(number(link), 
                  cons(number(startAt),
                       cons(number(endAt),NULL)
                      )
                 );
    } // if devcha

    if (etl_type == MESSAGE_ACCELEROMETER) {

      int ax = static_cast<accelerometer::ReadingStore*>(all_stores[etl_type])->at(record).data1;
      int ay = static_cast<accelerometer::ReadingStore*>(all_stores[etl_type])->at(record).data2;
      int az = static_cast<accelerometer::ReadingStore*>(all_stores[etl_type])->at(record).data3;

      // return a ulisp list of three items.
      return cons(number(ax), 
                  cons(number(ay),
                       cons(number(az),NULL)
                      )
                 );

    } // if devacc

    if (etl_type == MESSAGE_MEANACC) {

      int ax = static_cast<meanacc::ReadingStore*>(all_stores[etl_type])->at(record).data1;
      int ay = static_cast<meanacc::ReadingStore*>(all_stores[etl_type])->at(record).data2;
      int az = static_cast<meanacc::ReadingStore*>(all_stores[etl_type])->at(record).data3;

      // return a ulisp list of three items.
      return cons(number(ax), 
                  cons(number(ay),
                       cons(number(az),NULL)
                      )
                 );
    } // if devmea

    if (etl_type == MESSAGE_DEVFUN) {
      //NRF_LOG_INFO("base_store # 175 - generic output devfun");
      float funnphase = static_cast<devfun::ReadingStore*>(all_stores[etl_type])->at(record).data1;
      int funn = int(funnphase);
      float phase = funnphase-funn;

      int typeofeffect = static_cast<devfun::ReadingStore*>(all_stores[etl_type])->at(record).data2;
      int valueofeffect = static_cast<devfun::ReadingStore*>(all_stores[etl_type])->at(record).data3;

      // return a ulisp list of three items.
      return cons(number(funn), 
                  cons(makefloat(phase),
                       cons(number(typeofeffect), // newsymbol(pack40((char*)base_store::names[typeofeffect]))
                          cons(number(valueofeffect),NULL)
                           )
                      )
                 );
    } // if devfun
    if (etl_type == MESSAGE_C2FU) {
      //NRF_LOG_INFO("base_store # 175 - generic output devfun");
      float funnphase = static_cast<c2fu::ReadingStore*>(all_stores[etl_type])->at(record).data1;
      int funn = int(funnphase);
      float phase = funnphase-funn;

      int typeofeffect = static_cast<c2fu::ReadingStore*>(all_stores[etl_type])->at(record).data2;
      int valueofeffect = static_cast<c2fu::ReadingStore*>(all_stores[etl_type])->at(record).data3;

      // return a ulisp list of three items.
      return cons(number(funn), 
                  cons(makefloat(phase),
                       cons(number(typeofeffect), // newsymbol(pack40((char*)base_store::names[typeofeffect]))
                          cons(number(valueofeffect),NULL)
                           )
                      )
                 );
    } // if c2fu
  } // not enough records
 */
  }
  return number (-1); // unimplemented (as yet!)

}


object* base_store::generic_mock(int etl_type, float int1, float int2, float int3) {

  //bool handled = false;
  /*
  if (etl_type == MESSAGE_ACCELEROMETER) {
    accelerometer::mock.mock1 = int(int1); accelerometer::mock.mock2 = int(int2); accelerometer::mock.mock3 = int(int3); handled = true;
  }
  if (etl_type == MESSAGE_MEANACC) {
    meanacc::mock.mock1 = int(int1); meanacc::mock.mock2 = int(int2); meanacc::mock.mock3 = int(int3); handled = true;
  }
  if (etl_type == MESSAGE_DEVFUN) {
    // special handling required for float etl types.
    devfun::mock.mock1=int1; devfun::mock.mock2=int(int2); devfun::mock.mock3=int(int3); handled=true;
  }
  if (etl_type == MESSAGE_C2FU) {
    // special handling required for float etl types.
    c2fu::mock.mock1=int1; c2fu::mock.mock2=int(int2); c2fu::mock.mock3=int(int3); handled=true;
  }
  if (handled == false) {
    NRF_LOG_INFO("base_store.cpp unhandled int floatx3 generic mock %i",etl_type);
    return number(-1);
  } else {
    NRF_LOG_INFO("*** on_mock! ***");
  }
  */
  return number (0);
}

object* base_store::generic_mock(int etl_type, float firstinput) {
  //bool handled = false;
  /*
  // genuine floats
  if (etl_type == MESSAGE_C1OP) { c1op.etlmock(firstinput); handled= true;}
  if (etl_type == MESSAGE_C1OF) { c1of.etlmock(firstinput); handled= true;}
  if (etl_type == MESSAGE_C1TP) { c1tp.etlmock(firstinput); handled= true;}
  if (etl_type == MESSAGE_C2OP) { c2op.etlmock(firstinput); handled= true;}
  if (etl_type == MESSAGE_C2OF) { c2of.etlmock(firstinput); handled= true;}
  if (etl_type == MESSAGE_C2TP) { c2tp.etlmock(firstinput); handled= true;}

  int int1 = (int)firstinput;

  if (etl_lookup(etl_type)==nullptr) {
    NRF_LOG_INFO("generic_mock(float) etl_lookup returned nullptr");
    } else {
      etl_lookup(etl_type)->set_mock(int1); handled=true;
    }
  // byproducts of floatorint conversion that are defo ints

  if (handled == false) {
    NRF_LOG_INFO("base_store.cpp unhandled int float generic mock %i",etl_type);
    return number(-1);
  } else {
    NRF_LOG_INFO("*** on_mock! ***");
  }
  */
  return number(0);
}