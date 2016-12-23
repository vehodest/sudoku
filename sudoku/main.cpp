#include <iostream>
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
             "* * *  * * *  * * *"
             "* * *  * * *  * * *"
             "* * *  * * *  * * *"

             "* * *  1 2 3  * * *"
             "* * *  4 * 6  * * *"
             "* * *  7 8 9  * * *"

             "* * *  * * *  * * 5"
             "* * *  * * *  3 * *"
             "* * *  * * *  * * *"//*/
             );
   std::cout << "Before:" << std::endl;
   test.Print(Data::PrintType::Compact);

   test.Solve();
   std::cout << std::endl << "After:" << std::endl;
   test.Print(Data::PrintType::Verbose);

   std::cout << "Valid: " << (test.IsValid() ? "yes" : "NO!") << std::endl;
   return 0;
}