#pragma once

#include "MockValue.h"        // SingleMockValue
#include "trbmessage.h"       // Message
#include "base_store.h"       // SpecificDeque

#ifdef __cplusplus 
extern "C" { 
#endif

#include "app/etl_type/ulisp.h" // makefloat

#ifdef __cplusplus 
} 
#endif

//#include <zephyr/logging/log.h>
//#include <zephyr/logging/log_core.h>
//#include <zephyr/logging/log_ctrl.h>

//#include "chain.h"            // line #98
//#include "type_traits.h"      // etl::is_same

//using namespace etl;

// forwards declaration of a templated type
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct pubsub_store;

// define an on_create_impl thing because you can't specialise a function, you can only specialise a class or struct
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct create_impl {
  static void on_create( pubsub_store<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE> &t ) {
    // generic implementation
    //if (false) NRF_LOG_INFO("generic %s on create!",__PRETTY_FUNCTION__);
    //NRF_LOG_INFO("cr %s on create!",(char*)base_store::names[MESSAGE_TYPE]);
  }
};

// define an on_create_impl thing because you can't specialise a function, you can only specialise a class or struct
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct count_impl {
  static int on_count( pubsub_store<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE> &t ) {
    // generic implementation

    //if (false) NRF_LOG_INFO("generic %s on count!",__PRETTY_FUNCTION__);
    return t.messageStore.size();
  }
};

// define an on_create_impl thing because you can't specialise a function, you can only specialise a class or struct
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct clear_impl {
  static void on_clear( pubsub_store<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE> &t ) {
    // generic implementation
    t.messageStore.clear();
    //if (false) NRF_LOG_INFO("generic %s on clear!",__PRETTY_FUNCTION__);
  }
};

// define an on_create_impl thing because you can't specialise a function, you can only specialise a class or struct
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct output_impl {
  static object* on_output( pubsub_store<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE> &t , int record ) {
    // generic implementation
    //if (false) NRF_LOG_INFO("generic %s on output!",__PRETTY_FUNCTION__);
    // needs a compromise to make work with floats c1si, c1op, c1of, c1tp
    //float example;
    //if ( etl::is_same< decltype(t.messageStore.at(record).data1), float > ) {
    /*
    if (MESSAGE_TYPE == MESSAGE_C1OP || MESSAGE_TYPE == MESSAGE_C1OF 
      || MESSAGE_TYPE == MESSAGE_C1TP || MESSAGE_TYPE == MESSAGE_C2OP
      || MESSAGE_TYPE == MESSAGE_C2TP || MESSAGE_TYPE == MESSAGE_C2OF) {
      float floatdata = (float)t.messageStore.at(record).data1;
      //char buffer[50]; // Allocate enough space for the result
      //floatToString(buffer, floatdata, 2);
      return cons (newsymbol(pack40((char*)base_store::names[MESSAGE_TYPE])), 
                      cons (makefloat(floatdata),NULL)
                );
    }
    */
    int data = t.messageStore.at(record).data1; //static_cast<ch1_max::ReadingStore*>(all_stores[etl_type])->at(record).data1;
    //if (false) LOG_INF("pubsub_store::on_output data1 = ", data);
    return cons (intern(pack40((char*)base_store::names[MESSAGE_TYPE])), 
                      cons (number(data),NULL)
                );

  } // on_output
};

// https://stackoverflow.com/questions/5688355/partial-specialisation-of-member-function-with-non-type-parameter
// specialising based on values


// define an on_create_impl thing because you can't specialise a function, you can only specialise a class or struct
// check for support of on_get, if so use smart code

// define a pubsub_store thing with an on_create in it
template <int BUS_SUBSCRIBERS, int STORED_MESSAGES, int MESSAGE_TYPE, typename MOCK_TYPE>
struct pubsub_store : etl_base {

  // constructor
  pubsub_store<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE> (){}

  typedef SingleMockValue<MOCK_TYPE> smv;
  smv mock;

  using Message = SingleMessage<MOCK_TYPE, MESSAGE_TYPE>;
  typedef SpecificDeque<Message, STORED_MESSAGES> MessageStore;
  MessageStore messageStore;

  etl::message_bus<BUS_SUBSCRIBERS> bus;

  void store_and_publish(smv) {
    // store it / create and process
    if (messageStore.full()) messageStore.pop_back();
    messageStore.emplace_front( mock.mock1 );              
    // pass it onward
    etl::send_message(bus, messageStore[0]);
  };

  void on_create(/*smv mock*/) /*override*/{

    //uint32_t ty = MESSAGE_TYPE;
    //InstrumentTimer temp = InstrumentTimer(ty); // include "chain.h" too

    //NRF_LOG_INFO("cr: type:%s, time:%i value:%i or " NRF_LOG_FLOAT_MARKER,(char*)base_store::names[MESSAGE_TYPE], chain::timing());
 
    store_and_publish(mock); // reasoning - if the create template override uses the value, it needs to be there already
    create_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>::on_create( * this );

  }

  void on_clear(void) /*override*/ {
    clear_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>::on_clear( * this );
  }

  int on_count(void) {
    return count_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>::on_count( * this );
  }

  object* on_output(int record ) {
    return output_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>::on_output( * this, record );
  }
  void set_mock (int value) {
    mock.mock1=value;
  }

  object* etlmock (smv _mock) {
    mock.mock1 = _mock.mock1;
    return number(MESSAGE_TYPE);
  }
  object* etlcreate() {
    //InstrumentTimer(MESSAGE_TYPE);
    //NRF_LOG_FLOAT_MARKER,NRF_LOG_FLOAT(phase)
    //if (false) NRF_LOG_INFO("etl cr type:%s int:%i float:" NRF_LOG_FLOAT_MARKER,(char*)base_store::names[MESSAGE_TYPE], mock.mock1, NRF_LOG_FLOAT(mock.mock1));
    on_create(/*mock.mock1*/);
    return number(MESSAGE_TYPE);
  }

  object* etlcount() {
    return number(messageStore.count());
  }
  object* etloutput(uint8_t generations_ago) {
    //HACK this means all MOCK_TYPEs need to be able to be translated in to uLisp number type.
    if (messageStore.current_size > generations_ago) {
      return number(messageStore.at(generations_ago));
    } else {
      //NRF_LOG_INFO("asking for output on a generation we don't have!");
      return number(-1);
    }
  }
  friend create_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>;
  friend clear_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>;
  friend count_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>;
  friend output_impl<BUS_SUBSCRIBERS,STORED_MESSAGES, MESSAGE_TYPE, MOCK_TYPE>;
};