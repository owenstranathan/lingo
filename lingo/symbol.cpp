// Copyright (c) 2015 Andrew Sutton
// All rights reserved

#include "lingo/symbol.hpp"
#include "lingo/print.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

namespace lingo
{

// -------------------------------------------------------------------------- //
//                             Symbols
//
// TODO: Check that the symbol has the appropriate kind before 
// accessing its bindings or other (eventual) attributes.

Symbol::~Symbol()
{
  delete str.begin();
}


// Push a new binding onto the symbol.
void
push_binding(Symbol& s, void* p)
{
  Binding* bind = new Binding(p);
  Binding*& orig = s.data.bind;
  if (orig)
    bind->prev = orig;
  orig = bind;
}


// Pop a binding from the symbol.
void
pop_binding(Symbol& s)
{
  assert(s.data.bind);
  Binding*& orig = s.data.bind;
  Binding* bind = orig;
  orig = orig->prev;
  delete bind;
}


// Get the binding associated with the symbol. If there
// is no binding for this symbol, this returns nullptr.
void*
get_binding(Symbol const& s)
{
  if (s.data.bind)
    return s.data.bind->info;
  else
    return nullptr;
}


// -------------------------------------------------------------------------- //
//                           Symbol table

// Insert a symbol as having the given kind. If the symbol already
// exists, do nothing and return its index.
Symbol&
Symbol_table::insert(String_view s, Symbol_descriptor k)
{
  auto iter = map_.find(s);
  if (iter == map_.end()) {
    // Allocate a new string to store in the symbol
    // table. This lets us add entries without worrying
    // about the source of the input (and whether or not
    // it goes out of scope).
    //
    // TODO: Use a sequential allocator for this table.
    // That would make memory management trivial. See 
    // the Symbol destructor.
    char* str = new char[s.size()];
    std::strncpy(str, s.begin(), s.size());
    String_view v(str, str + s.size());

    // Create/insert a new symbol for the given string.
    syms_.emplace_back(v, k);
    Symbol* sym = &syms_.back();

    // Insert the string into the lookup table.
    map_.insert({v, sym});
    return *sym;
  } else {
    return *iter->second;
  }
}


Symbol*
Symbol_table::lookup(String_view s) const
{
  auto iter = map_.find(s);
  if (iter != map_.end())
    return iter->second;
  else
    return nullptr;
}


namespace
{

// The global symbol table.
Symbol_table sym_;

} // namespace


// Returns the global symbol table.
Symbol_table& 
symbols()
{
  return sym_;
}


} // namespace lingo
