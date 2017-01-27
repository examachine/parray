//
//
// C++ Interface for module: Array
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Array_Interface
#define Array_Interface

#include "General.hxx"

#include "Interval.hxx"
#include "Storage.hxx"
#include "Expression.hxx"
#include "Component_Reference.hxx"
#include "Storage_Classes.hxx"
#include "Array_Expression.hxx"

begin_namespace(Parallel_Arrays);

class Linear_Storage_Class;

// General purpose array class
// Supports 1 and 2 dimensional arrays
template <typename C, int _dimensions = 1,
  typename SC = Linear_Storage_Class >
class Array
{
public:
  typedef C Component;
  static const int dimensions = _dimensions;
  typedef SC Storage_Class;
  typedef typename Storage_Class::Traits<C, _dimensions>::Storage Storage;
  typedef int Index;
  //typedef Range
  typedef Array<Component, dimensions, Storage_Class> Self;
  typedef Array<Component, dimensions, Storage_Class> _Array;
  typedef Array_Reference_Expression<_Array> Array_Ref_Expr;
  typedef Array_Reference_Expression<_Array> Const_Array_Ref_Expr;

  typedef Array_Ref_Expr Reference;
  typedef Const_Array_Ref_Expr Const_Reference;
  
  // Traversal type
  friend class Traversal;
  class Traversal
  {
  public:
    typedef Self Base;
    //   typedef typename Base::Component Component;
    typedef typename Base::Storage::Iterator Iterator;

    Traversal() {}

    Traversal(const Base& container)
//       : container(&c)
    {
      iter = container.storage.begin();
      iter_end = container.storage.end();
    }

    Traversal& operator=(const Traversal &t ) {
      iter = t.iter;
      iter_end = t.iter_end;
    }

    void next() {
      I(iter <= iter_end);
      iter.next();
    }

    bool in() {
      return iter <= iter_end;
    }

    Component & refer() {
      return iter.refer();
    }

    int length() {
      return iter_end - iter + 1;
    }

  private:
//     const Base * container;
    Iterator iter;
    Iterator iter_end;
  };

  typedef Traversal Map;
  
  Map cons_map() {
    return Map(*this);
  }

  template <class _Function>
  class Function_Map : Traversal {
  public:
    typedef _Function Function;
    //       typedef typename Function::Return Return;

    Function_Map(Base & c) : Traversal(c) {}

    void apply() {
      for (; in(); next()) {
	f(refer());
      }
    }

    // apply
    void operator() () {
      apply();
    }

  private:
    Function f;
  };

  template <class Function>
  Function_Map<Function> cons_function_map() {
    return Function_Map<Function>(*this);
  }

  template <class _Function>
  class Binary_Function_Map  {
  public:
    typedef _Function Function;
    typedef Self Base;
    //       typedef typename Function::Return Return;

    Binary_Function_Map(const Base &l, const Base &r) : left(l), right(r) {
      // Assert that both have the same size
      I(l.size()==r.size());
    }

    void apply() {
      for (; left.in() /*&& right.in()*/; left.next(),right.next()) {
	f(left.refer(), right.refer());
      }
    }

    // apply
    void operator() () {
      apply();
    }

  private:
    Function f;
    Traversal left;
    Traversal right;
  };

  template <class Function>
  Binary_Function_Map<Function> cons_binary_function_map(Function func,
							 const Self&
							 another)
							 const {
    return Binary_Function_Map<Function>(*this, another);
  }

  friend class Loader;
  class Loader;

  Loader cons_loader() const {
    return Loader(this);
  }

  friend class Storer;
  class Storer;

  Storer cons_storer() const {
    return Storer(this);
  }

  Array(int size)
    : range(0,size-1), // C Style arrays by default
    storage(range)
  {
    storage.allocate();
  }

  Array(int size, const Component& value)
    : range(0,size-1),  // C Style arrays by default
    storage(range)
  {
    storage.allocate(num_components);
    for (int i = 0; i < size; i++) {
      mod_component(i, value);
    }
  }

  Array(const Component * c_array, int size )
    : range(0, size-1),
      storage(range)
  {
    storage.allocate();
    Loader loader = cons_loader();
    int array_size = size;
    int index = 0;
    while (!loader.eos()) {
      if (root_processor()) {
	Linear_Storage<Component, _dimensions>::Iterator
	  iter = loader.buffer.begin(), iter_end = loader.buffer.end();
	for (;iter <= iter_end && index<array_size;
	     iter.next(),index++) {
	  iter.refer() = c_array[index];
	}
      }
      loader.load();
    }
  }

  ~Array() {
    storage.free();
  }

  // component selector/modifier
  // no bounds checking
  // not supported...

  const Component & sel_component(Index index) const {
    return storage.ref_component(index);
  }

  const Component & scomp(Index index) const {
    return storage.ref_component(index);
  }

