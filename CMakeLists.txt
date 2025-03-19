cmake_minimum_required(VERSION 3.22)
project(TestFreeRTOS C ASM)

#Basic cross-compilation settings for ARM Cortex-M4
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

#Configure the ARM GCC toolchain
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY       ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE          ${TOOLCHAIN_PREFIX}size)

#Compiler and linker flags
#    - We enable some warnings, section garbage collection,
#      and link against a minimal set of standard libraries.
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Basic C compiler flags
set(CMAKE_C_FLAGS "${CPU_FLAGS} -Wall -Wextra -Wpedantic -ffunction-sections -fdata-sections -Os -g0")

#Linker flags:
set(CMAKE_EXE_LINKER_FLAGS
    "${CPU_FLAGS} \
     -Wl,--gc-sections \
     -T ${CMAKE_SOURCE_DIR}/STM32F405VGTx_FLASH.ld \
     --specs=nano.specs \
     -Wl,-Map=${PROJECT_NAME}.map \
     -Wl,--print-memory-usage \
     -Wl,--start-group -lc -lm -Wl,--end-group"
)

#List of source files for HAL, FreeRTOS, and Core application
#HAL modules
set(HAL_SOURCES
    # Core HAL source files
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c

    # ADC modules
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    
    # CAN module
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
)

#FreeRTOS kernel files
set(FREERTOS_SOURCES
    # Core FreeRTOS kernel
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
	
    # CMSIS-RTOS V2 wrapper
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
	
    # FreeRTOS portable layer for ARM Cortex-M4F
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

#Application code under Core/Src
set(CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/Core/Src/main.c
	${CMAKE_SOURCE_DIR}/Core/Src/can_task.c
	${CMAKE_SOURCE_DIR}/Core/Src/adc_task.c
	${CMAKE_SOURCE_DIR}/Core/Src/ntc_steinhart.c
	${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
    ${CMAKE_SOURCE_DIR}/Core/Src/freertos.c
    ${CMAKE_SOURCE_DIR}/Core/Src/gpio.c
    ${CMAKE_SOURCE_DIR}/Core/Src/adc.c
    ${CMAKE_SOURCE_DIR}/Core/Src/can.c
	${CMAKE_SOURCE_DIR}/Core/Src/dma.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f4xx_it.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f4xx_hal_msp.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f4xx_hal_timebase_tim.c
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32f4xx.c
    ${CMAKE_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_SOURCE_DIR}/Core/Src/syscalls.c
)

#Startup code
set(STARTUP_SOURCES
    ${CMAKE_SOURCE_DIR}/startup_stm32f405xx.s
)

#Combine all sources into one list
set(ALL_SOURCES
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${CORE_SOURCES}
    ${STARTUP_SOURCES}
)

#Create the ELF target with our combined sources
add_executable(${PROJECT_NAME}.elf ${ALL_SOURCES})

#Specify include directories for HAL, CMSIS, and FreeRTOS
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    #Application headers
    ${CMAKE_SOURCE_DIR}/Core/Inc

    #HAL driver includes
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy

    #CMSIS headers
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include

    #FreeRTOS
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

#Define necessary preprocessor macros for STM32 and HAL
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    USE_HAL_DRIVER
    STM32F405xx
)

#Post-build: display size 
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Displaying the size of the final ELF."
)

message(STATUS "Project configuration complete.")
