#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

class InterruptibleSleeper {
 public:
  InterruptibleSleeper() : m_locker(m_mtx) {}

  /// \brief This call blocks until either it returns true if this slept for the time given, or it will return false if
  /// woken up prematurely.
  template <class ChronoDuration>
  bool trySleepFor(ChronoDuration time) {
    // if we're here and wake() is called, then wake() will block since it's waiting to acquire the lock. wait_for()
    // will atomically release the lock and block, at which point wake() will acquire the lock and immeidately wake up
    // and notify the cv to continue.
    return m_cv.wait_for(m_locker, time) == std::cv_status::timeout;
  }

  /// \brief Wake up the sleeping thread if it is currently sleeping. This call blocks until the other thread has fallen
  /// asleep. In other words, this will block if the other thread is in the middle of processing. This way, next time it
  /// falls asleep, it will wake up immediately.
  void wake() {
    // if the cv is not currently waiting, then the lock has been acquired by it. This call will block until the next
    // time wait_for() unlocks. At that point it will acquire the lock.
    std::unique_lock<std::mutex> locker(m_mtx);
    m_cv.notify_one();
  }

 private:
  std::mutex                   m_mtx;
  std::unique_lock<std::mutex> m_locker;
  std::condition_variable      m_cv;
};
