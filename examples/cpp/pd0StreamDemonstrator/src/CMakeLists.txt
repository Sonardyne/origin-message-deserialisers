add_executable(pd0streamdemonstrator "")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/PDNMessages)

target_sources(pd0streamdemonstrator
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/PDNMessages/PDN_messages.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pd0streamdemonstrator.cpp
    )

