add_executable(hello)

target_sources(hello PRIVATE "main.cpp")

target_link_libraries(hello PRIVATE guinea)
