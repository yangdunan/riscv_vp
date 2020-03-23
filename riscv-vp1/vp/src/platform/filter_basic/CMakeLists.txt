file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(filter_basic
        main.cpp
        ethernet.cpp
        display.cpp
        SobelFilter.cpp
        ${HEADERS})

target_link_libraries(filter_basic rv32 platform-common ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS filter_basic RUNTIME DESTINATION bin)