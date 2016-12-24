#pragma once
#include <map>
#include <vector>
#include "Cell.h"

class Data
{
public:
   enum PrintType
   {
      Compact = 0,
      Verbose = 1
   };

   void Init(char const* data);
   void Print(PrintType type = PrintType::Compact) const;
   void Prepare();
   bool Solve(bool useRecursion, unsigned long long &steps);
   bool Brutforce(unsigned long long &steps);
   bool RBrutforce(unsigned long long &steps);
   bool IsValid() const;

private:
   static const char Any = '*';
   static const char Space = ' ';
   static const size_t TableLength = 9;
   static const size_t SquareLength = 3;

   class BasePart
   {
   public:
      BasePart(size_t param, Cell (&sudoku)[TableLength][TableLength]):
         Sudoku(sudoku), Param(param)
      {}

      virtual Cell &operator[](size_t index) = 0;

   protected:
      Cell (&Sudoku)[TableLength][TableLength];
      const size_t Param;
   };

   class ColumnPart: public BasePart
   {
   public:
      ColumnPart(size_t column, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(column, sudoku)
      {}

      Cell &operator[](size_t index)
      {
         return Sudoku[index][Param];
      }
   };

   class RowPart: public BasePart
   {
   public:
      RowPart(size_t row, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(row, sudoku)
      {}

      Cell &operator[](size_t index)
      {
         return Sudoku[Param][index];
      }
   };

   class SquarePart: public BasePart
   {
   public:
      SquarePart(size_t squareIndex, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(squareIndex, sudoku)
      {}

      SquarePart(size_t squareRow, size_t squareCol, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(squareRow + squareCol*SquareLength, sudoku)
      {}

      Cell &operator[](size_t index)
      {
         size_t row = (Param % SquareLength)*SquareLength + index % SquareLength;
         size_t col = (Param / SquareLength)*SquareLength + index / SquareLength;
         return Sudoku[row][col];
      }
   };

   Cell Sudoku[TableLength][TableLength]; //������, �������

   //��������������� ������ ��� Prepare
   bool RemoveFrom(BasePart& part, Cell::dataType value);

   //��������������� ������ ��� Solve
   bool CheckFrom(BasePart& part);

   //��������������� ������ ��� IsValid
   static bool IsValidFrom(BasePart& part);

   //��������������� ������ ��� Brutforce
   static inline size_t GetRow(size_t i) { return i / TableLength; };
   static inline size_t GetCol(size_t i) { return i % TableLength; };

   inline bool CanSetToCell(size_t row, size_t col, Cell::dataType value) const;

   bool Recursion(std::vector<size_t>::iterator current,
                  std::vector<size_t>::iterator end,
                  unsigned long long &steps);
};