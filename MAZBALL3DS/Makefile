TARGET      := MAZBALL3DS

BUILD       := build
SOURCES     := source
DATA        := data
INCLUDES    := include
GRAPHICS    := gfx

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS := -g -Wall -O2 -mword-relocations \
          -ffunction-sections \
          $(ARCH)

CFLAGS += -D__3DS__

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17 \
            -IC:/devkitPro/libctru/include

ASFLAGS := -g $(ARCH)

LDFLAGS := -specs=3dsx.specs -g $(ARCH)

LIBS := -lcitro3d -lctru -lm
LIBPATHS := -LC:/devkitPro/libctru/lib

LD := arm-none-eabi-g++

OFILES := $(BUILD)/source/main.o $(BUILD)/triangle.shbin.o

.PHONY: all clean

all: $(TARGET).3dsx $(SHADER)

include $(DEVKITARM)/3ds_rules

$(SHADER): $(GRAPHICS)/triangle.v.pica
	@mkdir -p $(BUILD)
	$(PICASSO) -o $@ $<

$(TARGET).elf: $(OFILES)

$(BUILD)/source/main.o: $(SOURCES)/main.cpp
	@mkdir -p $(BUILD)/source
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
	rm -f $(TARGET).3dsx
	rm -f $(TARGET).smdh
	rm -f $(TARGET).elf
	rm -f $(TARGET).map