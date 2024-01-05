#include <fstream>
#include <random>
#include <vector>

#include "../ims/ims.hpp"
#include "../ims/machine.hpp"
#include "profiler.hpp"

template <class T>
T blank_job(const T& job) {
  return job;
}

struct TestTimeResult {
  uint32_t jobs_count = 0;
  uint32_t machines_count = 0;
  uint32_t accur_time = 0;
  uint32_t appr_time = 0;
  friend std::ostream& operator<<(std::ostream& out,
                                  const TestTimeResult& k_test);
};

struct TestApproximationResult {
  uint32_t jobs_count = 0;
  uint32_t machines_count = 0;
  uint32_t accur_result = 0;
  uint32_t appr_result = 0;
  std::vector<uint32_t> jobs = std::vector<uint32_t>();
  friend std::ostream& operator<<(std::ostream& out,
                                  const TestApproximationResult& k_test);
};

std::ostream& operator<<(std::ostream& out, const TestTimeResult& k_test) {
  out << k_test.jobs_count << ',' << k_test.machines_count << ','
      << k_test.appr_time << ',' << k_test.accur_time << '\n';
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         const TestApproximationResult& k_test) {
  out << k_test.jobs_count << ',' << k_test.machines_count << ','
      << k_test.appr_result << ',' << k_test.accur_result << ',';
  bool success = k_test.appr_result <=
                 double(k_test.accur_result) *
                     (4.0 / 3.0 - 1.0 / (3.0 * double(k_test.machines_count)));
  out << success ? "OK" : "NOT OK";
  for (uint32_t job : k_test.jobs) {
    out << ',' << job;
  }
  out << '\n';
  return out;
}

TestTimeResult TestTime(uint32_t jobs_count, uint32_t machines_count) {
  Profiler micro_profiler = Profiler<std::chrono::microseconds>();
  Profiler milli_profiler = Profiler<std::chrono::milliseconds>();
  const uint32_t kMaxTime = 100;
  std::vector<uint32_t> jobs(jobs_count);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> jobs_gen(1, kMaxTime);
  for (auto& job : jobs) {
    job = jobs_gen(gen);
  }
  std::vector<Machine<uint32_t, uint32_t, blank_job>> machines(machines_count);
  IMS<uint32_t, Machine<uint32_t, uint32_t, blank_job>, uint32_t> ims(jobs,
                                                                      machines);
  micro_profiler.Reset();
  uint32_t appr_result = ims.ApproximateOptimalTime();
  uint32_t appr_time = micro_profiler.GetPassedTime();
  milli_profiler.Reset();
  uint32_t accurate_result = ims.FindOptimalTime();
  uint32_t accur_time = milli_profiler.GetPassedTime();
  return TestTimeResult{jobs_count, machines_count, accur_time, appr_time};
}

TestApproximationResult TestApproximation(uint32_t max_time, uint32_t max_jobs,
                                          uint32_t max_machines) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> num_jobs_gen(1, max_jobs);
  std::uniform_int_distribution<> machines_gen(1, max_machines);
  std::uniform_int_distribution<> jobs_gen(1, max_time);
  uint32_t jobs_count = num_jobs_gen(gen);
  uint32_t machines_count = machines_gen(gen);
  std::vector<uint32_t> jobs(jobs_count);
  for (auto& job : jobs) {
    job = jobs_gen(gen);
  }
  std::vector<Machine<uint32_t, uint32_t, blank_job>> machines(machines_count);
  IMS<uint32_t, Machine<uint32_t, uint32_t, blank_job>, uint32_t> ims(jobs,
                                                                      machines);
  uint32_t appr_result = ims.ApproximateOptimalTime();
  uint32_t accurate_result = ims.FindOptimalTime();
  return TestApproximationResult{jobs_count, machines_count, accurate_result,
                                 appr_result, jobs};
}

void Test1(std::ofstream& out) {
  out << "jobs count,machines count,appr time, accur time" << std::endl;
  uint32_t machines_count = 2;
  for (uint32_t jobs_count = 8; jobs_count <= 36; jobs_count += 4) {
    TestTimeResult res = TestTime(jobs_count, machines_count);
    out << res << std::flush;
  }
}

void Test2(std::ofstream& out) {
  out << "jobs count,machines count,appr time, accur time" << std::endl;
  uint32_t machines_count = 3;
  for (uint32_t jobs_count = 8; jobs_count <= 20; jobs_count += 4) {
    TestTimeResult res = TestTime(jobs_count, machines_count);
    out << res << std::flush;
  }
}

void Test3(std::ofstream& out) {
  out << "jobs count,machines count,appr result,accur result,status,jobs..."
      << std::endl;
  const uint32_t kNumberOfTests = 100;
  const uint32_t kMaxTime = 100;
  const uint32_t kMaxJobs = 12;
  const uint32_t kMaxMachines = 4;
  for (uint32_t test = 0; test < kNumberOfTests; ++test) {
    TestApproximationResult res =
        TestApproximation(kMaxTime, kMaxJobs, kMaxMachines);
    out << res << std::flush;
  }
}

int main() {
  std::ofstream out;
  out.open("../results/test_time_2_machines.csv");
  Test1(out);
  out.close();
  out.open("../results/test_time_3_machines.csv");
  Test2(out);
  out.close();
  out.open("../results/test_approximation.csv");
  Test3(out);
  out.close();
  return 0;
}