//Implement two versions of a calculator abstract syntax tree (the data structure we've been talking about for two weeks). In both versions, implement the following:
//Destructors (to avoid leaking memory)
//A virtual constructor (clone)
//An evaluation function (eval)
//A print function (print)
//The first implementation should use virtual functions and the second should use the visitor pattern.

#include "calc.hpp"
#include "visito.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Expr* e)
{
	return os;
}

int main(){
	
		//(5-2) * (4/2) * (3+2)
		Expr* e = new Mul(
			new Sub(new Int(5), new Int(2)),
			new Div(new Int(4), new Int(2)),
			new Add(new Int(3), new Int(2))
		)
				
		std::cout << e << " == " << e->evaluate() << '\n';

		Expr* e2 = e->reduce();
		delete e;
		delete e2;	
		
		EvalVisitor v;
		e->accept(v);
		std::cout << v.ret << '\n';

		std::cout << eval(e) << '\n';

}