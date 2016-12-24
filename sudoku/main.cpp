#include <iostream>
#include "Data.h"

int main(int argc, char **argv)
{
   Data test;
   test.Init(/*
             "* * *  * 3 7  6 * *"
             "* * *  6 * *  * 9 *"
             "* * 8  * * *  * * 4"

             "* 9 *  * * *  * * 1"
             "6 * *  * * *  * * 9"
             "3 * *  * * *  * 4 *"

             "7 * *  * * *  8 * *"
             "* 1 *  * * 9  * * *"
             "* * 2  5 4 *  * * *"//*/
             //*
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
             "8 * *  * * *  * * *"
             "* * 3  6 * *  * * *"
             "* 7 *  * 9 *  * 2 *"

             "* 5 *  * * 7  * * *"
             "* * *  * 4 5  7 * *"
             "* * *  1 * *  * 3 *"

             "* * 1  * * *  * 6 8"
             "* * 8  5 * *  * 1 *"
             "* 9 *  * * *  4 * *"//*/
             );
   std::cout << "Before:" << std::endl;
   test.Print(Data::PrintType::Compact);

   unsigned __int64 steps;
   bool result = test.Solve(steps);
   std::cout << std::endl << "After:" << std::endl;
   test.Print(Data::PrintType::Compact);

   std::cout << "Solved: " << (result ? "yes" : "no") << std::endl;
   std::cout << "Brutforce steps: " << steps << std::endl;
   std::cout << "Valid: " << (test.IsValid() ? "yes" : "NO!") << std::endl;
   return 0;
}