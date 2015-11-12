cmake_minimum_required(VERSION 2.8.11)
project( cppPac )

# Put binaries in /bin/${CMAKE_BUILD_TYPE}
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE_UPPER} ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})

# Source for cppPac
file(GLOB CPPPAC_SOURCES "source/*.cpp")

# C++11
set(CMAKE_CXX_STANDARD 11)

# executable file for the ea
add_executable(cppPac ${CPPPAC_SOURCES})
# Private headers in /source, public in /include, external in /dependencies
include_directories(cppPac ${CMAKE_CURRENT_SOURCE_DIR}/source
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
						   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tclap-1.2.1/include)

# Strict warnings, and treat warnings as errors
#if(MSVC)
#    # CMake might already have a flag for warning level
#    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
#        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#    else()
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#    endif()
#    # Treat warnings as errors
#    set(CMAKE_CXXFLAGS "${CMAKE_CXX_FLAGS} /WX")
#elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wformat-security")
#endif()

# Tell Visual Studio that we want to use C++ exceptions
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

