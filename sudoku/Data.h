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
   bool Solve(bool useRecursion, unsigned __int64 &steps);
   bool Brutforce(unsigned __int64 &steps);
   bool RBrutforce(unsigned __int64 &steps);
   bool IsValid() const;

private:
   static const char Any = '*';
   static const char Space = ' ';
   static const size_t TableLength = 9;
   static const size_t SquadLength = 3;

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

   class SquadPart: public BasePart
   {
   public:
      SquadPart(size_t squadIndex, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(squadIndex, sudoku)
      {}

      SquadPart(size_t squadRow, size_t squadCol, Cell (&sudoku)[TableLength][TableLength]):
         BasePart(squadRow + squadCol*SquadLength, sudoku)
      {}

      Cell &operator[](size_t index)
      {
         size_t row = (Param % SquadLength)*SquadLength + index % SquadLength;
         size_t col = (Param / SquadLength)*SquadLength + index / SquadLength;
         return Sudoku[row][col];
      }
   };

   Cell Sudoku[TableLength][TableLength]; //строка, столбец

   //Вспомогательные методы для Prepare
   static bool RemoveFrom(BasePart& part, Cell::dataType value);

   //Вспомогательные методы для Solve
   static bool CheckFrom(BasePart& part);

   //Вспомогательные методы для IsValid
   static bool IsValidFrom(BasePart& part);

   //Вспомогательные методы для Brutforce
   static inline size_t GetRow(size_t i) { return i / TableLength; };
   static inline size_t GetCol(size_t i) { return i % TableLength; };

   inline bool CanSetToCell(size_t row, size_t col, Cell::dataType value) const;

   bool Recursion(std::vector<size_t>::iterator current,
                  std::vector<size_t>::iterator end,
                  unsigned __int64 &steps);
};