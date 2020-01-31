#pragma once

#include "quill/TweakMe.h"

#include <chrono>

namespace quill::detail
{
/**
 * Converts tsc ticks to nanoseconds since epoch
 */
class RdtscClock
{
  /**
   * A static class that calculates the rdtsc ticks per second
   */
  class RdtscTicks
  {
  public:
    [[nodiscard]] static RdtscTicks& instance()
    {
      static RdtscTicks inst;
      return inst;
    }

    /***/
    double ticks_per_ns() const noexcept { return ticks_per_ns_; }

  private:
    /**
     * Constructor
     */
    RdtscTicks();

    double ticks_per_ns_{0};
  };

public:
  /**
   * Constructor
   * @param resync_interval the interval to resync the tsc clock with the real system wall clock
   */
  explicit RdtscClock(std::chrono::nanoseconds resync_interval = std::chrono::seconds{QUILL_RDTSC_RESYNC_INTERVAL});

  /**
   * Convert tsc cycles to nanoseconds
   * @param tsc
   * @return
   */
  std::chrono::system_clock::time_point time_since_epoch(uint64_t rdtsc_value) const noexcept;

  /**
   * Sync base wall time and base tsc.
   * @see static constexpr std::chrono::minutes resync_timer_{5};
   */
  void resync() const noexcept;

private:
  mutable uint64_t base_time_{0}; /**< Get the initial base time in nanoseconds from epopch */
  mutable uint64_t base_tsc_{0};  /**< Get the initial base tsc time */
  mutable uint64_t resync_interval_ticks_{0};
  uint64_t resync_interval_orginal_{0}; /**< stores the initial interval value as as if we fail to resync we increase the timer */
  double ticks_per_nanosecond_{0};
};
} // namespace quill::detail