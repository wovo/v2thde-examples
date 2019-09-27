#include "hwlib.hpp"

int main( void ){	
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   hwlib::cout << "Console input demo\n";
   
   for(;;){
      auto c = hwlib::cin.getc();
      hwlib::cout << "[" << c << "]\n";
   }
}
