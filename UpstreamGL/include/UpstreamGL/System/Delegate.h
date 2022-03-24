#pragma once

#include <cassert>
#include <functional>
#include <type_traits>

#include "Event.h"

namespace upgl {
template <auto F> struct ConnectFunc_t {};

template <auto Func> inline constexpr ConnectFunc_t<Func> ConnectFunc{};

template <typename> struct Delegate;

template <class Ret, class... Args> struct Delegate<Ret(Args...)> {
  using lambda_binder = Ret(void *, Args...);

public:
  using func_type = Ret(Args...);

  template <class Member> using member_type = Ret (Member::*)(Args...);

public:
  template <auto Func, class CallerType>
  Delegate(ConnectFunc_t<Func>, CallerType caller) : m_Binder(nullptr), m_Caller(nullptr) {
    Connect(ConnectFunc<Func>, caller);
  }

  template <auto Func> Delegate(ConnectFunc_t<Func>) : m_Binder(nullptr), m_Caller(nullptr) {
    Connect(ConnectFunc<Func>);
  }

  Delegate() : m_Binder(nullptr), m_Caller(nullptr) {}

  template <auto Func, class CallerType> void Connect(ConnectFunc_t<Func>, CallerType caller) {
    static_assert(std::is_same_v<decltype(Func), member_type<std::remove_pointer_t<CallerType>>>,
                  "Unable to connect a function that doesn't match the delegate type!");
    m_Caller = caller;
    m_Binder = [](void *caller, Args... args) -> Ret {
      return Ret(std::invoke(Func, static_cast<CallerType>(caller), std::forward<Args>(args)...));
    };
  }

  template <auto Func> void Connect(ConnectFunc_t<Func>) {
    static_assert(std::is_same_v<decltype(Func), Ret (*)(Args...)>,
                  "Unable to connect a function that doesn't match the delegate type!");
    m_Binder = [](void *caller, Args... args) -> Ret { return Func(std::forward<Args>(args)...); };
  }

  Ret Invoke(Args... args) { return m_Binder(m_Caller, std::forward<Args>(args)...); }

  Ret operator()(Args... args) { return Invoke(std::forward<Args>(args)...); }

  bool operator==(const Delegate<Ret(Args...)> other) const {
    return m_Caller == other.m_Caller && m_Binder == other.m_Binder;
  }

private:
  lambda_binder *m_Binder;
  void *m_Caller;
};

struct EventDelegateBase {
  virtual ~EventDelegateBase() = default;
  virtual bool Invoke(const Event *const e) = 0;
};
} // namespace upgl