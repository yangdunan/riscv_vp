file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
#file(GLOB src *.cpp *.h *.hpp)
#add_library(basic STATIC ${src})

add_executable(riscv-vp
        main.cpp
        ethernet.cpp
        display.cpp
        ${HEADERS}
        )
target_link_libraries(riscv-vp rv32 platform-common ${Boost_LIBRARIES} ${SystemC_LIBRARIES} ${SCML_LIB64_DIR} ${SCML_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
