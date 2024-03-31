function(make_darwin_package)
    message("Creating Darwin package...")
#    # Create bundle or executable
#    if (PACKAGE)
#        add_executable(${PRJ_TARGET} MACOSX_BUNDLE ${SOURCE_LIST})
#
#        set_target_properties(${PRJ_TARGET} PROPERTIES
#                BUNDLE True
#                MACOSX_BUNDLE_LOCATION "MacOS"
#                MACOSX_BUNDLE_BUNDLE_NAME ${PRJ_TARGET}
#                MACOSX_BUNDLE_BUNDLE_VERSION "${PRJ_VERSION}"
#                MACOSX_BUNDLE_SHORT_VERSION_STRING "${PRJ_VERSION}"
#                )
#    else ()
#        add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#    endif ()
endfunction(make_darwin_package)

function(make_win_package)
    message("Creating Win package...")
#    add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#
#    # Create package with CPack
#    if (PACKAGE)
#        # Tell CPack to generate a .exe package
#        set(CPACK_GENERATOR "NSIS")
#
#        # Binaries
#        install(TARGETS ${PRJ_TARGET}
#                DESTINATION bin)
#
#        # Create directory for the logs
#        install(DIRECTORY DESTINATION log DIRECTORY_PERMISSIONS
#                OWNER_WRITE OWNER_READ OWNER_EXECUTE
#                GROUP_WRITE GROUP_READ GROUP_EXECUTE
#                WORLD_WRITE WORLD_READ WORLD_EXECUTE)
#
#            set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PRJ_NAME}")
#
#        # Uninstall previous program before installing a new
#        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
#
#        # Set display name
#        set(CPACK_NSIS_DISPLAY_NAME ${PRJ_SHORT_DESCRIPTION})
#
#        # Set package name
#        set(CPACK_NSIS_PACKAGE_NAME ${PRJ_SHORT_DESCRIPTION})
#
#        # Set help link
#        set(CPACK_NSIS_HELP_LINK ${PRJ_HOMEPAGE})
#        set(CPACK_NSIS_URL_INFO_ABOUT ${PRJ_HOMEPAGE})
#
#        # Set a Package Version
#        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})
#
#        # Set package name
#        set(CPACK_PACKAGE_NAME "${PRJ_NAME}")
#
#        # Set a brief description
#        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})
#
#        # Set a long description
#        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})
#
#        # Include CPack
#        include(CPack)
endfunction(make_win_package)

function(make_unix_package)
    message("Creating Win package...")
#    add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#
#    # Create package with CPack
#    if (PACKAGE)
#        # Tell CPack to generate a .deb package
#        set(CPACK_GENERATOR "DEB")
#
#        # Binaries
#        install(TARGETS scc
#                DESTINATION bin)
#
#        # Create directory for the logs
#        install(DIRECTORY DESTINATION "/var/log/${PRJ_NAME}_log/")
#
#        # Set a Package Maintainer.
#        set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PRJ_MAINTAINERS})
#
#        # Set a brief description
#        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})
#
#        # Set a long description
#        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})
#
#        # Set a Package Version
#        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})
#
#        # Set dependencies
#        set(CPACK_DEBIAN_PACKAGE_DEPENDS "graphviz (>= 2.42)")
#
#        # Set home page
#        set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PRJ_HOMEPAGE})
#
#        # Include CPack
#        include(CPack)
#    endif()
endfunction(make_unix_package)
