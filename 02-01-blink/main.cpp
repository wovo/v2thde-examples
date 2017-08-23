#include "bmptk.h"
#include "hwlib.hpp"
#include "rtos.hpp"

class blinker : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   long long int delay;
   void main(){
HWLIB_TRACE;       
      for(;;){
         pin.set( 1 );
HWLIB_TRACE;             
         sleep( delay );
HWLIB_TRACE;             
         pin.set( 0 );
         sleep( delay );
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
      delay( delay )
   {}   
};

extern unsigned char bmptk_stack[ 81920 ];

extern unsigned int __stack_start;
extern unsigned int __stack_end;

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 ); 
   
   hwlib::cout << "blink (sleep) demo\n";   
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );   
   
   auto blinkl_led = blinker( "led_1", led_1, 200 * rtos::ms );
   rtos::run();
}