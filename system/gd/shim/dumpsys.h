/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <future>
#include <memory>
#include <string>

#include "dumpsys/reflection_schema.h"
#include "module.h"

namespace bluetooth {
namespace shim {

constexpr char kArgumentDeveloper[] = "--dev";

class Dumpsys : public bluetooth::Module {
 public:
  void Dump(int fd, const char** args);
  void Dump(int fd, const char** args, std::promise<void> promise);

  // Convenience thread used by shim layer for task execution
  os::Handler* GetGdShimHandler();

  Dumpsys(const std::string& pre_bundled_schema);
  ~Dumpsys() = default;

  static const ModuleFactory Factory;

 protected:
  void ListDependencies(ModuleList* list) const override;  // Module
  void Start() override;                             // Module
  void Stop() override;                              // Module
  std::string ToString() const override;             // Module
  DumpsysDataFinisher GetDumpsysData(flatbuffers::FlatBufferBuilder* builder) const override;  // Module

 private:
  struct impl;
  std::unique_ptr<impl> pimpl_;
  const dumpsys::ReflectionSchema reflection_schema_;
  DISALLOW_COPY_AND_ASSIGN(Dumpsys);
};

}  // namespace shim
}  // namespace bluetooth
