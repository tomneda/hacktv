cmake_minimum_required(VERSION 3.10)
project(HackTv)

find_package(Git REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty='%cd' --date=format:'%Y%m%d' --dirty --always OUTPUT_VARIABLE VERSION)

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

if(${PKGCONF} --exists libhackrf)
  list(APPEND SOURCES rf_hackrf.c)
  list(APPEND PKGS libhackrf)
  add_definitions("-DHAVE_HACKRF")
endif()

if(${PKGCONF} --exists SoapySDR)
  list(APPEND SOURCES rf_soapysdr.c)
  list(APPEND PKGS SoapySDR)
  add_definitions("-DHAVE_SOAPYSDR")
endif()

if(${PKGCONF} --exists libosmo-fl2k)
  list(APPEND SOURCES rf_fl2k.c)
  list(APPEND PKGS libosmo-fl2k)
  add_definitions("-DHAVE_FL2K")
endif()

add_executable(hacktv ${SOURCES})
target_include_directories(hacktv PRIVATE ${PKGS_INCLUDE_DIRS})
target_link_libraries(hacktv PRIVATE ${PKGS_LIBRARIES})

install(TARGETS hacktv RUNTIME DESTINATION bin)