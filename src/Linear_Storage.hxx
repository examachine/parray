//
//
// C++ Interface for module: Linear_Storage
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Linear_Storage_Interface
#define Linear_Storage_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays)

  // Simple storage class, similar to STL allocators
  // Provides linear storage for homogeneous components
  template <typename C, int _dimensions>
class Linear_Storage : public Storage_Base<C, _dimensions>
{
public:  
  typedef Storage_Base<C, _dimensions> Base;
  typedef Linear_Storage<C, _dimensions> Self;

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

  private:
    Pointer pointer;
  };


  //     Linear_Storage(Index_Space indices) : index_space(indices) {
  //     }

  Linear_Storage(const Index_Space & indices) : index_space(indices) {
  }

  void allocate() {
    num_components = index_space.sel_length() + 1;
    memory = new Component[index_space.sel_length() +1 ];
    last_component = &memory[index_space.sel_length()];
  }

  void free() {
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

private:
  Component* memory;
  Component* last_component;

  const Index_Space & index_space;
  int num_components;

};


end_namespace();

#endif
