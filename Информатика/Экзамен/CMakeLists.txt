cmake_minimum_required(VERSION 3.10)
project(OnlineStore LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(shop
  src/main.cpp
  src/Menu.cpp
  src/User.cpp
  src/Order.cpp
  src/Payment.cpp
  src/ReportGenerator.cpp
)

target_link_libraries(shop PRIVATE pqxx pq)
