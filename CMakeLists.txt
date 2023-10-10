# CMakeList.txt: projeto de CMake para simulador, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("simulador")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/Config.cpp
    src/DataService.cpp
    src/OrderService.cpp
    src/LogService.cpp
    src/Order.cpp
    src/PurchaseOrder.cpp
    src/SaleOrder.cpp
    src/Trader.cpp
    src/Semaphore.cpp
    src/Server.cpp
    src/ServerResponseSender.cpp
    utils/StringUtils.cpp
    utils/ArrayUtils.cpp
    utils/DataUtils.cpp
    utils/OrderUtils.cpp
    src/main.cpp
)

# Adicione a origem ao executável deste projeto.
add_executable (simulador ${SOURCES})

target_link_libraries(simulador PRIVATE nlohmann_json::nlohmann_json)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(simulador
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET simulador PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(simulador PRIVATE Threads::Threads)