#include <iostream>
#include "functor.h"

using namespace std;
template <class Comparator>
void DoStringCompare(const std::string& s1, const std::string& s2, Comparator comp)
{
  cout << comp(s1, s2) << endl;  // calls comp.operator()(s1,s2);
}
int main() {
  string s1 = "Blue///";
  string s2 = "RED////%";
  AlphaStrComp comp1;
  LengthStrComp comp2;
  NumStrComp comp3;

  // Here comp1(s1,s2) is calling comp1.operator() (s1, s2);
  cout << "Blue compared to Red using AlphaStrComp yields " << comp1(s1, s2) << endl;

  // Here comp2(s1,s2) is calling comp2.operator() (s1, s2);
  cout << "Blue compared to Red using LenStrComp yields " << comp2(s1, s2) << endl;

  DoStringCompare(s1, s2, comp1);
  DoStringCompare(s1, s2, comp2);
  DoStringCompare(s1, s2, comp3);
  return 0;
}
