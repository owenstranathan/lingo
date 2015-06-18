// Copyright (c) 2015 Andrew Sutton
// All rights reserved

#ifndef LINGO_SYMBOL_HPP
#define LINGO_SYMBOL_HPP

// The symbol module defines a symbol table and related
// facilities. A symbol is a view of a string in a source
// file and affiliated data (e.g., bindings).

#include "lingo/string.hpp"

#include <cstring>
#include <list>
#include <string>
#include <unordered_map>

namespace lingo
{

// -------------------------------------------------------------------------- //
//                         Identifier bindings

// A binding associates the use of a name with different
// kinds of information: type, value, etc. This information
// is represented as a pointer within the binding object.
//
// An identifier maybe bound to different types or values
// in different scopes. Each new binding results in a
// list with a pointer to the previous entry.
struct Binding 
{
  Binding(void* p)
    : info(p), prev(nullptr)
  { }

  void*    info;
  Binding* prev;
};


// -------------------------------------------------------------------------- //
//                           Symbol data

// Information associated with a symbol. This includes the kind
// of token, and token-specific data.
//
// TODO: The binding must a union. Improve this data structure.
struct Symbol_data
{
  Symbol_data()
    : bind(nullptr) { }
  
  Binding* bind;  // An identifier binding
};


// -------------------------------------------------------------------------- //
//                                  Symbols

// Determines the kinds of attributes associated with the symbol.
using Symbol_kind = int;
constexpr int unspecified_sym = 0;


// The symbol descriptor contains information about a symbol
// in the symbol table.
struct Symbol_descriptor
{
  int kind    : 8;  // The kind of keyword (see above)
  int token   : 16; // An associated token kind
};


// A Symbol represents a lexeme saved in the symbol table and
// its associated attributes. The string representation of symbols 
// are represented as a pair of pointers into a character array.
// Additional attributes include the kind of token and the token
// specific data.
//
// Note that the symbol owns the pointer to its string view and is
// responsible for de-allocating that memory.
//
// TODO: What kinds of information can we add to this entry
// to simplify functioning.
//
// TODO: When we start using a reasonable allocator for the symbol
// table, revisit the destructor.
struct Symbol
{
  ~Symbol();

  Symbol(String_view s, Symbol_descriptor d)
    : str(s), desc(d)
  { }

  Symbol(char const* s, Symbol_descriptor d)
    : str(s), desc(d)
  { }

  Symbol(char const* f, char const* l, Symbol_descriptor d)
    : str(f, l), desc(d)
  { }

  String_view       str;  // The string view
  Symbol_descriptor desc; // The kind of token
  Symbol_data       data; // Supplemental data
};


inline bool
operator==(Symbol const& a, Symbol const& b)
{
  return a.str == b.str;
}


inline bool
operator!=(Symbol const& a, Symbol const& b)
{
  return a.str != b.str;
}


void push_binding(Symbol&, void*);
void pop_binding(Symbol&);
void* get_binding(Symbol const&);


// -------------------------------------------------------------------------- //
//                           Symbol table

// A symbol table stores unique representations of strings in
// a program and their affiliated information (e.g., token kind, 
// etc.). The symbol table also supports efficient insertion and 
// lookup of those strings.
//
// The symbol table is implemented as a linked list of symbols 
// with a side table to support efficient lookup.
class Symbol_table
{
  using Hash = String_view_hash;
  using Eq = String_view_eq;
  using List = std::list<Symbol>;
  using Map = std::unordered_map<String_view, Symbol*, Hash, Eq>;

public:
  Symbol& insert(String_view, Symbol_descriptor);
  Symbol& insert(char const*, Symbol_descriptor);
  Symbol& insert(char const*, char const*, Symbol_descriptor);

  Symbol* lookup(String_view) const;
  Symbol* lookup(char const*) const;
  Symbol* lookup(char const*, char const*) const;

  void clear();

private:
  List syms_;
  Map  map_;
};


inline Symbol&
Symbol_table::insert(char const* s, Symbol_descriptor k)
{
  return insert(String_view(s), k);
}


inline Symbol& 
Symbol_table::insert(char const* f, char const* l, Symbol_descriptor k)
{
  return insert(String_view(f, l), k);
}


inline Symbol*
Symbol_table::lookup(char const* s) const
{
  return lookup(String_view(s));
}


inline Symbol* 
Symbol_table::lookup(char const* f, char const* l) const
{
  return lookup(String_view(f, l));
}


Symbol_table& symbols();


// Returns the symbol correspondng to `str`, inserting a new
// symbol if it is not already present.
inline Symbol&
get_symbol(char const* str)
{
  return symbols().insert(str, {});
}


// Returns the symbol correspondng to the string in `[first, last)`.
// Insert the symbol if it does not exist.
inline Symbol&
get_symbol(char const* first, char const* last)
{
  return symbols().insert(first, last, {});
}


// Returns the symbol corresponding to the stirng `s`. Insert
// the symbol if it does not exist.
inline Symbol&
get_symbol(String_view s)
{
  return symbols().insert(s.begin(), s.end(), {});
}


// Return a new string, interned in the symbol table. 
inline String_view
get_symbol_string(char const* str)
{
  return get_symbol(str).str;
}


inline Symbol*
lookup_symbol(char const* s)
{
  return symbols().lookup(s);
}


inline Symbol*
lookup_symbol(char const* first, char const* last)
{
  return symbols().lookup(first, last);
}


inline Symbol*
lookup_symbol(String_view s)
{
  return symbols().lookup(s);
}


} // namespace lingo

#endif
