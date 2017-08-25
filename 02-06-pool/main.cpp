#include "hwlib.hpp"
#include "rtos.hpp"

class logger : public rtos::task<> {
private:

   rtos::mutex count_mutex;
   rtos::pool< int > count;
   
   void main(){
      for(;;){
         hwlib::wait_ms( 1'000 );
         count_mutex.wait();
         hwlib::cout << count.read() << "\n";
         count_mutex.signal();         
      }
   }
   
public:
   logger( ):
      task( "logger" ),
      count_mutex( "counter_mutex" ),
      count( "count_pool" )
   {
         count.write( 0 );
   }   
   
   void increment(){
      count_mutex.wait();
      count.write( count.read() + 1 );
      count_mutex.signal();      
   }
};

class counter : public rtos::task<> {
private:

   logger & log;
   
   void main(){
      for(;;){
         hwlib::wait_ms( 800 );
         log.increment();
      }
   }
   
public:
   counter( 
      logger & log
   ):
      task( "counter" ),
      log( log )
   {}   
};

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   // wait for the PC console to start
   hwlib::wait_ms( 500 );
   
   hwlib::cout << "pool demo\n";
   
   namespace target = hwlib::target;
   
   auto log = logger();
   auto count = counter( log );
   rtos::run();
}
