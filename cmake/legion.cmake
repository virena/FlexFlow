include_directories(${LEGION_ROOT}/include ${LEGION_ROOT}/include/mappers)
add_library(legion STATIC IMPORTED)
set_property(TARGET legion PROPERTY IMPORTED_LOCATION ${LEGION_ROOT}/lib64/liblegion.a)
add_library(realm STATIC IMPORTED)
set_property(TARGET realm PROPERTY IMPORTED_LOCATION ${LEGION_ROOT}/lib64/librealm.a)