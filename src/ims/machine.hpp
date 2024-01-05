#include <compare>
#include <deque>

template <class Job, class Time, Time (*F)(const Job &)> class Machine {
public:
  void AddJob(const Job &job);
  Job FinishJob();
  Job CancelJob();
  void Reset();
  Time GetSummaryLoad() const;
  std::deque<Job> GetJobsQueue();
  std::strong_ordering operator<=>(const Machine &other);

private:
  Time summary_load = 0;
  std::deque<Job> jobs = std::deque<Job>();
};

template <class Job, class Time, Time (*F)(const Job &)>
std::strong_ordering
Machine<Job, Time, F>::operator<=>(const Machine<Job, Time, F> &other) {
  return summary_load <=> other.summary_load;
}

template <class Job, class Time, Time (*F)(const Job &)>
void Machine<Job, Time, F>::AddJob(const Job &job) {
  jobs.push_back(job);
  summary_load += F(job);
}

template <class Job, class Time, Time (*F)(const Job &)>
Time Machine<Job, Time, F>::GetSummaryLoad() const {
  return summary_load;
}

template <class Job, class Time, Time (*F)(const Job &)>
Job Machine<Job, Time, F>::CancelJob() {
  Job removed_job = jobs.back();
  jobs.pop_back();
  summary_load -= F(removed_job);
  return removed_job;
}

template <class Job, class Time, Time (*F)(const Job &)>
Job Machine<Job, Time, F>::FinishJob() {
  Job removed_job = jobs.front();
  jobs.pop_front();
  summary_load -= F(removed_job);
  return removed_job;
}

template <class Job, class Time, Time (*F)(const Job &)>
std::deque<Job> Machine<Job, Time, F>::GetJobsQueue() {
  return jobs;
}

template <class Job, class Time, Time (*F)(const Job &)>
void Machine<Job, Time, F>::Reset() {
  for (size_t i = 0; i < jobs.size(); ++i) {
    CancelJob();
  }
}
