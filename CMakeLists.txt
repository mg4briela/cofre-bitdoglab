# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa SDK
include(pico_sdk_import.cmake)

# Nome do projeto
project(cofre C CXX ASM)

# Inicializa SDK
pico_sdk_init()

# Importa FreeRTOS (CAMINHO CORRIGIDO!)
include("C:/Users/Maria Gabriela/Downloads/FreeRTOSv202406.01-LTS/FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")

# ---------------------------
# Arquivos do projeto
# ---------------------------
file(GLOB SOURCE_FILES 
    "src/*.c"
)

#file(GLOB DRIVER_FILES
#    "src/drivers/*.c"
#)

set(DRIVER_FILES
    src/drivers/teclado.c
)


add_executable(cofre
    ${SOURCE_FILES}
    ${DRIVER_FILES}

    # Tasks FreeRTOS
    src/tasks/task_teclado.c
    src/tasks/task_cofre.c
    src/tasks/task_display.c
    src/tasks/task_acelerometro.c
    src/tasks/task_buzzer.c
    src/queue_shared.c

    # Drivers
    src/drivers/display.c
    external/ssd1306/ssd1306.c
    src/drivers/matriz_led.c
    src/drivers/servo.c
    src/drivers/buzzer.c
    src/drivers/acelerometro.c
)

pico_generate_pio_header(cofre
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ws2818b.pio
)

pico_set_program_name(cofre "cofre")
pico_set_program_version(cofre "1.0")

# Saída via USB
pico_enable_stdio_uart(cofre 0)
pico_enable_stdio_usb(cofre 1)

# ---------------------------
# Bibliotecas usadas
# ---------------------------
target_link_libraries(cofre
    pico_stdlib
    hardware_i2c
    hardware_pwm
    hardware_pio
    hardware_clocks
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# ---------------------------
# Includes do projeto
# ---------------------------
target_include_directories(cofre PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers
    ${CMAKE_CURRENT_LIST_DIR}/external/ssd1306
)

# Gera UF2, bin, etc
pico_add_extra_outputs(cofre)
