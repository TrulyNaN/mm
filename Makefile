# Build options can be changed by modifying the makefile or by building with 'make SETTING=value'.
# It is also possible to override the settings in Defaults in a file called .make_options as 'SETTING=value'.

-include .make_options

MAKEFLAGS += --no-builtin-rules

#### Defaults ####

# If COMPARE is 1, check the output md5sum after building
COMPARE ?= 1
# If NON_MATCHING is 1, define the NON_MATCHING C flag when building
NON_MATCHING ?= 0
# If ORIG_COMPILER is 1, compile with QEMU_IRIX and the original compiler
ORIG_COMPILER ?= 0
# if WERROR is 1, pass -Werror to CC_CHECK, so warnings would be treated as errors
WERROR ?= 0
# Keep .mdebug section in build
KEEP_MDEBUG ?= 0
# Disassembles all asm from the ROM instead of skipping files which are entirely in C
FULL_DISASM ?= 0
# Check code syntax with host compiler
RUN_CC_CHECK ?= 1
# Dump build object files
OBJDUMP_BUILD ?= 0
# Number of threads to disassmble, extract, and compress with
N_THREADS ?= $(shell nproc)

#### Setup ####

ifeq ($(NON_MATCHING),1)
  CFLAGS := -DNON_MATCHING
  CPPFLAGS := -DNON_MATCHING
  COMPARE := 0
endif

DISASM_FLAGS := --reg-names=o32
ifneq ($(FULL_DISASM), 0)
  DISASM_FLAGS += --all
endif

PROJECT_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

MAKE = make
CPPFLAGS += -P

ifeq ($(OS),Windows_NT)
  DETECTED_OS=windows
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    DETECTED_OS=linux
  endif
  ifeq ($(UNAME_S),Darwin)
    DETECTED_OS=macos
    MAKE=gmake
    CPPFLAGS += -xc++
  endif
endif

#### Tools ####

ifeq ($(shell type mips-linux-gnu-ld >/dev/null 2>/dev/null; echo $$?), 0)
  MIPS_BINUTILS_PREFIX := mips-linux-gnu-
else
  MIPS_BINUTILS_PREFIX := mips64-elf-
endif

CC       := tools/ido_recomp/$(DETECTED_OS)/7.1/cc
CC_OLD   := tools/ido_recomp/$(DETECTED_OS)/5.3/cc
GCC      := gcc
QEMU_IRIX ?= ./tools/qemu-mips

# if ORIG_COMPILER is 1, check that either QEMU_IRIX is set or qemu-irix package installed
ifeq ($(ORIG_COMPILER),1)
  ifndef QEMU_IRIX
    QEMU_IRIX := $(shell which qemu-irix)
    ifeq (, $(QEMU_IRIX))
      $(error Please install qemu-irix package or set QEMU_IRIX env var to the full qemu-irix binary path)
    endif
  endif
  CC        = $(QEMU_IRIX) -L tools/ido7.1_compiler tools/ido7.1_compiler/usr/bin/cc
  CC_OLD    = $(QEMU_IRIX) -L tools/ido5.3_compiler tools/ido5.3_compiler/usr/bin/cc
endif

AS         := $(MIPS_BINUTILS_PREFIX)as
LD         := $(MIPS_BINUTILS_PREFIX)ld
OBJCOPY    := $(MIPS_BINUTILS_PREFIX)objcopy
OBJDUMP    := $(MIPS_BINUTILS_PREFIX)objdump
ASM_PROC   := python3 tools/asm-processor/build.py

ASM_PROC_FLAGS := --input-enc=utf-8 --output-enc=euc-jp

IINC       := -Iinclude -Isrc -Iassets -Ibuild -I.

ifeq ($(KEEP_MDEBUG),0)
  RM_MDEBUG = $(OBJCOPY) --remove-section .mdebug $@
else
  RM_MDEBUG = @:
endif

# Check code syntax with host compiler
ifneq ($(RUN_CC_CHECK),0)
  CHECK_WARNINGS := -Wall -Wextra -Wno-format-security -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-variable -Wno-missing-braces -Wno-int-conversion -Wno-unused-but-set-variable -Wno-unused-label
  CC_CHECK   := gcc -fno-builtin -fsyntax-only -funsigned-char -fdiagnostics-color -std=gnu89 -D _LANGUAGE_C -D NON_MATCHING $(IINC) -nostdinc $(CHECK_WARNINGS)
  ifneq ($(WERROR), 0)
    CC_CHECK += -Werror
  endif
else
  CC_CHECK := @:
endif

