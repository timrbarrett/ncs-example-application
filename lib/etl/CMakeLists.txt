# lib\etl level CMakeLists.txt

if (CONFIG_ETL)
  if (CONFIG_ETL_LIB)
    set(ETL_PROFILE ${CONFIG_ETL_PROFILE} CACHE STRING "ETL_PROFILE" FORCE)
  else()
    message(FATAL_ERROR "ETL_PROFILE does not set in Kconfig")
  endif()

  message(STATUS "Compiling with ETL profile set to: ${ETL_PROFILE}")

  option(BUILD_TESTS "Build unit tests" OFF)

  if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
  endif()

  zephyr_interface_library_named(etl)

  target_compile_definitions(etl
    INTERFACE
      ${ETL_PROFILE}
  )

  target_include_directories(etl
    INTERFACE
      include
      include/etl/profiles
      ${CMAKE_CURRENT_SOURCE_DIR}/../etl_setup  # <-- Add this line
  )

endif()

message(STATUS "ETL include directories: ${CMAKE_CURRENT_SOURCE_DIR}/include")

# EOF lib\etl level CMakeLists.txt