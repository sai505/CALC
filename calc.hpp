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


/// Represents expressions of the form 0, 1, 2, ..., n.
struct Int : Expr
{
  Int(int n)
    : val(n)
  { }

  // Returns a copy of this object.
  //
  // Covariant return type. You can override an inherited virtual
  // function using a different return type, but only if the new
  // return type is derived from the original.
  Int* clone() const override {
    return new Int(*this);
  }

  void print(std::ostream& os) const override {
    os << val;
  }

  int evaluate() const override {
    return val; 
  }

  Expr* reduce() const override {
    throw std::runtime_error("already reduced");
  }

  // An integer is value.
  bool is_value() const override { return true; }

  void compile(std::ostream& os) const override {
    os << "push " << val << '\n';
  }

  bool equal(const Expr* that) const override {
    // Steps outside of the "pure" programming style.
    // Prefer to program only in terms of the interface
    // provided.
    //
    // if (const Int* n = dynamic_cast<const Int*>(that))
    //   return val == n->val;

    // The type of the this pointer is const Int*.
    return that->equal(this);
  }
  bool equal(const Int* that) const override {
    // The static type of the this pointer is const Int.
    // The static type of the that pointer is const Int.
    return val == that->val;
  }

  int val;
};

/// Represents all expressions of the form e1 @ e2 where @
/// is one of the operations +, -, *, /.
///
/// Our previous implementation didn't include this class; the
/// members e1 and e2 appeared in all of the derived classes.
/// Because they are common to all those classes we can factor
/// them into a new base class (or lift them).
struct Binary : Expr
{
  Binary(const Binary& that)
    : e1(that.e1->clone()), e2(that.e2->clone())
  { }

  Binary(Expr *e1, Expr* e2)
    : e1(e1), e2(e2)
  { }

  ~Binary() override {
    delete e1;
    delete e2;
  }

  // As a general rule, if a member function does not use
  // the member variables of a class, it can be declared
  // static.
  static void print_enclosed(std::ostream& os, const Expr* e) {
    os << '(';
    e->print(os);
    os << ')';
  }

  Expr* e1;
  Expr* e2;
};

/// Represents expressions of the form e1 + e2.
struct Add : Binary
{
  // This is an inherited constructor.
  using Binary::Binary;

  Add* clone() const override {
    return new Add(*this);
  }

  // Prints the expression e1 + e2.
  void print(std::ostream& os) const override {
    print_enclosed(os, e1);
    os << " + ";
    print_enclosed(os, e2);
  }

  // The value of e1 + e2 is the value of e1 + the value of e2.
  int evaluate() const override {
    return e1->evaluate() + e2->evaluate();
  }

  // Reduces the expression e1 + e2.
  Expr* reduce() const override {
    if (e1->is_value()) {
      if (e2->is_value())
        return new Int(evaluate()); // v1 + v2
      // v1 + e2
      return new Add(e1->clone(), e2->reduce());
    }
    // e1 + e2
    return new Add(e1->reduce(), e2->clone());
  }

  void compile(std::ostream& os) const override {
    e1->compile(os);
    e2->compile(os);
    os << "add\n";
  }

  bool equal(const Expr* that) const override {
    return that->equal(this);
  }
  bool equal(const Add* that) const override {
    return *e1 == *that->e1 && *e2 == *that->e2;

    // Or equivalently...
    // return e1->equal(that->e1) && e2->equal(that->e2);
  }
};

/// Represents expressions of the form e1 - e2.
struct Sub : Binary
{
  using Binary::Binary;

  Sub* clone() const override {
    return new Sub(*this);
  }

  // Prints the expression e1 - e2.
  void print(std::ostream& os) const override {
    print_enclosed(os, e1);
    os << " - ";
    print_enclosed(os, e2);
  }

  int evaluate() const override {
    return e1->evaluate() - e2->evaluate();
  }

  Expr* reduce() const override {
    if (e1->is_value()) {
      if (e2->is_value())
        return new Int(evaluate()); // v1 - v2
      // v1 - e2
      return new Sub(e1->clone(), e2->reduce());
    }
    // e1 - e2
    return new Sub(e1->reduce(), e2->clone());
  }

  void compile(std::ostream& os) const override {
    e1->compile(os);
    e2->compile(os);
    os << "sub\n";
  }

  bool equal(const Expr* that) const override {
    return that->equal(this);
  }

  bool equal(const Sub* that) const override {
    return *e1 == *that->e1 && *e2 == *that->e2;
  }
};

/// Represents expressions of the form e1 * e2.
struct Mul : Binary
{
  using Binary::Binary;

  Mul* clone() const override {
    return new Mul(*this);
  }

  // Prints the expression e1 * e2.
  void print(std::ostream& os) const override {
    print_enclosed(os, e1);
    os << " * ";
    print_enclosed(os, e2);
  }

  int evaluate() const override {
    return e1->evaluate() * e2->evaluate();
  }

  Expr* reduce() const override {
    if (e1->is_value()) {
      if (e2->is_value())
        return new Int(evaluate()); // v1 * v2
      // v1 * e2
      return new Mul(e1->clone(), e2->reduce());
    }
    // e1 * e2
    return new Mul(e1->reduce(), e2->clone());
  }

  void compile(std::ostream& os) const override {
    e1->compile(os);
    e2->compile(os);
    os << "mul\n";
  }

  bool equal(const Expr* that) const override {
    return that->equal(this);
  }

  bool equal(const Mul* that) const override {
    return *e1 == *that->e1 && *e2 == *that->e2;
  }
};

/// Represents expressions of the form e1 / e2.
struct Div : Binary
{
  using Binary::Binary;

  Div* clone() const override {
    return new Div(*this);
  }

  // Prints the expression e1 / e2.
  void print(std::ostream& os) const override {
    print_enclosed(os, e1);
    os << " / ";
    print_enclosed(os, e2);
  }

  // The value of e1 / e2 is the value of e1 / the value of e2
  // unless e2 is 0.
  int evaluate() const override {
    int d = e2->evaluate();
    if (d == 0)
      throw std::runtime_error("division by zero");
    return e1->evaluate() / d;
  }

  Expr* reduce() const override {
    if (e1->is_value()) {
      if (e2->is_value())
        return new Int(evaluate()); // v1 / v2
      // v1 / e2
      return new Div(e1->clone(), e2->reduce());
    }
    // e1 / e2
    return new Div(e1->reduce(), e2->clone());
  }

  void compile(std::ostream& os) const override {
    e1->compile(os);
    e2->compile(os);
    os << "div\n";
  }

  bool equal(const Expr* that) const override {
    return that->equal(this);
  }

  bool equal(const Div* that) const override {
    return *e1 == *that->e1 && *e2 == *that->e2;
  }
};


















