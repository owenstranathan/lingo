// Copyright (c) 2015 Andrew Sutton
// All rights reserved

#ifndef LINGO_INTEGER_HPP
#define LINGO_INTEGER_HPP

// The integer module provides arbitrary precision integers
// and associated functions.

#include "lingo/string.hpp"
#include "lingo/print.hpp"
#include "lingo/debug.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>

#include <gmp.h>

namespace lingo 
{

// The Integer class represents arbitrary precision integer 
// values. The integer class also tracks the base in which
// it was originally written. If unspecified, the base is 10.
// This allows for pretty printing of tokens in the base in
// which they were originally written.
//
// FIXME: Make a default constructor that takes a width 
// specification.
class Integer 
{
public:
  // Default constructor
  Integer();

  // Copy semantics
  Integer(Integer const&);
  Integer& operator=(Integer const&);

  // Value initialization. Allows implicit conversion.
  Integer(std::intmax_t, int = 10);

  // String initialization
  Integer(std::string const&, int = 10);

  // Destructor
  ~Integer();

  // Arithmetic compound assignmnt operators.
  Integer& operator+=(Integer const&);
  Integer& operator-=(Integer const&);
  Integer& operator*=(Integer const&);
  Integer& operator/=(Integer const&);
  Integer& operator%=(Integer const&);

  Integer& operator&=(Integer const&);
  Integer& operator|=(Integer const&);
  Integer& operator^=(Integer const&);

  Integer& operator<<=(Integer const&);
  Integer& operator>>=(Integer const&);

  Integer& neg();
  Integer& abs();
  Integer& comp();

  // Sign
  int sign() const;
  bool is_positive() const;
  bool is_negative() const;
  bool is_nonpositive() const;
  bool is_nonnegative() const;

  // Truth value
  bool truth_value() const;

  int bits() const;
  int base() const;
  std::uintmax_t getu() const;
  std::intmax_t gets() const;
  const mpz_t& data() const;

private:
  mpz_t value_;
  int   base_;
};

// Equality
bool operator==(Integer const&, Integer const&);
bool operator!=(Integer const&, Integer const&);

// Ordering
bool operator<(Integer const&, Integer const&);
bool operator>(Integer const&, Integer const&);
bool operator<=(Integer const&, Integer const&);
bool operator>=(Integer const&, Integer const&);

// Arithmetic
Integer operator+(Integer const&, Integer const&);
Integer operator-(Integer const&, Integer const&);
Integer operator*(Integer const&, Integer const&);
Integer operator/(Integer const&, Integer const&);
Integer operator%(Integer const&, Integer const&);
Integer operator-(Integer const&);
Integer operator+(Integer const&);


// Bit manipulation
Integer operator&(Integer const&, Integer const&);
Integer operator|(Integer const&, Integer const&);
Integer operator^(Integer const&, Integer const&);
Integer operator~(Integer const&);

Integer operator<<(Integer const&, Integer const&);
Integer operator>>(Integer const&, Integer const&);


// Streaming

void print(Printer&, Integer const&);
void debug(Printer&, Integer const&);

std::ostream& operator<<(std::ostream&, Integer const&);

} // namespace lingo

#include <lingo/integer.ipp>

#endif
