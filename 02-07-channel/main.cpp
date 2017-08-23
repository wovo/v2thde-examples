#include "hwlib.hpp"
#include "rtos.hpp"

class logger : public rtos::task<> {
private:

   rtos::channel< char, 1024 > buffer;
   
   void main(){
      for(;;){
         auto c = buffer.read();
         hwlib::cout << c;
      }
   }
   
public:
   logger( ):
      task( "logger" ),
      buffer( this, "character buffer" )
   { }   
   
   void add( char c ){
      buffer.write( c );     
   }
   void add( const char * s ){
      while( *s != '\0' ){
         buffer.write( *s++ );     
      }
   }
};

class writer : public rtos::task<> {
private:

   hwlib::pin_out & pin;
   logger & log;
   rtos::clock blink_clock;
   rtos::clock log_clock;
   
   void main(){
      bool level = 0;
      for(;;){
         auto event = wait( blink_clock + log_clock );
         if( event == blink_clock ){
            level = ! level;
            pin.set( level ); 
         } else {
            log.add( "Hello\n" );
         }
      }
   }
   
public:
   writer( 
      hwlib::pin_out & pin,
      logger & log
   ):
      task( "writer" ),
      pin( pin ),
      log( log ),
      blink_clock( this, 100 * rtos::ms, "blink_clock"),
      log_clock( this, 1'000 * rtos::ms, "log_clock")
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "channel demo\n";
   
   namespace target = hwlib::target;
   
   auto led = target::pin_out( target::pins::d42 );      
   
   auto log = logger();
   auto write = writer( led, log );
   rtos::run();
}
