add_executable(pd0filedemonstrator "")


include_directories(${CMAKE_CURRENT_LIST_DIR})
# include external headers - this should really be an import from a shared code
# repository (needs CMakeLists.txt in those repos...)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../src/PDNMessages)

target_sources(pd0filedemonstrator
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/PDNMessages/PDN_messages.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UserPD0Block.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pd0filedemonstrator.cpp
    )

