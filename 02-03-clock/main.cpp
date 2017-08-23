#include "bmptk.h"
#include "hwlib.hpp"
#include "rtos.hpp"

class blinker : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   rtos::clock blink_clock;
   
   void main(){
      bool level = 0;
      for(;;){
HWLIB_TRACE;	  
         wait( blink_clock );
HWLIB_TRACE;		 
         level = ! level;
         pin.set( level );
      }
   }
public:
   blinker( 
      const char * name, 
      hwlib::pin_out & pin, 
      long long int delay 
   ):
      task( name ),
      pin( pin ),
      blink_clock( this, delay, "blink_clock" )
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "clock demo\n";
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );   
   
   auto blinkl_led = blinker( "led_1", led_1, 200 * rtos::ms );
   rtos::run();
}