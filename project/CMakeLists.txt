find_library(PTHREAD_LIBRARY pthread)

include_directories(include)

set(SUCCESSIVE_SRC src/comb_sort.c)
set(PARALLEL_SRC src/merge_sort.c)

add_library(successive_sort STATIC ${SUCCESSIVE_SRC})
add_library(parallel_sort SHARED ${PARALLEL_SRC})

target_link_libraries(parallel_sort ${PTHREAD_LIBRARY})

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} 
    successive_sort
    parallel_sort
)

add_subdirectory(tests)
