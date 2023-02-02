#include "tests_checking.h"

#include <algorithm>
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string_view> papers_order;
    std::for_each(student_actions.begin(), student_actions.end(), [&](const StudentAction& paper) {
        if (paper.side == Side::Bottom) {
            papers_order.push_back(paper.name);
        } else {
            papers_order.push_front(paper.name);
        }
    });
    std::vector<std::string> queried_positions_owners;
    queried_positions_owners.reserve(queries.size());
    std::for_each(queries.begin(), queries.end(),
                  [&](size_t position) { queried_positions_owners.emplace_back(papers_order[position - 1]); });
    return queried_positions_owners;
}
