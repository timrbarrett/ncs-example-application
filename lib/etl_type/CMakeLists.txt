#add_library(etl_type etl_type_setup.cpp base_store.cpp)

# Include directories for etl_type
#target_include_directories(etl_type PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add source files for ulisp
#target_sources(etl_type PRIVATE 
#    ${CMAKE_CURRENT_SOURCE_DIR}/base_store.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/etl_type_setup.cpp
#    )

#target_link_libraries(etl_type PRIVATE etl_atomic etl_private etl_profiles etl ulisp zephyr)


#copied directly from lib/custom/CMakeLists.txt
# lib/etl_type CMakeLists.txt

zephyr_library()
zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
zephyr_library_sources(base_store.cpp etl_type_setup.cpp ulisp.c)

# EOF lib/etl_type CMakeLists.txt
##zephyr_include_directories($CMAKE_CURRENT_BINARY_DIR)
#zephyr_interface_library_named(etl_type)
# I think I still need to link to etl..
# perhaps not because it is all .h files