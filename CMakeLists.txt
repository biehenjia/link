cmake_minimum_required(VERSION 3.21)  # PROJECT_IS_TOP_LEVEL
project(cr_core VERSION 0.1.0 LANGUAGES CXX)

find_package(SymEngine CONFIG REQUIRED)

find_path(MPFR_INCLUDE_DIR mpfr.h)
find_library(MPFR_LIBRARY mpfr)
find_library(GMP_LIBRARY gmp)

add_library(cr_core INTERFACE)
add_library(cr_core::cr_core ALIAS cr_core)
target_compile_features(cr_core INTERFACE cxx_std_20)
target_include_directories(cr_core INTERFACE include)
target_include_directories(cr_core INTERFACE ${SYMENGINE_INCLUDE_DIRS})
target_include_directories(cr_core INTERFACE ${MPFR_INCLUDE_DIR})
target_link_libraries(cr_core INTERFACE ${SYMENGINE_LIBRARIES})
target_link_libraries(cr_core INTERFACE ${MPFR_LIBRARY} ${GMP_LIBRARY})

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)  # defines BUILD_TESTING (ON by default)
    if(BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
