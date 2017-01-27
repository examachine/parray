//
//
// C++ Interface for module: Functions
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Functions_Interface
#define Functions_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);


template <typename _Arg1, typename _Arg2, typename _Return>
class Binary_Function {
public:
  typedef _Arg1 Arg1;
  typedef _Arg2 Arg2;
  typedef _Return Return;

  Binary_Function() {
  }

  // function
//   Return operator()(Arg1 x, Arg2 y) {
//     nlog << "NOT IMPLEMENTED" << endl;
//     I(false);
//   }
};

template <typename _LValue, typename _RValue>
class Assign : public Binary_Function<_LValue &, _RValue &, void> {
public:
  typedef _LValue & LValue; typedef _RValue & RValue;
  Assign() {}
  void operator()(LValue lval, RValue rval) {
    //       cout << "Assign: " << &lval << " := " << &rval << endl;
    lval = rval;
  }
};

template <typename _Value>
class Add : public Binary_Function<_Value &, _Value &, _Value> {
public:
  typedef _Value Value;
  const static MPI::Op mpi_op() { return MPI::SUM; }

  Add() {}
  Return operator()(Arg1 lhs, Arg2  rhs) {
    //       cout << "Add: " << &lhs << " + " << &rhs << endl;
    return lhs + rhs;
  }
};


end_namespace();

#endif