  void mod_component(Index index, const Component & value) {
    storage.mod_component(index, value);
  }

  void mcomp(Index index, const Component & value) {
    storage.mod_component(index, value);
  }

  Component & ref_component(Index index) {
    return storage.ref_component(index);
  }

  Component & comp(Index index) const {
    return storage.comp(index);
  }

  Index first() const {
    return range.sel_first();
  }

  Index last() const {
    return range.sel_last();
  }

  int size() const {
    return storage.sel_num_components();
  }

  const Storage & sel_storage() const {
    return storage;
  }

  //     Component & operator[] (Index index) {
  //       return storage.ref_component(index);
  //     }

  //     typedef NUMA_Reference_Expression<Component> Comp_Ref;

  // subscripting operators / component access

  typedef NUMA_Reference<Component> Comp_Ref;
  typedef NUMA_Reference_Expression<Component> Comp_Ref_Expr;
  Comp_Ref_Expr operator[] (Index index) {
    return Comp_Ref_Expr(storage.reference(index));
  }

  // assignment operators

  //     typedef Array_Reference<Component> Array_Ref;

  //     Gen_Expr<Const_Array_Ref_Expr> operator = (const _Array & array) {
  //       nlog << "array assignment " << endl;
  //       return Gen_Expr<Const_Array_Ref_Expr>( Const_Array_Ref_Expr(array) );
  //     }

  // Assignment to another array
  Self& operator =(const _Array & rvalue) {
//     nlog << "array := array " << endl;
    // delegate assignment to array reference expression
    Array_Ref_Expr lvalue(*this);
    Array_Ref_Expr rvalue_expr(const_cast<Array&>(rvalue));
    lvalue.assign(rvalue_expr);
    return *this;
  }

  // Assignment to a generic expression
  template <typename RHS_Gen_Expr>
  Self& operator =(const RHS_Gen_Expr & gen_expr) {
//     nlog << "array := gen_expr " << endl;
    // delegate assignment to generic expression
    Gen_Expr<Array_Ref_Expr> lvalue(Array_Ref_Expr(*this));
    lvalue = const_cast<RHS_Gen_Expr &>(gen_expr);
    return *this;
  }

  operator Gen_Expr<Array_Ref_Expr>() {
    nlog << "converting array to Gen_Expr " << endl;
    return Gen_Expr<Array_Ref_Expr>(*this);
  }

  // returns a generic expr object
  Gen_Expr<Array_Ref_Expr> generic_reference()  {
    return Gen_Expr<Array_Ref_Expr>(Array_Ref_Expr(*this));
  }

  // returns array reference object
  Array_Ref_Expr reference() {
    return Array_Ref_Expr(*this);
  }

  // returns a generic constant expr object
  Gen_Expr<Const_Array_Ref_Expr> generic_const_reference() const {
    return Gen_Expr<Const_Array_Ref_Expr>(Const_Array_Ref_Expr(*this));
  }

  // returns constant array reference object
  Const_Array_Ref_Expr const_reference() const {
    return Const_Array_Ref_Expr(*this);
  }
  
  // Addition with another array
  // Makes the leaf nodes of an expression tree!
  Gen_Expr< Array_Add_Expression<Reference,Reference> >
   operator +(const _Array & right) {
//     nlog << "array + array " << endl;
    // delegate addition to generic expression
    Gen_Expr<Reference> left(Reference(*this));
    return left + right;
  }  

  // Subtract another array
  // Makes the leaf nodes of an expression tree!
  Gen_Expr< Array_Sub_Expression<Reference,Reference> >
   operator -(const _Array & right) {
//     nlog << "array - array " << endl;
    // delegate addition to generic expression
    Gen_Expr<Reference> left(Reference(*this));
    return left - right;
  }

  // Multiply with another array
  // Makes the leaf nodes of an expression tree!
  Gen_Expr< Array_Mul_Expression<Reference,Reference> >
   operator *(const _Array & right) {
//     nlog << "array * array " << endl;
    // delegate addition to generic expression
    Gen_Expr<Reference> left(Reference(*this));
    return left * right;
  }

protected:

private:
  Range range;
  Storage storage;
};

template <typename C, int d, class SC>
struct Object_Traits<Array<C,d,SC> >
{
  typedef typename Array<C,d,SC>::Reference Expression;
  typedef typename Array<C,d,SC>::Const_Reference Const_Expression;
  typedef Gen_Expr<Expression> Generic_Expression;

  static Expression reference
  (Array<C,d,SC> &o) {
    return o.reference();
  }

  static Const_Expression const_reference
  (const Array<C,d,SC> &o) {
    return o.const_reference();
  }

  static Generic_Expression generic_reference
  (Array<C,d,SC> &o) {
    return o.generic_reference();
  }
};


end_namespace();

#endif


