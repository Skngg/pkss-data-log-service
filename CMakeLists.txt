cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(pkss-data-log-service VERSION 0.4)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_LIBRARY(LibsModule 
    "src/BuildingData.cpp"
    "src/ProviderData.cpp"
    "src/HeatExchangerData.cpp"
    "src/ControllerData.cpp"
    "src/JSONData.cpp"
)

target_link_libraries(LibsModule -L/usr/local/include/pistache)
target_link_libraries(LibsModule -L/usr/local/include/rapidJSON)
target_link_libraries(LibsModule -L/usr/local/lib/libpqxx)
target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -lpqxx)
target_link_libraries(LibsModule -lpq)
target_link_libraries(LibsModule -lpistache)

add_executable(pkss-data-log-service "src/main.cpp")
target_link_libraries(pkss-data-log-service LibsModule)
