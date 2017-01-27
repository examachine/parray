//
//
// C++ Interface for module: Storage
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Storage_Interface
#define Storage_Interface

#include "General.hxx"

// namespace Parallel_Arrays {
begin_namespace(Parallel_Arrays);

template <typename C, int _dimensions>
class Storage_Base
{
public:  

  typedef C Component;
  typedef C* Pointer;
  typedef C& Reference;
  static const int dimensions = _dimensions;

  // BUG: these two are plain wrong!
  typedef int Index;
  typedef Interval<int, dimensions> Index_Space;


};



// } // namespace end
end_namespace();

#endif
