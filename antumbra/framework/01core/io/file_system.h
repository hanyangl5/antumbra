/* Copyright (c) 2019-2020, Arm Limited and Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "framework/01core/memory/container.h"
#include "framework/01core/utils/utils.h"

namespace ant::io {

/**
 * @brief Helper to tell if a given path is a directory
 * @param path A path to a directory
 * @return True if the path points to a valid directory, false if not
 */
bool is_directory(const std::filesystem::path &path);

/**
 * @brief Checks if a file exists
 * @param filename The filename to check
 * @return True if the path points to a valid file, false if not
 */
bool is_file(const std::filesystem::path &filename);

/**
 * @brief Platform specific implementation to create a directory
 * @param path A path to a directory
 */
void create_directory(const std::filesystem::path &path);

void remove(const std::filesystem::path &path);
/**
 * @brief Recursively creates a directory
 * @param root The root directory that the path is relative to
 * @param path A path in the format 'this/is/an/example/path/'
 */
void create_path(const std::filesystem::path &root, const std::filesystem::path &path);

// bool write_json(nlohmann::json &data, const std::filesystem::path &filename);

ant::str read_text_file(const std::filesystem::path filename);

ant::vector<u8> read_binary_file(const std::filesystem::path &filename, const uint32_t count = 0);

void write_text_file(const std::filesystem::path filename, void *data, u64 size);

void write_binary_file(const ant::vector<u8> &data, const std::filesystem::path &filename,
                       const uint32_t count); // TODO(hyl5): what count means
} // namespace ant::io