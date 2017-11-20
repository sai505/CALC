#pragma once

#include <stdexcept>
#include <iostream>

//priority
struct Expr;
struct Int;
struct Add;
struct Sub;
struct Mul;
struct Div;

//data types = int, add, sub, mul, div
//data types is subset of Expression

//Expr is base class of all expression
struct Expr{
	//virtual constructor
	virtual Expr* clone() const =0;
	
	virtual ~Expr() = default;
	
	virtual void print (std::ostream& os) const =0;

	virtual int evaluate () const =0;
	
	virtual Expr* reduce() const =0;
	
	//return true if not reducable
	virtual bool is_value const{
		return false;
	}
	
	virtual void compile (std::ostream& os) const =0;
	
	//return treu is this equal to that
	virtual bool equal(const Expr* that) const =0;
	virtual bool equal(const Int* that) const {return false;}
	virtual bool equal(const Add* that) const {return false;}
	virtual bool equal(const Sub* that) const {return false;}
	virtual bool equal(const Mul* that) const {return false;}
	virtual bool equal(const Div* that) const { return false;}
};

bool operator==(const Expr* e1, const Expr* e2);
















