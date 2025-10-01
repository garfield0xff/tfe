# FindSource.cmake
# Utility functions for finding and collecting source files

# tfe_find_glob
# Finds all source files matching a glob pattern and stores them in a variable
#
# Usage:
#   tfe_find_glob(<output_var> <pattern1> [pattern2 ...])
#
# Arguments:
#   output_var - The variable name to store the found files
#   pattern    - Glob pattern(s) to match files (e.g., "src/parser/*.cpp")
#
# Example:
#   tfe_find_glob(PARSER_SOURCES "src/parser/*.cpp" "src/parser/*.hpp")
#   add_executable(myapp ${PARSER_SOURCES})
#
function(tfe_find_glob OUTPUT_VAR)
    set(ALL_SOURCES "")

    foreach(PATTERN ${ARGN})
        file(GLOB MATCHED_FILES ${PATTERN})
        list(APPEND ALL_SOURCES ${MATCHED_FILES})
    endforeach()

    list(REMOVE_DUPLICATES ALL_SOURCES)

    set(${OUTPUT_VAR} ${ALL_SOURCES} PARENT_SCOPE)

    # Print found files for debugging
    if(ALL_SOURCES)
        list(LENGTH ALL_SOURCES FILE_COUNT)
        foreach(FILE ${ALL_SOURCES})
            message(STATUS "  - ${FILE}")
        endforeach()
    else()
        message(WARNING "No files found matching patterns for ${OUTPUT_VAR}")
    endif()
endfunction()

# tfe_find_glob_source_recursive
# Recursively finds all source files matching a glob pattern
#
# Usage:
#   tfe_find_glob_source_recursive(<output_var> <pattern1> [pattern2 ...])
#
# Example:
#   tfe_find_glob_source_recursive(ALL_SOURCES "src/**/*.cpp")
#
function(tfe_find_glob_source_recursive OUTPUT_VAR)
    set(ALL_SOURCES "")

    foreach(PATTERN ${ARGN})
        file(GLOB_RECURSE MATCHED_FILES ${PATTERN})
        list(APPEND ALL_SOURCES ${MATCHED_FILES})
    endforeach()

    list(REMOVE_DUPLICATES ALL_SOURCES)

    set(${OUTPUT_VAR} ${ALL_SOURCES} PARENT_SCOPE)

    if(ALL_SOURCES)
        list(LENGTH ALL_SOURCES FILE_COUNT)
        message(STATUS "Found ${FILE_COUNT} file(s) recursively for ${OUTPUT_VAR}:")
        foreach(FILE ${ALL_SOURCES})
            message(STATUS "  - ${FILE}")
        endforeach()
    else()
        message(WARNING "No files found matching patterns for ${OUTPUT_VAR}")
    endif()
endfunction()
