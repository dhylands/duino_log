add_library(duino_log STATIC
    src/DumpMem.cpp
    src/Log.cpp
    src/PicoColorLog.cpp
    src/Str.cpp
    src/StrPrintf.cpp
)

target_include_directories(duino_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(duino_log pico_stdlib)
