//
//
// C++ Interface for module: Algorithms
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Algorithms_Interface
#define Algorithms_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays)

template <typename Array>
void fill(Array & array, typename Array::Component x)
{
  typename Array::Traversal t(array);
  for (; t.in(); t.next()) {
//     nlog << node_str << "fill: " << t.refer() << " <- " << x <<
    //     endl;
//     cout << "fill: " << &t.refer() << endl;
    t.refer() = x;
  }
}

template <typename Array>
typename Array::Component sum(const Array & array)
{
  typename Array::Component sum = 0;
  typename Array::Traversal t(array);
  for (; t.in(); t.next()) {
    sum += t.refer();
  }
  return sum;
}

template <typename Array, typename Reduce_Fun>
typename Array::Component reduce(const Array & array)
{
  typedef typename Array::Component Component;
  Component local_value = 0;
  typename Array::Traversal t(array);
  Reduce_Fun fun;
  for (; t.in(); t.next()) {
    local_value = fun(local_value, t.refer());
  }
  Component global_value;
  MPI::COMM_WORLD.Reduce((void*)(&local_value) ,
			 (void*)(&global_value) ,
			 sizeof(Component),
			 MPI_Type<Component>::datatype(),
			 Reduce_Fun::mpi_op(),
			 0);
  return global_value;
}

end_namespace();

#endif
