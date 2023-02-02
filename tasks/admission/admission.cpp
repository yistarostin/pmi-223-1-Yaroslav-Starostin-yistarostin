#include "admission.h"

#include <algorithm>
#include <functional>
#include <ranges>
#include <tuple>

bool operator<(const Date& a, const Date& b) {
    return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

bool operator<(const Student& a, const Student& b) {
    return std::tie(a.birth_date, a.name) < std::tie(b.birth_date, b.name);
}

bool operator<(const Applicant& a, const Applicant& b) {
    return std::tie(b.points, a.student) < std::tie(a.points, b.student);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> sorted_applicants;
    sorted_applicants.reserve(applicants.size());
    for (auto& applicant : applicants) {
        sorted_applicants.push_back(&applicant);
    }
    std::sort(sorted_applicants.begin(), sorted_applicants.end(),
              [](const Applicant*& a, const Applicant*& b) { return *a < *b; });
    std::unordered_map<std::string_view, size_t> left_space;
    left_space.reserve(universities.size());
    for (const auto& uni : universities) {
        left_space[uni.name] = uni.max_students;
    }

    AdmissionTable admissions;
    for (const auto& abi : sorted_applicants) {
        for (const auto& uni : abi->wish_list) {
            if (left_space[uni] > 0) {
                --left_space[uni];
                admissions[uni].push_back(&(abi->student));
                break;
            }
        }
    }

    for (auto& [_, uni_abiturs] : admissions) {
        std::sort(uni_abiturs.begin(), uni_abiturs.end(),
                  [](const Student*& a, const Student*& b) { return a->name < b->name; });
    }
    return admissions;
}
