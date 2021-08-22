message("\n############################################################################")
message("## webfr-module-install.cmake\n")

message("WEBFR_THIS_MODULE_NAME=${WEBFR_THIS_MODULE_NAME}")
message("WEBFR_THIS_MODULE_VERSION=${WEBFR_THIS_MODULE_VERSION}")
message("WEBFR_THIS_MODULE_LIBRARIES=${WEBFR_THIS_MODULE_LIBRARIES}")
message("WEBFR_THIS_MODULE_TARGETS=${WEBFR_THIS_MODULE_TARGETS}")
message("WEBFR_THIS_MODULE_DIRECTORIES=${WEBFR_THIS_MODULE_DIRECTORIES}")

message("\n############################################################################\n")

include(GNUInstallDirs)

set(WEBFR_MODULE_NAME ${WEBFR_THIS_MODULE_NAME} CACHE STRING "webfr module name")
set(WEBFR_MODULE_VERSION "${WEBFR_THIS_MODULE_VERSION}" CACHE STRING "webfr module version")
set(WEBFR_MODULE_LIBRARIES
        "${WEBFR_THIS_MODULE_LIBRARIES}" 
        CACHE INTERNAL "webfr module libraries"
)
set(WEBFR_MODULE_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING "lib folder name")

foreach(CURR_DIR ${WEBFR_THIS_MODULE_DIRECTORIES})
    list(APPEND WEBFR_DIRS_TO_INSTALL ${CMAKE_CURRENT_LIST_DIR}/../src/${CURR_DIR})
endforeach()

install(TARGETS ${WEBFR_THIS_MODULE_TARGETS}
        EXPORT "${WEBFR_MODULE_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/webfr-${WEBFR_MODULE_VERSION}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/webfr-${WEBFR_MODULE_VERSION}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/webfr-${WEBFR_MODULE_VERSION}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/webfr-${WEBFR_MODULE_VERSION}/${WEBFR_MODULE_NAME}"
)

install(DIRECTORY ${WEBFR_DIRS_TO_INSTALL}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/webfr-${WEBFR_MODULE_VERSION}/${WEBFR_MODULE_NAME}"
        FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT "${WEBFR_MODULE_NAME}Targets"
        FILE "${WEBFR_MODULE_NAME}Targets.cmake"
        NAMESPACE webfr::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${WEBFR_MODULE_NAME}-${WEBFR_MODULE_VERSION}"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${WEBFR_MODULE_NAME}ConfigVersion.cmake"
        VERSION ${WEBFR_MODULE_VERSION}
        COMPATIBILITY ExactVersion 
)

configure_package_config_file(
            "${CMAKE_CURRENT_LIST_DIR}/module-config.cmake.in"
            "${WEBFR_MODULE_NAME}Config.cmake"
        INSTALL_DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/${WEBFR_MODULE_NAME}-${WEBFR_MODULE_VERSION}"
        PATH_VARS
            WEBFR_MODULE_NAME
            WEBFR_MODULE_VERSION
            WEBFR_MODULE_LIBRARIES
            WEBFR_MODULE_LIBDIR
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${WEBFR_MODULE_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${WEBFR_MODULE_NAME}ConfigVersion.cmake"
        DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/${WEBFR_MODULE_NAME}-${WEBFR_MODULE_VERSION}"
)