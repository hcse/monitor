project(monitor)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( monitor monitor monitor.cpp admin.cpp)
target_include_directories( monitor PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( monitor ${CMAKE_SOURCE_DIR}/../ricardian )