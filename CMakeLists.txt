# CMakeList.txt : CMake project for chloride, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("chloride")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
set(BUILD_SHARED_LIBS true)


set(ng-log_DIR "C:\\Users\\Alessia\\source\\repos\\ng-log\\out\\install\\x64-Clang-Release\\lib\\cmake\\ng-log")
set(glfw3_DIR "C:\\Users\\Alessia\\source\\repos\\glfw\\out\\install\\x64-Clang-Release\\lib\\cmake\\glfw3")
set(assimp_DIR "C:\\Users\\Alessia\\source\\repos\\assimp\\out\\install\\x64-Clang-Release\\lib\\cmake\\assimp-5.4")
set(PNG_DIR "C:\\Users\\Alessia\\source\\repos\\libpng\\out\\install\\x64-Clang-Release\\lib\\cmake\\PNG")
set(ZLIB_INCLUDE_DIR "C:\\Users\\Alessia\\source\\repos\\zlib\\out\\install\\x64-Clang-Release\\include")
set(ZLIB_LIBRARY "C:\\Users\\Alessia\\source\\repos\\zlib\\out\\install\\x64-Clang-Release\\lib")
set(PNG_PNG_INCLUDE_DIR "C:\\Users\\Alessia\\source\\repos\\libpng\\out\\install\\x64-Clang-Release\\include")
set(PNG_LIBRARY "C:\\Users\\Alessia\\source\\repos\\libpng\\out\\install\\x64-Clang-Release\\lib")
link_directories("C:\\Users\\Alessia\\source\\repos\\libpng\\out\\install\\x64-Clang-Release\\bin")
link_directories("C:\\Users\\Alessia\\source\\repos\\zlib\\out\\install\\x64-Clang-Release\\bin")
include_directories(${PNG_PNG_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIR})
link_directories(${PNG_LIBRARY})
link_directories(${ZLIB_LIBRARY})

find_package(ng-log REQUIRED)
find_package(assimp REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${CMAKE_SOURCE_DIR})

# Add source to this project's executable.
add_executable (chloride "chloride.cpp" "chloride.h" )
add_library (chlorideEngine SHARED
	"graphic/Interface.h" 
	"graphic/Interface.cpp"
	"glad/glad.c"
	 
	"engine/Engine.h" 
	"engine/Engine.cpp" 
	"engine/Stage.h" 
	"engine/Stage.cpp" 
	"engine/Actor.h" 
	"engine/Actor.cpp" 
	"graphic/RenderHandler.h" 
	"graphic/RenderHandler.cpp" 
	"engine/components/Mesh.h" 
	"engine/components/Mesh.cpp" 
	"graphic/RenderManager.h" 
	"graphic/RenderManager.cpp" 
	"utils/MeshFactory.h" 
	"utils/MeshFactory.cpp" 
	"graphic/handler/BaseMeshHandler.h" 
	"graphic/handler/BaseMeshHandler.cpp" 
	"utils/Spinlock.h" 
	"engine/Object.h" 
	"graphic/ShaderFactory.h" 
	"graphic/ShaderFactory.cpp" 
	"engine/Camera.h" 
	"engine/Camera.cpp")
target_link_libraries (chlorideEngine
	glfw
	OpenGL::GL
	ng-log::ng-log
	assimp::assimp
	libpng16
	z
)
add_executable(staticMesh 
	"examples/StaticMesh.cpp"
)
target_link_libraries(staticMesh
	chlorideEngine
)

list(APPEND ShaderList 
	"shader/generic.vert"
	"shader/generic.frag"
)

foreach(Shader IN LISTS ShaderList) 
	file(COPY ${Shader} DESTINATION ${CMAKE_BINARY_DIR}/shader/)
endforeach()

set_property(TARGET chloride PROPERTY CXX_STANDARD 17)
set_property(TARGET chlorideEngine PROPERTY CXX_STANDARD 17)
set_property(TARGET staticMesh PROPERTY CXX_STANDARD 17)

# TODO: Add tests and install targets if needed.
