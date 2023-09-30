#pragma once

namespace scc::common {

class NonCopyable {
public:
  NonCopyable(const NonCopyable &rhs) = delete;
  NonCopyable(NonCopyable &&rhs) = delete;
  NonCopyable &operator=(const NonCopyable &rhs) = delete;
  NonCopyable &operator=(NonCopyable &&rhs) = delete;
  ~NonCopyable() = default;

protected:
  NonCopyable() = default;
};

template<class T>
class Singleton : public NonCopyable {
public:
  Singleton() {
    instance_ = static_cast<T*>(this);
  }
  ~Singleton() {
    instance_ = nullptr;
  }

protected:
  static T *get_instance() {
    return instance_;
  }

private:
  static T *instance_;
};

template<class T>
T *Singleton<T>::instance_ = nullptr;

} // scc::common
