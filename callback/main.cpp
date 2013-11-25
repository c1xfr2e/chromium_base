// Copyright (c) 2012 The Wandoujia Authors. All rights reserved.
#include <objbase.h>
#include "base/callback.h"
#include "base/bind.h"
#include "base/bind_internal.h"
using namespace base;
using namespace internal;

int Sum(int a, int b) {
  return a + b;
}

int Sum3(int a, int b, int c) {
  return a + b + c;
}

int Sum4(int a, int b, int c, int d) {
  return a + b + c + d;
}

class Foo {
public:
  Foo(int x) : i_(x) {}
  int Add(int x) { return x + i_; }
  ~Foo() { }
  int i_;
};

class Bar {
public:
  Bar(int x) : i_(x) {}
  Bar(const Bar& f) : i_(f.i_) {}
  int Add(int x, int y) { return i_ + x + y; }
  int Add2(int x) const { return i_ + x; }
private:
  int i_;
};

template<typename T>
void function(void* p, int x) {
  T* T = reinterpret_cast<T*>(p);
}


int DoSomething(int arg) { 
  int i = arg++;
  i++;
  return i;
}

int ProcessBar(const Bar& b) {
  return b.Add2(1);
}

int main() {
  // const reference to function pointer
  int (*const& Ptr)(int,int) = &Sum;

  typedef BindState<RunnableAdapter<int(*)(int,int)>, int(int,int), void()> BindStateType1;
  BindStateType1 bst1((RunnableAdapter<int(*)(int,int)>)(&Sum));
  Callback<int(int,int)> callback1(&bst1);
  callback1.Run(1998, 2004);

  typedef BindState<RunnableAdapter<int(*)(int,int,int)>,
                    int(int,int,int),
                    void(int,int)> 
                    BindStateType2;
  BindStateType2 bst2(MakeRunnable(&Sum3), 1, 2);
  Callback<int(int)> callback2(&bst2);
  int callback2_result = callback2.Run(3);

  typedef BindState<RunnableAdapter<int(Bar::*)(int,int)>,
                    int(Bar*,int,int),
                    void(UnretainedWrapper<Bar>, int)> 
                    BindStateType3;
  Bar object_bar(2);
  BindStateType3 bst3(MakeRunnable(&Bar::Add), Unretained(&object_bar), 200);
  Callback<int(int)> callback3(&bst3);
  int callback3_result = callback3.Run(300);

  typedef BindState<RunnableAdapter<int(*)(int,int,int,int)>,
    int(int,int,int,int),
    void(int,int)> 
    BindStateType4;
  BindStateType4 bst4(MakeRunnable(&Sum4), 1, 2);
  Callback<int(int,int)> callback4(&bst4);
  int callback4_result = callback4.Run(3,4);

  // ignore result
  Callback<void(int)> cbvi = Bind(IgnoreResult(&DoSomething));
  cbvi.Run(1);

  Callback<int(void)> cbb = Bind(&ProcessBar, object_bar);
  int result = cbb.Run();


  return 0;
} 
