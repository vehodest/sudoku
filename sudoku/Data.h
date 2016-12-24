#pragma once
#include <map>
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
   bool Solve(unsigned __int64 &steps);
   bool Brutforce(unsigned __int64 &steps);
   bool IsValid() const;

private:
   static const char Any = '*';
   static const char Space = ' ';

   Cell Sudoku[9][9]; //строка, столбец

   //Вспомогательные методы для Prepare
   bool RemoveFromRow(size_t row, Cell::dataType value);
   bool RemoveFromCol(size_t col, Cell::dataType value);
   bool RemoveFromSquad(size_t row, size_t col, Cell::dataType value);

   //Вспомогательные методы для Solve
   bool CheckSquad(size_t squadRow, size_t squadCol);
   bool CheckRow(size_t row);
   bool CheckCol(size_t col);

   //Вспомогательные методы для Brutforce
   inline bool CanSetToCell(size_t row, size_t col, Cell::dataType value) const;
};