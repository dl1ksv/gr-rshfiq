# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX
include(GrMiscUtils)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND rshfiq_sources
    rshfiq_impl.cc
)

set(rshfiq_sources "${rshfiq_sources}" PARENT_SCOPE)
if(NOT rshfiq_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT rshfiq_sources)

add_library(gnuradio-rshfiq SHARED ${rshfiq_sources})
#target_link_libraries(gnuradio-rshfiq ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-rshfiq PROPERTIES DEFINE_SYMBOL "gnuradio_rshfiq_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-rshfiq PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-rshfiq
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)


list(APPEND test_rshfiq_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_rshfiq.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_rshfiq.cc
)


target_link_libraries(
  gnuradio-rshfiq
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${GNURADIO_BLOCKS_LIBRARIES}
  ${Boost_LIBRARIES}
  ${GNURADIO_AUDIO_LIBRARIES}

)
GR_LIBRARY_FOO(gnuradio-rshfiq)

