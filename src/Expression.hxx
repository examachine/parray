//
//
// C++ Interface for module: Expression
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Expression_Interface
#define Expression_Interface

#include "General.hxx"
#include "Functions.hxx"

begin_namespace(Parallel_Arrays);

// forward declaration
template<typename A, typename B>
class Array_Add_Expression;

template<typename A, typename B>
class Array_Sub_Expression;

template<typename A, typename B>
class Array_Mul_Expression;

template<typename A>
class Array_Ref_Expression;

template<typename C, int d, typename SC>
class Array;

// template <typename Object>
// typename Object::Reference
// generic_reference(const Object & object )
// {
//   return object.generic_reference();
// }


template <typename Object>
struct Object_Traits
{
//   typedef typename Object::Expression Expression;
//   static typename Object::Expression generic_reference(Object &o) {
//     return o;
//   }
};

// Abstract Expression class
// put common expression functionality here
class Abstract_Expression
{
public:
  Abstract_Expression() {}
private:
};

// inline ostream& operator << (ostream& out, const Abstract_Expression& expr) {
//   out << "Abstract Expression instance " << endl;
// }

// Generic expression
// Adapts expressions to maintain type compatibility
// Nice way to designate an expression object
// This is needed because we can't ask for type compatibility
// when specifying a template parameter, otherwise we could
// simply ask for subclasses of Abstract_Expression
// Generic_Expression is *not* a subclass of Abstract_Expression
template <class _Expression>
class Gen_Expr
{
public:
  typedef _Expression Expression;
  typedef Gen_Expr<Expression> Self;
  
//   typedef typename Expression::Binary_Expression Binary_Expression;

//   Gen_Expr() {}

  Gen_Expr(Expression e) : expression(e) {
//     nlog << "gen_expr: cons <== " << expression << endl;
//     nlog << "gen_expr@" << this << endl;
  }

  Gen_Expr(const Self & x) : expression(x.expression) {
//     nlog << "gen_expr : copy cons" <<  expression << endl;     
//     nlog << "gen_expr@" << this << endl;
//     nlog << "gen_expr:" << *this << endl;
  }

  // copy from another generic expression
  void copy(const Self & x) {
    expression = x.expression;
  }

  Expression & operator*() {
    return expression;
  }
  Expression & operator->() {
    return expression;
  }

  // assignment to self
  Gen_Expr& operator =(const Self & x) {
    nlog << "gen_expr : memberwise assignment" /*<<  expr*/ << endl;     

    // delegate initialization
    expr.assign(x. expr);
    return *this;
  }

//   // assignment to expression
//   template<typename Gen_Expr, typename RHS_Expression>
//   void operator =(Gen_Expr<RHS_Expression> rhs_expr) {
//     // delegate assignment to <sub>class
//     nlog << "gen_expr : assignment to expr" << *this << ", " << rhs_expr << endl;
//     nlog << "NOT IMPLEMENTED " << endl;
//     //expr = rhs;
//   }

  // Generic expression delegates all operations to <sub>classes

  // Generic expression delegates assignment to other generic expression
  template<typename Gen_Expression>
  Gen_Expr& operator= (Gen_Expression rhs_expr) {
    // delegate assignment to <sub>class
//     nlog << "gen_expr : assignment to expr " << rhs_expr << endl;
    expression.assign(rhs_expr.expression);    
    return *this;
  }

//   // assignment to non-expression object
//   template <typename Object>
//   Gen_Expr& operator =(Object rhs ) {
//     // delegate assignment to <sub>class
//     nlog << "gen_expr : assignment to object " << *this << ", " << rhs << endl;
//     expression = rhs;
//   }
//   template <typename C, int d, typename SC>
//   Gen_Expr< Array_Add_Expression<Expression,
//    Array_Ref_Expression<const Array<C,d,SC> > > >
//   operator+ (const Array<C,d,SC> & array) {
// #error "not implemented"
//   }

  // addition with another expression
  template <typename RHS_Gen_Expr>
  Gen_Expr< Array_Add_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression > >
  operator+ (const RHS_Gen_Expr& gen_right);

  // sub another expression
  template <typename RHS_Gen_Expr>
  Gen_Expr< Array_Sub_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression > >
  operator- (const RHS_Gen_Expr& gen_right);

  // mul another expression
  template <typename RHS_Gen_Expr>
  Gen_Expr< Array_Mul_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression > >
  operator* (const RHS_Gen_Expr& gen_right);


//   // conversion operator
//   template <typename Object>
//   operator Object() {
//     // delegate conversion to <sub>class
//     return Object(expr);
//   }

  Expression expression;

};

// specialization for the generic expression class
template <typename Expr>
struct Object_Traits< Gen_Expr<Expr> >
{
  typedef Expr Expression;
  static Expr generic_reference(const Gen_Expr<Expr> &o) {
    return o.expression;
  }
};


template <typename Expression>
inline ostream& operator << (ostream& out,
			     const Gen_Expr<Expression>& expr) {
  out << "Gen_Expr[" << expr.expression << "]";
}

// Unary Expression class
template <typename _Object>
class Unary_Expression : public Abstract_Expression
{
public:
  typedef _Object Object;
  typedef Unary_Expression<Object> Expression;

