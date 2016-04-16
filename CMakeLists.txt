cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14 CACHE STRING "Must be at least 14")

project(LB/json
	VERSION 0.0.0
	LANGUAGES CXX
)
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_VERSION}")

add_library(json
	"src/json.cpp"
)
target_include_directories(json
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:include>
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

option(BUILD_UTIL "Whether to build the optional utilities component" ON)
if(BUILD_UTIL)
	add_library(util
		"src/util.cpp"
	)
	target_include_directories(json
		PUBLIC
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:include>
	)
	target_link_libraries(util
		PUBLIC
			json
	)

	install(
		TARGETS
			util
		EXPORT util
		ARCHIVE DESTINATION lib/${PROJECT_NAME}
	)
	install(
		EXPORT util
		DESTINATION cmake/${PROJECT_NAME}
		NAMESPACE LB::json::
	)
	install(
		FILES
			"src/util.hpp"
		DESTINATION include/${PROJECT_NAME}
	)
endif()

option(BUILD_TESTS "Whether to build the tests" ON)
if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
