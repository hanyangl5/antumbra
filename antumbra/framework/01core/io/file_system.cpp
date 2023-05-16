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

#include "file_system.h"

#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb_image_write.h>

#include <framework/01core/logging/log.h>

namespace ant::io {

bool is_directory(const std::filesystem::path &path) { return std::filesystem::is_directory(path); }

void create_directory(const std::filesystem::path &path) {
    if (io::is_directory(path) == false) {
        std::filesystem::create_directory(path);
    }
}

void remove(const std::filesystem::path &path) { std::filesystem::remove(path); }

bool is_file(const std::filesystem::path &filename) { return std::filesystem::is_regular_file(filename); }

void create_path(const std::filesystem::path &root, const std::filesystem::path &path) {
    auto str = path.string();
    for (auto it = str.begin(); it != str.end(); ++it) {
        it = std::find(it, path.string().end(), '/');
        auto dir_path = std::string(str.begin(), it);
        std::filesystem::path out_path = root / dir_path;
        io::create_directory(out_path);
    }
}

ant::str read_text_file(const std::filesystem::path filename) {
    ant::vector<ant::str> data;

    std::ifstream file;

    file.open(filename, std::ios::in);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open file : {}", filename.string());
        return {};
    }

    return ant::str{(std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())};
}

ant::vector<u8> read_binary_file(const std::filesystem::path &filename, const uint32_t count) {
    ant::vector<u8> data;

    std::ifstream file;

    file.open(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open file : {}", filename.string());
        return {};
    }

    uint64_t read_count = count;
    if (count == 0) {
        file.seekg(0, std::ios::end);
        read_count = static_cast<uint64_t>(file.tellg());
        file.seekg(0, std::ios::beg);
    }

    data.resize(static_cast<size_t>(read_count));
    file.read(reinterpret_cast<char *>(data.data()), read_count);
    file.close();

    return data;
}

void write_text_file(const std::filesystem::path filename, void *data, u64 size) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open file : {}", filename.string());
        return;
    }

    file.write(reinterpret_cast<const char *>(data), size);
    file.close();
}

void write_binary_file(const ant::vector<u8> &data, const std::filesystem::path &filename, const uint32_t count) {
    std::ofstream file;

    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open file : {}", filename.string());
        return;
    }

    uint64_t write_count = count;
    if (count == 0) {
        write_count = data.size();
    }

    file.write(reinterpret_cast<const char *>(data.data()), write_count);
    file.close();
}

bool write_json(nlohmann::json &data, const std::filesystem::path &filename) {
    std::stringstream json;
    json << data << " ";

    if (!nlohmann::json::accept(json.str())) {
        LOG_ERROR("Invalid JSON string");
        return false;
    }

    std::ofstream out_stream;
    out_stream.open(filename, std::ios::out | std::ios::trunc);

    if (out_stream.good()) {
        out_stream << json.str();
    } else {
        LOG_ERROR("Could not load JSON file {}", filename.string());
        return false;
    }

    out_stream.close();
    return true;
}

} // namespace ant::io