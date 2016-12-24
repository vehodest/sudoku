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
      size_t row = number / TableLength;
      size_t col = number % TableLength - 1;
      Cell::dataType value = *cur - '1' + 1;
      Sudoku[row][col].MakeUniquely(value);
   }
}

void Data::Print(PrintType type) const
{
   static const char * uSpace[] = {"", "#"};

   for (size_t i = 0; i < TableLength; ++i)
   {
      if (i != 0 && i % 3 == 0)
         std::cout << std::endl;

      for (size_t j = 0; j < TableLength; ++j)
      {
         if (j != 0 && j % 3 == 0)
            std::cout << Space;

         Cell const &current = Sudoku[i][j];
         if (current.IsUniquely())
            std::cout << uSpace[type] << static_cast<int>(*current.values.begin()) << uSpace[type];
         else
         {
            if (type == Data::PrintType::Compact)
               std::cout << Any;
            else
               std::cout << "(" << current.values.size() << ")";
         }

         std::cout << Space;
      }
      std::cout << std::endl;
   }
}

void Data::Prepare()
{
   while(true)
   {
      bool changes = false;

      for (size_t row = 0; row < TableLength; ++row)
      {
         for(size_t col = 0; col < TableLength; ++col)
         {
            Cell &cell = Sudoku[row][col];
            if (cell.IsUniquely())
            {
               Cell::dataType value = *cell.values.begin();
               
               RowPart rowPart(row, Sudoku);
               bool rowRemove = RemoveFrom(rowPart, value);

               ColumnPart colPart(col, Sudoku);
               bool colRemove = RemoveFrom(colPart, value);

               SquarePart squarePart(row / SquareLength, col / SquareLength, Sudoku);
               bool squareRemove = RemoveFrom(squarePart, value);

               if (rowRemove || colRemove || squareRemove)
                  changes = true;
            }
         }
      }

      if (!changes) break;
   }
}

bool Data::Solve(bool useRecursion, unsigned long long &steps)
{
   while(true)
   {
      bool changes = false;
      
      Prepare();
      for (size_t i = 0; i < TableLength; ++i)
      {
         SquarePart part(i, Sudoku);
         if (CheckFrom(part))
            changes = true;
      }

      Prepare();
      for (size_t i = 0; i < TableLength; ++i)
      {
         RowPart part(i, Sudoku);
         if (CheckFrom(part))
            changes = true;
      }

      Prepare();
      for (size_t i = 0; i < TableLength; ++i)
      {
         ColumnPart part(i, Sudoku);
         if (CheckFrom(part))
            changes = true;
      }

      if (!changes) break;
   }

   return useRecursion ? RBrutforce(steps) : Brutforce(steps);
}

bool Data::IsValid() const
{
   for (size_t i = 0; i < TableLength; ++i)
   {
      RowPart rowPart(i, const_cast<Cell (&)[TableLength][TableLength]>(Sudoku));
      ColumnPart columnPart(i, const_cast<Cell (&)[TableLength][TableLength]>(Sudoku));
      SquarePart squarePart(i, const_cast<Cell (&)[TableLength][TableLength]>(Sudoku));
      if (!IsValidFrom(rowPart) ||
          !IsValidFrom(columnPart) ||
          !IsValidFrom(squarePart))
      {
         return false;
      }
   }

   return true;
}

bool Data::IsValidFrom(BasePart& part)
{
   bool freq[10] = { 0 };
   for (size_t i = 0; i < TableLength; ++i)
   {
      auto const &cell = part[i];
      if (cell.IsUniquely())
      {
         if (freq[*cell.values.begin()] == true)
            return false;
         else
            freq[*cell.values.begin()] = true;
      }
   }
   return true;
}

bool Data::RemoveFrom(BasePart& part, Cell::dataType value)
{
   bool changes = false;

   for (size_t i = 0; i < TableLength; ++i)
   {
      Cell& cell = part[i];
      if (!cell.IsUniquely() && cell.values.count(value) > 0)
      {
         cell.values.erase(value);
         changes = true;
      }
   }

   return changes;
}

