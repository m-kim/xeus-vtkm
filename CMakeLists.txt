cmake_minimum_required(VERSION 3.6)
project(VTKmXeusRender LANGUAGES CXX)

add_library(vtkmxeusrender INTERFACE)
#target_sources(vtkmxeusrender INTERFACE JSONPNGConvert.h VTKmXeusConvertPng.h VTKmXeusRender.h)
target_include_directories(vtkmxeusrender INTERFACE 
   ${PROJECT_SOURCE_DIR}/include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
