####################################################################################################
#                                  Ceedling test target & rules                                    #
####################################################################################################
GCOVR_EXE := gcovr
ARM		  := arm-none-eabi
CC		  := $(ARM)-gcc
LD		  := $(ARM)-gcc
OBJCOPY	  := $(ARM)-objcopy
AS        := $(ARM)-as
AR        := $(ARM)-ar
NM        := $(ARM)-nm
STRIP     := $(ARM)-strip

PROJECT_NAME	:= STM32_RF_HUB
STM32_SRC_DIRS  := Src Workspace

SW_VER := $(subst .,_,$(shell sVersion -l))
VERSION := $(SW_VER)
export VERSION

GCC_ARM_OUT_DIR          := Build_output
GCC_ARM_OBJ_OUT_DIR      := $(GCC_ARM_OUT_DIR)/object_files
BUILD_SUPPORT            := Workspace/build
STM32_LINKER_SCRIPT 	 := $(BUILD_SUPPORT)/arm-gcc-link.ld
STM32_ELF_FILE           := $(PROJECT_NAME).elf
STM32_MAP_FILE  	     := $(PROJECT_NAME).map
STM32_COMPILER_OUTPUT    := compile_log.txt
STM32_MEM_OUTPUT_FILE    := memory_analysis.txt
STM32_MEM_OUTPUT_FILE    := memory_analysis.txt
RELEASE_PACKAGE_BASE_DIR := Release_package
RELEASE_PACKAGE_NAME     := $(RELEASE_PACKAGE_BASE_DIR)/$(RELEASE_PACKAGE_BASE_DIR)_$(SW_VER)




# Various test reports
CEEDLING_GCOV_DIR					   := test/CodeCoverage
CEEDLING_TEST_XML_TEST_REPORT_ORIGIN   := test/build/artifacts/test
CEEDLING_GCOV_XML_TEST_REPORT_ORIGIN   := test/build/artifacts/gcov
CEEDLING_TEST_XML_TEST_REPORT_DEST 	   := $(CEEDLING_GCOV_DIR)/Test_Report
CEEDLING_LCOV_XML_TEST_REPORT_DEST	   := $(CEEDLING_GCOV_DIR)/LCOV

# Output files
GCOV_OUTPUT_DIR := test/build/gcov/out

