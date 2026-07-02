find_package(
        Qt${QT_VERSION_MAJOR}
        CONFIG
        REQUIRED
        Core
        Gui
        Widgets
        Network
        Svg
        LinguistTools
)

if (UNIX)
find_package(
        Qt${QT_VERSION_MAJOR}
        CONFIG
        REQUIRED
        DBus
)
endif()

if (USE_WAYLAND_CLIPBOARD)
    find_package(KF6GuiAddons)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set application icon
if (APPLE)
    set(CapShot_ICONSET ${CMAKE_BINARY_DIR}/CapShot.iconset)
    set(CapShot_ICNS ${CMAKE_BINARY_DIR}/CapShot.icns)

    # generate iconset
    execute_process(
            COMMAND bash "-c" "mkdir -p \"${CapShot_ICONSET}\""
    )

    execute_process(
            COMMAND bash "-c" "sips -z 16 16     \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_16x16.png"
            COMMAND bash "-c" "sips -z 32 32     \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_16x16@2x.png"
            COMMAND bash "-c" "sips -z 32 32     \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_32x32.png"
            COMMAND bash "-c" "sips -z 64 64     \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_32x32@2x.png"
            COMMAND bash "-c" "sips -z 64 64     \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_64x64x.png"
            COMMAND bash "-c" "sips -z 128 128   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_64x64@2.png"
            COMMAND bash "-c" "sips -z 128 128   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_128x128.png"
            COMMAND bash "-c" "sips -z 256 256   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_128x128@2x.png"
            COMMAND bash "-c" "sips -z 256 256   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_256x256.png"
            COMMAND bash "-c" "sips -z 512 512   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_256x256@2x.png"
            COMMAND bash "-c" "sips -z 512 512   \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_512x512.png"
            COMMAND bash "-c" "sips -z 1024 1024 \"${CMAKE_SOURCE_DIR}\"/data/img/app/in.letmegrab.CapShot-1024.png --out \"${CapShot_ICONSET}\"/icon_512x512@2x.png"

            COMMAND bash "-c" "iconutil -o \"${CapShot_ICNS}\" -c icns \"${CapShot_ICONSET}\""
    )

    execute_process(
        COMMAND bash "-c" "rm -R \"${CapShot_ICONSET}\""
    )

    execute_process(
        # copy icon from cache generated on the localhost if generation on CI failed
        COMMAND bash "-c" "[[ -r '\"${CapShot_ICNS}\"' ]] || cp \"${CMAKE_SOURCE_DIR}\"/packaging/macos/CapShot.icns \"${CapShot_ICNS}\""
    )

    # Set application icon
    set(MACOSX_BUNDLE_ICON_FILE CapShot.icns)

    # And this part tells CMake where to find and install the file itself
    set(APP_ICON_MACOSX ${CapShot_ICNS})
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(CapShot MACOSX_BUNDLE main.cpp ${APP_ICON_MACOSX})
else ()
    add_executable(CapShot)
endif ()

add_executable(CapShot::CapShot ALIAS CapShot)

if(FLAMESHOT_ICON)
  target_compile_definitions(CapShot PUBLIC FLAMESHOT_ICON="${FLAMESHOT_ICON}")
endif()

if (WIN32)
    add_executable(CapShot-cli)
    target_sources(
        CapShot-cli
        PRIVATE
        windows-cli.cpp)
    set_target_properties(CapShot-cli PROPERTIES OUTPUT_NAME "CapShot-cli")
    target_link_options(CapShot-cli PRIVATE /SUBSYSTEM:CONSOLE)
    set_property(TARGET CapShot PROPERTY WIN32_EXECUTABLE true)
    # Getting error D8016 - /utf-8 automatically set by current cmake/msvc ?
    #if (MSVC)
    #    target_compile_options(CapShot PRIVATE /source-charset:utf-8)
    #endif ()
endif ()

if(MSVC)
    OPTION(USE_MP "use multiple" ON)
    OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP
            "Set The Global Option COMPILE_FLAGS /MP to target." ON)
    if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
endif()

add_subdirectory(cli)
add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(widgets)
add_subdirectory(tools)


set(CapShot_TS_FILES
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_bg.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ca.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_cs.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_da.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_de_DE.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_el.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_en.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_es.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_et.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_eu.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_fa.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_fi.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_fr.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ga.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_gl.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_grc.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_he.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_hu.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_id.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_it_IT.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ja.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ka.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ko.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_nb_NO.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_nl_NL.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_nl.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_pt.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_pl.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_pt_BR.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ro.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ru.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_sk.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_sr_SP.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_sv_SE.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_sw.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_ta.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_th.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_tr.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_uk.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_vi.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_zh_CN.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_zh_HK.ts
      ${CMAKE_SOURCE_DIR}/data/translations/Internationalization_zh_TW.ts
         )

