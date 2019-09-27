#include "hwlib.hpp"

int main( void ){	
   
   // IR output LED
   auto ir = hwlib::target::d2_36kHz();
   
   // red output LED
   auto red = hwlib::target::pin_out( hwlib::target::pins::d42 );
   
   // switch which enables the 36 kHz output
   auto sw = hwlib::target::pin_in( hwlib::target::pins::d43 );
   
   // when the switch is pressed, 
   // repeat sending 1 ms signal and 1 ms silence
   // and show this on the LED
   for(;;){
      hwlib::wait_ms( 1 ); 
      
      sw.refresh();
      ir.write( ! sw.read() );
      red.write( ! sw.read() );
      ir.flush();
      red.flush();
      
      hwlib::wait_ms( 1 );
      ir.write( 0 );
      ir.flush();
   }
}

