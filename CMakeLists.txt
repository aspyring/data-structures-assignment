cmake_minimum_required(VERSION 3.10)
project(data_structures_assignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(ds_app
        main.cpp

        # array version
        array_version/array_impl.cpp
        array_version/array_data_structure.hpp

        # linked-list version
        linkedlist_version/list_impl.cpp
        linkedlist_version/linked_list.hpp

        # common code
        common/clean_data.cpp
        common/clean_data.hpp
        common/load_data.cpp
        common/load_data.hpp
)

target_include_directories(ds_app PRIVATE
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}/array_version
        ${CMAKE_SOURCE_DIR}/linkedlist_version
)
