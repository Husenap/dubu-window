message("-- External Project: dubu_event")
include(FetchContent)

FetchContent_Declare(
    dubu_event
    GIT_REPOSITORY  https://github.com/Husenap/dubu-event.git
    GIT_TAG         v2.0
)

set(dubu_event_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(dubu_event_FOLDER "thirdparty/dubu_event" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(dubu_event)