bool Data::CheckFrom(BasePart& part)
{
   bool changes = false;

   //Подсчет частот для каждой цифры (уже определенные цифры не учатсвуют)
   std::map<Cell::dataType, size_t> freq; //контейнер для частот
   for (size_t i = 0; i < TableLength; ++i)
   {
      Cell& cell = part[i];

      if (cell.IsUniquely()) continue;
      for (auto const value: cell.values)
         ++freq[value];
   }

   //Для каждой единичной частоты выставляем определенное значение:
   for (auto const pair: freq)
   {
      if (pair.second != 1 ) continue;

      const Cell::dataType& digit = pair.first;
      for (size_t i = 0; i < TableLength; ++i)
      {
         Cell& cell = part[i];

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

inline bool Data::CanSetToCell(size_t row, size_t col, Cell::dataType value) const
{
   static auto isCoincides = [](Cell const& cell, Cell::dataType value) -> bool
   {
      if (cell.IsUniquely() && *cell.values.begin() == value)
         return false;

      if (cell.search != cell.values.end() && *cell.search == value)
         return false;

      return true;
   };

   //Проверка строки и столбца
   for (size_t i = 0; i < TableLength; ++i)
   {
      //проверка строки
      if (i != col && !isCoincides(Sudoku[row][i], value))
         return false;

      //проверка столбца
      if (i != row && !isCoincides(Sudoku[i][col], value))
         return false;
   }

   //Проверка квадрата
   size_t squareRow = row / SquareLength;
   size_t squareCol = col / SquareLength;
   for (size_t i = SquareLength*squareRow; i < SquareLength*squareRow + SquareLength; ++i)
   {
      for (size_t j = SquareLength*squareCol; j < SquareLength*squareCol + SquareLength; ++j)
      {
         if (i != row && j != col && !isCoincides(Sudoku[i][j], value))
            return false;
      }
   }

   return true;
}

bool Data::Brutforce(unsigned long long &steps)
{
   std::vector<size_t> values; //номера ячеек с неопределенными значениями
   //Заполнение адресов с неопределенными значениями и выставление указателей
   for(size_t i = 0; i < 81; ++i)
   {
      Cell &cell = Sudoku[GetRow(i)][GetCol(i)];
      if (cell.IsUniquely())
         continue;
      values.push_back(i);
      cell.search = cell.values.end();
   }

   //Поиск перебором
   std::vector<size_t>::iterator current = values.begin();
   bool isNext = true; //флаг о том, что перешли к следующему, а не предыдущему элементу
   steps = 0; //количество шагов в поиске решения
   while (current != values.end())
   {
      //Определение текущей ячейки
      size_t row = GetRow(*current);
      size_t col = GetCol(*current);
      Cell &cell = Sudoku[row][col];

      if (isNext) //если передвинулись на следующую ячейку - начинаем перебор сначала
         cell.search = cell.values.begin();
      else //если передвинулись назад - начинаем перебор со следующего элемента
         ++cell.search;

      isNext = false;
      while(cell.search != cell.values.end())
      {
         ++steps;
         if (CanSetToCell(row, col, *cell.search)) //ищем возможное значение
         {
            //двигаться вперед
            isNext = true;
            break;
         }
         ++cell.search;
      }

      if (isNext) //двигаемся вперед
         ++current;
      else //двигаемся назад
      {
         if (current == values.begin()) //если двигатсья некуда, то решение невозможно
            return false;
         --current;
      }
   }

   //Расстановка найденых значений
   for(size_t i = 0; i < 81; ++i)
   {
      Cell &cell = Sudoku[GetRow(i)][GetCol(i)];
      if (cell.IsUniquely())
         continue;
      cell.MakeUniquely(*cell.search);
   }

   return true;
}

bool Data::RBrutforce(unsigned long long &steps)
{
   std::vector<size_t> values; //номера ячеек с неопределенными значениями
   //Заполнение адресов с неопределенными значениями и выставление указателей
   for(size_t i = 0; i < 81; ++i)
   {
      Cell &cell = Sudoku[GetRow(i)][GetCol(i)];
      if (cell.IsUniquely())
         continue;
      values.push_back(i);
      cell.search = cell.values.end();
   }

   steps = 0; //количество шагов в поиске решения
   bool result = Recursion(values.begin(), values.end(), steps);
   
   //Расстановка найденых значений
   for(size_t i = 0; i < 81; ++i)
   {
      Cell &cell = Sudoku[GetRow(i)][GetCol(i)];
      if (cell.IsUniquely())
         continue;
      cell.MakeUniquely(*cell.search);
   }

   return result;
}

bool Data::Recursion(std::vector<size_t>::iterator current, std::vector<size_t>::iterator end,
                     unsigned long long &steps)
{
   if (current == end)
      return true;

   //Определение текущей ячейки
   size_t row = GetRow(*current);
   size_t col = GetCol(*current);
   Cell &cell = Sudoku[row][col];

   for(cell.search = cell.values.begin(); cell.search != cell.values.end(); ++cell.search)
   {
      ++steps;
      if (CanSetToCell(row, col, *cell.search)) //ищем возможное значение
      {
         //двигаться вперед
         if (Recursion(current + 1, end, steps))
            return true;
      }
   }

   return false;
}