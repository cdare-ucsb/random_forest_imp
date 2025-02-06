add_executable(Node_tests Node_tests.cpp) # add this executable
add_executable(DataFrame_tests DataFrame_tests.cpp) # add this executable
add_executable(DecisionTree_tests DecisionTree_tests.cpp) # add this executable

# Link your library (or source files) and Google Test libraries
target_link_libraries(Node_tests PRIVATE
        Node_lib
        gtest_main  # Google Test main library; or gtest and define your own main
)

# Link your library (or source files) and Google Test libraries
target_link_libraries(DataFrame_tests PRIVATE
        DataFrame_lib
        gtest_main  # Google Test main library; or gtest and define your own main
)

# Link your library (or source files) and Google Test libraries
target_link_libraries(DecisionTree_tests PRIVATE
        DecisionTree_lib
        DataFrame_lib
        Node_lib
        gtest_main  # Google Test main library; or gtest and define your own main
)


# Register the tests with CTest
include(GoogleTest)
gtest_discover_tests(Node_tests)
gtest_discover_tests(DataFrame_tests)
gtest_discover_tests(DecisionTree_tests)