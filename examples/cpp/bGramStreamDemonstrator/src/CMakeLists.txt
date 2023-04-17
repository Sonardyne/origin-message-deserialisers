add_executable(bgramstreamdemonstrator "")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/DSPGram)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/CMakeLists.txt)

target_sources(bgramstreamdemonstrator
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/DSPGram/adcp_coredata_messages.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bgramstreamdemonstrator.cpp
    )

