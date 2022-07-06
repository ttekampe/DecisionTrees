set(HEADER_LIST "${DecisionTree_SOURCE_DIR}/include/data.h")

include_directories(${DecisionTree_SOURCE_DIR}/include)

add_library(dataset dataset.cc)

add_library(loss loss.cc)

add_library(decisiontree decisiontree.cc)
target_link_libraries(decisiontree dataset loss)

source_group(
  TREE "${DecisionTree_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
