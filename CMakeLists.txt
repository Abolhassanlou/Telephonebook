cmake_minimum_required(VERSION 3.18)

project(TelephoneBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find wxWidgets with core and base components
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

# Include CTest for testing support
include(CTest)

# FetchContent for Catch2 unit testing framework
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# Source files for main application
set(APP_SRCS
    main.cpp
    TelephoneBook.cpp
    TelephoneBookLogic.cpp
    Contact.cpp
)

# Source files for tests
set(TEST_SRCS
    test.cpp
    TelephoneBookLogic.cpp
    Contact.cpp
)

# Main executable
add_executable(${PROJECT_NAME} ${APP_SRCS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wconversion)
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} sqlite3)

# Test executable
add_executable(runTests ${TEST_SRCS})
target_compile_features(runTests PRIVATE cxx_std_20)
target_compile_options(runTests PRIVATE -Wall -Wextra -Wconversion)
# Link wxWidgets here as well because test sources use wxWidgets code
target_link_libraries(runTests PRIVATE sqlite3 Catch2::Catch2WithMain ${wxWidgets_LIBRARIES})

# Enable CTest testing integration
include(Catch)
catch_discover_tests(runTests)
