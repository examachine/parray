//
//
// C++ Interface for module: Array_IO
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Array_IO_Interface
#define Array_IO_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);

  extern istream & pin;
  extern ostream & pout;

  const static int buffer_size = 3;

  template <typename C, int d, typename SC>
  class Array<C,d,SC>::Loader
  {
  public:
    Linear_Storage<C,d> buffer;
    Loader(const Array<C,d,SC> *_array)  : buffer( Range(0,buffer_size-1) ) {
      array = _array;
      range = array->range;
      index = range.sel_first();
      remaining_size = array->size();
      buffer.allocate();
    }
    
    void load()
      {
      int buffer_size = buffer.sel_num_components();
      int load_size;
      if (buffer_size < remaining_size) {
	load_size = buffer_size;
	remaining_size -= buffer_size;
      }
      else {
	load_size = remaining_size;
	remaining_size = 0;
      }

      // lame version
      if (root_processor()) {
	// send to each processor in index order
	int buffer_index = 0;
	for (int p=0; p<num_processors; p++) {
	  pair<bool, Range> local_range =
	    array->storage.sel_partition().
	    part_indices(Range(index, index+load_size-1), p);
	  if (local_range.first) {
// 	    nlog << "[i/o node] send to processor " << p << " "
// 		 << local_range.second << endl;
	    int load_size = local_range.second.sel_length() + 1;
// 	    nlog << node_str << "sending buffer " << buffer << endl;
	    MPI::COMM_WORLD.Send((void*)&buffer.ref_component(buffer_index)
				 , sizeof(Component) * load_size,
				 MPI::CHAR, p, load_tag);
	    buffer_index += load_size;
	  }
	}
      }

      // for each
      {
	Range range(index, index+load_size-1);
	pair<bool, Range> local_range =
	  array->storage.sel_partition().local_indices(range);
	if (local_range.first) {
// 	  nlog << node_str << "recv from i/o node " << range <<
// 	    ", local_range " <<local_range.second << endl;
	  int first = local_range.second.sel_first();
	  int store_size = local_range.second.sel_length() + 1;
	  MPI::COMM_WORLD.Recv((void*)&array->storage.ref_component(first)
			       , sizeof(Component) * load_size,
			       MPI::CHAR, 0, load_tag);
	}
      }

      index += load_size;
    }

    bool eos() {
      return remaining_size == 0;
    }
    
    const Array<C,d,SC> *array;
    Range range;
    Index index;
    int remaining_size;
  };

  template <typename C, int d, typename SC>
  istream& operator >>(istream &in, Array<C,d,SC> & array)
  {
    typedef Array<C,d,SC> Array;
    typedef typename Array::Storage Storage;
    if (in) {
//       if (root_processor())
// 	in >> "Array <";
      Array::Loader loader = array.cons_loader();
      int array_size =  array.size();
      int index = 0;
      while (!loader.eos()) {
	if (root_processor()) {
	  Linear_Storage<C,d>::Iterator iter = loader.buffer.begin();
	  Linear_Storage<C,d>::Iterator iter_end = loader.buffer.end();
	  for (;iter <= iter_end && index<array_size;
	       iter.next(),index++) {
	    in >> iter.refer() >> ws;
	  }
	}
// 	nlog << node_str << "buffer " << loader.buffer << endl;
	loader.load();
      }

//       if (root_processor())
// 	in >> ">";
      return in;
    }
  }

  template <typename C, int d, typename SC>
  class Array<C,d,SC>::Storer
  {
  public:
    Linear_Storage<C,d> buffer;
//     MPI_Win buffer_win;

    Storer(const Array<C,d,SC> *_array) : buffer( Range(0,buffer_size-1) ) {
      array = _array;
      range = array->range;
      index = range.sel_first();
      remaining_size = array->size();
      buffer.allocate();
//       MPI_Win_create(reinterpret_cast<void*>(&buffer.ref_component(0)),
// 		     buffer.sel_num_components()*sizeof(C),
// 		     sizeof(C),
// 		     MPI_INFO_NULL,
// 		     MPI_COMM_WORLD,
// 		     &buffer_win);
    }
    
    void store() {
      // CODE: make io node configurable
      int io_node = 0;

      int buffer_size = buffer.sel_num_components();
      int store_size;
      if (buffer_size < remaining_size) {
	store_size = buffer_size;
	remaining_size -= buffer_size;
      }
      else {
	store_size = remaining_size;
	remaining_size = 0;
      }
//       nlog << "store_size " << store_size
// 	   << ", remaining " << remaining_size << endl;

      // for each
      {
	Range range(index, index+store_size-1);
	pair<bool, Range> local_range =
	  array->storage.sel_partition().local_indices(range);
	if (local_range.first) {
// 	  nlog << node_str << "send to i/o node " << range <<
// 	    "local_range" <<local_range.second
// 	       << endl;
	  int first = local_range.second.sel_first();
	  int store_size = local_range.second.sel_length() + 1;
	  MPI::COMM_WORLD.Send((void*)&array->storage.ref_component(first)
			       , sizeof(Component) * store_size,
			       MPI::CHAR, 0, store_tag);
// 	  MPI_Win_fence(0, buffer_win);
// 	  if (!root_processor())
// 	    MPI_Put((void*)&array->storage.ref_component(first),
// 		    sizeof(Component) * store_size, MPI_CHAR,
// 		    io_node, 0, sizeof(Component)*store_size, MPI_CHAR,
// 		    buffer_win);
// 	  MPI_Win_fence(0, buffer_win);
// 	  nlog << node_str << "put complete!" << endl;
	}
      }

      // lame version
      if (root_processor()) {
	// collect from each processor in index order
	int buffer_index = 0;
	for (int p=0; p<num_processors; p++) {
	  pair<bool, Range> local_range =
	    array->storage.sel_partition().
	    part_indices(Range(index, index+store_size-1), p);
	  if (local_range.first) {
// 	    nlog << "[i/o node] fetch from processor " << p << " "
// 		 << local_range.second << endl;
	    int store_size = local_range.second.sel_length() + 1;
	    MPI::COMM_WORLD.Recv((void*)&buffer.ref_component(buffer_index)
				 , sizeof(Component) * store_size,
				 MPI::CHAR, p, store_tag);
// 	    MPI::COMM_WORLD.Recv((void*)&buffer.ref_component(buffer_index)
// 				 , store_size,
// 				 MPI_Type<Component>::datatype(), p, store_tag);
	    buffer_index += store_size;
	  }
	}
      }


      index += store_size;
    }

    bool eos() {
      return remaining_size == 0;
    }
    
    const Array<C,d,SC> *array;
    Range range;
    Index index;
    int remaining_size;
  };


  template <typename C, int d, typename SC>
  ostream& operator <<(ostream &out, const Array<C,d,SC>& array)
  {
    typedef Array<C,d,SC> Array;
    typedef typename Array::Storage Storage;
    if (out) {
      if (root_processor())
	out << "Array <";
      Array::Storer storer = array.cons_storer();
      int array_size =  array.size();
      int index = 0;
      while (!storer.eos()) {
	storer.store();
	if (root_processor()) {
	  Linear_Storage<C,d>::Iterator iter = storer.buffer.begin();
	  Linear_Storage<C,d>::Iterator iter_end = storer.buffer.end();
	  for (;iter <= iter_end && index<array_size;
	       iter.next(),index++)
	    out << iter.refer() << " ";
	}
      }

      if (root_processor())
	out << ">";
      return out;
    }
  }

  template <typename C, int d>
  ostream& operator <<(ostream &out, const Linear_Storage<C,d>& storage)
  {
    typedef typename Linear_Storage<C,d>::Iterator Iterator;

    if (out) {
      out << "part <";
      Iterator iter = storage.begin();
      Iterator iter_end = storage.end();
      out << iter.refer();
      for (iter.next(); iter <= iter_end; iter.next())
	out << ", " << iter.refer();
      out << ">";
    }
    return out;
  
  }

  template <typename C, int d>
  ostream& operator <<(ostream &out, const Block_Partitioning<C,d>& storage)
  {
    typedef typename Block_Partitioning<C,d>::Iterator Iterator;

    if (out) {
      out << "part <";
      Iterator iter = storage.begin();
      Iterator iter_end = storage.end();
      out << iter.refer();
      for (iter.next(); iter <= iter_end; iter.next())
	out << ", " << iter.refer();
      out << ">";
    }
    return out;
  
  }

end_namespace();

#endif
