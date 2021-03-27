#pragma once

#include <gtest/gtest.h>

//  #include <filesystem>

#include <experimental/filesystem>
#include <fstream>

extern std::experimental::filesystem::path glob_test_dir;

extern "C" {
    #include "comb_sort.h"
    #include "merge_sort.h"
}
