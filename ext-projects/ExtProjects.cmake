include(ExternalProject)

if(${SELECTED_FEATURES} STREQUAL "ALL")
    include(ext-projects/cmake-files/DIPaCUS.cmake)
    include(ext-projects/cmake-files/GEOC.cmake)
    include(ext-projects/cmake-files/SCaBOliC.cmake)
elseif(${SELECTED_FEATURES} STREQUAL "SEED_SELECTION")
    include(ext-projects/cmake-files/DIPaCUS.cmake)
    include(ext-projects/cmake-files/GEOC.cmake)
endif()
