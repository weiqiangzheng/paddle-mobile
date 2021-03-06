/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "framework/scope.h"

#include <set>
#include <string>
#include <vector>

namespace paddle_mobile {
namespace framework {

Scope &Scope::NewScope() const {
  kids_.push_back(new Scope(this));
  return *kids_.back();
}

Variable *Scope::Var(const std::string &name) {
  auto *pvar = FindVarLocally(name);
  if (pvar != nullptr) {
    return pvar;
  }
  pvar = new Variable;
  vars_[name] = pvar;
  pvar->name_ = vars_.find(name)->first;
  return pvar;
}

Variable *Scope::FindVar(const std::string &name) const {
  auto *pvar = FindVarLocally(name);
  if (pvar != nullptr) {
    return pvar;
  }
  return (parent_ == nullptr) ? nullptr : parent_->FindVar(name);
}

const Scope *Scope::FindScope(const Variable *var) const {
  for (auto &name_var : vars_) {
    if (name_var.second == var) {
      return this;
    }
  }
  return (parent_ == nullptr) ? nullptr : parent_->FindScope(var);
}

void Scope::DropKids() {
  for (Scope *s : kids_) {
    delete s;
  }
  kids_.clear();
}

std::vector<std::string> Scope::LocalVarNames() const {
  std::vector<std::string> known_vars;
  known_vars.reserve(vars_.size());
  for (auto &name_var : vars_) {
    known_vars.emplace_back(name_var.first);
  }
  return known_vars;
}

void Scope::DeleteScope(Scope *scope) const {
  auto it = std::find(kids_.begin(), kids_.end(), scope);
  kids_.erase(it);
  delete scope;
}

void Scope::EraseVars(const std::vector<std::string> &var_names) {
  std::set<std::string> var_set(var_names.begin(), var_names.end());
  for (auto it = vars_.begin(); it != vars_.end();) {
    if (var_set.find(it->first) != var_set.end()) {
      delete it->second;
      it = vars_.erase(it);
    } else {
      ++it;
    }
  }
}

void Scope::Rename(const std::string &origin_name,
                   const std::string &new_name) const {
  auto origin_it = vars_.find(origin_name);
  if (origin_it == vars_.end()) {
    return;
  }
  auto new_it = vars_.find(new_name);
  if (new_it != vars_.end()) {
    return;
  }
  vars_[new_name] = origin_it->second;
  vars_.erase(origin_it);
}

Variable *Scope::FindVarLocally(const std::string &name) const {
  auto it = vars_.find(name);
  if (it != vars_.end()) {
    return it->second;
  }
  return nullptr;
}

}  // namespace framework
}  // namespace paddle_mobile
