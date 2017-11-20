#include "calc.hpp"

bool 
operator==(const Expr& e1, const Expr& e2)
{
  // If e1 and e2 are identical, then they're equal.
  if (&e1 == &e2)
    return true;

  // Otherwise, call a virtual function to "discover" the
  // dynamic type of e1.
  return e1.equal(&e2);
}

