//
//
// C++ Interface for module: Partition
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Partition_Interface
#define Partition_Interface

#include "General.hxx"

// A Partition class maps an index space to a local index space
// for each processor. 

// namespace Parallel_Arrays {
begin_namespace(Parallel_Arrays);

template <int _dimensions>
class Block_Partition
{
public:
  static const int dimensions = _dimensions;
  //typedef _Index_Space Index_Space;
  typedef Interval<int, dimensions> Index_Space;
 
  void operator() (const Index_Space & index_space) {
    // do modulo based block partitioning
    Interval<int, 1> range = index_space; // 1 dimensional
    first_index = range.sel_first();
    total_size = range.sel_length() + 1;
    block_size = total_size / num_processors;
    //       nlog << node_str << "total size " << total_size << " block_size " <<
    // 	block_size << endl;
    local_index_space = 
      Index_Space (0, !last_processor() ? block_size - 1
		   : (total_size - block_size*(num_processors-1)) - 1 );
    part_index_space =
      Index_Space(first_index + block_size * processor_id,
		  first_index + block_size * processor_id +
		  local_index_space.sel_length());
    //       nlog << node_str << "part " << part_index_space << endl;
    num_local_components = local_index_space.sel_length() + 1;
    // last block is irregular
    irregular_block_start = block_size * (num_processors-1);
  }
    
  // given a subspace, find a local index subspace
  pair<bool, Index_Space> local_indices(const Index_Space &
					index_subspace) const {
    pair<bool, Index_Space> part_subspace;
    part_subspace = part_indices(index_subspace);
    //       nlog << node_str << "part subspace" << part_subspace << endl;
    if (part_subspace.first) {
      Range & range = part_subspace.second;
      range = Range(local_index(range.sel_first()).second,
		    local_index(range.sel_last()).second);
    }
    //       nlog << node_str << "part subspace(local) " << part_subspace << endl;
    return part_subspace;
  }

  // given a subspace, find someone else's local index subspace
  pair<bool, Index_Space> local_indices(const Index_Space &
					index_subspace, int proc_id) const {
    pair<bool, Index_Space> part_subspace;
    part_subspace = part_indices(index_subspace, proc_id);
    //       nlog << node_str << "part subspace" << part_subspace << endl;
    if (part_subspace.first) {
      Range & range = part_subspace.second;
      range = Range(local_index(range.sel_first()).second,
		    local_index(range.sel_last()).second);
    }
    //       nlog << node_str << "part subspace(local) " << part_subspace << endl;
    return part_subspace;
  }

  // find the local index given a global index
  pair<bool, int> local_index(int index) const {
    // do modulo based block partitioning
    if (part_index_space.contains(index)) {
      index -= first_index;
      if (index < irregular_block_start)
	return pair<bool,int>(true, index % block_size);
      else
	return pair<bool,int>(true, index - irregular_block_start); 
    }
    else
      return pair<bool,int>(false, 0);
  }

  // find a processor given a global index
  int processor(int index) const {
    return index/block_size;
  }

  pair<bool, Index_Space> part_indices(const Index_Space &
				       index_subspace) const {
    //       nlog << node_str << "index_subspace " << index_subspace << endl;
    //       nlog << node_str << "part " << part_index_space << endl;
    return part_index_space.intersection(index_subspace);
  }

  // given a subspace, find someone else's subspace
  pair<bool, Index_Space> part_indices(const Index_Space &
				       index_subspace, int proc_id) const {
    //       nlog << node_str << "index_subspace " << index_subspace << endl;
    int part_size = (proc_id!=num_processors-1) ? block_size
      : (total_size - block_size*(num_processors-1)) ;
    //       nlog << "part_size " << part_size << endl;
    Index_Space part_index_space (first_index + block_size * proc_id,
				  first_index + block_size * proc_id +
				  part_size - 1);
    //       nlog << node_str << "part for " << proc_id << " " << part_index_space << endl;
    return part_index_space.intersection(index_subspace);
  }

  Index_Space part_index_space;
  Index_Space local_index_space;
  int num_local_components;
  int first_index;
  int total_size;
  int block_size;
  int irregular_block_start;
};

// } // namespace end
end_namespace();

#endif
