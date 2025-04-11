cmake_minimum_required(VERSION 3.10)
project(lab02)

# Ustaw standard C++ (np. C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Znajdź Pythona 3 wraz z NumPy (wymagane przez matplotlib-cpp)
find_package(Python3 COMPONENTS Interpreter NumPy REQUIRED)

# Dodaj katalog z plikami nagłówkowymi biblioteki (matplotlibcpp.h)
# ${CMAKE_CURRENT_SOURCE_DIR}/matplotlib-cpp to ścieżka do folderu, gdzie jest plik .h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/matplotlib-cpp)

# Dodaj plik wykonywalny (Twoja aplikacja)
add_executable(lab02 main.cpp)

# Połącz z bibliotekami Pythona (Interpreter, NumPy).
# Python3::NumPy jest dostępny od nowszych wersji CMake (3.18+).
# Jeśli używasz starszego CMake, możesz musieć linkować się przez Python3::Python i ręcznie dołączyć ścieżkę do NumPy.
target_link_libraries(lab02 PRIVATE Python3::Python Python3::NumPy)