ZAPD := tools/ZAPD/ZAPD.out

OPTFLAGS := -O2 -g3
ASFLAGS := -march=vr4300 -32
MIPS_VERSION := -mips2

# we support Microsoft extensions such as anonymous structs, which the compiler does support but warns for their usage. Surpress the warnings with -woff.
CFLAGS += -G 0 -non_shared -fullwarn -verbose -Xcpluscomm $(IINC) -nostdinc -Wab,-r4300_mul -woff 624,649,838,712,516

# Use relocations and abi fpr names in the dump
OBJDUMP_FLAGS := --disassemble --reloc --disassemble-zeroes -Mreg-names=32

ifneq ($(OBJDUMP_BUILD), 0)
  OBJDUMP_CMD = $(OBJDUMP) $(OBJDUMP_FLAGS) $@ > $(@:.o=.s)
  OBJCOPY_BIN = $(OBJCOPY) -O binary $@ $@.bin
else
  OBJDUMP_CMD = @:
  OBJCOPY_BIN = @:
endif

ifeq ($(shell getconf LONG_BIT), 32)
  # Work around memory allocation bug in QEMU
  export QEMU_GUEST_BASE := 1
else ifneq ($(RUN_CC_CHECK),0)
  # Ensure that gcc treats the code as 32-bit
  CC_CHECK += -m32
endif

# rom compression flags
COMPFLAGS := --threads $(N_THREADS)
ifneq ($(NON_MATCHING),1)
  COMPFLAGS += --matching
endif

#### Files ####

# ROM image
MM_BASEROM ?= baserom.mm.us.rev1.z64
MM_ROM_NAME ?= mm.us.rev1.rom
ROM := $(MM_ROM_NAME).z64
UNCOMPRESSED_ROM := $(MM_ROM_NAME)_uncompressed.z64
ELF := $(MM_ROM_NAME).elf

SRC_DIRS := $(shell find src -type d)
ASM_DIRS := $(shell find asm -type d -not -path "asm/non_matchings*") $(shell find data -type d)

## Assets binaries (PNGs, JPGs, etc)
ASSET_BIN_DIRS := $(shell find assets/* -type d -not -path "assets/xml*")
# Prevents building C files that will be #include'd
ASSET_BIN_DIRS_C_FILES := $(shell find assets/* -type d -not -path "assets/xml*" -not -path "assets/code*" -not -path "assets/overlays*")

ASSET_FILES_XML := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.xml))
ASSET_FILES_BIN := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.bin))
ASSET_FILES_OUT := $(foreach f,$(ASSET_FILES_XML:.xml=.c),$f) \
				   $(foreach f,$(ASSET_FILES_BIN:.bin=.bin.inc.c),build/$f)

TEXTURE_FILES_PNG := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.png))
TEXTURE_FILES_JPG := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.jpg))
TEXTURE_FILES_OUT := $(foreach f,$(TEXTURE_FILES_PNG:.png=.inc.c),build/$f) \
					 $(foreach f,$(TEXTURE_FILES_JPG:.jpg=.jpg.inc.c),build/$f) \

C_FILES       := $(foreach dir,$(SRC_DIRS) $(ASSET_BIN_DIRS_C_FILES),$(wildcard $(dir)/*.c))
S_FILES       := $(shell grep -F "build/asm" spec | sed 's/.*build\/// ; s/\.o\".*/.s/') \
                 $(shell grep -F "build/data" spec | sed 's/.*build\/// ; s/\.o\".*/.s/')
BASEROM_FILES := $(shell grep -F "build/baserom" spec | sed 's/.*build\/// ; s/\.o\".*//')
O_FILES       := $(foreach f,$(S_FILES:.s=.o),build/$f) \
                 $(foreach f,$(C_FILES:.c=.o),build/$f) \
                 $(foreach f,$(BASEROM_FILES),build/$f.o)

# Automatic dependency files
# (Only asm_processor dependencies are handled for now)
DEP_FILES := $(O_FILES:.o=.asmproc.d)

# create build directories
$(shell mkdir -p build/linker_scripts build/asm build/asm/boot build/asm/code build/asm/overlays $(foreach dir, $(COMP_DIRS) $(BINARY_DIRS) $(SRC_DIRS) $(ASSET_BIN_DIRS),$(shell mkdir -p build/$(dir))))

# This file defines `ROM_FILES`, `UNCOMPRESSED_ROM_FILES`, and rules for generating `.yaz0` files
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), distclean)
$(shell tools/dmadata_dependencies.py \
	--dmadata-table=tables/dmadata_table.txt \
	--output-deps=build/rom_dependencies.d)
