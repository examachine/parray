//
//
// C++ Interface for module: Messages
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Messages_Interface
#define Messages_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);

  enum Message_Tags { load_tag, store_tag, obj_single_tag, obj_array_tag };

  template <typename X>
  struct MPI_Type {
  };

  template <>
  struct MPI_Type<double> {
    static const MPI::Datatype & datatype () { return  MPI::DOUBLE; }
  };

  template <>
  struct MPI_Type<int> {
    static const MPI::Datatype & datatype () { return  MPI::INT; }
  };

end_namespace();

#endif
