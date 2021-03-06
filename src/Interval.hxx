//
//
// C++ Interface for module: Interval
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Interval_Interface
#define Interval_Interface

#include "General.hxx"

begin_namespace(Parallel_Arrays);

// Generic interval class
template <typename _Number, int _dimensions >
class Interval {
public:
  typedef _Number Number;
  static const int dimensions = _dimensions;
  typedef Interval<Number,dimensions> Self;


  Interval() {}

  Interval(Number _first, Number _last)
    : first(_first), last(_last) {
  }

  //   void mod(Number f, Number l) {
  //     first = f; last = l;
  //   }

  // returns the first number of the interval
  Number sel_first() const {
    return first;
  }

  // returns the last number of the interval
  Number sel_last() const {
    return last;
  }

  // returns length of the interval
  Number sel_length() const {
    return last-first;
  }

  bool contains(Number x) const {
    return first <= x && x <= last;
  }

  pair<bool, Self> intersection(const Self & a) const {
    Self intersection = Self(max(sel_first(), a.sel_first()),
			     min(sel_last(), a.sel_last()));
    if (intersection.sel_first() <= intersection.sel_last()) {
      return pair<bool, Self>(true, intersection);
    }
    else
      return pair<bool, Self>(false, intersection);
    
  }

  Self operator + (Number increment) {
    first += increment;
    last += increment;
  }

  Self operator - (Number decrement) {
    first -= increment;
    last -= increment;
  }

  Self operator * (Number factor) {
    first *= factor;
    last *= factor;
  }


private:
  Number first, last;
};

template <typename Number, int d >
ostream& operator<<(ostream& os,
		    const Interval<Number,d>& i)
{
  os << "Interval[" << i.sel_first() << "," << i.sel_last() << "]";
  
  return os;
}

//template <typename N>
// class Range : public Interval<int, 1>
// {
// public:
//   typedef Interval<int, 1> Base;

  //   Range(int f, int l) : Base(f,l) {}
  
  // };
typedef Interval<int, 1> Range;

// Integer range is the default range type
//typedef Range<int> Range;

end_namespace();

#endif
