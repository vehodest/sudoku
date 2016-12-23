#include <iostream>
#include "Data.h"

void Data::Init(char const* data)
{
   size_t number = 0;

   for (char const *cur = data; *cur != 0; ++cur)
   {
      if (!((*cur >= '1' && *cur <= '9') || *cur == Any)) continue;

      ++number;
      if (*cur == Any) continue;
      size_t row = number / 9;
      size_t col = number % 9 - 1;
      Cell::dataType value = *cur - '1' + 1;
      Sudoku[row][col].MakeUniquely(value);
   }
}

void Data::Print(PrintType type) const
{
   static const char * uSpace[] = {"", "#"};

   unsigned __int64 variants = 1;
   for (size_t i = 0; i < 9; ++i)
   {
      for (size_t j = 0; j < 9; ++j)
      {
         Cell const &current = Sudoku[i][j];
         if (current.IsUniquely())
            std::cout << uSpace[type] << static_cast<int>(*current.values.begin()) << uSpace[type];
         else
         {
            if (type == Data::PrintType::Compact)
            {
               std::cout << Any;
            }
            else
            {
               std::cout << "(" << current.values.size() << ")";
               variants *= current.values.size();
            }
         }

         std::cout << Space;
         if (j == 2 || j == 5)
            std::cout << Space;
      }
      std::cout << std::endl;
      if (i == 2 || i == 5)
         std::cout << std::endl;
   }
   std::cout << "Variants: " << std::hex << variants << std::endl;
}

void Data::Prepare()
{
   while(true)
   {
      bool changes = false;

      for (size_t row = 0; row < 9; ++row)
      {
         for(size_t col = 0; col < 9; ++col)
         {
            Cell &cell = Sudoku[row][col];
            if (cell.IsUniquely())
            {
               Cell::dataType value = *cell.values.begin();
               bool rowRemove = RemoveFromRow(row, value);
               bool colRemove = RemoveFromCol(col, value);
               bool sqadRemove = RemoveFromSquad(row, col, value);

               if (rowRemove || colRemove || sqadRemove)
                  changes = true;
            }
         }
      }

      if (!changes) break;
   }
}

void Data::Solve()
{
   while(true)
   {
      Prepare();

      bool changes = false;

      for (size_t i = 0; i < 3; ++i)
      {
         for(size_t j = 0; j < 3; ++j)
         {
            if (CheckSquad(i, j))
               changes = true;
         }
      }

      Prepare();
      for (size_t i = 0; i < 9; ++i)
      {
         if (CheckRow(i))
            changes = true;
      }

      Prepare();
      for (size_t i = 0; i < 9; ++i)
      {
         if (CheckCol(i))
            changes = true;
      }

      if (!changes) break;
   }
}

bool Data::IsValid() const
{
   bool freq[10];
   //Проверка строк и столбцов
   for (size_t step = 0; step < 2; ++step)
   {
      for (size_t i = 0; i < 9; ++i)
      {
         memset(freq, 0, sizeof(freq));
         for (size_t j = 0; j < 9; ++j)
         {
            auto const &cell = (step == 0 ? Sudoku[i][j] : Sudoku[j][i]);
            if (cell.IsUniquely())
            {
               if (freq[*cell.values.begin()] == true)
                  return false;
               else
                  freq[*cell.values.begin()] = true;
            }
         }
      }
   }

   //Проверка квадратов
   for (size_t squadi = 0; squadi < 3; ++squadi)
   {
      for (size_t squadj = 0; squadj < 3; ++squadj)
      {
         memset(freq, 0, sizeof(freq));
         for(size_t i = 0; i < 3; ++i)
         {
            for(size_t j = 0; j < 3; ++j)
            {
               auto const &cell = Sudoku[squadi*3 + i][squadj*3 + j];
               if (cell.IsUniquely())
               {
                  if (freq[*cell.values.begin()] == true)
                     return false;
                  else
                     freq[*cell.values.begin()] = true;
               }
            }
         }
      }
   }

   return true;
}

