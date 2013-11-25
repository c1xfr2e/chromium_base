#pragma once

template <typename T>
struct CallbackParamTraits {
  typedef const T& ForwardType;
  typedef T StorageType;
};

template <typename T>
T& CallbackForward(T& t) { return t; }

template <typename RunnableType>
class RunnableAdapter;


// Function: Arity 2.
template <typename R, typename A1, typename A2>
class RunnableAdapter<R(*)(A1, A2)> {
public:
  typedef R (RunType)(A1, A2);

  explicit RunnableAdapter(R(*function)(A1, A2))
    : function_(function) {
  }

  R Run(typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2) {
      return function_(CallbackForward(a1), CallbackForward(a2));
  }

private:
  R (*function_)(A1, A2);
};

// Method: Arity 2.
template <typename R, typename T, typename A1, typename A2>
class RunnableAdapter<R(T::*)(A1, A2)> {
public:
  typedef R (RunType)(T*, A1, A2);

  explicit RunnableAdapter(R(T::*method)(A1, A2))
    : method_(method) {
  }

  R Run(T* object, typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2) {
      return (object->*method_)(CallbackForward(a1), CallbackForward(a2));
  }

private:
  R (T::*method_)(A1, A2);
};

// Const Method: Arity 2.
template <typename R, typename T, typename A1, typename A2>
class RunnableAdapter<R(T::*)(A1, A2) const> {
public:
  typedef R (RunType)(const T*, A1, A2);

  explicit RunnableAdapter(R(T::*method)(A1, A2) const)
    : method_(method) {
  }

  R Run(const T* object, typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2) {
      return (object->*method_)(CallbackForward(a1), CallbackForward(a2));
  }

private:
  R (T::*method_)(A1, A2) const;
};

// Function: Arity 3.
template <typename R, typename A1, typename A2, typename A3>
class RunnableAdapter<R(*)(A1, A2, A3)> {
public:
  typedef R (RunType)(A1, A2, A3);

  explicit RunnableAdapter(R(*function)(A1, A2, A3))
    : function_(function) {
  }

  R Run(typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2,
    typename CallbackParamTraits<A3>::ForwardType a3) {
      return function_(CallbackForward(a1), CallbackForward(a2),
        CallbackForward(a3));
  }

private:
  R (*function_)(A1, A2, A3);
};

// Method: Arity 3.
template <typename R, typename T, typename A1, typename A2, typename A3>
class RunnableAdapter<R(T::*)(A1, A2, A3)> {
public:
  typedef R (RunType)(T*, A1, A2, A3);

  explicit RunnableAdapter(R(T::*method)(A1, A2, A3))
    : method_(method) {
  }

  R Run(T* object, typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2,
    typename CallbackParamTraits<A3>::ForwardType a3) {
      return (object->*method_)(CallbackForward(a1), CallbackForward(a2),
        CallbackForward(a3));
  }

private:
  R (T::*method_)(A1, A2, A3);
};

// Const Method: Arity 3.
template <typename R, typename T, typename A1, typename A2, typename A3>
class RunnableAdapter<R(T::*)(A1, A2, A3) const> {
public:
  typedef R (RunType)(const T*, A1, A2, A3);

  explicit RunnableAdapter(R(T::*method)(A1, A2, A3) const)
    : method_(method) {
  }

  R Run(const T* object, typename CallbackParamTraits<A1>::ForwardType a1,
    typename CallbackParamTraits<A2>::ForwardType a2,
    typename CallbackParamTraits<A3>::ForwardType a3) {
      return (object->*method_)(CallbackForward(a1), CallbackForward(a2),
        CallbackForward(a3));
  }

private:
  R (T::*method_)(A1, A2, A3) const;
};


class BindStateBase {
protected:
  virtual ~BindStateBase() {}
};

// BindState<>
template <typename Runnable, typename RunType, typename BoundArgsType, typename ReturnType>
struct BindState;

// Bind 2 arguments in 3.
template <typename Runnable, typename P1, typename P2, typename P3, typename R>
struct BindState<Runnable, R(P1, P2, P3), void(P1, P2), R> : public BindStateBase {
  typedef Runnable RunnableType;

  static R Run(BindStateBase* bind_state, typename CallbackParamTraits<P3>::ForwardType x3) {
    BindState* this_bind_state = static_cast<BindState*>(bind_state);
    return this_bind_state->runnable_.Run(this_bind_state->p1_, this_bind_state->p2_, x3);
  }

  BindState(const Runnable& runnable, const P1& p1, const P2& p2)
    : runnable_(runnable),
    p1_(p1),
    p2_(p2) {
  }

  RunnableType runnable_;
  P1 p1_;
  P2 p2_;
};

// Holds the Callback methods that don't require specialization to reduce
// template bloat.
class CallbackBase {
protected:
  typedef void(*InvokeFuncStorage)(void);
  explicit CallbackBase(BindStateBase* bind_state) 
    : bind_state_(bind_state), 
    polymorphic_invoke_(0) {
  }
  ~CallbackBase() {}

  BindStateBase* bind_state_;
  InvokeFuncStorage polymorphic_invoke_;
};


template <class RunType>
class Callback;

template <typename R, typename A1>
class Callback<R(A1)> : public CallbackBase {
public:
  typedef R(RunType)(A1);

  template <typename Runnable, typename RunType, typename BoundArgsType>
  Callback(BindState<Runnable, RunType, BoundArgsType, R>* bind_state)
    : CallbackBase(bind_state) {
      PolymorphicInvoke invoke_func = BindState<Runnable, RunType, BoundArgsType, R>::Run;
      polymorphic_invoke_ = reinterpret_cast<InvokeFuncStorage>(invoke_func);
  }

  R Run(typename CallbackParamTraits<A1>::ForwardType a1) const {
    PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);
    return f(bind_state_, CallbackForward(a1));
  }

private:
  typedef R(*PolymorphicInvoke)(BindStateBase*, typename CallbackParamTraits<A1>::ForwardType);

};