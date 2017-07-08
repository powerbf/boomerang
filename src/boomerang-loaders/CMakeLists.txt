
include_directories(
	"${CMAKE_SOURCE_DIR}/src/boomerang"
)

set(IFC_SOURCES
	${CMAKE_SOURCE_DIR}/src/boomerang/loader/IFileLoader.h
	${CMAKE_SOURCE_DIR}/src/boomerang/loader/IBinaryFile.h
)


macro(BOOMERANG_ADD_LOADER name)
	option(BOOM_BUILD_LOADER_${name} "Build the ${name} loader." ON)

	if (BOOM_BUILD_LOADER_${name})
		set(target_name "boomerang_${name}Loader")
		add_library(${target_name} SHARED ${IFC_SOURCES} ${ARGN})

		set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${BOOMERANG_OUTPUT_DIR}/lib")
		if (WIN32)
			set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BOOMERANG_OUTPUT_DIR}/lib")
		endif (WIN32)
		
		qt5_use_modules(${target_name} Core)

		install(TARGETS ${target_name}
			ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib
			RUNTIME DESTINATION bin
		)
	endif ()
endmacro()


BOOMERANG_ADD_LOADER(Elf      elf/ElfBinaryLoader.cpp elf/ElfBinaryLoader.h elf/ElfTypes.h)
BOOMERANG_ADD_LOADER(Exe      exe/dos/ExeBinaryLoader.cpp exe/dos/ExeBinaryLoader.h)
BOOMERANG_ADD_LOADER(DOS4GW   exe/dos4gw/DOS4GWBinaryLoader.cpp exe/dos4gw/DOS4GWBinaryLoader.h microX86dis.c)
BOOMERANG_ADD_LOADER(Win32    exe/pe/Win32BinaryLoader.cpp exe/pe/Win32BinaryLoader.h microX86dis.c)
if (MSVC AND BOOM_BUILD_LOADER_Win32)
    target_link_libraries(boomerang_Win32Loader Dbghelp.lib)
endif ()

BOOMERANG_ADD_LOADER(HpSom    hpsom/HpSomBinaryLoader.cpp hpsom/HpSomBinaryLoader.h)
BOOMERANG_ADD_LOADER(Palm     palm/PalmBinaryLoader.cpp palm/PalmBinaryLoader.h palm/palmsystraps.h)
BOOMERANG_ADD_LOADER(MachO    machO/MachOBinaryLoader.cpp machO/MachOBinaryLoader.h machO/MachOBinaryLoader.cpp machO/macho-apple.h)
