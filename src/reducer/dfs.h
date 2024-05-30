#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "model/terms.h"
#include "reducer/term_info.h"

namespace reducer::algorithm {

TermInfo DFSFindRedex(std::shared_ptr<model::term::Term> root, bool weak = false);
TermInfo DFSFindLeftmostInerrmostRedex(std::shared_ptr<model::term::Term> root, bool weak);
size_t RecursiveDFS(TermInfo&& parent, std::unordered_set<TermInfo>& visited,
                    std::unordered_map<size_t, std::vector<TermInfo>>& levels);
size_t WeakRecursiveDFS(TermInfo&& parent, std::unordered_set<TermInfo>& visited,
                        std::unordered_map<size_t, std::vector<TermInfo>>& levels,
                        bool skip = false);

}  // namespace reducer::algorithm
