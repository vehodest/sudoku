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
             "*2* 9*3 ***"
             "357 *** ***"
             "*** 6** *38"

             "**9 *84 ***"
             "*** *9* 3*6"
             "7*3 **6 *4*"

             "*** *3* 891"
             "*35 1** *2*"
             "29* 47* ***"//*/
             );
   std::cout << "Before:" << std::endl;
   test.Print(Data::PrintType::Compact);

   unsigned __int64 steps;
   
   auto start = std::chrono::high_resolution_clock::now();
   bool result = test.Solve(steps);
   auto end = std::chrono::high_resolution_clock::now();
   
   std::cout << std::endl << "After:" << std::endl;
   test.Print(Data::PrintType::Compact);

   std::cout << "Solved: " << (result ? "yes" : "no") << std::endl;
   std::cout << "Brutforce steps: " << steps << std::endl;
   std::cout << "Valid: " << (test.IsValid() ? "yes" : "NO!") << std::endl;
   std::cout << "Duration: " << std::chrono::duration<double, std::milli>(end-start).count() << " ms" << std::endl;
   return 0;
}