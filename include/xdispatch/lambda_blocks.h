/*
* Copyright (c) 2008-2009 Apple Inc. All rights reserved.
* Copyright (c) 2011 MLBA. All rights reserved.
*
* @MLBA_OPEN_LICENSE_HEADER_START@
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
*
* @MLBA_OPEN_LICENSE_HEADER_END@
*/


/* When building on 10.6 with gcc 4.5.1 we can bypass
    Apple's lambda functions implementation in C++ as we have lambdas.
	This prevents a lot of errors from occuring
	*/

#ifndef XDISPATCH_BLOCKS_H_
#define XDISPATCH_BLOCKS_H_

#ifndef __XDISPATCH_INDIRECT__
#error "Please #include <xdispatch/dispatch.h> instead of this file directly."
#endif

/**
  @def $
  Defines a cross platform way of writing code blocks (or lambdas).
  The block can take arguments and all local variables are available as
  read-only within the block. The variables values are the same as they were
  at runtime when declaring this block.

  The macro will either use c++0x lambda functions or blocks (as found in clang)
  depending on the individual availability. When you are sure, that you will use
  exactly one type of compiler only, you can use the original way of writing as
  well. All declared code (may it be blocks, lambdas or the cross-platform version)
  can be stored within a dispatch_block_t. This type of variable will be used
  throughout the entire xdispatch project.

  Currently supported compilers are:
  <ul>
    <li>clang 2.0</li>
    <li>MS Visual Studio 10</li>
    <li>gcc 4.5+ (with activated C++0x support)</li>
  </ul>

  If you're worried about namespace pollution, you can disable this macro by defining
  the following macro before including xdispatch/dispatch.h:

  @code
   #define XDISPATCH_NO_KEYWORDS
   #include <xdispatch/dispatch.h>
  @endcode

  Below you can find a small example for using the '$' macro:
  @code

  // the following code is working on all supported compilers equally
  dispatch_block_t myBlock = ${
        printf("Hello World!\n");
    };

  // working on clang 2.0 and the gcc 4.3 shipped with Mac OS 10.6
  dispatch_block_t myBlock = ^{
        printf("Hello World!\n");
    };

  // working on compilers with c++0x support (MSVC 10, gcc 4.5+)
  dispatch_block_t myBlock = []{
        printf("Hello Wolrd!\n");
    };

  // in all cases you can call the defined block in the same way:
  // -> output: "Hello World"
    myBlock();

  @endcode

  @see XDISPATCH_BLOCK
  */

// clang 2.0, gcc 4.3 from mac os 10.6
#ifdef __BLOCKS__

# include <Block.h>
# include <stddef.h>
# define XDISPATCH_BLOCK ^
# ifndef XDISPATCH_NO_KEYWORDS
#  define $ ^
# endif
# define XDISPATCH_BLOCK_COPY(A) Block_copy(A)
# define XDISPATCH_BLOCK_RELEASE(A) Block_release(A)
  //typedef void (^dispatch_block_t)(void);
  typedef void (^dispatch_iteration_block_t)(size_t);
# define XDISPATCH_HAS_BLOCKS
# if defined(__cplusplus) && !defined(__clang__)
#  warning "Sadly blocks are currently broken in C++ on this platform, we recommend using gcc 4.5.1 or clang 2.0 instead"
# endif

// visual studio 2010
#elif _MSC_VER >= 1600

# include <functional>
# define XDISPATCH_BLOCK [=]
# ifndef XDISPATCH_NO_KEYWORDS
#  define $ [=]
# endif
# define XDISPATCH_BLOCK_COPY(A) A
# define Block_copy(A) A
# define XDISPATCH_BLOCK_RELEASE(A) (A)
# define Block_release(A) A
 typedef std::tr1::function< void (void) > dispatch_block_t;
 typedef std::tr1::function< void (size_t) > dispatch_iteration_block_t;
# define XDISPATCH_HAS_BLOCKS

// gcc 4.5 with c++0x enabled
#elif defined __GXX_EXPERIMENTAL_CXX0X__

# include <tr1/functional>
# define XDISPATCH_BLOCK [=]
# ifndef XDISPATCH_NO_KEYWORDS
#  define $ [=]
# endif
# define XDISPATCH_BLOCK_COPY(A) A
# define Block_copy(A) A
# define XDISPATCH_BLOCK_RELEASE(A) (A)
# define Block_release(A) A
 typedef std::tr1::function< void (void) > dispatch_block_t;
 typedef std::tr1::function< void (size_t) > dispatch_iteration_block_t;
# define XDISPATCH_HAS_BLOCKS

#else

# define XDISPATCH_BLOCK "No Block support!"

#endif

#if defined(XDISPATCH_HAS_BLOCKS) && !defined(__BLOCKS__)

void dispatch_async(dispatch_queue_t queue, dispatch_block_t function);

void dispatch_after(dispatch_time_t when, dispatch_queue_t queue, dispatch_block_t function);

void dispatch_sync(dispatch_queue_t queue, dispatch_block_t function);

void dispatch_apply(size_t iterations, dispatch_queue_t queue, dispatch_iteration_block_t function);

void dispatch_group_async(dispatch_group_t group, dispatch_queue_t queue, dispatch_block_t function);

void dispatch_group_notify(dispatch_group_t group, dispatch_queue_t queue, dispatch_block_t block);

#endif

#endif /* XDISPATCH_BLOCKS_H_ */