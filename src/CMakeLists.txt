project(events)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( events events events.cpp )
target_include_directories( events PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( events ${CMAKE_SOURCE_DIR}/../ricardian )