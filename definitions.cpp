#include "definitions.hpp"

template<typename T, typename U = T>
class ChainRange {
  public:
    class iterator : public std::iterator<std::input_iterator_tag, T> {
      public:
        explicit iterator(const ChainRange& cr, T value) : cr(cr), value(value) {}

        T operator*() const {
          if (firstValue) {
            firstValue = false;
            return (cr.extraValue != cr.start) ? cr.extraValue : value; // Return extraValue only if it's different from start
          }
          return value;
        }

        iterator& operator++() {
          value += cr.step;
          return *this;
        }

        bool operator!=(const iterator& other) const {
          return (cr.step > 0 && value < other.value) || (cr.step < 0 && value > other.value);
        }

      private:
        const ChainRange& cr;
        T value;
        mutable bool firstValue = true; // Track if it's the first value
    };

    // Constructor with extraValue, handles the case when start and extraValue are the same
    ChainRange(T start, T stop, T step, U extraValue) : start(start), stop(stop), step(step), extraValue(extraValue) {}

    // Original constructor for backward compatibility
    ChainRange(T start, T stop, T step) : ChainRange(start, stop, step, start) {}

    iterator begin() const {
      return iterator(*this, start);
    }

    iterator end() const {
      return iterator(*this, stop);
    }

  private:
    T start;
    T stop;
    T step;
    U extraValue;
};

template<typename T>
class ChainArray {
  public:
    class iterator : public std::iterator<std::input_iterator_tag, T> {
      public:
        explicit iterator(const ChainArray& ca, const std::vector<T>& values, size_t currentIteration)
          : ca(ca), values(values), currentIteration(currentIteration) {}

        T operator*() const {
          return values[currentIteration];
        }

        iterator& operator++() {
          ++currentIteration;
          return *this;
        }

        bool operator!=(const iterator& other) const {
          return currentIteration != other.currentIteration;
        }

      private:
        const ChainArray& ca;
        const std::vector<T>& values;
        size_t currentIteration;

        friend class ChainArray;  // Allow ChainArray to access private members
    };

    // Constructor with array input and starting offset
    ChainArray(const std::vector<T>& values, size_t iterations, size_t offset = 0) : values(values), iterations(iterations), offset(std::min(offset, values.size() - 1)) {}

    iterator begin() const {
      return iterator(*this, values, offset);
    }

    iterator end() const {
      iterator iter(*this, values, std::min(offset + iterations, values.size()));
      return iter;
    }

  private:
    std::vector<T> values;
    size_t iterations;
    size_t offset;
};

std::vector<int> iter::LEDS_BOTH(int r1start, int r1stop, int r2start, int r2stop, int step = 1) {
  std::vector<int> result;

  ChainRange<int> range1(rings.large.relative.ringStartLed_r[r1start], rings.large.relative.ringEndLed_r[r1stop], step);
  ChainRange<int> range2(rings.small.relative.ringStartLed_r[r2start], rings.small.relative.ringEndLed_r[r2stop], step);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));
  std::copy(range2.begin(), range2.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_LARGE(int r1start, int r1stop, int step = 1) {
  std::vector<int> result;

  ChainRange<int> range1(rings.large.relative.ringStartLed_r[r1start], rings.large.relative.ringEndLed_r[r1stop], step);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_SMALL(int r1start, int r1stop, int step = 1) {
  std::vector<int> result;

  ChainRange<int> range1(rings.small.relative.ringStartLed_r[r1start], rings.small.relative.ringEndLed_r[r1stop], step);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_BOTH_BAR(int r1start, int r1stop, int r2start, int r2stop) {
  std::vector<int> result;

  if (!(r1start < 0 || r1start >= ledmap.large.size())) {
    ChainArray<int> range1({ledmap.large[r1start]}, r1stop, 0);

    std::copy(range1.begin(), range1.end(), std::back_inserter(result));
  }

  if (!(r2start < 0 || r2start >= ledmap.small.size())) {
    ChainArray<int> range2({ledmap.small[r2start]}, r2stop, 0);

    std::copy(range2.begin(), range2.end(), std::back_inserter(result));
  }

  return result;
}

std::vector<int> iter::LEDS_LARGE_BAR(int r1start, int r1stop) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.large.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.large[r1start]}, r1stop, 0);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_SMALL_BAR(int r1start, int r1stop) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.small.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.small[r1start]}, r1stop, 0);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_BOTH_BAR_INV(int r1start, int r1stop, int r2start, int r2stop) {
  std::vector<int> result;

  if (!(r1start < 0 || r1start >= ledmap.largeRev.size())) {
    ChainArray<int> range1({ledmap.largeRev[r1start]}, r1stop, 0);

    std::copy(range1.begin(), range1.end(), std::back_inserter(result));
  }

  if (!(r2start < 0 || r2start >= ledmap.smallRev.size())) {
    ChainArray<int> range2({ledmap.smallRev[r2start]}, r2stop, 0);

    std::copy(range2.begin(), range2.end(), std::back_inserter(result));
  }

  return result;
}

std::vector<int> iter::LEDS_LARGE_BAR_INV(int r1start, int r1stop) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.largeRev.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.largeRev[r1start]}, r1stop, 0);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_SMALL_BAR_INV(int r1start, int r1stop) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.smallRev.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.smallRev[r1start]}, r1stop, 0);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_BOTH_PEAK(int r1start, int r1offset, int r2start, int r2offset) {
  std::vector<int> result;

  if (!(r1start < 0 || r1start >= ledmap.large.size())) {
    ChainArray<int> range1({ledmap.large[r1start]}, 1, r1offset);

    std::copy(range1.begin(), range1.end(), std::back_inserter(result));
  }

  if (!(r2start < 0 || r2start >= ledmap.small.size())) {
    ChainArray<int> range2({ledmap.small[r2start]}, 1, r2offset);

    std::copy(range2.begin(), range2.end(), std::back_inserter(result));
  }

  return result;
}

std::vector<int> iter::LEDS_LARGE_PEAK(int r1start, int r1offset) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.large.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.large[r1start]}, 1, r1offset);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_SMALL_PEAK(int r1start, int r1offset) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.small.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.small[r1start]}, 1, r1offset);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_BOTH_PEAK_INV(int r1start, int r1offset, int r2start, int r2offset) {
  std::vector<int> result;

  if (!(r1start < 0 || r1start >= ledmap.largeRev.size())) {
    ChainArray<int> range1({ledmap.largeRev[r1start]}, 1, r1offset);

    std::copy(range1.begin(), range1.end(), std::back_inserter(result));
  }

  if (!(r2start < 0 || r2start >= ledmap.smallRev.size())) {
    ChainArray<int> range2({ledmap.smallRev[r2start]}, 1, r2offset);

    std::copy(range2.begin(), range2.end(), std::back_inserter(result));
  }

  return result;
}

std::vector<int> iter::LEDS_LARGE_PEAK_INV(int r1start, int r1offset) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.largeRev.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.largeRev[r1start]}, 1, r1offset);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}

std::vector<int> iter::LEDS_SMALL_PEAK_INV(int r1start, int r1offset) {
  std::vector<int> result;

  if (r1start < 0 || r1start >= ledmap.smallRev.size()) {
    return result;
  }

  ChainArray<int> range1({ledmap.smallRev[r1start]}, 1, r1offset);

  std::copy(range1.begin(), range1.end(), std::back_inserter(result));

  return result;
}