#pragma once

#ifndef NDEBUG
  #define ASSERT(Expr, Msg) \
    core::__ASSERT(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
  #define ASSERT(Expr, Msg) ;
#endif

namespace core {
void __ASSERT(const char* expr_str, bool expr, const char* file, int line, const char* msg);
}// namespace core
