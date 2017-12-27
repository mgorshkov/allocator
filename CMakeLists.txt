cmake_minimum_required(VERSION 3.2)
project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_executable(allocator allocator.cpp slist.cpp)
add_executable(allocator_test allocator_test.cpp slist.cpp)
add_library(allocatorlib allocatorlib.cpp)

set_target_properties(allocator allocator_test PROPERTIES
#	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
	CMAKE_CXX_STANDARD 17
	CMAKE_CXX_STANDARD_REQUIRED ON
)

target_link_libraries(allocator
	allocatorlib
)

set_target_properties(allocator_test PROPERTIES
	COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
	INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(allocator_test
	${Boost_LIBRARIES}
	allocatorlib
)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mikhail.gorshkov@gmail.com)

include(CPack)

enable_testing()

add_test(allocator_test, allocator_test)
