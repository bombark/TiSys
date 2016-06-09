cmake_minimum_required(VERSION 2.6)

# Configure
project( TiSys )
add_definitions(-std=gnu++0x)
#set (CMAKE_INSTALL_PREFIX ./deb)
#find_package(tiobj)
set(LIBS tisys tiarg tiobj)

# Compile
add_library(tisys SHARED src/tisys-system.cpp src/tisys-filesystem.cpp)    # src/tisys-process.cpp)
#add_executable(tisys.info src/tisys-info.cpp)
#add_executable(tisys.ls     src/tisys-ls.cpp)
#add_executable(node-class   src/node-class.cpp)
#add_executable(folder-ls    src/folder-ls.cpp)
#add_executable(tisys.ctx    src/tisys-ctx.cpp)



# Examples
#add_executable(ex-listdir   examples/listdir.cpp)
#target_link_libraries(ex-listdir ${LIBS})

add_executable(ex-info   examples/info.cpp)
target_link_libraries(ex-info ${LIBS})


#target_link_libraries(tisys.info ${LIBS})
#target_link_libraries(  tisys.ls ${LIBS})
#target_link_libraries(node-class ${LIBS})
#target_link_libraries(folder-ls  ${LIBS})
#target_link_libraries( tisys.ctx ${LIBS})


# Install
#install(TARGETS tisys.info  DESTINATION bin)
#install(TARGETS tisys.ls    DESTINATION bin)
#install(TARGETS tisys.ctx   DESTINATION bin)
#install(TARGETS node-class  DESTINATION bin)
#install(TARGETS folder-ls   DESTINATION bin)
install(TARGETS tisys       DESTINATION lib)
install(FILES src/tisys.hpp DESTINATION include)
