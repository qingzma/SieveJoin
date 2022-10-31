#ifndef QJOIN_UTIL_TIMER_H__
#define QJOIN_UTIL_TIMER_H__
#include <chrono>

namespace qjoin {

time_t GetEpoch();

class Timer {
 private:
  bool activate_;
  // std::chrono::high_resolution_clock::time_point start_point_;
  // std::chrono::high_resolution_clock::duration duration_;
  std::chrono::steady_clock::time_point start_point_;
  std::chrono::steady_clock::time_point mark_point_;
  double duration_;

 public:
  Timer(){};
  virtual ~Timer(){};
  void Start() {
    activate_ = true;
    start_point_ = std::chrono::steady_clock::now();
    mark_point_ = start_point_;
  }

  void Stop() {
    if (activate_) {
      duration_ += std::chrono::duration_cast<std::chrono::duration<double>>(
                       std::chrono::steady_clock::now() - start_point_)
                       .count();
      start_point_ = std::chrono::steady_clock::time_point();
      activate_ = false;
    }
  }

  // void Pause() { Stop(); }

  void Mark() { mark_point_ = std::chrono::steady_clock::now(); }

  double Seconds() {
    return std::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::steady_clock::now() - start_point_)
        .count();
  }

  double SecondsSinceMarked() {
    return std ::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::steady_clock::now() - mark_point_)
        .count();
  }
};
}  // namespace qjoin

#endif  // QJOIN_UTIL_TIMER_H__