bool Data::RemoveFromRow(size_t row, Cell::dataType value)
{
   bool changes = false;

   for (size_t i = 0; i < 9; ++i)
   {
      Cell& cell = Sudoku[row][i];
      if (cell.IsUniquely()) continue;
      if (cell.values.count(value) > 0)
      {
         cell.values.erase(value);
         changes = true;
      }
   }

   return changes;
}

bool Data::RemoveFromCol(size_t col, Cell::dataType value)
{
   bool changes = false;

   for (size_t i = 0; i < 9; ++i)
   {
      Cell& cell = Sudoku[i][col];
      if (cell.IsUniquely()) continue;
      if (cell.values.count(value) > 0)
      {
         cell.values.erase(value);
         changes = true;
      }
   }

   return changes;
}

bool Data::RemoveFromSquad(size_t row, size_t col, Cell::dataType value)
{
   bool changes = false;
   size_t squadRow = row / 3;
   size_t squadCol = col / 3;

   for (size_t i = 0; i < 3; ++i)
   {
      for (size_t j = 0; j < 3; ++j)
      {
         Cell& cell = Sudoku[squadRow*3 + i][squadCol*3 + j];
         if (cell.IsUniquely()) continue;
         if (cell.values.count(value) > 0)
         {
            cell.values.erase(value);
            changes = true;
         }
      }
   }

   return changes;
}

bool Data::CheckSquad(size_t squadRow, size_t squadCol)
{
   bool changes = false;

   //Подсчет частот для каждой цифры в квадрате (уже определенные цифры не учатсвуют)
   std::map<Cell::dataType, size_t> freq; //контейнер для частот
   for (size_t i = 0; i < 3; ++i)
   {
      for (size_t j = 0; j < 3; ++j)
      {
         size_t row = squadRow * 3 + i;
         size_t col = squadCol * 3 + j;
         Cell& cell = Sudoku[row][col];

         if (cell.IsUniquely()) continue;
         for (auto const value: cell.values)
            ++freq[value];
      }
   }

   //Для каждой единичной частоты выставляем определенное значение:
   for (auto const pair: freq)
   {
      if (pair.second != 1 ) continue;

      const Cell::dataType& digit = pair.first;
      for (size_t i = 0; i < 9; ++i)
      {
         size_t row = squadRow * 3 + i % 3;
         size_t col = squadCol * 3 + i / 3;
         Cell& cell = Sudoku[row][col];

         if (cell.values.count(digit) > 0)
         {
            cell.MakeUniquely(digit);
            changes = true;
            break;
         }
      }
   }

   return changes;
}

bool Data::CheckRow(size_t row)
{
   bool changes = false;

   //Подсчет частот для каждой цифры в строке (уже определенные цифры не учатсвуют)
   std::map<Cell::dataType, size_t> freq; //контейнер для частот
   for (size_t i = 0; i < 9; ++i)
   {
      Cell& cell = Sudoku[row][i];

      if (cell.IsUniquely()) continue;
      for (auto const value: cell.values)
         ++freq[value];
   }

   //Для каждой единичной частоты выставляем определенное значение:
   for (auto const pair: freq)
   {
      if (pair.second != 1 ) continue;

      const Cell::dataType& digit = pair.first;
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[row][i];

         if (cell.values.count(digit) > 0)
         {
            cell.MakeUniquely(digit);
            changes = true;
            break;
         }
      }
   }

   return changes;
}

bool Data::CheckCol(size_t col)
{
   bool changes = false;

   //Подсчет частот для каждой цифры в строке (уже определенные цифры не учатсвуют)
   std::map<Cell::dataType, size_t> freq; //контейнер для частот
   for (size_t i = 0; i < 9; ++i)
   {
      Cell& cell = Sudoku[i][col];

      if (cell.IsUniquely()) continue;
      for (auto const value: cell.values)
         ++freq[value];
   }

   //Для каждой единичной частоты выставляем определенное значение:
   for (auto const pair: freq)
   {
      if (pair.second != 1 ) continue;

      const Cell::dataType& digit = pair.first;
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[i][col];

         if (cell.values.count(digit) > 0)
         {
            cell.MakeUniquely(digit);
            changes = true;
            break;
         }
      }
   }

   return changes;
}
