cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set(LIBPNG_PATH src/main/cpp/lpng1635)

# OUR SOURCES
set(libpng_public_hdrs
        ${LIBPNG_PATH}/png.h
        ${LIBPNG_PATH}/pngconf.h
        ${LIBPNG_PATH}/pnglibconf.h
        )
set(libpng_private_hdrs
        ${LIBPNG_PATH}/pngpriv.h
        ${LIBPNG_PATH}/pngdebug.h
        ${LIBPNG_PATH}/pnginfo.h
        ${LIBPNG_PATH}/pngstruct.h
        )

set(libpng_arm_sources
        ${LIBPNG_PATH}/arm/arm_init.c
        ${LIBPNG_PATH}/arm/filter_neon.S
        ${LIBPNG_PATH}/arm/filter_neon_intrinsics.c)

set(libpng_intel_sources
        ${LIBPNG_PATH}/intel/intel_init.c
        ${LIBPNG_PATH}/intel/filter_sse2_intrinsics.c)

set(libpng_mips_sources
        ${LIBPNG_PATH}/mips/mips_init.c
        ${LIBPNG_PATH}/mips/filter_msa_intrinsics.c)

set(libpng_powerpc_sources
        ${LIBPNG_PATH}/powerpc/powerpc_init.c
        ${LIBPNG_PATH}/powerpc/filter_vsx_intrinsics.c)

set(libpng_sources
        ${libpng_public_hdrs}
        ${libpng_private_hdrs}
        ${LIBPNG_PATH}/pngdebug.h
        ${LIBPNG_PATH}/pnginfo.h
        ${LIBPNG_PATH}/pngpriv.h
        ${LIBPNG_PATH}/pngstruct.h
        ${LIBPNG_PATH}/png.c
        ${LIBPNG_PATH}/pngerror.c
        ${LIBPNG_PATH}/pngget.c
        ${LIBPNG_PATH}/pngmem.c
        ${LIBPNG_PATH}/pngpread.c
        ${LIBPNG_PATH}/pngread.c
        ${LIBPNG_PATH}/pngrio.c
        ${LIBPNG_PATH}/pngrtran.c
        ${LIBPNG_PATH}/pngrutil.c
        ${LIBPNG_PATH}/pngset.c
        ${LIBPNG_PATH}/pngtrans.c
        ${LIBPNG_PATH}/pngwio.c
        ${LIBPNG_PATH}/pngwrite.c
        ${LIBPNG_PATH}/pngwtran.c
        ${LIBPNG_PATH}/pngwutil.c
        ${libpng_arm_sources}
        ${libpng_intel_sources}
        ${libpng_mips_sources}
        ${libpng_powerpc_sources}
        )

add_library(
        png SHARED
        ${libpng_sources}
)

target_link_libraries(
        png
        log
        z)

include_directories(src/main/cpp)

add_library(
        PNGRender SHARED
        src/main/cpp/JNIPNGRender.cpp
        src/main/cpp/PNGUtil.cpp
)

target_link_libraries(
        PNGRender
        png
        log
        android
        jnigraphics
        z)
