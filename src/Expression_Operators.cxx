//
//
// C++ Implementation for module: Expression_Operators
//
// Description: 
//
//
// Author: exa
//
//

// #include "Expression_Operators.hxx"

template <typename _Expression> template <typename RHS_Gen_Expr>
Gen_Expr< Array_Add_Expression<_Expression,
  typename Object_Traits<RHS_Gen_Expr>::Expression> >
Gen_Expr<_Expression>::operator+ (const RHS_Gen_Expr& gen_right)
{
  typedef typename Object_Traits<RHS_Gen_Expr>::Expression RHS;
  typedef Array_Add_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression> Add_Expression;
  typedef Gen_Expr<Add_Expression> Gen_Add;
  
  RHS right = Object_Traits<RHS_Gen_Expr>::reference
    (const_cast<RHS_Gen_Expr&>(gen_right));
  
//   nlog << node_str << *this << " + " << right << endl;
  Gen_Add return_expr(Add_Expression(expression, right));
  return return_expr;
}

template <typename _Expression> template <typename RHS_Gen_Expr>
Gen_Expr< Array_Sub_Expression<_Expression,
  typename Object_Traits<RHS_Gen_Expr>::Expression> >
Gen_Expr<_Expression>::operator- (const RHS_Gen_Expr& gen_right)
{
  typedef typename Object_Traits<RHS_Gen_Expr>::Expression RHS;
  typedef Array_Sub_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression> Sub_Expression;
  typedef Gen_Expr<Sub_Expression> Gen_Sub;
  
  RHS right = Object_Traits<RHS_Gen_Expr>::reference
    (const_cast<RHS_Gen_Expr&>(gen_right));
  
//   nlog << node_str << *this << " - " << right << endl;
  Gen_Sub return_expr(Sub_Expression(expression, right));
  return return_expr;
}

template <typename _Expression> template <typename RHS_Gen_Expr>
Gen_Expr< Array_Mul_Expression<_Expression,
  typename Object_Traits<RHS_Gen_Expr>::Expression> >
Gen_Expr<_Expression>::operator* (const RHS_Gen_Expr& gen_right)
{
  typedef typename Object_Traits<RHS_Gen_Expr>::Expression RHS;
  typedef Array_Mul_Expression<Expression,
    typename Object_Traits<RHS_Gen_Expr>::Expression> Mul_Expression;
  typedef Gen_Expr<Mul_Expression> Gen_Mul;
  
  RHS right = Object_Traits<RHS_Gen_Expr>::reference
    (const_cast<RHS_Gen_Expr&>(gen_right));
  
//   nlog << node_str << *this << " * " << right << endl;
  Gen_Mul return_expr(Mul_Expression(expression, right));
  return return_expr;
}
