// Collatz specfic structures and routines
#pragma once

#include "global.h"

static const bool EVEN = false;
static const bool ODD = true;

class ParityVector {
 public:
  ParityVector() { m_span = 0; }
  ParityVector(const std::string& parvec_str) { from_string(parvec_str); }

  size_t norm() { return parvec.size(); }
  size_t span() { return m_span; }

  std::vector<bool> parvec;

  void from_string(const std::string& parvec_str) {
    for (char c : parvec_str) {
      if (!(c == '0' || c == '1')) {
        fatal_error_log(
            "String `%s` does not represent a valid parity vector. Abort.",
            parvec_str.c_str());
      }

      parvec.push_back(c == '1');
      m_span += (size_t)(c == '1');
    }
  }

 private:
  size_t m_span;
};
