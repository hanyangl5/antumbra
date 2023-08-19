#include "framework/01core/io/file_system.h"
#include "framework/01core/utils/utils.h"

//#include <catch2/catch_test_macros.hpp>
#include <doctest.h>

#include <iostream>

TEST_CASE("test io") {
    using namespace ante;
    std::filesystem::path folder("test_folder");

    ante::io::create_directory(folder);

    REQUIRE(ante::io::is_directory(folder) == true);
    REQUIRE(ante::io::is_file(folder) == false);
    ante::io::remove(folder);
    REQUIRE(ante::io::is_directory(folder) == false);

    char data[] = "test data";

    std::filesystem::path text_path("text_file.txt");

    ante::io::write_text_file(text_path, data, sizeof(data));
    REQUIRE(ante::io::is_file(text_path) == true);
    ante::str text = ante::io::read_text_file(text_path);
    REQUIRE(strcmp(text.c_str(), data) == 0);
    ante::io::remove(text_path);
    REQUIRE(ante::io::is_file(text_path) == false);

    const ante::vector<ante::u8> binary_data = {1, 2, 3, 4, 5};
    std::filesystem::path binary_path("text_file.txt");

    ante::io::write_binary_file(binary_data, binary_path, 0);
    REQUIRE(ante::io::is_file(binary_path) == true);
    auto _binary_data = ante::io::read_binary_file(binary_path);

    REQUIRE(_binary_data.size() == binary_data.size());
    for (u32 i = 0; i < binary_data.size(); i++) {
        REQUIRE(_binary_data[i] == binary_data[i]);
    }
    ante::io::remove(binary_path);
    REQUIRE(ante::io::is_file(binary_path) == false);

    // TODO(hyl5): json
}
