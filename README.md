# sudoku
_solve sudoku for fun_

This is small C++ program for solve Sudoku. First of all program will simplify Sudoku and then will solve it using simple search.
Program was tested on **Visual Studio 12 Update 5** and **g++ 5.4.0**.

## How set your own Sudoku
1. Open `sudoku/main.cpp` file.
2. Pass as first parameter in method `test.Init` your Sudoku as string in following format:
   ```
   "* * *  * 3 7  6 * *"
   "* * *  6 * *  * 9 *"
   "* * 8  * * *  * * 4"
   
   "* 9 *  * * *  * * 1"
   "6 * *  * * *  * * 9"
   "3 * *  * * *  * 4 *"

   "7 * *  * * *  8 * *"
   "* 1 *  * * 9  * * *"
   "* * 2  5 4 *  * * *"
   ```
3. Use number to set a certain digit or asterisk `*` to set unknown digit.
4. Build program and run it for solve.
