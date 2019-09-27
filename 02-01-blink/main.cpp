#include "hwlib.hpp"
#include "rtos.hpp"

class blinker : public rtos::task<> {
private:
   hwlib::pin_out & pin;
   long long int delay;
   void main(){
      for(;;){
         pin.write( 1 );             
         pin.flush();
         hwlib::wait_ms( delay );          
         pin.write( 0 );
         pin.flush();
         hwlib::wait_ms( delay );
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
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 ); 
   
   hwlib::cout << "blink (sleep) demo\n";   
   
   namespace target = hwlib::target;
   auto led_1 = target::pin_out( target::pins::d42 );   
   
   auto blinkl_led = blinker( "led_1", led_1, 200 );
   rtos::run();
}