#include "framework/01core/io/file_system.h"
#include "framework/01core/utils/utils.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("test io") {
    using namespace ant;
    std::filesystem::path folder("test_folder");

    ant::io::create_directory(folder);

    REQUIRE(ant::io::is_directory(folder) == true);
    REQUIRE(ant::io::is_file(folder) == false);
    ant::io::remove(folder);
    REQUIRE(ant::io::is_directory(folder) == false);

    char data[] = "test data";

    std::filesystem::path text_path("text_file.txt");

    ant::io::write_text_file(text_path, data, sizeof(data));
    REQUIRE(ant::io::is_file(text_path) == true);
    ant::str text = ant::io::read_text_file(text_path);
    REQUIRE(strcmp(text.c_str(), data) == 0);
    ant::io::remove(text_path);
    REQUIRE(ant::io::is_file(text_path) == false);

    const ant::vector<ant::u8> binary_data = {1, 2, 3, 4, 5};
    std::filesystem::path binary_path("text_file.txt");

    ant::io::write_binary_file(binary_data, binary_path, 0);
    REQUIRE(ant::io::is_file(binary_path) == true);
    auto _binary_data = ant::io::read_binary_file(binary_path);

    REQUIRE(_binary_data.size() == binary_data.size());
    for (u32 i = 0; i < binary_data.size(); i++) {
        REQUIRE(_binary_data[i] == binary_data[i]);
    }
    ant::io::remove(binary_path);
    REQUIRE(ant::io::is_file(binary_path) == false);

    // TODO(hyl5): json
}
