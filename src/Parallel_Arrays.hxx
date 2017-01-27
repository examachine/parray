//
//
// C++ Interface for module: Parallel_Arrays
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Parallel_Arrays_Interface
#define Parallel_Arrays_Interface

#include "General.hxx"

// namespace Parallel_Arrays {
begin_namespace(Parallel_Arrays);

void initialize();
void finalize();

extern int num_processors;
extern int processor_id;
static inline bool root_processor() {
  return processor_id == 0;
}
static inline bool last_processor() {
  return processor_id == num_processors-1;
}

extern string node_str;

// } // end namespace
end_namespace();

#include "Array.hxx"
#include "Expression_Operators.cxx"
#include "Array_Operators.hxx"
#include "Messages.hxx"
#include "Algorithms.hxx"
#include "Array_IO.hxx"

#endif
