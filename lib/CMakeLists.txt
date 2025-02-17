# SPDX-License-Identifier: Apache-2.0
# /lib level CMakeList.txt

add_subdirectory_ifdef(CONFIG_CUSTOM custom)
add_subdirectory_ifdef(CONFIG_ETL etl)
add_subdirectory(etl_type)

# EOF /lib level CMakeList.txt