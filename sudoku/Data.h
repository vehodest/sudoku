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
   void Solve();
   bool IsValid() const;

private:
   static const char Any = '*';
   static const char Space = ' ';

   Cell Sudoku[9][9]; //строка, столбец

   bool RemoveFromRow(size_t row, Cell::dataType value);
   bool RemoveFromCol(size_t col, Cell::dataType value);
   bool RemoveFromSquad(size_t row, size_t col, Cell::dataType value);
   bool CheckSquad(size_t squadRow, size_t squadCol);
   bool CheckRow(size_t row);
   bool CheckCol(size_t col);
   void Brutforce();
};