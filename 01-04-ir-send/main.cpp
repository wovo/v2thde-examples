#include "hwlib.hpp"

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC terminal to start
   hwlib::wait_ms( 500 );
   
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
      ir.set( ! sw.get() );
      red.set( ! sw.get() );
      
      hwlib::wait_ms( 1 );
      ir.set( 0 );
   }
}

