#include <stdio.h>
  //#include <iostream>
  
  //#ifdef M5
#include "./gem5/m5ops.h"
  
int main() {
  
    //m5_gadia_call(45683559);
    //m5_gadia_call(0, 1, 2, 0, 0, 0);
    //m5_receive(1);
    m5_gadia_receive(1);
  
    //printf("%ld", temp);
    //  std::cout << temp << std::endl;
    return 0;
 }
