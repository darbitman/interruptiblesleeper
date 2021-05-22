#include <iostream>

#include "InterruptibleSleeper.hpp"

InterruptibleSleeper sleeper;

void process() {
  while (true) {
    if (sleeper.trySleepFor(std::chrono::seconds(1))) {
      std::cout << "Timeout: "
                << std::chrono::duration_cast<std::chrono::seconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch())
                       .count()
                << '\n';
    } else {
      std::cout << "Woke up: "
                << std::chrono::duration_cast<std::chrono::seconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch())
                       .count()
                << '\n';
      return;
    }
  }
}

int main() {
  std::thread t1(process);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  sleeper.wake();
  t1.join();
  return 0;
}
