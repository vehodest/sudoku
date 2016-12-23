#pragma once
#include <set>

struct Cell
{
   typedef char dataType;
   typedef std::set<dataType> valuesContainer;
   valuesContainer values;
   valuesContainer::const_iterator search;

   Cell();
   bool IsUniquely() const;
   void MakeUniquely(dataType n);
};