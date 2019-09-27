#include "hwlib.hpp"

int main( void ){	
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );
   auto sw_1  = target::pin_in( target::pins::d43 );
   auto led_2 = target::pin_out( target::pins::d44 );
   auto sw_2  = target::pin_in( target::pins::d45 );
   auto led_3 = target::pin_out( target::pins::d46 );
   auto sw_3  = target::pin_in( target::pins::d47 );
   
   for(;;){
      sw_1.refresh();
      led_1.write( ! sw_1.read() );
      led_1.flush();
      
      sw_2.refresh();
      led_2.write( ! sw_2.read() );
      led_2.flush();
      
      sw_3.refresh();
      led_3.write( ! sw_3.read() );
      led_3.flush();
   }
}
