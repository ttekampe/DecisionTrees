include_directories(${DecisionTree_SOURCE_DIR}/include)

add_executable(test_dataset test_dataset.cc)
target_link_libraries(test_dataset dataset ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME test_dataset COMMAND test_dataset)
set_tests_properties(test_dataset PROPERTIES 
                     ENVIRONMENT
                     TEST_DATA_DIR=${DecisionTree_SOURCE_DIR}/..)

add_executable(test_log_loss test_log_loss.cc)
target_link_libraries(test_log_loss loss ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME test_log_loss COMMAND test_log_loss)

add_executable(test_decisiontree test_decisiontree.cc)
target_link_libraries(test_decisiontree decisiontree loss ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME test_decisiontree COMMAND test_decisiontree)
set_tests_properties(test_decisiontree PROPERTIES 
                     ENVIRONMENT
                     TEST_DATA_DIR=${DecisionTree_SOURCE_DIR}/..)