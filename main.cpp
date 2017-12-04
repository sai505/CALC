//Implement two versions of a calculator abstract syntax tree (the data structure we've been talking about for two weeks). In both versions, implement the following:
//Destructors (to avoid leaking memory)
//A virtual constructor (clone)
//An evaluation function (eval)
//A print function (print)
//The first implementation should use virtual functions and the second should use the visitor pattern.

#include "calc.hpp"
//#include "visitor.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Expr* e)
{
	return os;
}

int main() {

	//(5-2) * (4/2) * (3+2)
	Expr* s = new Sub(new Int(5), new Int(2));
	Expr* d = new Div(new Int(4), new Int(2));
	Expr* e = new Mul(s,d);

	std::cout << e << " == " << e->evaluate() << '\n';

	 EvalVisitor v;
	  e->accept(v);
	  std::cout << v.ret << '\n';

	  std::cout << eval(e) << '\n';

	  delete e;


}