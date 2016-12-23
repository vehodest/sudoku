#include "Cell.h"

Cell::Cell()
{
   for (dataType i = 1; i < 10; ++i)
   {
      values.insert(i);
   }
   search = values.end();
}

bool Cell::IsUniquely() const
{
   return values.size() == 1;
}

void Cell::MakeUniquely(dataType n)
{
   values.clear();
   values.insert(n);
}