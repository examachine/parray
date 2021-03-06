//
//
// C++ Interface for module: Block_Partitioning
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Block_Partitioning_Interface
#define Block_Partitioning_Interface

#include "General.hxx"
#include "Storage.hxx"

#include "Partition.hxx"

begin_namespace(Parallel_Arrays);

template <typename C, int _dimensions>
class Block_Partitioning : public Storage_Base<C, _dimensions>
{
public:  
  typedef Storage_Base<C, _dimensions> Base;
  typedef Block_Partitioning<C, _dimensions> Self;

  class Iterator {
  public:
    typedef Self Base;

    Iterator() {
    }

    Iterator(Pointer p) {
      pointer = p;
    }

    void next() {
      pointer++;
    }

    Reference refer() const {
      return *pointer;
    }

    // operators
    bool operator==(Iterator rhs) const {
      return pointer == rhs.pointer;
    }

    bool operator<=(Iterator rhs) const {
      return pointer <= rhs.pointer;
    }

    int operator-(Iterator rhs) const {
      return pointer - rhs.pointer;
    }

  private:
    Pointer pointer;
  };

  Block_Partitioning(Index_Space & indices) : index_space(indices) {
  }
    

  void allocate() {
//     nlog << node_str << "partitioning " << index_space << endl;
    num_components = index_space.sel_length() + 1;
    partition(index_space);
    memory = new Component[partition.num_local_components];
    last_component = &memory[partition.num_local_components-1];
    MPI_Win_create(reinterpret_cast<void*>(memory),
		   partition.num_local_components*sizeof(Component),
		   sizeof(Component),
		   MPI_INFO_NULL,
		   MPI_COMM_WORLD,
		   &mpi_win);
//     nlog << node_str << "local size " << partition.num_local_components
// 	 << endl;
  }

  typedef NUMA_Reference<Component> NUMA_Reference;

  NUMA_Reference reference(Index index) {
    pair<bool,int> i = partition.local_index(index);
    //       nlog << node_str << "partition local index for "
    // 	   << index << " is " << i << endl;
    if (i.first) {
      return NUMA_Reference(&ref_component(i.second));
    }
    else {
      int p = partition.processor(index);
      return NUMA_Reference(mpi_win, p, i.second);
    }
  }

  void free() {
    //       if (!MPI::Is_finalized())
    // 	MPI_Win_free(&mpi_win);
    //       if (!MPI_Win_free(&mpi_win))
    // 	throw Exception(node_str + "MPI window cannot be freed");
    delete[] memory;
  }

  int sel_num_components() const {
    return num_components;
  }

  Component & ref_component(int index) const {
    return memory[index];
  }

  const Component & sel_component(int index) const {
    return memory[index];
  }

  void mod_component(int index, const Component &obj) {
    memory[index] = obj;
  }

  Iterator begin() const {
    return Iterator(memory);
  }
  
  Iterator end() const {
    return Iterator(last_component);
  }

  const Block_Partition<_dimensions> &sel_partition() const {
    return partition;
  }

private:
  // Index space related
  Index_Space & index_space;
  int num_components;
  Block_Partition<_dimensions> partition;

  // Storage related
  Component* memory;
  Component* last_component;
  MPI_Win mpi_win;

  Self& operator =(const Self& rval) {
    //       nlog << "don't copy arrays" << endl;
    //       I(false);
    //       index_space = rval.index_space;
    //       num_components = rval.num_components;
    //       partition;

    //       // Storage related
    //       memory = rval.memory;
    //       last_component = rval.last_component;
    //       mpi_win = rval.mpi_win;
    return *this;
  }

};

end_namespace();

#endif
