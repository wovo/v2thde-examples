#include "hwlib.hpp"


int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );
   auto sw_1  = target::pin_in( target::pins::d43 );
   auto led_2 = target::pin_out( target::pins::d44 );
   auto sw_2  = target::pin_in( target::pins::d45 );
   auto led_3 = target::pin_out( target::pins::d46 );
   auto sw_3  = target::pin_in( target::pins::d47 );
   
   for(;;){
      led_1.set( ! sw_1.get() );
      led_2.set( ! sw_2.get() );
      led_3.set( ! sw_3.get() );
   }
}
