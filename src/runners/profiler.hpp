#include <chrono>
#include <vector>

template <class Unit = std::chrono::nanoseconds>
class Profiler {
    public:
    Profiler(): start(std::chrono::high_resolution_clock::now()), intervals() {};
    uint32_t  GetPassedTime() const;
    std::vector<uint32_t> GetInrevals() const;
    void SetInterval();
    void Reset();
    private:
    std::chrono::time_point<std::chrono::system_clock> start;
    std::vector<uint32_t> intervals;
};

template <class Unit>
uint32_t Profiler<Unit>::GetPassedTime() const {
    return std::chrono::duration_cast<Unit>(std::chrono::high_resolution_clock::now() - start).count();
}

template <class Unit>
std::vector<uint32_t> Profiler<Unit>::GetInrevals() const {
    return intervals;
}

template <class Unit>
void Profiler<Unit>::Reset() {
    start = std::chrono::high_resolution_clock::now();
    intervals.clear();
}

template <class Unit>
void Profiler<Unit>::SetInterval() {
    intervals.push_back(GetPassedTime());
}