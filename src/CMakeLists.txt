# # Build an executable from your source files
# add_executable(node node.cpp)

# Optionally, create a library for testing
add_library(Node_lib Node.cpp Node.h)

add_library(DataFrame_lib DataFrame.cpp DataFrame.h)

add_library(DecisionTree_lib DecisionTree.cpp DecisionTree.h)

add_library(RandomForest_lib RandomForest.cpp RandomForest.h)

add_library(GradientBoostedTrees_lib GradientBoostedTrees.cpp GradientBoostedTrees.h)