cmake_minimum_required(VERSION 3.5)

project(eventhandlers-Test2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(eventhandlers-Test2 main.cpp
    event_dispatcher.h event_dispatcher.cpp
    custom_event_handlers.h)

include(GNUInstallDirs)
install(TARGETS eventhandlers-Test2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
