#include "chain.h"

namespace chain {
		
	ReadingStore 						readingStore;
	Router 							router;
	etl::message_bus<numberOfBusSubscribers> 		bus; 							
        TripleMockValue<dev_devchn>                                mock(0,0,0);

}

void chain::subscriptions(bool connect) {

  if (connect) {
    chain::bus.subscribe(chain::router);
  } else {
    chain::bus.unsubscribe(chain::router);
  }

}

void chain::on_create(TripleMockValue<dev_devchn> mock) {
  
  /*
    only collect chain records before a first step
  */
  if (int_bridgeb[CHAIN]==1) {

    if (readingStore.full()) 
      readingStore.pop_back();

    readingStore.emplace_front( mock.mock1, mock.mock2, mock.mock3 );

    //NRF_LOG_INFO("store link");

  } else {
    //NRF_LOG_INFO("throw link");
  }

}

uint32_t chain::timing() {
  return app_timer_cnt_get();
}

