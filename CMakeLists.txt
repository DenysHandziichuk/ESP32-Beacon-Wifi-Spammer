cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(beacon_spammer)

# Add console components
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/components/esp_console)