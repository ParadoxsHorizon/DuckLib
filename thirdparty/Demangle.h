/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file has been modified from its original form:
 *  Merged the header and implementation file into a single header file.
 * See Demangled.h.patch for a detailed record of the performed revisions!
 */

#pragma once
#ifndef __FOLLY_DEMANGLE_H__
#define __FOLLY_DEMANGLE_H__

#include <string>
#include <typeinfo>
#include <algorithm>
#include <cstring>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif

//  The headers <libiberty.h> (binutils) and <string.h> (glibc) both declare the
//  symbol basename. Unfortunately, the declarations are different. So including
//  both headers in the same translation unit fails due to the two conflicting
//  declarations. Since <demangle.h> includes <libiberty.h> we must be careful.
#if __has_include(<demangle.h>)
#pragma push_macro("HAVE_DECL_BASENAME")
#define HAVE_DECL_BASENAME 1
#include <demangle.h> // @manual
#pragma pop_macro("HAVE_DECL_BASENAME")
#endif

//  try to find cxxabi demangle
//
//  prefer using a weakref

#if __has_include(<cxxabi.h>)

[[gnu::weakref("__cxa_demangle")]] static char* cxxabi_demangle(
    char const*, char*, size_t*, int*);

#else // __has_include(<cxxabi.h>)

static constexpr auto cxxabi_demangle = static_cast<char* (*)(...)>(nullptr);

#endif // __has_include(<cxxabi.h>)

//  try to find liberty demangle
//
//  cannot use a weak symbol since it may be the only referenced symbol in
//  liberty
//
//  in contrast with cxxabi, where there are certainly other referenced symbols

#if __has_include(<demangle.h>)

static constexpr auto liberty_demangle = cplus_demangle_v3_callback;

#if defined(DMGL_NO_RECURSE_LIMIT)
static constexpr auto liberty_demangle_options_no_recurse_limit =
    DMGL_NO_RECURSE_LIMIT;
#else
static constexpr auto liberty_demangle_options_no_recurse_limit = 0;
#endif

static constexpr auto liberty_demangle_options = //
    DMGL_PARAMS | DMGL_ANSI | DMGL_TYPES | //
    liberty_demangle_options_no_recurse_limit;

#else // __has_include(<demangle.h>)

static constexpr auto liberty_demangle = static_cast<int (*)(...)>(nullptr);
static constexpr auto liberty_demangle_options = 0;

#endif // __has_include(<demangle.h>)


namespace folly {

inline constexpr size_t demangle_max_symbol_size =
#if defined(FOLLY_DEMANGLE_MAX_SYMBOL_SIZE)
    FOLLY_DEMANGLE_MAX_SYMBOL_SIZE;
#else
    0;
#endif

//  implementations

//  reinterpret-cast currently evades -Waddress
bool const demangle_build_has_cxxabi = cxxabi_demangle;
bool const demangle_build_has_liberty =
    reinterpret_cast<void*>(liberty_demangle);

inline std::string demangle(const char* name) {
  if (!name) {
    return std::string();
  }

  if (demangle_max_symbol_size) {
    // GCC's __cxa_demangle() uses on-stack data structures for the
    // parser state which are linear in the number of components of the
    // symbol. For extremely long symbols, this can cause a stack
    // overflow. We set an arbitrary symbol length limit above which we
    // just return the mangled name.
    size_t mangledLen = strlen(name);
    if (mangledLen > demangle_max_symbol_size) {
      return std::string(name);
    }
  }

  if (cxxabi_demangle) {
    int status;
    size_t len = 0;
    // malloc() memory for the demangled type name
    char* demangled = cxxabi_demangle(name, nullptr, &len, &status);
    if (status != 0) {
      return name;
    }
    // len is the length of the buffer (including NUL terminator and maybe
    // other junk)
    return std::string(demangled);
  } else {
    return std::string(name);
  }
}

namespace {

struct DemangleBuf {
  char* dest;
  size_t remaining;
  size_t total;
};

void demangleCallback(const char* str, size_t size, void* p) {
  DemangleBuf* buf = static_cast<DemangleBuf*>(p);
  size_t n = std::min(buf->remaining, size);
  memcpy(buf->dest, str, n);
  buf->dest += n;
  buf->remaining -= n;
  buf->total += size;
}

} // namespace

inline std::size_t strlcpy(
    char* const dest, char const* const src, std::size_t const size) {
  std::size_t const len = std::strlen(src);
  if (size != 0) {
    std::size_t const n = std::min(len, size - 1); // always null terminate!
    std::memcpy(dest, src, n);
    dest[n] = '\0';
  }
  return len;
}

inline size_t demangle(const char* name, char* out, size_t outSize) {
  if (demangle_max_symbol_size) {
    size_t mangledLen = strlen(name);
    if (mangledLen > demangle_max_symbol_size) {
      if (outSize) {
        size_t n = std::min(mangledLen, outSize - 1);
        memcpy(out, name, n);
        out[n] = '\0';
      }
      return mangledLen;
    }
  }

  if (reinterpret_cast<void*>(liberty_demangle)) {
    DemangleBuf dbuf;
    dbuf.dest = out;
    dbuf.remaining = outSize ? outSize - 1 : 0; // leave room for null term
    dbuf.total = 0;

    // Unlike most library functions, this returns 1 on success and 0 on failure
    int status = liberty_demangle(
        name, liberty_demangle_options, demangleCallback, &dbuf);
    if (status == 0) { // failed, return original
      return strlcpy(out, name, outSize);
    }
    if (outSize != 0) {
      *dbuf.dest = '\0';
    }
    return dbuf.total;
  } else {
    return strlcpy(out, name, outSize);
  }
}

/**
 * Return the demangled (prettyfied) version of a C++ type.
 *
 * This function tries to produce a human-readable type, but the type name will
 * be returned unchanged in case of error or if demangling isn't supported on
 * your system.
 *
 * Use for debugging -- do not rely on demangle() returning anything useful.
 *
 * This function may allocate memory (and therefore throw std::bad_alloc).
 */
inline std::string demangle(const std::type_info& type) {
  return demangle(type.name());
}

/**
 * Return the demangled (prettyfied) version of a C++ type in a user-provided
 * buffer.
 *
 * The semantics are the same as for snprintf or strlcpy: bufSize is the size
 * of the buffer, the string is always null-terminated, and the return value is
 * the number of characters (not including the null terminator) that would have
 * been written if the buffer was big enough. (So a return value >= bufSize
 * indicates that the output was truncated)
 *
 * This function does not allocate memory and is async-signal-safe.
 *
 * Note that the underlying function for the fbstring-returning demangle is
 * somewhat standard (abi::__cxa_demangle, which uses malloc), the underlying
 * function for this version is less so (cplus_demangle_v3_callback from
 * libiberty), so it is possible for the fbstring version to work, while this
 * version returns the original, mangled name.
 */
inline size_t demangle(const std::type_info& type, char* buf, size_t bufSize) {
  return demangle(type.name(), buf, bufSize);
}

} // namespace folly

#endif // __FOLLY_DEMANGLE_H__