# Files to be excluded from gcov coverage
UNWANTED_GEN_COVERAGE :=        \
$(GCOV_OUTPUT_DIR)/cmock*       \
$(GCOV_OUTPUT_DIR)/SELMATH*     \
$(GCOV_OUTPUT_DIR)/CHKSUM*      \
$(GCOV_OUTPUT_DIR)/STDC*        \
$(GCOV_OUTPUT_DIR)/test_*       \
$(GCOV_OUTPUT_DIR)/unity.*      \
$(GCOV_OUTPUT_DIR)/*helper*     \

# location of autoversion header
AUTOVERS_HEADER := Src/autoversion.h

#Find all the source file in the given directories
SRCS := $(shell find $(STM32_SRC_DIRS) -type f -name '*.c')

#Transform the list of c files into a list of object files
OBJS := $(SRCS:.c=.o)

#add the "-I" to all folders found by the find command :)
INCLUDES := $(addprefix -I ,$(shell find $(STM32_SRC_DIRS) -type d))

ifeq ($(RELEASE),y)
	OPTIMISATION := -O3
else
	OPTIMISATION := -O0
endif

CFLAGS :=  \
		-mcpu=cortex-m3 \
		-mthumb \
		-Wall \
		-ffunction-sections \
		-g \
		$(OPTIMISATION) \
		-c \
		-DSTM32DRIVERS \
		-DSTM32F103C8 \
		-DSTM32F10X_MD \
		-DUSE_STDPERIPH_DRIVER \
		-D__ASSEMBLY__

LDFLAGS := \
		-mcpu=cortex-m3 \
		-mthumb \
		-g \
		-nostartfiles \
		"-Wl,-Map=$(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).map" \
		$(OPTIMISATION) \
		-Wl,--gc-sections \
		-L$(BUILD_SUPPORT) \
		-T$(STM32_LINKER_SCRIPT) -g -o $(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).elf \


.PHONY: all
all: GCC_ARM


.PHONY: GCC_ARM
GCC_ARM: build_clean $(AUTOVERS_HEADER) $(OBJS)
	@echo "--> Compiling Completed..."
	@mkdir -p $(GCC_ARM_OBJ_OUT_DIR)
	@echo "--> Linking Object Files..."
	@$(LD) $(LDFLAGS) $(OBJS)
	@echo "--> Linking Completed"
	@echo "--> $(PROJECT_NAME).map and $(PROJECT_NAME).elf files generated at: $(GCC_ARM_OUT_DIR)"
	@$(OBJCOPY) -O binary $(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).elf $(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).bin
	@$(OBJCOPY) -O ihex $(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).elf $(GCC_ARM_OUT_DIR)/$(PROJECT_NAME).hex
	@echo "--> Copying object files to $(GCC_ARM_OUT_DIR) ...."
	@find . -type f -name "*.o" -print0 | xargs -0 -I{} cp "{}" -fr $(GCC_ARM_OBJ_OUT_DIR)
	@mv $(STM32_COMPILER_OUTPUT) $(GCC_ARM_OUT_DIR)
	@-sed -i '1 i\Compiler warnings generated by GCC for $(PROJECT_NAME) ' $(GCC_ARM_OUT_DIR)/$(STM32_COMPILER_OUTPUT)
	@-sed -i '2 i\ ' $(GCC_ARM_OUT_DIR)/$(STM32_COMPILER_OUTPUT)
	@echo "--> Build Succesfully Completed..."


%.o: %.c
	@echo "--> Compiling $(notdir $@) ..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@  2>&1 | tee -a $(STM32_COMPILER_OUTPUT)


# Creates Ceedling environment if it does not exist
test/vendor:
	@createCeedlingEnv

# Wild card test will allow any test to run once the name is provided
%.test: test/vendor
	@$(eval TEST_FILE := $(subst .test,.c,$@))
	@echo Testing $(TEST_FILE)...
	cd test && rake test:$(TEST_FILE)


# Wild card test will allow any test to run once the name is provided
%.test_with_coverage: test/vendor
	@$(eval TEST_FILE := $(subst .test_with_coverage,.c,$@))
	@echo "--> Testing $(TEST_FILE)..."
	cd test && rake gcov:$(TEST_FILE)


.PHONY: test_all
test_all: test/vendor
	cd test && rake test:all
	@mv $(CEEDLING_TEST_XML_TEST_REPORT_ORIGIN)/report.xml $(CEEDLING_TEST_XML_TEST_REPORT_DEST)
	#@ceedling-gen-report $(CEEDLING_TEST_XML_TEST_REPORT_DEST)report.xml $(CEEDLING_TEST_XML_TEST_REPORT_DEST)SoftwareCeedlingTestReport.html
	#@$(call check_test_result)

	
test_all_with_coverage: test/vendor
	@mkdir -p $(CEEDLING_GCOV_DIR)/{Test_Report,LCOV}
	@cd test/unit_test && rake gcov:all
	#@mv $(CEEDLING_TEST_XML_GCOV_REPORT_ORIGIN)/report.xml $(CEEDLING_TEST_XML_TEST_REPORT_DEST)
	#@ceedling-gen-report $(CEEDLING_TEST_XML_TEST_REPORT_DEST)report.xml $(CEEDLING_TEST_XML_TEST_REPORT_DEST)SoftwareCeedlingTestReport.html
	@-rm -f $(UNWANTED_GEN_COVERAGE)


# Make Target: runs lcov coverage tool
.PHONY: gen_lcov_report
# Note: Test report checked for pass at end to ensure that test report is generated regardless of test case failures
gen_lcov_report: test_all_with_coverage
	@echo "--> Generating LCOV reports.. Please be patient..."
	@cd test && lcov -t 'LCov_report' -o CodeCoverage/LCOV/output_file.info -c -d . --rc lcov_branch_coverage=1
	@cd $(CEEDLING_LCOV_XML_TEST_REPORT_DEST) && genhtml -o . output_file.info --rc lcov_branch_coverage=1 --sort --show-details
	@cd $(CEEDLING_LCOV_XML_TEST_REPORT_DEST) && start index.html
	#@$(call check_test_result)




# user fn check overall test result for Pass \ Fail.  Exits with error code on failure
define check_test_result
	@if grep -q "Overall Test Result: PASS" $(CEEDLING_TEST_XML_TEST_REPORT_DEST)SoftwareCeedlingTestReport.html; \
	then \
	  echo "--> Overall Test Result: PASS"; \
	else \
	  echo "--> Overall Test Result: FAIL"; \
	  exit 1; \
	fi
endef


####################################################################################################
#                                  Doxygen targets & rules                                         #
####################################################################################################
DOXYGEN_EXE    := doxygen.exe
DOXYGEN_OUTPUT := doc/doxygen

.PHONY: doxygen
doxygen:
	@mkdir -p $(DOXYGEN_OUTPUT)
	@$(DOXYGEN_EXE) -b Tool_Cfg/Doxygen/Doxygen_config
	@cd $(DOXYGEN_OUTPUT) && find -type f \( -name '*.md5' -o -name '*.map' -o -name '*globals*' -o -name '*dir_*' \) -print0 | xargs -0 rm -f

# Cleans up any generated files from doxygen
.PHONY: doxygen_clean
doxygen_clean:
	@echo "--> Cleaning doxygen..."
	@rm -rf $(DOXYGEN_OUTPUT)/*



####################################################################################################
#                                  Miscellaneous targets & rules                                   #
####################################################################################################

# Print all targets if no target has been supplied
.PHONY: no_targets__ list
no_targets__:
list:
	@echo "--> Available Targets:"
	@sh -c "$(MAKE) -p no_targets__ | awk -F':' '/^[a-zA-Z0-9][^\$$#\/\\t=]*:([^=]|$$)/ {split(\$$1,A,/ /);for(i in A)print A[i]}' | grep -v '__\$$' | sort"


$(AUTOVERS_HEADER):
	sVersion --autoversion

$(GCC_ARM_OUT_DIR)/$(STM32_MAP_FILE):
	@echo "--> $(STM32_MAP_FILE) does not exist, generating now"
	@$(MAKE) -s GCC_ARM

.PHONY: total_clean
total_clean: build_clean
	@cd test && rm -fR build vendor
	@-rm -fR CodeCoverage
	@echo Clean Complete..


.PHONY: build_clean
build_clean:
	@echo cleaning up old build objects..... Please wait.
	@find -type f \( -name '*.o' -o -name '*.bak' \) -print0 | xargs -0 rm -f
	@- rm -fr $(GCC_ARM_OUT_DIR) $(AUTOVERS_HEADER)
	@echo "--> Build cleaned.."



# Set memory limits to match the Resource Usage in the Software Integration Spec

# ROM limits
ROM_SIZE := $(shell awk '/rom/ { rom = strtonum($$9); exit; } END { print rom } ' $(STM32_LINKER_SCRIPT) )
ROM_PERC_LIMIT := 80
ROM_CALC_LIMIT := $(shell echo $(($(ROM_SIZE) * $(ROM_PERC_LIMIT) / 100 )))

# RAM limits
RAM_SIZE := $(shell awk '/ram/ { ram = strtonum($$9); exit; } END { print ram } ' $(STM32_LINKER_SCRIPT) )
RAM_PERC_LIMIT := 80
RAM_CALC_LIMIT := $(shell echo $$(($(RAM_SIZE) * $(RAM_PERC_LIMIT) / 100 )))



.PHONY: memory_stats
memory_stats: $(GCC_ARM_OUT_DIR)/$(STM32_MAP_FILE)
	@echo "--> Analysing elf file for memory stats...."
	@echo "--> Output from GNU \"size\" tool.." | tee $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE)
	@size -B $(GCC_ARM_OUT_DIR)/$(STM32_ELF_FILE) | tee -a $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE)

	@awk 'BEGIN{ printf "\n=======================================================\n\
	====================== RAM STATS ======================\n\
	=======================================================\n\n"; } \
	/$(STM32_ELF_FILE)/ { RamUsed = ($$2) + ($$3); } \
	END{ printf "Available Ram in Bytes: %-6d\nRam used in Bytes:      %-6d\n%%Ram used:              %-3.0f%\n", $(RAM_SIZE), RamUsed, ( RamUsed/$(RAM_SIZE) * 100 ); }' $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE) | tee -a $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE)

	@awk 'BEGIN{ printf "\n=======================================================\n\
	==================== FLASH STATS ======================\n\
	=======================================================\n\n"; } \
	/$(STM32_ELF_FILE)/ { FlashUsed = ($$1);} \
	END{ printf "Available Flash in Bytes: %-6d\nFlash used in Bytes:      %-6d\n%%Flash used:              %-3.0f%\n\
	-------------------------------------------------------\n\n", $(ROM_SIZE), FlashUsed, ( FlashUsed/$(ROM_SIZE) * 100 ); }' $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE) | tee -a $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE)
	
	@awk 'BEGIN{ printf "\n=======================================================\n\
	====================== NVM STATS ======================\n\
	=======================================================\n\n"; } \
	/NVM_info_s/ { NVM_used = strtonum($$2); exit; } \
	END{ printf "NVM used in Bytes: %-6d\n\
	-------------------------------------------------------\n\n", NVM_used; }' $(GCC_ARM_OUT_DIR)/$(STM32_MAP_FILE) | tee -a $(GCC_ARM_OUT_DIR)/$(STM32_MEM_OUTPUT_FILE)
	@echo "--> Output file \"$(STM32_MEM_OUTPUT_FILE)\" created @ $(PROJECT_NAME)/$(GCC_ARM_OUT_DIR)/"

	
ttt:= Build_output/STM32_RFM69_receiver.map

.PHONY: test
test:
	@echo "dir = $(dir $(ttt))"
	@echo "notdir = $(notdir $(ttt))"
	@echo "suffix = $(suffix $(ttt))"
	@echo "basename = $(basename $(ttt))"
	@echo "realpath = $(realpath $(ttt))"
	@echo "abspath = $(abspath $(ttt))"
	@echo "name = $(basename $(notdir $(ttt)))"
	test_MODE_MGR.test
	


.PHONY: release_package
release_package:
	@echo "--> $(SW_VER)"
	@echo "--> Creating release package..."
	@-rm -fr $(RELEASE_PACKAGE_BASE_DIR)
	@mkdir -p $(RELEASE_PACKAGE_NAME)
	@$(MAKE) -s --no-print-directory GCC_ARM
	@echo "--> Generating memory stats..."
	@$(MAKE) -s --no-print-directory memory_stats > /dev/null
	@echo "--> Copying build output to $(RELEASE_PACKAGE_NAME) folder.."
	@cp -r $(GCC_ARM_OUT_DIR)/. $(RELEASE_PACKAGE_NAME)
	@echo "--> Running tests..."
	@$(MAKE) -s --no-print-directory gen_lcov_report
	@echo "--> Copying test results to $(RELEASE_PACKAGE_NAME) folder.."
	@cp -r $(CEEDLING_GCOV_DIR) $(RELEASE_PACKAGE_NAME)
	@echo "--> Running doxygen..."
	#@$(MAKE) -s --no-print-directory doxygen
	@echo "--> Copying doxygen output to $(RELEASE_PACKAGE_NAME) folder.."
	@cp -r $(DOXYGEN_OUTPUT) $(RELEASE_PACKAGE_NAME)
	@echo "--> Copying version file info to $(RELEASE_PACKAGE_NAME) folder.."
	@touch $(RELEASE_PACKAGE_NAME)/version_$(SW_VER)
	@find  $(RELEASE_PACKAGE_NAME) -type f -print0 | sort -z | xargs -0 sha1sum > $(RELEASE_PACKAGE_NAME)/chksum.txt
	@echo "--> Checksum file generated for $(RELEASE_PACKAGE_NAME)/$(RELEASE_PACKAGE_NAME)_$(SW_VER)"
	@echo "--> Zipping up the release package.."
	@cd $(RELEASE_PACKAGE_BASE_DIR) && 7za a "$(notdir $(RELEASE_PACKAGE_NAME))_.zip" ./* > /dev/null
	@-rm -fr $(RELEASE_PACKAGE_NAME)
	@echo "--> Release package created $(SW_VER)"