  Unary_Expression() {}
  Unary_Expression(Object &o) : object(o) {}

//   Expression& operator=(const Expression& expr) {
//     nlog << "Unary_Expression assignment " << expr << endl;
//     object = expr.object;
//     return *this;
//   }

  const Object & sel_object() const {
    return object;
  }

  Object & ref_object() {
    return object;
  }

  Object  object;
private:
};

// Unary Reference Expression class
template <typename _Object>
class Unary_Ref_Expression : public Abstract_Expression
{
public:
  typedef _Object Object;
  typedef Unary_Ref_Expression<Object> Expression;

  Unary_Ref_Expression(Object &o) : object(o) {
//     cout << "ref@" << &o << endl; 
  }

//   Expression& operator=(const Expression& expr) {
//     nlog << "Unary_Ref_Expression assignment " << expr << endl;
//     object = expr.object;
//     return *this;
//   }

//   const Object & sel_object() const {
//     return object;
//   }

  const Object & sel_object() const {
//     cout << "ref@" << &object << endl; 
    return object;
  }

  Object & ref_object() {
//     cout << "ref@" << &object << endl; 
    return object;
  }

private:
  Object & object;
};

template <typename O>
ostream& operator << (ostream& out, const Unary_Ref_Expression<O>& expr) {
  out << "Ref_Expr[" <</* expr.object << */"@"
      << &expr.sel_object() << "]";
}

template <typename _Object1,typename  _Object2>
class Binary_Expression : public Abstract_Expression
{
public:
  typedef _Object1 Object1;
  typedef _Object2 Object2;
  
  Binary_Expression(Object1 & obj1, Object2 & obj2)
    : Obj1(obj1), Obj2(obj2) {}

  const Object1 & sel_object1() const {
//     cout << "ref1@" << &Ref1.sel_object() << endl; 
    return Obj1.sel_object();
  }

  const Object2 & sel_object2() const {
//     cout << "ref2@" << &Ref2.sel_object() << endl; 
    return Obj2.sel_object();
  }

  Object1 & ref_object1() {
    return Obj1.ref_object();
  }

  Object2 & ref_object2() {
    return Obj2.ref_object();
  }

  // shorthand for func
  Object1 & lhs() {
    return Obj1.ref_object();
  }  

  // shorthand for func
  Object2 & rhs() {
    return Obj2.ref_object();
  }

private:
  Unary_Expression<Object1> Obj1;
  Unary_Expression<Object2> Obj2;
};

template <typename O1, typename O2>
ostream& operator << (ostream& out, const Binary_Expression<O1,O2>& expr) {
  out << "Binary_Expression[" << expr.sel_object1() << ","
      << expr.sel_object2() << "]";
}

template <typename _Object1,typename  _Object2>
class Binary_Ref_Expression : public Abstract_Expression
{
public:
  typedef _Object1 Object1;
  typedef _Object2 Object2;
  
  Binary_Ref_Expression(Object1 & obj1, Object2 & obj2)
    : Ref1(obj1), Ref2(obj2) {}

  const Object1 & sel_object1() const {
//     cout << "ref1@" << &Ref1.sel_object() << endl; 
    return Ref1.sel_object();
  }

  const Object2 & sel_object2() const {
//     cout << "ref2@" << &Ref2.sel_object() << endl; 
    return Ref2.sel_object();
  }

  Object1 & ref_object1() {
    return Ref1.ref_object();
  }

  Object2 & ref_object2() {
    return Ref2.ref_object();
  }

  // shorthand for func
  Object1 & lhs() {
    return Ref1.ref_object();
  }  

  // shorthand for func
  Object2 & rhs() {
    return Ref2.ref_object();
  }

private:
  Unary_Ref_Expression<Object1> Ref1;
  Unary_Ref_Expression<Object2> Ref2;
};

template <typename O1, typename O2>
ostream& operator << (ostream& out, const Binary_Ref_Expression<O1,O2>& expr) {
  out << "Binary_Ref_Expression[" << expr.sel_object1() << ","
      << expr.sel_object2() << "]";
}

// An expression that adapts a constant value
// And a constant can be safely referenced
template <typename _Object>
class Constant_Expression : public Unary_Ref_Expression<const _Object>
{
public:
  Constant_Expression(const Object & o) :
    Unary_Ref_Expression<const _Object>(o) {}

};

template <typename O>
ostream& operator << (ostream& out, const Constant_Expression<O>& expr) {
  out << "Constant[" << expr.object << "]" << endl;
}

// A plain unary expression, but since it can
// query lvalue, it can perform assignments
template <typename _Object>
class Reference_Expression : public Unary_Ref_Expression<_Object>
{
public:
  Reference_Expression(_Object & o) :
    Unary_Ref_Expression<_Object>(o) {}

  // Assignment to an object
  Reference_Expression<Object>& operator =(Object rhs)
  {
//     nlog << &object << " := " << rhs << endl;
    object = rhs;
    return *this;
  }

  Object & operator*() {
    return sel_object;
  }
  Object & operator->() {
    return sel_object;
  }

};

template <typename O>
ostream& operator << (ostream& out, const Reference_Expression<O>& expr) {
  out << "Reference[" << expr.object_ptr << "]" << endl;
}

end_namespace();

#endif

