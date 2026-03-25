add_executable(starling_validator_test 
    unit/test_starling_validator.cpp
    ../src/starling_validator.cpp
)
target_link_libraries(starling_validator_test pthread)
target_include_directories(starling_validator_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME StarlingValidatorTest COMMAND starling_validator_test)