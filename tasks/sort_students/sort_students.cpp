#include "sort_students.h"

#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>

bool CompareByName(const Student& a, const Student& b) {
    return std::tie(a.last_name, a.name, a.birth_date.year, a.birth_date.month, a.birth_date.day) <
           std::tie(b.last_name, b.name, b.birth_date.year, b.birth_date.month, b.birth_date.day);
}

bool CompareByDate(const Student& a, const Student& b) {
    return std::tie(a.birth_date.year, a.birth_date.month, a.birth_date.day, a.last_name, a.name) <
           std::tie(b.birth_date.year, b.birth_date.month, b.birth_date.day, b.last_name, b.name);
}

static const auto COMPARERS = std::unordered_map<SortKind, std::function<bool(Student const&, Student const&)>>{
    {SortKind::Date, CompareByDate}, {SortKind::Name, CompareByName}};

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    std::sort(students.begin(), students.end(), COMPARERS.at(sortKind));
}
