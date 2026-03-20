# This file is designed to enable FreeRTOS and floating point registers for a variety of STM32's. It should work, but your mileage may vary.

Import("env")

# Access the board configuration
board_config = env.BoardConfig()
cpu = board_config.get("build.cpu", "")

# Define flags based on CPU architecture
fpu_flags = []

if "cortex-m4" in cpu:
    fpu_flags = ["-mfloat-abi=hard", "-mfpu=fpv4-sp-d16"]
elif "cortex-m7" in cpu:
    # Most M7s use fpv5. For double-precision chips, 
    # you might need -sp (single precision) on certain M7 processors (STM32F7 sometimes are this case, at which point double precision will break things)
    # fpu_flags = ["-mfloat-abi=hard", "-mfpu=fpv5-sp-d16"]
    fpu_flags = ["-mfloat-abi=hard", "-mfpu=fpv5-d16"]
elif "cortex-m33" in cpu:
    fpu_flags = ["-mfloat-abi=hard", "-mfpu=fpv5-sp-d16"]
else:
    # Fallback or notification for unsupported cores
    print(f"--- force_fpu.py: No specific FPU flags for CPU {cpu} ---")

if fpu_flags:
    env.Append(
        ASFLAGS=fpu_flags,
        CCFLAGS=fpu_flags,
        LINKFLAGS=fpu_flags
    )
    print(f"--- force_fpu.py: Applied {fpu_flags} for {cpu} ---")