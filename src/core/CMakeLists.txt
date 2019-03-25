
add_library(core STATIC
    command_line.h
    command_line.cpp
    common.h
    functional.h
    guid.h
    guid.h
    integer.h
    list.h
    log.cpp
    log.h
    murmur.cpp
    murmur.h
    os.cpp
    os.h
    pair.h
    platform.h
    time.cpp
    time.h
    types.h
    unit_tests.cpp
    unit_tests.h
)

target_sources(core PRIVATE
	containers/array.h
	containers/event_stream.h
	containers/hash_map.h
	containers/hash_set.h
	containers/map.h
	containers/queue.h
	containers/sort_map.h
	containers/types.h
	containers/vector.h
)

target_sources(core PRIVATE
	error/callstack.h
	error/callstack_android.cpp
	error/callstack_linux.cpp
	error/callstack_windows.cpp
	error/error.cpp
	error/error.h
)

target_sources(core PRIVATE
	filesystem/file.h
	filesystem/file_monitor.h
	filesystem/file_monitor_linux.cpp
	filesystem/file_monitor_noop.cpp
	filesystem/filesystem.h
	filesystem/filesystem_apk.cpp
	filesystem/filesystem_apk.h
	filesystem/filesystem_disk.cpp
	filesystem/filesystem_disk.h
	filesystem/path.cpp
	filesystem/path.h
	filesystem/reader_writer.h
	filesystem/types.h
)

target_sources(core PRIVATE
	json/json.cpp
	json/json.h
	json/json_object.h
	json/sjson.cpp
	json/sjson.h
	json/types.h
)

target_sources(core PRIVATE
	math/aabb.cpp
    math/aabb.h
    math/color4.h
    math/frustum.h
    math/intersection.cpp
    math/intersection.h
    math/math.cpp
    math/math.h
    math/matrix3x3.cpp
    math/matrix3x3.h
    math/matrix4x4.cpp
    math/matrix4x4.h
    math/plane3.h
    math/quaternion.cpp
    math/quaternion.h
    math/random.h
    math/sphere.cpp
    math/sphere.h
    math/types.h
    math/vector2.h
    math/vector3.h
    math/vector4.h
)

target_sources(core PRIVATE
    memory/allocator.h
    memory/linear_allocator.cpp
    memory/linear_allocator.h
    memory/memory.cpp
    memory/memory.h
    memory/pool_allocator.cpp
    memory/pool_allocator.h
    memory/proxy_allocator.cpp
    memory/proxy_allocator.h
    memory/stack_allocator.cpp
    memory/stack_allocator.h
    memory/temp_allocator.h
    memory/types.h
)

target_sources(core PRIVATE
    network/ip_address.h
    network/socket.cpp
    network/socket.h
    network/types.h
)

target_sources(core PRIVATE
    strings/dynamic_string.h
    strings/fixed_string.h
    strings/string.h
    strings/string_id.cpp
    strings/string_id.h
    strings/string_stream.h
    strings/types.h
    strings/utf8.cpp
    strings/utf8.h
)

target_sources(core PRIVATE
    thread/atomic_int.cpp
    thread/atomic_int.h
    thread/condition_variable.cpp
    thread/condition_variable.h
    thread/mutex.cpp
    thread/mutex.h
    thread/semaphore.cpp
    thread/semaphore.h
    thread/thread.cpp
    thread/thread.h
    thread/types.h
)

target_compile_features(core
    PUBLIC
        cxx_std_11
)

target_link_libraries(core
    PUBLIC
        externals::glad
        externals::stb
        externals::spdlog
)

target_compile_options(core
    PUBLIC
        -no-pie
        -Wno-reorder
)

if(LINUX)
target_link_libraries(core PUBLIC
	pthread
)
endif()

if(DAWN_DEBUG_MODE)
target_compile_definitions(core PUBLIC
	-DDAWN_DEBUG=1
)
endif()
