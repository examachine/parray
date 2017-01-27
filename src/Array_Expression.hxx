//
//
// C++ Interface for module: Array_Expression
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Array_Expression_Interface
#define Array_Expression_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);
  
// included before array

// each array expression object provides virtual iterators

//An array expression has some common
//typedefs
// template <typename Binary_Ref_Expression<Left,Right> >
// class Array_Expression
// {
// public:

//   typedef Binary_Ref_Expression<Left,Right> Base;
//   typedef Array_Add_Expression<Left,Right> Add_Expr;

//   // Assume component type will be of the right one's
//   typedef typename Left::Component Component;
// };


  // A plain unary expression, but since it can
  // query lvalue, it can perform assignments
template <typename _Array>
class Array_Reference_Expression
  : public Unary_Ref_Expression<_Array>
{
public:
  typedef _Array Array;
  typedef Array & Reference;
  typedef Unary_Ref_Expression< Array > Base;
  typedef Array_Reference_Expression<Array> Ref_Expr;
  typedef const Array_Reference_Expression<const Array> Const_Ref_Expr;
  typedef typename Array::Component Component;

//   typedef typename Array_Add_Expression Binary_Expression;

  Array_Reference_Expression() {}

  Array_Reference_Expression(Array& ref) : Base(ref) {
  }

  // Assignment to another array, trivial case
//   Ref_Expr &operator =(Const_Ref_Expr &rhs_expr) {
    
//     Reference lvalue = sel_object();
//     const Reference rvalue = rhs_expr.sel_object();

//     nlog << node_str << "Array " << &lvalue << " := "
// 	 << " Array " << &rvalue << endl;

//     lvalue.cons_binary_function_map(Assign<Component,Component>(),rvalue).apply();

//     return *this;
//   }

  // Assignment to an expression
  template <typename Array_Expression>
  void assign(const Array_Expression & _expression) {
    // for each element evaluate expression,
    // follow the iterators, and there you go!
    Array_Expression expression = const_cast<Array_Expression&>(_expression);
    start_map();
    expression.start_map();
    I(size() == expression.size());
    while ( in() ) {
      eval_comp() = expression.eval_comp();
    }
  }

  // Assignment to an object
  //     Ref_Expr& operator =(Object rhs)
  //     {
  //       Reference ref = object;
  //       nlog << node_str << ref << " := " << rhs << endl;
  //       if (ref.local)
  // 	*object.local_object = rhs;
  //       return *this;
  //     }

  Reference array() {
    sel_object();
  }

  // Dereference operators
  Reference operator*() {
    return sel_object();
  }
  Reference operator->() {
    return sel_object();
  }

  // Conversion operator
  //   operator Object() {
  //     Reference & ref = object;
  //     nlog << node_str << ref << " -<type>-> " << typeid(Object).name() << endl;

  //     Object obj;
  //     ref.broadcast(&obj);
  //     return obj;
  //   }

  int size() {
    return array().size();
  }

  void start_map() {
    map = array().cons_map();
  }
 
  Component & eval_comp() {
    Component & value = map.refer();
    map.next();
    return value;
  }

  bool in() {
    return map.in();
  }

private:
  typename Array::Map map;
};


// array addition expression
template <typename _Expr1, typename _Expr2>
class Array_Add_Expression
  : public Binary_Expression<_Expr1,_Expr2>
{
public:
  typedef _Expr1 Left;
  typedef _Expr2 Right;

  typedef Binary_Expression<Left, Right> Base;
  typedef Array_Add_Expression<Left,Right> Add_Expr;

  // Assume component type will be of the right one's
  typedef typename Left::Component Component;

  Array_Add_Expression() {
  }

  Array_Add_Expression(Left & l, Right & r) : Base(l,r) {
  }

  // Map
  int size() {
    I(lhs().size()==rhs().size());
    return lhs().size();
  }

  void start_map() {
    lhs().start_map();
    rhs().start_map();
  }

  Component eval_comp() {
    Component value = lhs().eval_comp() + rhs().eval_comp();
    return value;
  }

  bool in() {
    return lhs().in();
  }

};

template <typename O1, typename O2>
ostream& operator << (ostream& out, const Array_Add_Expression<O1,O2>& expr) {
  out << "Array_Add_Expression[" << expr.sel_object1() << ","
      << expr.sel_object2() << "]";
}



// array addition expression
template <typename _Expr1, typename _Expr2>
class Array_Sub_Expression
  : public Binary_Expression<_Expr1,_Expr2>
{
public:
  typedef _Expr1 Left;
  typedef _Expr2 Right;

  typedef Binary_Expression<Left, Right> Base;
  typedef Array_Sub_Expression<Left,Right> Add_Expr;

  // Assume component type will be of the right one's
  typedef typename Left::Component Component;

  Array_Sub_Expression() {
  }

  Array_Sub_Expression(Left & l, Right & r) : Base(l,r) {
  }

  // Map
  int size() {
    I(lhs().size()==rhs().size());
    return lhs().size();
  }

  void start_map() {
    lhs().start_map();
    rhs().start_map();
  }

  Component eval_comp() {
    Component value = lhs().eval_comp() - rhs().eval_comp();
    return value;
  }

  bool in() {
    return lhs().in();
  }

};

template <typename O1, typename O2>
ostream& operator << (ostream& out, const Array_Sub_Expression<O1,O2>& expr) {
  out << "Array_Sub_Expression[" << expr.sel_object1() << ","
      << expr.sel_object2() << "]";
}

// array addition expression
template <typename _Expr1, typename _Expr2>
class Array_Mul_Expression
  : public Binary_Expression<_Expr1,_Expr2>
{
public:
  typedef _Expr1 Left;
  typedef _Expr2 Right;

  typedef Binary_Expression<Left, Right> Base;
  typedef Array_Mul_Expression<Left,Right> Add_Expr;

  // Assume component type will be of the right one's
  typedef typename Left::Component Component;

  Array_Mul_Expression() {
  }

  Array_Mul_Expression(Left & l, Right & r) : Base(l,r) {
  }

  // Map
  int size() {
    I(lhs().size()==rhs().size());
    return lhs().size();
  }

  void start_map() {
    lhs().start_map();
    rhs().start_map();
  }

  Component eval_comp() {
    Component value = lhs().eval_comp() * rhs().eval_comp();
    return value;
  }

  bool in() {
    return lhs().in();
  }

};

template <typename O1, typename O2>
ostream& operator << (ostream& out, const Array_Mul_Expression<O1,O2>& expr) {
  out << "Array_Sub_Expression[" << expr.sel_object1() << ","
      << expr.sel_object2() << "]";
}

end_namespace();
#endif