if (GENERATE_TS)
  qt6_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${CapShot_TS_FILES})
else ()
  qt6_add_translation(QM_FILES ${CapShot_TS_FILES})
endif ()

target_sources(
        CapShot
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/graphics.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/CapShot.rc # windows binary icon resource file
        ${QM_FILES}
        main.cpp)


target_include_directories(CapShot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>  # src/
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/>
)

target_link_libraries(
        CapShot
        project_warnings
        project_options
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        QtColorWidgets
)
if (UNIX)
target_link_libraries(
        CapShot
        Qt${QT_VERSION_MAJOR}::DBus
)
endif()

if (USE_KDSINGLEAPPLICATION)
  message(STATUS "KDSingleApplication is used!")
  add_compile_definitions(USE_KDSINGLEAPPLICATION=1)

  if (USE_BUNDLED_KDSINGLEAPPLICATION)
    target_include_directories(CapShot PRIVATE ${kdsingleapplication_SOURCE_DIR} ${kdsingleapplication_BINARY_DIR})
  endif()

  target_link_libraries(
          CapShot
          KDAB::kdsingleapplication
  )
endif()

if (USE_WAYLAND_CLIPBOARD)
  target_compile_definitions(CapShot PRIVATE USE_WAYLAND_CLIPBOARD=1)
  target_link_libraries(CapShot KF6::GuiAddons)
endif()

if (APPLE)
    set_target_properties(CapShot PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "CapShot"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_IDENTIFIER "in.letmegrab.CapShot"
        MACOSX_BUNDLE_GUI_IDENTIFIER "in.letmegrab.CapShot" 
    ) 
    set_property(TARGET CapShot PROPERTY
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/packaging/macos/Info.plist.in
    )
    target_link_libraries(
            CapShot
            qhotkey
    )
endif ()


set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")

#  will be organized according to the actual directory structure, .h.cpp is put together
source_group(TREE ${_src_root_path} FILES ${_source_list})

if (WIN32)
    set(USE_OPENSSL FALSE)
    if (ENABLE_OPENSSL)
        find_package(OpenSSL)
        if (OPENSSL_FOUND)
            message(STATUS "OpenSSL support enabled.")
            set(USE_OPENSSL TRUE)
        endif ()
    else ()
        mark_as_advanced(CLEAR OPENSSL_LIBRARIES OPENSSL_INCLUDE_DIR)
    endif ()

    if (NOT USE_OPENSSL)
        message(WARNING "OpenSSL is required to upload screenshots")
    endif ()

    target_link_libraries(
            CapShot
            qhotkey
    )
endif ()

# Choose default color palette (small or large)
if($ENV{CapShot_PREDEFINED_COLOR_PALETTE_LARGE})
    set(CapShot_PREDEFINED_COLOR_PALETTE_LARGE true)
else()
    set(CapShot_PREDEFINED_COLOR_PALETTE_LARGE false)
endif()
message("CapShot predefined color palette large: " ${CapShot_PREDEFINED_COLOR_PALETTE_LARGE})
target_compile_definitions(CapShot PRIVATE PREDEFINED_COLOR_PALETTE_LARGE=${CapShot_PREDEFINED_COLOR_PALETTE_LARGE})


find_package (Git)
if( DEFINED ENV{GIT_HASH})
    message("Using provided git_commit_hash: $ENV{GIT_HASH}")
    set(FLAMESHOT_GIT_HASH $ENV{GIT_HASH})
    target_compile_definitions(CapShot PRIVATE FLAMESHOT_GIT_HASH="${FLAMESHOT_GIT_HASH}")
elseif (GIT_FOUND)
    message("git found: ${GIT_EXECUTABLE} in version     ${GIT_VERSION_STRING}")
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD OUTPUT_VARIABLE FLAMESHOT_GIT_HASH)
    string(REGEX REPLACE "\r*\n$" "" FLAMESHOT_GIT_HASH "${FLAMESHOT_GIT_HASH}")
    target_compile_definitions(CapShot PRIVATE FLAMESHOT_GIT_HASH="${FLAMESHOT_GIT_HASH}")
    message("FLAMESHOT_GIT_HASH: ${FLAMESHOT_GIT_HASH}")
else()
    target_compile_definitions(CapShot PRIVATE FLAMESHOT_GIT_HASH="-")
    message(WARNING "Compiling without git commit hash")
endif ()

target_compile_definitions(CapShot PRIVATE APP_PREFIX="${CMAKE_INSTALL_PREFIX}")
target_compile_definitions(CapShot PRIVATE APP_VERSION="v${PROJECT_VERSION}")
#target_compile_definitions(CapShot PRIVATE QAPPLICATION_CLASS=QApplication)
target_compile_definitions(CapShot PRIVATE FLAMESHOT_APP_VERSION_URL="${GIT_API_URL}")
# Enable easier debugging of screenshot capture mode
if (FLAMESHOT_DEBUG_CAPTURE)
    target_compile_definitions(CapShot PRIVATE FLAMESHOT_DEBUG_CAPTURE)
