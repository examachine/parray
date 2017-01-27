//
//
// C++ Interface for module: Storage_Classes
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Storage_Classes_Interface
#define Storage_Classes_Interface

#include "General.hxx"

#include "Storage.hxx"
#include "Linear_Storage.hxx"
#include "Block_Partitioning.hxx"

// namespace Parallel_Arrays {
begin_namespace(Parallel_Arrays);

// template <typename Array>
// class Storage_Class
// {
// public:
//   typedef typename Array::Component Component;
//   static const int dimensions = Array::dimensions;
// };


class Linear_Storage_Class
{
public:
  template <typename Component, int dimensions>
  class Traits {
  public:
    typedef Linear_Storage<Component, dimensions> Storage;
  };
};

class Block_Partitioning_Class
{
public:
  template <typename Component, int dimensions>
  class Traits {
  public:
    typedef Block_Partitioning<Component, dimensions> Storage;
  };
};

// } // namespace end
end_namespace();

#endif
