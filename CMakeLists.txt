cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14 CACHE STRING "Must be at least 14")

project(LB/json
	VERSION 0.0.0
	LANGUAGES CXX
)
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_VERSION}")

find_package(Boost REQUIRED
	COMPONENTS
		locale
		system
)

add_library(json
	"src/json.cpp"
)
target_include_directories(json
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(json
	PUBLIC
		Boost::locale
		Boost::system
)

install(
	TARGETS
		json
	EXPORT json
	ARCHIVE DESTINATION lib/${PROJECT_NAME}
)
install(
	EXPORT json
	DESTINATION cmake/LB
	NAMESPACE LB::
)
install(
	FILES
		"src/json.hpp"
	DESTINATION include/${PROJECT_NAME}
)

option(BUILD_TESTS "Whether to build the tests" ON)
if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
