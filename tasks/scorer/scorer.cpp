#include "scorer.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

bool operator<(const Event& lhs, const Event& rhs) {
    return lhs.time < rhs.time;
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::set<Event>
        sorted_happened_events;  // anyway we will need O(N logN) complexity so lets just use set for simplicity
    std::copy_if(events.begin(), events.end(), std::inserter(sorted_happened_events, sorted_happened_events.begin()),
                 [score_time](const auto& event) { return event.time <= score_time; });
    std::unordered_map<TaskName, std::unordered_set<StudentName>> merges_per_task;
    ScoreTable currently_accepted;
    for (auto& [guy, task, time, type] : sorted_happened_events) {
        switch (type) {
            case EventType::MergeRequestClosed:
                merges_per_task[task].erase(guy);
                break;
            case EventType::MergeRequestOpen:
                merges_per_task[task].insert(guy);
                break;
            case EventType::CheckFailed:
                currently_accepted[guy].erase(task);
                break;
            case EventType::CheckSuccess:
                if (merges_per_task[task].count(guy) == 0) {
                    currently_accepted[guy].insert(task);
                }
                break;
        }
    }
    std::erase_if(currently_accepted, [](const auto& student_results) { return student_results.second.empty(); });
    return currently_accepted;
}
