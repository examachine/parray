//
//
// C++ Interface for module: Component_Reference
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Component_Reference_Interface
#define Component_Reference_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);

template <class Object>
class NUMA_Reference {
public:
  NUMA_Reference() {}

  NUMA_Reference(Object* object)
    : local(true), local_object(object) {}

  NUMA_Reference(MPI_Win _win, int _remote_proc, int _win_offset)
    : local(false), win(_win), remote_proc(_remote_proc),
      win_offset(_win_offset) {}

  // this has somewhat funny semantics
  void broadcast(Object *object) {
    // everybody wants
    int root;
    if (local) {
      *object = *local_object;
      root = processor_id;
    }
    else
      root = remote_proc;
    MPI::COMM_WORLD.Bcast(reinterpret_cast<void*>(object),
			  sizeof(Object), MPI::CHAR, root);
		    
  }

  void send(int dest_id) {
//     nlog << node_str << "send to " << dest_id << endl;
    MPI::COMM_WORLD.Send(reinterpret_cast<void*>(local_object),
			 sizeof(Object), MPI::CHAR, dest_id, obj_single_tag);
  }

  void receive(Object *object, int src_id) {
    MPI::Status status;
    MPI::COMM_WORLD.Recv(reinterpret_cast<void*>(object),
			 sizeof(Object), MPI::CHAR, src_id,
			 obj_single_tag, status);
  }

  // one sided, totally brain dead
  void get(Object* object) {
    MPI_Win_fence(0, win);
    if (!local)
      MPI_Get(reinterpret_cast<void*>(object),
	      sizeof(Object), MPI_CHAR,
	      remote_proc, win_offset, sizeof(Object), MPI_CHAR,
	      win);
    MPI_Win_fence(0, win);
    nlog << "get done! " << endl;
  }

  bool local;
  Object* local_object;

  MPI_Win win;
  int remote_proc;
  int win_offset;
  
};

template <typename O>
ostream& operator << (ostream& out, const NUMA_Reference<O>& ref) {
  out << "NUMA_Ref[";
  if (ref.local) {
    out << "local:" << ref.local_object;
  }
  else {
    out << "remote: win" << ref.win << " proc: " << ref.remote_proc
	<< " offset : " << ref.win_offset;
  }
  out << "]";
}

// A plain unary expression, but since it can
// query lvalue, it can perform assignments
template <typename _Object>
class NUMA_Reference_Expression
  : public Unary_Expression< NUMA_Reference<_Object> >
{
public:
  typedef _Object Object; // override object def in base class
  typedef NUMA_Reference<Object> Reference;
  typedef NUMA_Reference_Expression<Object> Ref_Expr;
  typedef Unary_Expression< NUMA_Reference<_Object> > Base;

  NUMA_Reference_Expression() {}

  NUMA_Reference_Expression( Reference ref)
    : Base(ref) {
  }

  // Default assignment, have to override this somehow?
  // Assignment to another NUMA reference
  Ref_Expr &operator =(const Ref_Expr &rhs_expr) {
    // delegate assignment to <sub>class
//     nlog << "numa_ref : assignment" << *this << ", "
// 	 << rhs_expr << endl;
    
    Reference & ref = object;
    const Reference & rhs_ref = rhs_expr.object;

//     nlog << node_str << ref << " := " << rhs_ref << endl;
    // simplest case first
    if (ref.local && rhs_ref.local)
      *object.local_object = *rhs_ref.local_object;
    else if (ref.local && !rhs_ref.local) {
      // from remote to local
      // get it from the remote
      ref.receive(ref.local_object, rhs_ref.remote_proc);
    }
    else if (!ref.local && rhs_ref.local) {
      // from local to remote
      ref.send(ref.remote_proc);
    }
    return *this;
  }

  // Assignment to an object
  Ref_Expr& operator =(Object rhs)
  {
    Reference & ref = object;
//     nlog << node_str << ref << " := " << rhs << endl;
    if (ref.local)
      *object.local_object = rhs;
    return *this;
  }

  // Dereference
  Reference & operator*() {
    return sel_object;
  }
//   Object & operator->() {
//     return sel_object;
//   }


  // Conversion operator
  operator Object() {
    Reference & ref = object;
//     nlog << node_str << ref << " -<type>-> " << typeid(Object).name() << endl;

    Object obj;
    ref.broadcast(&obj);
    return obj;
  }

};

end_namespace();

#endif
