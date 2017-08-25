#include "bmptk.h"
#include "hwlib.hpp"
#include "rtos.hpp"

class led_driver : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   rtos::flag enable_flag;
   
   void main(){
      for(;;){                   
         wait( enable_flag );              
         pin.set( 1 );           
         hwlib::wait_ms( 200 );       
         pin.set( 0 );         
      }
   }   
   
public:
   void enable(){        
      enable_flag.set();
   }

   led_driver(  
      hwlib::pin_out & pin
   ):
      task( "led_driver" ),
      pin( pin ),
      enable_flag( this, "enable_flag " )
   {}   
};

class blinker : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   rtos::clock blink_clock;
   led_driver & driver;
   
   void main(){
      for(;;){         
         wait( blink_clock );   
         driver.enable();        
      }
   }
   
public:
   blinker( 
      led_driver & driver
   ):
      task( "blinker" ),
      pin( pin ),
      blink_clock( this, 1'000 * rtos::ms, "blink_clock" ),
      driver( driver )
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "flag demo\n";
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );   
   
   auto driver = led_driver( led_1 );
   auto blink = blinker( driver );
   rtos::run();
}