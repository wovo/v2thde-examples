// simple IR signal detector

#include "hwlib.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   namespace target = hwlib::target;
   
   auto tsop_signal = target::pin_in( target::pins::d8 );
   auto tsop_gnd    = target::pin_out( target::pins::d9 );
   auto tsop_vdd    = target::pin_out( target::pins::d10 );
   tsop_gnd.set( 0 );
   tsop_vdd.set( 1 );
   
   auto led         = target::pin_out( target::pins::led );
   
   auto const active = 100'000;
   auto last_signal = hwlib::now_us() - active;
   
   for(;;){
      if( tsop_signal.get() == 0 ){
         last_signal = hwlib::now_us();
      }
      led.set( ( last_signal + active) > hwlib::now_us() );
   }
}

