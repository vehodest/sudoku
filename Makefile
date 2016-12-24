all:
	g++ -std=c++11 -Ofast -o mysudoku sudoku/Cell.cpp sudoku/Data.cpp sudoku/main.cpp
