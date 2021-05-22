#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

class InterruptibleSleeper {
 public:
  /// \brief Returns true if this slept for the time given. Otherwise, returns false if woken up prematurely.
  template <class ChronoDuration>
  bool trySleepFor(ChronoDuration time) {
    m_timed_locker.lock();
    m_sleeping.store(true, std::memory_order_release);

    const bool sleptTilEnd = !m_timed_locker.try_lock_for(time);
    // In the case that wake() wasn't called, the mutex will fail to lock (ie. try_lock timed out), then it slept
    // successfully, and it means it's still locked.
    // In the case wake() was called, the mutex will lock, (ie. try_lock succeeded).
    // In both cases, woken up or timed out, the lock is still locked at this point, so it needs to be unlocked.
    m_sleeping.store(false, std::memory_order_release);
    m_timed_locker.unlock();
    return sleptTilEnd;
  }

  bool wake() {
    if (m_sleeping.load(std::memory_order::memory_order_acquire)) {
      m_timed_locker.unlock();
      m_sleeping.store(false, std::memory_order_release);
      return true;
    }
    return false;
  }

 private:
  std::atomic_bool m_sleeping;
  std::timed_mutex m_timed_locker;
};
