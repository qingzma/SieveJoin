#ifndef QJOIN_UTIL_TIMER_H_
#define QJOIN_UTIL_TIMER_H_
#include <chrono>

namespace qjoin {

time_t GetEpoch() {
  struct tm epoch;
  epoch.tm_year = 70;
  epoch.tm_hour = 0;
  epoch.tm_isdst = 0;
  epoch.tm_mday = 1;
  epoch.tm_min = 0;
  epoch.tm_mon = 0;
  epoch.tm_sec = 0;
  return mktime(&epoch);
}

class Timer {
 private:
  bool activate_;
  std::chrono::high_resolution_clock::time_point start_point_;
  std::chrono::high_resolution_clock::duration duration_;

 public:
  Timer(){};
  virtual ~Timer(){};
  void Start() {
    activate_ = true;
    start_point_ = std::chrono::high_resolution_clock::now();
  }

  void Stop() {
    if (activate_) {
      duration_ += std::chrono::duration_cast<
          std::chrono::high_resolution_clock::duration>(
          std::chrono::high_resolution_clock::now() - start_point_);
      start_point_ = std::chrono::high_resolution_clock::time_point();
      activate_ = false;
    }
  }

  void Pause() { Stop(); }

  void Reset() {
    start_point_ = std::chrono::high_resolution_clock::time_point();
    duration_ = std::chrono::high_resolution_clock::duration();
    activate_ = false;
  }

  void UpdateDuration() {
    Stop();
    Start();
  }

  double Seconds() {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::seconds>(duration_).count());
  }

  double MilliSeconds() {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(duration_)
            .count());
  }

  double MicroSeconds() {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::microseconds>(duration_)
            .count());
  }
};
}  // namespace qjoin

#endif  // QJOIN_UTIL_TIMER_H_