endif ()

if (USE_MONOCHROME_ICON)
    target_compile_definitions(CapShot PRIVATE USE_MONOCHROME_ICON)
endif ()

foreach (FILE ${QM_FILES})
    get_filename_component(F_NAME ${FILE} NAME)
    add_custom_command(
            TARGET CapShot
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${F_NAME}
            ${CMAKE_CURRENT_BINARY_DIR}/translations/${F_NAME})
endforeach ()

# ######################################################################################################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/CapShot)

if(USE_LAUNCHER_ABSOLUTE_PATH)
    set(LAUNCHER_EXECUTABLE "${CMAKE_INSTALL_FULL_BINDIR}/CapShot")
else()
    set(LAUNCHER_EXECUTABLE "CapShot")
endif()

if (WIN32)
    set(INSTALL_DEST ".")
else ()
    set(INSTALL_DEST ${CMAKE_INSTALL_BINDIR})
endif ()

# Install binary
install(TARGETS CapShot
        EXPORT CapShot-targets
        BUNDLE DESTINATION ${INSTALL_DEST}
        RUNTIME DESTINATION ${INSTALL_DEST})
if (WIN32)
    install(TARGETS CapShot-cli
            RUNTIME DESTINATION ${INSTALL_DEST})
endif ()

if (UNIX)
    # Install desktop files, completion and dbus files
    configure_file(${CMAKE_SOURCE_DIR}/data/desktopEntry/package/in.letmegrab.CapShot.desktop
            ${CMAKE_CURRENT_BINARY_DIR}/share/applications/in.letmegrab.CapShot.desktop @ONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/appdata/in.letmegrab.CapShot.metainfo.xml
            ${CMAKE_CURRENT_BINARY_DIR}/share/metainfo/in.letmegrab.CapShot.metainfo.xml COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/shell-completion/CapShot.bash
            ${CMAKE_CURRENT_BINARY_DIR}/share/bash-completion/completions/CapShot COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/shell-completion/CapShot.zsh
            ${CMAKE_CURRENT_BINARY_DIR}/share/zsh/site-functions/_CapShot COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/shell-completion/CapShot.fish
            ${CMAKE_CURRENT_BINARY_DIR}/share/fish/vendor_completions.d/CapShot.fish COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/dbus/in.letmegrab.CapShot.xml
            ${CMAKE_CURRENT_BINARY_DIR}/share/dbus-1/interfaces/in.letmegrab.CapShot.xml COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/dbus/in.letmegrab.CapShot.service.in
            ${CMAKE_CURRENT_BINARY_DIR}/share/dbus-1/services/in.letmegrab.CapShot.service)

    # Install man pages
    configure_file(${CMAKE_SOURCE_DIR}/data/man/man1/CapShot.1
            ${CMAKE_CURRENT_BINARY_DIR}/share/man/man1/CapShot.1 COPYONLY)

    # Install Icons
    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/48x48/apps/in.letmegrab.CapShot.png
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/48x48/apps/in.letmegrab.CapShot.png COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/128x128/apps/in.letmegrab.CapShot.png
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/128x128/apps/in.letmegrab.CapShot.png COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/scalable/apps/in.letmegrab.CapShot.svg
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/scalable/apps/in.letmegrab.CapShot.svg COPYONLY)

    # Install icon with both names
    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/48x48/apps/CapShot.png
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/48x48/apps/CapShot.png COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/128x128/apps/CapShot.png
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/128x128/apps/CapShot.png COPYONLY)

    configure_file(${CMAKE_SOURCE_DIR}/data/img/hicolor/scalable/apps/CapShot.svg
            ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/scalable/apps/CapShot.svg COPYONLY)

    # Install assets
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})

    # Install Translations
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/CapShot/translations)
endif ()

