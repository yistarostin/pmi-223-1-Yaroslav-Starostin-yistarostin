#include "scorer.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

bool operator<(const Event& lhs, const Event& rhs) {
    return lhs.time < rhs.time;
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::multiset<Event>
        sorted_happened_events;  // anyway we will need O(N logN) complexity so lets just use set for simplicity
    std::copy_if(events.begin(), events.end(), std::inserter(sorted_happened_events, sorted_happened_events.begin()),
                 [score_time](const auto& event) { return event.time <= score_time; });
    std::unordered_map<StudentName, std::map<TaskName, int>> opened_merge_requsts;
    std::unordered_map<StudentName, std::map<TaskName, bool>> last_submit_results;
    ScoreTable currently_accepted;
    for (auto& [guy, task, time, type] : sorted_happened_events) {
        switch (type) {
            case EventType::MergeRequestClosed:
                --opened_merge_requsts[guy][task];
                break;
            case EventType::MergeRequestOpen:
                ++opened_merge_requsts[guy][task];
                break;
            case EventType::CheckFailed:
                last_submit_results[guy][task] = false;
                break;
            case EventType::CheckSuccess:
                last_submit_results[guy][task] = true;
                break;
        }
    }
    for (auto& [student, tasks_attempts] : last_submit_results) {
        for (auto& [task, result] : tasks_attempts) {
            if (result == true && opened_merge_requsts[student][task] == 0) {
                currently_accepted[student].insert(task);
            }
        }
    }
    std::erase_if(currently_accepted, [](const auto& student_results) { return student_results.second.empty(); });
    return currently_accepted;
}