-include build/rom_dependencies.d
endif
endif

build/src/libultra/os/%: OPTFLAGS := -O1
build/src/libultra/voice/%: OPTFLAGS := -O2
build/src/libultra/io/%: OPTFLAGS := -O2
build/src/libultra/libc/%: OPTFLAGS := -O2
build/src/libultra/libc/ll%: OPTFLAGS := -O1
build/src/libultra/libc/ll%: MIPS_VERSION := -mips3 -32
build/src/libultra/gu/%: OPTFLAGS := -O2
build/src/libultra/rmon/%: OPTFLAGS := -O2
build/src/boot_O1/%: OPTFLAGS := -O1
build/src/boot_O2/%: OPTFLAGS := -O2
build/src/boot_O2_g3/%: OPTFLAGS := -O2 -g3
build/src/boot_O2_g3_trapuv/%: OPTFLAGS := -O2 -g3
build/src/boot_O2_g3_trapuv/%: CFLAGS := $(CFLAGS) -trapuv

build/src/libultra/%: CC := $(CC_OLD)
build/src/libultra/io/%: CC := ./tools/preprocess.py $(CC_OLD) -- $(AS) $(ASFLAGS) --
build/src/libultra/voice/%: CC := ./tools/preprocess.py $(CC_OLD) -- $(AS) $(ASFLAGS) --

CC := ./tools/preprocess.py $(CC) -- $(AS) $(ASFLAGS) --

.PHONY: all clean setup diff-init init assetclean distclean assembly
# make will delete any generated assembly files that are not a prerequisite for anything, so keep it from doing so
.PRECIOUS: asm/%.asm
.DEFAULT_GOAL := $(UNCOMPRESSED_ROM)

# just using build/baserom still probably has some race condiction/dependency bug, but since
# it is first and should be completed relatively fast, it should not occur all that often.
$(UNCOMPRESSED_ROM): build/baserom $(TEXTURE_FILES_OUT) $(ASSET_FILES_OUT) $(UNCOMPRESSED_ROM_FILES)
	./tools/makerom.py ./tables/dmadata_table.txt $@
ifeq ($(COMPARE),1)
	@md5sum $(UNCOMPRESSED_ROM)
	@md5sum -c checksum_uncompressed.md5
endif

$(ROM): build/baserom $(ROM_FILES)
	./tools/makerom.py ./tables/dmadata_table.txt $@ -c
ifeq ($(COMPARE),1)
	@md5sum $(ROM)
	@md5sum -c checksum.md5
endif

build/assets/%.o: OPTFLAGS := -O1
build/assets/%.o: ASM_PROC_FLAGS := 

# file flags
build/src/boot_O2_g3/fault.o: CFLAGS += -trapuv
build/src/boot_O2_g3/fault_drawer.o: CFLAGS += -trapuv

build/code.elf: $(O_FILES) build/linker_scripts/code_script.ld undef.txt build/linker_scripts/object_script.ld build/dmadata_script.ld
	$(LD) -T build/linker_scripts/code_script.ld -T undef.txt -T build/linker_scripts/object_script.ld -T build/dmadata_script.ld --no-check-sections --accept-unknown-input-arch -Map build/mm.map -N -o $@

build/code_pre_dmadata.elf: $(O_FILES) build/linker_scripts/code_script.ld undef.txt build/linker_scripts/object_script.ld
	$(LD) -r -T build/linker_scripts/code_script.ld -T undef.txt -T build/linker_scripts/object_script.ld --no-check-sections --accept-unknown-input-arch -N -o $@

# cc & asm-processor
build/src/boot_O2/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --
build/src/boot_O2_g3/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --

build/src/libultra/%.o: CC := $(CC_OLD)
# Needed at least until voice is decompiled
build/src/libultra/voice/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC_OLD) -- $(AS) $(ASFLAGS) --

build/src/code/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --
build/src/code/audio/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --

build/src/overlays/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --

build/assets/%.o: CC := $(ASM_PROC) $(ASM_PROC_FLAGS) $(CC) -- $(AS) $(ASFLAGS) --

build/binary/assets/scenes/%: build/code.elf
	@$(OBJCOPY) --dump-section $*=$@ $< /dev/null

build/binary/overlays/%: build/code.elf
	@$(OBJCOPY) --dump-section $*=$@ $< /dev/null


.PHONY: all uncompressed compressed clean assetclean distclean assets disasm init setup
.DEFAULT_GOAL := uncompressed
all: uncompressed compressed

