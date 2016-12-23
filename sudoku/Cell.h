#pragma once
#include <set>

struct Cell
{
   typedef char dataType;
   std::set<dataType> values;

   Cell();

   bool IsUniquely() const;

   void MakeUniquely(dataType n);
};