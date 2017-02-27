#ifndef MATRIXPROD_PAPI_HPP_
#define MATRIXPROD_PAPI_HPP_

#include <vector>

class Papi {
 private:
  std::vector<int> installed_events;
  int event_set;

 public:
  Papi();
  ~Papi();
  int Init();
  int InstallEvents();
  int Start();
  int StopAndReset();
  int Destroy();

 private:
  int AddEvent(int);
  void PrintCounters(long long *values);
};

#endif  // MATRIXPROD_PAPI_HPP_
