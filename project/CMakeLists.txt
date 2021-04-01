find_library(PTHREAD_LIBRARY pthread)

include_directories(include)

set(SUCCESSIVE_SRC src/array.c src/comb_sort.c)
set(PARALLEL_SRC src/array.c src/merge_sort.c)

add_library(successive_sort STATIC ${SUCCESSIVE_SRC})
add_library(parallel_sort SHARED ${PARALLEL_SRC})

target_link_libraries(parallel_sort ${PTHREAD_LIBRARY})

add_executable(successive_program src/main.c)
target_link_libraries(successive_program successive_sort)

add_executable(parallel_program src/main.c)
target_link_libraries(parallel_program parallel_sort)

add_subdirectory(tests)
