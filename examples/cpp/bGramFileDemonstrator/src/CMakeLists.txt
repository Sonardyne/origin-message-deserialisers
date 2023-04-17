add_executable(bgramfiledemonstrator "")


include_directories(${CMAKE_CURRENT_LIST_DIR})
# include external headers - this should really be an import from a shared code
# repository (needs CMakeLists.txt in those repos...)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/DSPGram)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/CMakeLists.txt)

target_sources(bgramfiledemonstrator
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/DSPGram/adcp_coredata_messages.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bgramfiledemonstrator.cpp
    )

