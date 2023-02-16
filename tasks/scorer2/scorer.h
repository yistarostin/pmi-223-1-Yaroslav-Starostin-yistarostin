#pragma once

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
    std::unordered_map<StudentName, std::unordered_set<TaskName>> student_succesfull_submissions_;
    std::unordered_map<StudentName, std::unordered_map<TaskName, size_t>> opened_merge_requests_count_;

public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();

    ScoreTable GetScoreTable() const;
};
