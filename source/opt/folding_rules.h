// Copyright (c) 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIBSPIRV_UTIL_FOLDING_RULES_H_
#define LIBSPIRV_UTIL_FOLDING_RULES_H_

#include <cstdint>
#include <vector>

#include "constants.h"
#include "def_use_manager.h"
#include "ir_builder.h"
#include "ir_context.h"

namespace spvtools {
namespace opt {

// Folding Rules:
//
// The folding mechanism is built around the concept of a |FoldingRule|.  A
// folding rule is a function that implements a method of simplifying an
// instruction.
//
// The inputs to a folding rule are:
//     |inst| - the instruction to be simplified.
//     |constants| - if an in-operands is an id of a constant, then the
//                   corresponding value in |constants| contains that
//                   constant value.  Otherwise, the corresponding entry in
//                   |constants| is |nullptr|.
//
// A folding rule returns true if |inst| can be simplified using this rule.  If
// the instruction can be simplified, then |inst| is changed to the simplified
// instruction.  Otherwise, |inst| remains the same.
//
// See folding_rules.cpp for examples on how to write a folding rule.  It is
// important to note that if |inst| can be folded to the result of an
// instruction that feed it, then |inst| should be changed to an OpCopyObject
// that copies that id.
//
// Be sure to add new folding rules to the table of folding rules in the
// constructor for FoldingRules.  The new rule should be added to the list for
// every opcode that it applies to.  Note that earlier rules in the list are
// given priority.  That is, if an earlier rule is able to fold an instruction,
// the later rules will not be attempted.

using FoldingRule = std::function<bool(
    ir::Instruction* inst,
    const std::vector<const analysis::Constant*>& constants)>;

class FoldingRules {
 public:
  FoldingRules();

  const std::vector<FoldingRule>& GetRulesForOpcode(SpvOp opcode) {
    auto it = rules.find(opcode);
    if (it != rules.end()) {
      return it->second;
    }
    return empty_vector_;
  }

 private:
  std::unordered_map<uint32_t, std::vector<FoldingRule>> rules;
  std::vector<FoldingRule> empty_vector_;
};

}  // namespace opt
}  // namespace spvtools

#endif  // LIBSPIRV_UTIL_FOLDING_RULES_H_
