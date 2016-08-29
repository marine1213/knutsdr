cmake_minimum_required(VERSION 2.8)
project( selfDrivingCar )

# Locate libraries and headers
find_package(wiringPi REQUIRED)

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS})
#include_directories("../source/")

# Include files
file(GLOB SOURCES
	../source/*.cpp
	../source/*.h
	../source/CmdInterface/*.cpp
	../source/CmdInterface/*.h
	../source/CommInterface/*.cpp
	../source/CommInterface/*.h
	../source/CalcInterface/*.cpp
	../source/CalcInterface/*.h
)
# Link against libraries to executable file
add_executable( selfDrivingCarExe ${SOURCES})
target_link_libraries(selfDrivingCarExe ${WIRINGPI_LIBRARIES})


#===================== create FindwiringPi.cmake =====================
######################################################################
############ put this in /usr/share/cmake-3.0/Modules ################
######################################################################
#find_library(WIRINGPI_LIBRARIES NAMES wiringPi PATHS /home/pi/wiringPi/wiringPi)
#find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h PATHS /home/pi/wiringPi/wiringPi/wiringPi.h)

#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)
#=====================================================================


