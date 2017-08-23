#include "bmptk.h"
#include "hwlib.hpp"
#include "rtos.hpp"

class blinker : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   long long int delay;
   rtos::timer blink_timer;
   
   void main(){
      bool level = 0;
      for(;;){
         blink_timer.set( delay );
         wait( blink_timer );
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
      delay( delay ),
      blink_timer( this, "blink_timer")
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "timer demo\n";   
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );   
   
   auto blinkl_led = blinker( "led_1", led_1, 200 * rtos::ms );
   rtos::run();
}
