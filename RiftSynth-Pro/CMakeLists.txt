cmake_minimum_required(VERSION 3.22)
project(RiftSynthPro VERSION 0.10.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED JUCE_DIR)
  message(WARNING "JUCE_DIR is not set. Configure with -DJUCE_DIR=/path/to/JUCE")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(RiftWaveCommon OPTIONAL)

add_subdirectory(Plugins/RiftSynthPro)
