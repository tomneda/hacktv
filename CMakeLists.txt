# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project's name
project(HackTv C)

# Set the C Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pthread -O3")

# Inform CMake where the source files are
set(SOURCE_FILES
        src/hacktv.c
        src/common.c
        src/fir.c
        src/vbidata.c
        src/teletext.c
        src/wss.c
        src/video.c
        src/mac.c
        src/dance.c
        src/eurocrypt.c
        src/videocrypt.c
        src/videocrypts.c
        src/syster.c
        src/acp.c
        src/vits.c
        src/vitc.c
        src/nicam728.c
        src/sis.c
        src/av.c
        src/av_test.c
        src/av_ffmpeg.c
        src/rf.c
        src/rf_file.c
)

# Conditionally add source files
# PkgConfig would be useful here, but to keep this simple you may manually uncomment based on your needs 

#list(APPEND SOURCE_FILES rf_hackrf.c)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_HACKRF")

#list(APPEND SOURCE_FILES rf_soapysdr.c)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SOAPYSDR")

#list(APPEND SOURCE_FILES rf_fl2k.c)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_FL2K")

# Add the binary tree directory to the search path for include files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Tell CMake to create the executable
add_executable(HackTv ${SOURCE_FILES})

# Link the required libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(AV REQUIRED libavcodec libavformat libavdevice libswscale libswresample libavutil)
target_link_libraries(HackTv ${AV_LIBRARIES})
target_include_directories(HackTv PUBLIC ${AV_INCLUDE_DIRS})
target_compile_options(HackTv PUBLIC ${AV_CFLAGS_OTHER})

#Extra libraries if they exist. Uncomment if you need.
#pkg_check_modules(HACKRF REQUIRED libhackrf)
#target_link_libraries(HackTv ${HACKRF_LIBRARIES})
#target_include_directories(HackTv PUBLIC ${HACKRF_INCLUDE_DIRS})
#target_compile_options(HackTv PUBLIC ${HACKRF_CFLAGS_OTHER})

#pkg_check_modules(SOAPYSDR REQUIRED SoapySDR)
#target_link_libraries(HackTv ${SOAPYSDR_LIBRARIES})
#target_include_directories(HackTv PUBLIC ${SOAPYSDR_INCLUDE_DIRS})
#target_compile_options(HackTv PUBLIC ${SOAPYSDR_CFLAGS_OTHER})

#pkg_check_modules(FL2K REQUIRED libosmo-fl2k)
#target_link_libraries(HackTv ${FL2K_LIBRARIES})
#target_include_directories(HackTv PUBLIC ${FL2K_INCLUDE_DIRS})
#target_compile_options(HackTv PUBLIC ${FL2K_CFLAGS_OTHER})