# windeployqt
if (WIN32)
    # Add CMAKE_PREFIX_PATH to search paths
    foreach(prefix ${CMAKE_PREFIX_PATH})
        list(APPEND WINDEPLOYQT_SEARCH_PATHS "${prefix}/bin")
    endforeach()
    list(APPEND WINDEPLOYQT_SEARCH_PATHS "$ENV{QTDIR}/bin")
    
    find_program(WINDEPLOYQT_EXE windeployqt.exe
        PATHS ${WINDEPLOYQT_SEARCH_PATHS}
    )

    if(WINDEPLOYQT_EXE)
        message(STATUS "windeployqt found: ${WINDEPLOYQT_EXE}")

        add_custom_command(
                TARGET CapShot
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E touch $<TARGET_FILE_DIR:CapShot>/CapShot.ini
                COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/windeployqt_stuff
                COMMAND
                ${WINDEPLOYQT_EXE} $<IF:$<CONFIG:Debug>,--debug,--release> --no-translations --compiler-runtime --no-system-d3d-compiler
                --no-quick-import --dir ${CMAKE_BINARY_DIR}/windeployqt_stuff $<TARGET_FILE:CapShot>
                # copy translations manually QM_FILES
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/windeployqt_stuff/translations
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/src/translations
                ${CMAKE_BINARY_DIR}/windeployqt_stuff/translations)

        install(DIRECTORY ${CMAKE_BINARY_DIR}/windeployqt_stuff/ DESTINATION ${INSTALL_DEST})

        install(FILES
                $<TARGET_FILE_DIR:CapShot>/CapShot.ini
                $<TARGET_FILE_DIR:CapShot>/CapShot.lib
                $<TARGET_FILE_DIR:CapShot>/CapShot.exp
                DESTINATION ${INSTALL_DEST}
                OPTIONAL)

        STRING(REGEX REPLACE "\\\\" "/" OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT_DIR}" )

        if (ENABLE_OPENSSL)
            if (EXISTS ${OPENSSL_ROOT_DIR}/bin)

                install(
                        DIRECTORY ${OPENSSL_ROOT_DIR}/bin/
                        DESTINATION ${INSTALL_DEST}
                        FILES_MATCHING
                        PATTERN "*.dll")
            else ()
                message(WARNING "Unable to find OpenSSL dlls.")
            endif ()
        endif ()

    else ()
        message(WARNING "Unable to find executable windeployqt.")
    endif ()
endif ()

# macdeployqt
if (APPLE)
# Code signing settings - optional, set to empty string to skip signing
    set(CODE_SIGN_IDENTITY "" CACHE STRING "Code signing identity (leave empty to skip signing)")
    set(DMG_SIGN_IDENTITY "" CACHE STRING "DMG signing identity (leave empty to skip signing)")
    # Custom target to create DMG (signed or unsigned)
    add_custom_target(create_dmg
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../packaging/macos/create_dmg.sh 
            "${CMAKE_CURRENT_BINARY_DIR}/CapShot.app"
            "${CMAKE_CURRENT_BINARY_DIR}/CapShot-${PROJECT_VERSION}.dmg"
            "${CODE_SIGN_IDENTITY}"
            "${DMG_SIGN_IDENTITY}"
    DEPENDS CapShot
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Creating DMG"
    VERBATIM
    )

# Always sign the app bundle (either with identity or ad hoc)
    if(CODE_SIGN_IDENTITY AND NOT CODE_SIGN_IDENTITY STREQUAL "")
        # Identity-based signing (requires Developer ID)
        add_custom_command(TARGET CapShot POST_BUILD
            COMMAND codesign --force --deep --sign "${CODE_SIGN_IDENTITY}" 
                    --options runtime --timestamp "$<TARGET_BUNDLE_DIR:CapShot>"
            COMMENT "Code signing app bundle with ${CODE_SIGN_IDENTITY}"
        )
    else()
        # Ad hoc signing 
        add_custom_command(TARGET CapShot POST_BUILD
            COMMAND codesign --force --deep --sign - "$<TARGET_BUNDLE_DIR:CapShot>"
            COMMENT "Ad hoc code signing app bundle (no identity required)"
        )
    endif()
   # Deploy Qt libraries and dependencies
   find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS ${Qt6_DIR}/../../../bin)
    
   if(MACDEPLOYQT_EXECUTABLE)
       add_custom_command(TARGET CapShot POST_BUILD
           COMMAND ${MACDEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_DIR:CapShot>" -verbose=2
           COMMENT "Deploying Qt libraries"
       )
       
       # Re-sign after macdeployqt (it modifies the bundle)
       if(CODE_SIGN_IDENTITY AND NOT CODE_SIGN_IDENTITY STREQUAL "")
           add_custom_command(TARGET CapShot POST_BUILD
               COMMAND codesign --force --deep --sign "${CODE_SIGN_IDENTITY}" 
                       --options runtime --timestamp "$<TARGET_BUNDLE_DIR:CapShot>"
               COMMENT "Re-signing app bundle after Qt deployment"
           )
       else()
           add_custom_command(TARGET CapShot POST_BUILD
               COMMAND codesign --force --deep --sign - "$<TARGET_BUNDLE_DIR:CapShot>"
               COMMENT "Re-signing app bundle after Qt deployment (ad hoc)"
           )
       endif()
   else()
       message(WARNING "macdeployqt not found. App may not run on systems without Qt installed.")
   endif()


endif ()