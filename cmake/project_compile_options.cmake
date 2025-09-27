function(project_compile_common)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		# using Clang
		target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
		target_compile_options(${PROJECT_NAME} PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
		
		if(BUILD_ENABLE_TSAN)
			target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=thread)
			target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=thread)
		endif()

		if(BUILD_ENABLE_ASAN)
			target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
			target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
		endif()

		if(BUILD_ENABLE_MSAN)
			target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=memory -fno-omit-frame-pointer -fsanitize=memory-track-origins=2)
			target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=memory)
		endif()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		# using GCC
		target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		# using Visual Studio C++
		target_compile_options(${PROJECT_NAME} PRIVATE /MP /permissive- /W4)
		target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)
	endif()
	
	if(BUILD_ENABLE_HARDENED)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# using Clang
			# target_compile_options(${PROJECT_NAME} PRIVATE -fstack-clash-protection)
			target_compile_options(${PROJECT_NAME} PRIVATE -fstack-protector-strong)
			target_compile_options(${PROJECT_NAME} PRIVATE -mretpoline)

			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,noexecstack)
			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,relro)
			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,now)

			if(${CMAKE_BUILD_TYPE} STREQUAL Release)
				target_compile_options(${PROJECT_NAME} PRIVATE -D_FORTIFY_SOURCE=2)
			endif()
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# using GCC
			target_compile_options(${PROJECT_NAME} PRIVATE -fstack-clash-protection)
			target_compile_options(${PROJECT_NAME} PRIVATE -fstack-protector-strong)
			target_compile_options(${PROJECT_NAME} PRIVATE -mindirect-branch=thunk -mfunction-return=thunk)
			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,noexecstack)
			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,relro)
			target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,now)

			if(${CMAKE_BUILD_TYPE} STREQUAL Release)
				target_compile_options(${PROJECT_NAME} PRIVATE -D_FORTIFY_SOURCE=2)
			endif()
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			# using Visual Studio C++
			target_compile_options(${PROJECT_NAME} PRIVATE /GS /DYNAMICBASE)
		endif()
	endif()
endfunction()

function(project_compile_lib)
	project_compile_common()

	if(BUILD_ENABLE_HARDENED)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# using Clang
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# using GCC
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			# using Visual Studio C++
		endif()
	endif()
endfunction()

function(project_compile_exe)
	project_compile_common()

	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

	if(BUILD_ENABLED_HARDENED)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# using Clang
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIE)
			target_link_options(${PROJECT_NAME} PRIVATE -pie)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# using GCC
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIE)
			target_link_options(${PROJECT_NAME} PRIVATE -pie)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			# using Visual Studio C++
		endif()
	endif()
endfunction()


function(project_compile_options_exe)
    project_compile_options()

	if(BUILD_ENABLE_HARDENED)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# using Clang
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# using GCC
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			# using Visual Studio C++
		endif()
	endif()
endfunction()

function(project_compile_exe)
	project_compile_common()

	if(BUILD_ENABLED_HARDENED)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# using Clang
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIE)
			target_link_options(${PROJECT_NAME} PRIVATE -pie)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# using GCC
			target_compile_options(${PROJECT_NAME} PRIVATE -fPIE)
			target_link_options(${PROJECT_NAME} PRIVATE -pie)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			# using Visual Studio C++
		endif()
	endif()
endfunction()
