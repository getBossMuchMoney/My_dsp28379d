################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Cpu/BitField/headers/source/%.obj: ../Cpu/BitField/headers/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="E:/project/28379DCPU1/Driver" --include_path="E:/project/28379DCPU1/Cpu/DriverLib/inc" --include_path="E:/project/28379DCPU1/Cpu/DriverLib" --include_path="E:/project/28379DCPU1" --include_path="D:/ti/c2000/C2000Ware_5_03_00_00/driverlib/f2837xd/driverlib" --include_path="D:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="E:/project/28379DCPU1/App" --include_path="E:/project/28379DCPU1/Kernel" --include_path="E:/project/28379DCPU1/Module" --include_path="E:/project/28379DCPU1/Cpu/BitField/Common/include" --include_path="E:/project/28379DCPU1/Cpu/BitField/headers/include" --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Cpu/BitField/headers/source/$(basename $(<F)).d_raw" --include_path="E:/project/28379DCPU1/CPU1_FLASH/syscfg" --obj_directory="Cpu/BitField/headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


