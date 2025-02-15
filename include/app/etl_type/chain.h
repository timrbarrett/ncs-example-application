#pragma once

#include "algorithm.h"        // etl::move
#include "deque.h"            // etl::deque
#include "message_bus.h"      // from etl
#include "message_enum.h"     // enumeration of trb defined messages
#include "trbmessage.h"          // TripleMessage
#include "MockValue.h"        // SingleMockValue
#include "base_store.h"       // SpecificDeque
//#include "app_timer.h"        // cnt_get
/*
	chain is called directly in the on_receive method of every trb defined type
        it subscribes to its own bus, it publishes nothing, 
        the on_recieve method stores the message_enum number (and, perhaps, the timer tick.)
	
*/
namespace chain {

const int numberOfBusSubscribers = 1;       // itself to bus
const int numberOfReadings = 300;             // the longest chain...

extern TripleMockValue<dev_devchn> mock;

using Reading = TripleMessage<dev_devchn, MESSAGE_CHAIN_LINK>;

// 1b) a global, permanent store for the last #n Reading messages
typedef SpecificDeque<Reading, numberOfReadings> ReadingStore;
extern ReadingStore readingStore;

// 2) the accelerometerBus
extern etl::message_bus<numberOfBusSubscribers> bus;

void on_create(TripleMockValue<dev_devchn> mock);
// 3a) the accelerometerRouter that processes the received message
class Router : public etl::message_router<Router, chain::Reading> {
public:
  typedef etl::message_router<Router, chain::Reading> Base_t;
  using Base_t::receive;

  // Construct the router with an id of 0.
  Router()
      : message_router(0) {
  }

  // when bus receives msg,
  // it pull values into mock
  // on receive triggers true values always
  // note: calling on_create directly triggers the mock value
  // then calls on_create

  void on_receive(const chain::Reading &msg) {
    mock.mock1 =msg.data1;
    mock.mock2 =msg.data2;
    mock.mock3 =msg.data3;
    on_create(mock);

    NRF_LOG_INFO("type:%s, time:%i", (char*)base_store::names[mock.mock1], mock.mock2);
  }

  // Received an unknown message.
  void on_receive_unknown( const etl::imessage &msg) {
    //globalbp.print("on_receive_unknown");
  }

};

// 3b) the global representing the router
extern Router router; // say it's going to be instantiated in another file

void subscriptions(bool create);
uint32_t timing();
}

class InstrumentTimer
{

  uint32_t m_startAt;
  int32_t m_type;
  bool m_stopped;

  public:
    InstrumentTimer(int32_t type)
      : m_stopped(false), m_type(type)
      {
        m_startAt=0//app_timer_cnt_get();
      } // end constructor

    ~InstrumentTimer()
    { 
      uint32_t m_endAt = 0//app_timer_cnt_get();
      
      /*
        concern: the TripleMockValue may leak memory
        Perhaps I need to fix a TripleMock in memory and constantly reuse it.
      */
      chain::on_create(TripleMockValue<dev_devchn>(m_type, m_startAt, m_endAt));
      if (false) NRF_LOG_INFO("LOG: %i %i %i",m_type, m_startAt, m_endAt);
      m_stopped=true;
    } // end destructor
}; // end class InstrumentTimer