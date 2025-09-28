set(PROJECT_VENDOR "geugenm")
set(PROJECT_CONTACT "glebajk@gmail.com")
set(PROJECT_LICENSE "GPL-2.0") # Use SPDX identifier (required by DEB/RPM)
set(PROJECT_GROUP "System")

# Optional: detect architecture automatically (don't hardcode)
if(NOT DEFINED PROJECT_ARCH)
    set(PROJECT_ARCH ${CMAKE_SYSTEM_PROCESSOR})
    # Normalize common names
    if(PROJECT_ARCH STREQUAL "x86_64")
        set(PROJECT_ARCH "amd64") # Debian uses 'amd64'
    elseif(PROJECT_ARCH MATCHES "aarch64|arm64")
        set(PROJECT_ARCH "arm64")
    endif()
endif()

set(PROJECT_ICON_FILE "${CMAKE_CURRENT_LIST_DIR}/icon.png")
set(PROJECT_LICENSE_FILE "${CMAKE_SOURCE_DIR}/license") # uppercase (convention)
set(PROJECT_README_FILE "${CMAKE_SOURCE_DIR}/readme.md")

foreach(file ${PROJECT_LICENSE_FILE} ${PROJECT_README_FILE}
             ${PROJECT_ICON_FILE})
    if(NOT EXISTS "${file}")
        message(WARNING "CPack resource missing: ${file}")
    endif()
endforeach()

configure_file("${CMAKE_CURRENT_LIST_DIR}/package.desktop.in"
               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop" @ONLY)

# Documentation
install(
    FILES "${PROJECT_README_FILE}" "${PROJECT_LICENSE_FILE}"
    DESTINATION "share/doc/${PROJECT_NAME}"
    COMPONENT documentation
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# Icon (standard locations)
install(FILES "${PROJECT_ICON_FILE}" DESTINATION "share/pixmaps"
        RENAME "${PROJECT_NAME}.png")
install(FILES "${PROJECT_ICON_FILE}"
        DESTINATION "share/icons/hicolor/256x256/apps"
        RENAME "${PROJECT_NAME}.png")

# Desktop entry (Linux GUI menu)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop"
        DESTINATION "share/applications")

include(CPack)
cpack_add_component(
    documentation
    DISPLAY_NAME "Documentation"
    DESCRIPTION "Project README and license"
    GROUP "Documentation")

# Optional: add 'runtime' component if you have executables
# cpack_add_component(runtime ...)

# === CPack Core Configuration ===
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
set(CPACK_PACKAGE_CONTACT "${PROJECT_CONTACT}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")

# Use SPDX license (required for DEB/RPM)
set(CPACK_PACKAGE_LICENSE "${PROJECT_LICENSE}")

# Resources
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_LICENSE_FILE}")
set(CPACK_RESOURCE_FILE_README "${PROJECT_README_FILE}")

# Description file (optional)
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_LIST_DIR}/description.txt")
if(NOT EXISTS "${CPACK_PACKAGE_DESCRIPTION_FILE}")
    # Fallback to README
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_README_FILE}")
endif()

# Icon (used by macOS/Windows installers)
set(CPACK_PACKAGE_ICON "${PROJECT_ICON_FILE}")

# === Generator-Specific Settings ===

# DEB
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROJECT_CONTACT}")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel") # or "games", "utils", etc.
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# RPM
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_LICENSE}") # SPDX
set(CPACK_RPM_PACKAGE_GROUP "${PROJECT_GROUP}")
set(CPACK_RPM_PACKAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_RPM_PACKAGE_AUTOREQPROV "yes")

# macOS
set(CPACK_MACOSX_PACKAGE_NAME "${PROJECT_NAME}")
if(EXISTS "${PROJECT_ICON_FILE}")
    set(CPACK_MACOSX_BUNDLE_ICON "${PROJECT_ICON_FILE}")
endif()

# Windows (NSIS)
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_MUI_ICON "${PROJECT_ICON_FILE}")
set(CPACK_NSIS_MUI_UNIICON "${PROJECT_ICON_FILE}")
