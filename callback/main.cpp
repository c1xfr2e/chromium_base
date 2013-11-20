// Copyright (c) 2012 The Wandoujia Authors. All rights reserved.
#include <objbase.h>
#include "base/callback.h"
#include "base/bind.h"
#include "base/bind_internal.h"
using namespace base;

int Sum(int a, int b) {
  return a + b;
}

int Sum3(int a, int b, int c) {
  return a + b + c;
}

class Foo {
public:
  Foo(int x) : i_(x) {}
  int Add(int x) { return x + i_; }
  ~Foo() { }
private:
  int i_;
};

class Bar {
public:
  Bar(int x) : i_(x) {}
  int Add(int x, int y) { return i_ + x + y; }
private:
  int i_;
};

template<typename T>
void function(void* p, int x) {
  T* T = reinterpret_cast<T*>(p);
}

int main() {
  // const reference to function pointer
  int (*const& Ptr)(int,int) = &Sum;
  
  Foo f(2222);
  typedef int (*Functor1)(int,int);
  typedef int (Bar::*Functor2)(int,int);
  typedef int (*Functor3)(int,int,int);
  typedef void (*Functor4)(int,int,int);

  Functor1 functor = &Sum;

  typedef internal::BindState<typename internal::FunctorTraits<Functor1>::RunnableType,
                              typename internal::FunctorTraits<Functor1>::RunType,
                              void()> BindStateType1;
  BindStateType1 bst1(internal::MakeRunnable(&Sum));
  base::Callback<int(int,int)> callback1(&bst1);
  callback1.Run(1998, 2004);

  typedef internal::BindState<typename internal::FunctorTraits<Functor3>::RunnableType,
                              typename internal::FunctorTraits<Functor3>::RunType,
                              void(int,int)> 
                              BindStateType3;
  BindStateType3 bst3(internal::MakeRunnable(&Sum3), 1, 2); // IgnoreResult(&Sum3)
  base::Callback<int(int)> callback3(&bst3);
  int callback3_result = callback3.Run(3);

  typedef internal::BindState<typename internal::FunctorTraits<Functor3>::RunnableType,
                              typename internal::FunctorTraits<Functor3>::RunType,
                              void(int,int)> 
                              BindStateType_IgnoreResult;
  BindStateType_IgnoreResult bst_ignore_result(internal::MakeRunnable(base::IgnoreResult(&Sum3)), 3, 4);
  base::Callback<int(int)> callback_ignore_result(&bst_ignore_result);
  int re = callback_ignore_result.Run(23);

  typedef internal::BindState<typename internal::FunctorTraits<Functor2>::RunnableType,
                              typename internal::FunctorTraits<Functor2>::RunType,
                              void(base::internal::UnretainedWrapper<Bar>, int)> 
                              BindStateType4;
  Bar object_bar(2);
  BindStateType4 bst4(internal::MakeRunnable(&Bar::Add), Unretained(&object_bar), 200);
  base::Callback<int(int)> callback4(&bst4);
  int callback4_result = callback4.Run(300);

  typedef internal::BindState<typename internal::FunctorTraits<Functor2>::RunnableType,
                              typename internal::FunctorTraits<Functor2>::RunType,
                              void(base::internal::UnretainedWrapper<Bar>)> BindStateType2;

  Bar b(2);
  BindStateType2 bst2(internal::MakeRunnable(&Bar::Add), Unretained(&b));

  Bar bbbb(2);
  base::Callback<int(int,int)> c4(new BindStateType2(internal::MakeRunnable(&Bar::Add), Unretained(&bbbb)));
  c4.Run(1,2);

  base::Callback<int(int,int)> c5 = base::Bind(&Bar::Add, Unretained(&bbbb));

  return 0;
} 
