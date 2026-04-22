cmake_minimum_required(VERSION 3.16)

project(VisiCalc
    VERSION   1.0.0
    LANGUAGES CXX
    DESCRIPTION "Terminal-based spreadsheet application with formula parsing"
)

# ── C++ Standard ────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Sources ─────────────────────────────────────────────────────────────────────
set(SOURCES
    main.cpp
    Cell.cpp
    Spreadsheet.cpp
    FormulaParser.cpp
    FileManager.cpp
    AnsiTerminal.cpp
)

set(HEADERS
    Cell.h
    Spreadsheet.h
    FormulaParser.h
    FileManager.h
    AnsiTerminal.h
)

# ── Executable ──────────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    $<$<CONFIG:Debug>:-g -O0 -fsanitize=address,undefined>
    $<$<CONFIG:Release>:-O2>
)

target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# ── Install ─────────────────────────────────────────────────────────────────────
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# ── Summary ─────────────────────────────────────────────────────────────────────
message(STATUS "")
message(STATUS "  VisiCalc v${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:   ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "")
