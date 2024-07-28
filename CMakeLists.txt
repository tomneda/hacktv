cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFunctions.cmake)

project(HackTv)

get_version_from_git()  # sets VERSION
print(VERSION)

set(EXTRA_CFLAGS "")
set(EXTRA_LDFLAGS "")
set(EXTRA_PKGS "")
set(CROSS_HOST "")

set(CMAKE_C_COMPILER ${CROSS_HOST}gcc)
set(PKGCONF pkg-config)
set(CMAKE_C_FLAGS "-g -Wall -pthread -O3 ${EXTRA_CFLAGS} -DVERSION=\\\"${VERSION}\\\"")
set(CMAKE_EXE_LINKER_FLAGS "-g -lm -pthread ${EXTRA_LDFLAGS}")

include(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED libavcodec libavformat libavdevice libswscale libswresample libavutil ${EXTRA_PKGS})

set(SOURCES
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

execute_process(COMMAND ${PKGCONF} --exists libhackrf RESULT_VARIABLE PKGCONF_RESULT)
if(${PKGCONF_RESULT} EQUAL 0)
  list(APPEND SOURCES src/rf_hackrf.c)
  list(APPEND PKGS_LIBRARIES libhackrf)
  add_definitions("-DHAVE_HACKRF")
endif()

execute_process(COMMAND ${PKGCONF} --exists SoapySDR RESULT_VARIABLE PKGCONF_RESULT)
if(${PKGCONF_RESULT} EQUAL 0)
  list(APPEND SOURCES src/rf_soapysdr.c)
  list(APPEND PKGS_LIBRARIES SoapySDR)
  add_definitions("-DHAVE_SOAPYSDR")
endif()

execute_process(COMMAND ${PKGCONF} --exists libosmo-fl2k RESULT_VARIABLE PKGCONF_RESULT)
if(${PKGCONF_RESULT} EQUAL 0)
  list(APPEND SOURCES src/rf_fl2k.c)
  list(APPEND PKGS_LIBRARIES libosmo-fl2k)
  add_definitions("-DHAVE_FL2K")
endif()

print(SOURCES)
print(PKGS)
print(PKGS_INCLUDE_DIRS)
print(PKGS_LIBRARIES)
print(EXTRA_PKGS)
print(VERSION)


add_executable(hacktv ${SOURCES})
target_include_directories(hacktv PRIVATE ${PKGS_INCLUDE_DIRS})
target_link_libraries(hacktv PRIVATE ${PKGS_LIBRARIES} m)

install(TARGETS hacktv RUNTIME DESTINATION bin)