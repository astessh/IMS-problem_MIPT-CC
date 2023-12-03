#include <../ims/ims.hpp>
#include <../ims/machine.hpp>
#include <vector>

template <class T> T blank(const T &job) { return job; }

void Print(uint32_t jobs_num, uint32_t machines_num, uint32_t appr,
           uint32_t accur) {
  bool estimation_correct = appr <= static_cast<double>(4 * accur) / 3.0;
  std::string state =
      (appr <= static_cast<double>(4 * accur) / 3.0) ? " OK;" : " NOT OK;";
  std::cout << "job_count: " << jobs_num << " machines_num: " << machines_num
            << " approximatated time: " << appr << " accurate time: " << accur
            << state << std::endl;
}

void Test() {
  const size_t kMaxTime = 100;
  for (uint32_t jobs_num = 5; jobs_num < 15; jobs_num += 1) {
    std::vector<uint32_t> jobs(jobs_num);
    for (auto &job : jobs) {
      job = rand() % kMaxTime;
    }
    uint32_t machines_num = 2 * (jobs_num / 5);
    std::vector<Machine<uint32_t, uint32_t, blank>> machines(machines_num);
    IMS<uint32_t, Machine<uint32_t, uint32_t, blank>, uint32_t> ims(jobs,
                                                                    machines);
    uint32_t appr_result = ims.ApproximateOptimalTime();
    uint32_t accurate_result = ims.FindOptimalTime();
    Print(jobs_num, machines_num, appr_result, accurate_result);
  }
}

int main() { Test(); }