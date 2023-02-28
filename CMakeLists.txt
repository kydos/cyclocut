cmake_minimum_required(VERSION 3.0.0)
project(cdds-util VERSION 0.1.6)

include(CTest)
enable_testing()

# include_directories(./include /usr/local/include ${CYCLONE_INCLUDE})
# link_directories(/usr/local/lib ${CYCLONE_LIB})

include_directories(./include $ENV{CYCLONE_INCLUDE})
link_directories($ENV{CYCLONE_LIB})

if (BUILD_SHARED_LIBS OR NOT DEFINED BUILD_SHARED_LIBS)
    add_library(cdds-util SHARED src/cdds_blob.c src/builtin.c)
else()
    add_library(cdds-util src/cdds_blob.c src/builtin.c)
endif()

target_link_libraries(cdds-util PUBLIC ddsc)

add_compile_definitions(DEBUG)

option(BUILD_CDDS_UTIL_EXAMPLES "Build cdds-util examples" ON)
if(BUILD_CDDS_UTIL_EXAMPLES)
    add_executable(bsub example/blob_subscriber.c)
    target_link_libraries(bsub ddsc cdds-util)
    add_executable(bbridge example/blob_bridge.c)
    target_link_libraries(bbridge ddsc cdds-util)
    add_executable(kl_pub example/kl_pub.c)
    target_link_libraries(kl_pub ddsc cdds-util)
    add_executable(k_pub example/k_pub.c)
    target_link_libraries(k_pub ddsc cdds-util)
    add_executable(kl_sub example/kl_sub.c)
    target_link_libraries(kl_sub ddsc cdds-util)
    add_executable(k_sub example/k_sub.c)
    target_link_libraries(k_sub ddsc cdds-util)
    add_executable(nb_sub example/nb_sub.c)
    target_link_libraries(nb_sub ddsc cdds-util)
    add_executable(nb_pub example/nb_pub.c)
    target_link_libraries(nb_pub ddsc cdds-util)
    install(TARGETS bsub DESTINATION bin)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

install(TARGETS cdds-util DESTINATION lib)
install(DIRECTORY include/cdds DESTINATION include)
