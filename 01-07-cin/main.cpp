#include "hwlib.hpp"

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   hwlib::cout << "Console input demo\n";
   
   for(;;){
      auto c = hwlib::cin.getc();
      hwlib::cout << "[" << c << "]\n";
   }
}
