#include "pubsub_store.h" // pubsub_store
//#include "nano33ble.h" // RGBLED_Blue
//#include "nrf_gpio.h"


static void toggle_rgb(int rgb) {
  //LOG_INF("toggle_rgb");
  // toggle nrf52840 pin RGBLED_Blue
  //nrf_gpio_cfg_output(Blue_LED);
  //nrf_gpio_pin_toggle(Blue_LED);

  // Log the new state of the pin
  //bool pin_state = nrf_gpio_pin_read(Blue_LED);
  //NRF_LOG_INFO("New pin state: %d", pin_state);

}
//static void set_rbg(int rgb) {
  //LOG_INF("set_rbg %i", rgb);
  // set nrf52840 pin RGBLED_Blue to high
  // set nrf52840 pin RGBLED_Blue to high
  //nrf_gpio_pin_set(RGBLED_Blue);
//}

template <>
struct create_impl<def_drgb> {
  static void on_create( pubsub_store<def_drgb> &t ) {

    // c1re on_create
    //LOG_INF("drgb %i", t.mock.mock1);

          // store it / create and process
        if (t.messageStore.full()) t.messageStore.pop_back();
        t.messageStore.emplace_front( t.mock.mock1 ); 
			
      // pass it onwards
        etl::send_message(t.bus, t.messageStore[0]);

    toggle_rgb(t.mock.mock1);
  }
};

// note instantiations must be added to etl_setup & handler code to base_store.cpp too

