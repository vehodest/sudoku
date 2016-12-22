#include <iostream>
#include <set>
#include <map>

struct Cell
{
   typedef char dataType;
   std::set<dataType> values;

   Cell()
   {
      for (dataType i = 1; i < 10; ++i)
      {
         values.insert(i);
      }
   }

   bool IsUniquely()
   {
      return values.size() == 1;
   }

   void MakeUniquely(dataType n)
   {
      values.clear();
      values.insert(n);
   }
};

class Data
{
public:
   void Init(char const* data)
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

   void Print()
   {
      unsigned __int64 variants = 1;
      for (size_t i = 0; i < 9; ++i)
      {
         for (size_t j = 0; j < 9; ++j)
         {
            Cell &current = Sudoku[i][j];
            if (current.IsUniquely())
               std::cout << "#" << static_cast<int>(*current.values.begin()) << "#";
            else
            {
               std::cout << /*Any*/ "(" << current.values.size() << ")";
               variants *= current.values.size();
            }

            std::cout << Space;
            if (j == 2 || j == 5)
               std::cout << Space;
         }
         std::cout << std::endl;
         if (i == 2 || i == 5)
            std::cout << std::endl;
      }
      std::cout << "Variants: " << variants << std::endl;
   }

   void Prepare()
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

   void Solve()
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

         if (!changes) break;
      }
   }

private:
   static const char Any = '*';
   static const char Space = ' ';

   Cell Sudoku[9][9]; //строка, столбец

   bool RemoveFromRow(size_t row, Cell::dataType value)
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

   bool RemoveFromCol(size_t col, Cell::dataType value)
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

   bool RemoveFromSquad(size_t row, size_t col, Cell::dataType value)
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

   bool CheckSquad(size_t squadRow, size_t squadCol)
   {
      bool changes = false;
      std::map<Cell::dataType, size_t> freq;

      //для каждого квадрата определяем частоту вхождения каждой цифры
      for (size_t i = 0; i < 3; ++i)
      {
         for (size_t j = 0; j < 3; ++j)
         {
            Cell& cell = Sudoku[squadRow*3 + i][squadCol*3 + j];
            if (cell.IsUniquely()) continue;

            for (auto iter = cell.values.begin(); iter != cell.values.end(); ++iter)
            {
               ++freq[*iter];
            }
         }
      }

      for (auto iter = freq.begin(); iter != freq.end(); ++iter)
      {
         if (iter->second != 1) continue;
         //если нашли цифру которая входит единожды -- ищем ее:
         for (size_t i = 0; i < 3; ++i)
         {
            for (size_t j = 0; j < 3; ++j)
            {
               size_t row = squadRow*3 + i;
               size_t col = squadCol*3 + j;
               Cell& cell = Sudoku[row][col];

               if (cell.values.count(iter->first) > 0)
               {
                  cell.MakeUniquely(iter->first);
                  /*RemoveFromCol(col, iter->first);
                  RemoveFromRow(row, iter->first);*/
                  return true;
               }
            }
         }
      }

      /*for (auto iter = freq.begin(); iter != freq.end(); ++iter)
      {
         std::cout << static_cast<unsigned int>(iter->first) << ") " << iter->second << std::endl;
      }//*/

      return false;
   }
};

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
   test.Print();

   /*test.Prepare();
   std::cout << std::endl << "Prepared:" << std::endl;
   test.Print();*/

   test.Solve();
   std::cout << std::endl << "Solved:" << std::endl;
   test.Print();
   return 0;
}