#include "my_callback.h"

int Sum3(int a, int b, int c) {
  return a + b + c;
}

class Bar {
public:
  Bar(int x) : i_(x) {}
  int Add(int x, int y) { return i_ + x + y; }
private:
  int i_;
};

void f1()

int main() {
  typedef BindState<RunnableAdapter<int(*)(int,int,int)>,
                    int(int,int,int),
                    void(int,int),
                    int> BS1;
  
  typedef Callback<int(int)> SameCallbackType;

  BS1 bs1(RunnableAdapter<int(*)(int,int,int)>(&Sum3), 10, 20);
  SameCallbackType cb1(&bs1);
  int total = cb1.Run(30);

  typedef BindState<RunnableAdapter<int(Bar::*)(int,int)>,
                    int(Bar*,int,int),
                    void(Bar*,int),
                    int> BS2;
  Bar object_bar(1);
  BS2 bs2(RunnableAdapter<int(Bar::*)(int,int)>(&Bar::Add), &object_bar, 800);
  SameCallbackType cb2(&bs2);
  total = cb2.Run(60);

  return 0;
}