# Use an empty sentinel file (dep) to track the directory as a dependency, and
# emulate GNU Make's order-only dependency.
# The `touch $@; action || rm $@` pattern ensures that the `dep` file is older
# than the output files from `action`, and only exists if `action` succeeds.
asm/non_matchings/%/dep: asm/%.asm
	@mkdir -p $(dir $@)
	@touch $@
	./tools/split_asm.py $< asm/non_matchings/$* || rm $@

$(ROMC): $(ROM)
	python3 tools/z64compress_wrapper.py $(COMPFLAGS) $(ROM) $@ $(ELF) build/$(SPEC)

$(ELF): $(TEXTURE_FILES_OUT) $(ASSET_FILES_OUT) $(O_FILES) build/ldscript.txt build/undefined_syms.txt
	$(LD) -T build/undefined_syms.txt -T build/ldscript.txt --no-check-sections --accept-unknown-input-arch --emit-relocs -Map build/mm.map -o $@


#### Main commands ####

## Cleaning ##
clean:
	$(RM) -rf $(ROM) $(UNCOMPRESSED_ROM) build

assetclean:
	$(RM) -rf $(ASSET_BIN_DIRS)
	$(RM) -rf build/assets

distclean: assetclean clean
	$(RM) -rf baserom/ asm/ expected/
	$(MAKE) -C tools clean

## Extraction step
setup:
	$(MAKE) -C tools
	python3 tools/fixbaserom.py
	python3 tools/extract_baserom.py

assets:
	python3 extract_assets.py -j $(N_THREADS)

## Assembly generation
disasm:
	$(RM) -rf asm data
	python3 tools/disasm/disasm.py -j $(N_THREADS) $(DISASM_FLAGS)

diff-init: all
	$(RM) -rf expected/
	mkdir -p expected/
	cp -r build expected/build
	cp $(UNCOMPRESSED_ROM) expected/$(UNCOMPRESSED_ROM)
	cp $(ROM) expected/$(ROM)

init:
	$(MAKE) distclean
	$(MAKE) setup
	$(MAKE) assets
	$(MAKE) disasm
	$(MAKE) all
	$(MAKE) diff-init

#### Various Recipes ####

build/undefined_syms.txt: undefined_syms.txt
	$(CPP) $(CPPFLAGS) $< > build/undefined_syms.txt

build/baserom:
	cp -r baserom/ build/baserom/

build/asm/%.o: asm/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/assets/%.o: assets/%.c
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(OBJCOPY_BIN)
	$(RM_MDEBUG)

build/baserom/%.o: baserom/%
	$(OBJCOPY) -I binary -O elf32-big $< $@

build/data/%.o: data/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/src/overlays/%.o: src/overlays/%.c
	$(CC_CHECK) $<
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(OBJDUMP_CMD)
# TODO: `() || true` is currently necessary to suppress `Error 1 (ignored)` make warnings caused by `test`, but this will go away if 
# 	the following is moved to a separate rule that is only run once when all the required objects have been compiled. 
	$(ZAPD) bovl -eh -i $@ -cfg $< --outputpath $(@D)/$(notdir $(@D))_reloc.s
	(test -f $(@D)/$(notdir $(@D))_reloc.s && $(AS) $(ASFLAGS) $(@D)/$(notdir $(@D))_reloc.s -o $(@D)/$(notdir $(@D))_reloc.o) || true
	$(RM_MDEBUG)

build/src/%.o: src/%.c
	$(CC_CHECK) $<
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(OBJDUMP_CMD)
	$(RM_MDEBUG)

build/src/libultra/libc/ll.o: src/libultra/libc/ll.c
	$(CC_CHECK) $<
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	python3 tools/set_o32abi_bit.py $@
	$(OBJDUMP_CMD)
	$(RM_MDEBUG)

build/src/libultra/libc/llcvt.o: src/libultra/libc/llcvt.c
	$(CC_CHECK) $<
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	python3 tools/set_o32abi_bit.py $@
	$(OBJDUMP_CMD)
	$(RM_MDEBUG)

# Build C files from assets

build/%.inc.c: %.png
	$(ZAPD) btex -eh -tt $(subst .,,$(suffix $*)) -i $< -o $@

build/assets/%.bin.inc.c: assets/%.bin
	$(ZAPD) bblb -eh -i $< -o $@

build/assets/%.jpg.inc.c: assets/%.jpg
	$(ZAPD) bren -eh -i $< -o $@

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), distclean)
-include $(C_FILES:%.c=build/%.d)
endif
endif
