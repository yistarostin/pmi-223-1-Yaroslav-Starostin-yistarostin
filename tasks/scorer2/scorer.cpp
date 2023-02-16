#include "scorer.h"

#include <algorithm>
#include <unordered_map>

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    auto checks_it = student_succesfull_submissions_.find(student_name);
    if (checks_it != student_succesfull_submissions_.end() && checks_it->second.contains(task_name)) {
        checks_it->second.erase(task_name);
        if (checks_it->second.empty()) {
            student_succesfull_submissions_.erase(checks_it);
        }
    }
}

void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    auto checks_it = student_succesfull_submissions_.find(student_name);
    if (checks_it == student_succesfull_submissions_.end()) {
        student_succesfull_submissions_[student_name] = {task_name};
    } else {
        checks_it->second.insert(task_name);
    }
}

void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    ++opened_merge_requests_count_[student_name][task_name];
}

void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    // assert(opened_merge_requests_count_.at(student_name)[task_name] > 0); <-- Expected
    // Поскольку каждое обращения opened_merge_requests_count_[student_name].[...] весит большую константу, выполним его
    // 1 раз и далее будем использовать итератор на него
    auto merge_requests_it = opened_merge_requests_count_.find(student_name);
    if (merge_requests_it != opened_merge_requests_count_.end()) {
        auto task_it = merge_requests_it->second.find(task_name);
        --task_it->second;
        if (task_it->second == 0) {
            merge_requests_it->second.erase(task_it);
        }
    }
}

void Scorer::Reset() {
    student_succesfull_submissions_.clear();
    opened_merge_requests_count_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable scoretable;
    for (const auto &[student_name, tasks] : student_succesfull_submissions_) {
        auto current_student_merges_it = opened_merge_requests_count_.find(student_name);
        const bool has_no_opened_merges = current_student_merges_it == opened_merge_requests_count_.end();
        for (const TaskName &task_name : tasks) {
            if (has_no_opened_merges || !current_student_merges_it->second.contains(task_name)) {
                scoretable[student_name].insert(task_name);
            }
        }
    }
    return scoretable;
}
