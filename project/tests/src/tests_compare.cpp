#include "tools.h"
#include <ctime>

TEST(CompareTime, Stress) {
    for (auto &entry : std::experimental::filesystem::directory_iterator(glob_test_dir / "compare")) {
        std::cout << entry.path() << std::endl;

        auto in = entry.path() / "in.txt";

        FILE *file = fopen(in.c_str(), "r");
        size_t size;
        fscanf(file, "%lu", &size);
        auto successive = create_array(size);
        read_array(successive, file);
        fclose(file);

        file = fopen(in.c_str(), "r");
        fscanf(file, "%lu", &size);
        auto parallel = create_array(size);
        read_array(parallel, file);
        fclose(file);

        ASSERT_EQ(comb_sort(successive), 0);
        ASSERT_EQ(merge_sort(parallel), 0);

        ASSERT_EQ(is_equal(successive, parallel), true);

        free_array(successive);
        free_array(parallel);
    }
}
