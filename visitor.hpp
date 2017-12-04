#pragma once

#include <stdexcept>
#include <iostream>

// Forward declarations declares what something is prior
// to its definition.
struct Expr;
struct Int;
struct Add;
struct Sub;
struct Mul;
//struct Div;

// A visitor or abstract dispatch table for the
// Expr hierarchy below.
struct Visitor
{
  virtual void visit(Int* e) = 0;
  virtual void visit(Add* e) = 0;
  virtual void visit(Sub* e) = 0;
  virtual void visit(Mul* e) = 0;
  virtual void visit(Div* e) = 0;
};

// A concrete algorithm. This class provides behaviors
// for the abstract dispatch table/visitor above.
struct EvalVisitor : Visitor
{
  // Return values have to be declared as member
  // variables. Must be initialized by each override.
  int ret;
  
  void visit(Int* e) override;
  void visit(Add* e) override;
  void visit(Sub* e) override;
  void visit(Mul* e) override;
  void visit(Div* e) override;
};

int eval(Expr* e);

// This data type represents the set of all expressions,
// as defined by the following set:
//
// e ::= 0 | 1 | 2 | ... | n -- integers
//       e1 + e2             -- addition
//       e1 - e2             -- subtraction
//       e1 * e2             -- multiplication
//       e1 / e2             -- division
//       -e1                 -- negation
//
// Expr is the base class of all expressions.
struct Expr
{
  virtual ~Expr() = default;

  virtual void accept(Visitor& v) = 0;
};

/// Represents expressions of the form 0, 1, 2, ..., n.
struct Int : Expr
{
  Int(int n) : val(n) { }

  void accept(Visitor& v) override { v.visit(this); }

  int val;
};

/// Represents all expressions of the form e1 @ e2 where @
/// is one of the operations +, -, *, /.
struct Binary : Expr
{
  Binary(Expr *e1, Expr* e2) : e1(e1), e2(e2) { }

  ~Binary() override {
    delete e1;
    delete e2;
  }

  Expr* e1;
  Expr* e2;
};

/// Represents expressions of the form e1 + e2.
struct Add : Binary
{
  // This is an inherited constructor.
  using Binary::Binary;

  void accept(Visitor& v) override { v.visit(this); }
};

/// Represents expressions of the form e1 - e2.
struct Sub : Binary
{
  using Binary::Binary;

  void accept(Visitor& v) override { v.visit(this); }
};

/// Represents expressions of the form e1 * e2.
struct Mul : Binary
{
  using Binary::Binary;

  void accept(Visitor& v) override { v.visit(this); }
};

/// Represents expressions of the form e1 / e2.
struct Div : Binary
{
  using Binary::Binary;

  void accept(Visitor& v) override { v.visit(this); }
};
