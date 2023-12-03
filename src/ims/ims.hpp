#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

template <class Job, class Machine, class Time> class IMS {
public:
  IMS(const std::vector<Job> &jobs, const std::vector<Machine> &machines);
  Time FindOptimalTime() const;
  Time ApproximateOptimalTime() const;

private:
  Time IterateNaive(std::vector<Machine> &machines_iter, size_t job_pos) const;
  std::vector<Job> jobs = std::vector<Job>();
  std::vector<Machine> machines = std::vector<Machine>();
  size_t num_of_jobs = 0;
  size_t num_of_machines = 0;
};

template <class Job, class Machine, class Time>
IMS<Job, Machine, Time>::IMS(const std::vector<Job> &jobs,
                             const std::vector<Machine> &machines)
    : jobs(jobs), machines(machines), num_of_jobs(jobs.size()),
      num_of_machines(machines.size()) {}

template <class Job, class Machine, class Time>
Time IMS<Job, Machine, Time>::ApproximateOptimalTime() const {
  auto machines_copy = machines;
  auto job_copy = jobs;
  std::make_heap(machines_copy.begin(), machines_copy.end(), std::greater<>{});
  std::sort(job_copy.begin(), job_copy.end());
  for (const auto &job : job_copy) {
    std::pop_heap(machines_copy.begin(), machines_copy.end(), std::greater<>{});
    machines_copy.back().AddJob(job);
    std::push_heap(machines_copy.begin(), machines_copy.end(),
                   std::greater<>{});
  }
  return std::max_element(machines_copy.begin(), machines_copy.end())
      ->GetSummaryLoad();
}

template <class Job, class Machine, class Time>
Time IMS<Job, Machine, Time>::FindOptimalTime() const {
  auto machines_copy = machines;
  return IterateNaive(machines_copy, 0);
}

template <class Job, class Machine, class Time>
Time IMS<Job, Machine, Time>::IterateNaive(std::vector<Machine> &machines_iter,
                                           size_t job_pos) const {
  Time t_min = std::accumulate(jobs.begin(), jobs.end(), 0);
  for (auto &machine : machines_iter) {
    machine.AddJob(jobs[job_pos]);
    if (job_pos + 1 < jobs.size()) {
      t_min = std::min(t_min, IterateNaive(machines_iter, job_pos + 1));
    } else {
      t_min = std::max_element(machines_iter.begin(), machines_iter.end())
                  ->GetSummaryLoad();
    }
    machine.CancelJob();
  }
  return t_min;
}