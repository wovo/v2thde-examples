#include "hwlib.hpp"
#include "rtos.hpp"

class writer : public rtos::task<> {
private:
   rtos::mutex & mutex;
   hwlib::console & out;
   char c;
   
   void main(){
      for(;;){
         mutex.wait();
         out.clear();
         for( int i = 0; i < 10; i++ ){
            out << static_cast<char>( c + i );
            sleep( 1'000 * rtos::ms );
         }
         mutex.signal();
         sleep( 10'000 * rtos::ms );
      }
   }
   
public:
   writer( 
      const char * name, 
      rtos::mutex & mutex,
      hwlib::console & out,
      char c
   ):
      task( name ),
      mutex( mutex ),
      out( out ),
      c( c )
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "oled mutex demo\n";
   
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( sda, scl );
   auto pin_gnd = target::pin_out( target::pins::d19 );
   pin_gnd.set( 1 );
   auto pin_vcc = target::pin_out( target::pins::d18 );
   pin_vcc.set( 0 );
   
   auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
   auto w1 = hwlib::window_part( 
      oled, 
      hwlib::location( 0, 0 ),
      hwlib::location( 128, 32));
   auto w2 = hwlib::window_part( 
      oled, 
      hwlib::location( 0, 32 ),
      hwlib::location( 128, 32));
      
   auto font = hwlib::font_default_8x8();
   auto d1 = hwlib::window_ostream( w1, font );
   auto d2 = hwlib::window_ostream( w2, font );
   
   auto m1 = rtos::mutex( "m1" );
   auto m2 = rtos::mutex( "m2" );
   
   (void)d2;(void)m2;
   
   auto writer_1 = writer( "writer_1", m1, d1, 'a' );
   auto writer_2 = writer( "writer_2", m1, d2, 'A' );
   rtos::run();
}
