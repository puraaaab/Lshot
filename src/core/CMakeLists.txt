target_sources(lshot PRIVATE
    lshot.h
    lshotdaemon.h
    qguiappcurrentscreen.h
)

target_sources(lshot PRIVATE
    capturerequest.cpp
    lshot.cpp
    lshotdaemon.cpp
    qguiappcurrentscreen.cpp
)

if (UNIX AND NOT APPLE)
target_sources(lshot PRIVATE
    lshotdbusadapter.h
    lshotdbusadapter.cpp
)
endif ()

if (USE_KDSINGLEAPPLICATION)
if (NOT WIN32)
target_sources(lshot PRIVATE
    signaldaemon.h
    signaldaemon.cpp
)
endif ()
endif ()

if (WIN32)
    target_sources(lshot PRIVATE globalshortcutfilter.h globalshortcutfilter.cpp)
endif ()
