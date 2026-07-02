target_sources(CapShot PRIVATE
    CapShot.h
    CapShotdaemon.h
    qguiappcurrentscreen.h
)

target_sources(CapShot PRIVATE
    capturerequest.cpp
    CapShot.cpp
    CapShotdaemon.cpp
    qguiappcurrentscreen.cpp
)

if (UNIX AND NOT APPLE)
target_sources(CapShot PRIVATE
    CapShotdbusadapter.h
    CapShotdbusadapter.cpp
)
endif ()

if (USE_KDSINGLEAPPLICATION)
if (NOT WIN32)
target_sources(CapShot PRIVATE
    signaldaemon.h
    signaldaemon.cpp
)
endif ()
endif ()

if (WIN32)
    target_sources(CapShot PRIVATE globalshortcutfilter.h globalshortcutfilter.cpp)
endif ()
