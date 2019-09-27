#include "hwlib.hpp"
#include "rtos.hpp"

class writer : public rtos::task<> {
private:
   rtos::mutex & mutex;
   hwlib::terminal & out;
   char c;
   
   void main(){
      for(;;){
         mutex.wait();
         out.clear();
         for( int i = 0; i < 10; i++ ){
            out << static_cast<char>( c + i );
            hwlib::wait_ms( 1'000 );
         }
         mutex.signal();
         hwlib::wait_ms( 10'000 );
      }
   }
   
public:
   writer( 
      const char * name, 
      rtos::mutex & mutex,
      hwlib::terminal & out,
      char c
   ):
      task( name ),
      mutex( mutex ),
      out( out ),
      c( c )
   {}   
};

int main( void ){	
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "oled mutex demo\n";
   
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( sda, scl );
   
   auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
   auto w1 = hwlib::part( 
      oled, 
      hwlib::xy( 0, 0 ),
      hwlib::xy( 128, 32));
   auto w2 = hwlib::part( 
      oled, 
      hwlib::xy( 0, 32 ),
      hwlib::xy( 128, 32));
      
   auto font = hwlib::font_default_8x8();
   auto d1 = hwlib::terminal_from( w1, font );
   auto d2 = hwlib::terminal_from( w2, font );
   
   auto m1 = rtos::mutex( "m1" );
   auto m2 = rtos::mutex( "m2" );
   
   (void)d2;(void)m2;
   
   auto writer_1 = writer( "writer_1", m1, d1, 'a' );
   auto writer_2 = writer( "writer_2", m1, d2, 'A' );
   rtos::run();
}
