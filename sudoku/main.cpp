#include <iostream>
#include <chrono>
#include "Data.h"

int main(int argc, char **argv)
{
   Data test;
   test.Init(//*
             "* * *  * 3 7  6 * *"
             "* * *  6 * *  * 9 *"
             "* * 8  * * *  * * 4"

             "* 9 *  * * *  * * 1"
             "6 * *  * * *  * * 9"
             "3 * *  * * *  * 4 *"

             "7 * *  * * *  8 * *"
             "* 1 *  * * 9  * * *"
             "* * 2  5 4 *  * * *"//*/
             /*
             "* * *  1 * *  9 2 *"
             "* 6 1  * * *  * * *"
             "5 * *  * * *  8 * *"

             "* 4 *  * 1 *  5 * *"
             "* * *  * 8 *  * * 7"
             "6 1 *  * * 5  * * 3"

             "* 7 9  3 * *  * * 5"
             "* * 8  * * *  3 * *"
             "* * *  * * *  * 1 *"//*/
             /*
             "9 * *  * * 5  * * 1"
             "* * *  * 4 *  * * 8"
             "* * *  7 * *  3 2 *"

             "* 9 7  2 6 *  * 5 *"
             "* * *  * * *  * * *"
             "* 2 *  * 5 4  7 8 *"

             "* 4 1  * * 6  * * *"
             "3 * *  * 1 *  * * *"
             "6 * *  8 * *  * * 9"//*/
             /*
             "*5* 2*7 ***"
             "*** **9 4**"
             "7** **4 ***"

             "2*1 *5* **7"
             "*** 3** 1**"
             "**4 *9* *8*"

             "*49 *** 5*8"
             "**6 *** ***"
             "*1* *** *3*"//*/
             );
   std::cout << "Before:" << std::endl;
   test.Print(Data::PrintType::Compact);

   unsigned long long steps;
   
   auto start = std::chrono::high_resolution_clock::now();
   bool result = test.Solve(true, steps);
   auto end = std::chrono::high_resolution_clock::now();
   auto ms = std::chrono::duration<double, std::milli>(end-start).count();

   std::cout << std::endl << "After:" << std::endl;
   test.Print(Data::PrintType::Compact);

   std::cout << std::endl
             << "Result      : " << (result ? "solved" : "can't solve") << std::endl
             << "Valid       : " << (test.IsValid() ? "yes" : "no") << std::endl
             << "Performance : " << steps << " steps in " << ms << " ms" << std::endl;
   return 0;
}