#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

HIDE := @

LOCAL_PATH := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ifeq (true, $(APP_DEBUG))
	BIN_DIR := $(LOCAL_PATH)/bin/debug
	OBJ_DIR := $(LOCAL_PATH)/obj/debug
else
	BIN_DIR := $(LOCAL_PATH)/bin/release
	OBJ_DIR := $(LOCAL_PATH)/obj/release
endif
SOURCE_DIR := $(LOCAL_PATH)/../src
THIRD_PARTY_DIR := $(LOCAL_PATH)/../thirdparty

CC := clang
AR := ar

C_FLAGS := 
C_FLAGS += -Wall 
C_FLAGS += -Werror=return-type
C_FLAGS += -fPIC
C_FLAGS += -pthread
ifeq (true, $(APP_DEBUG))
	C_FLAGS += -g -O0 -UNDEBUG
else
	C_FLAGS += -O2 -DNDEBUG
endif
C_FLAGS += -fvisibility=hidden
C_FLAGS += -DFT2_BUILD_LIBRARY=1
C_FLAGS += -DFT_EXPORT\(x\)=x
C_FLAGS += -I$(LOCAL_PATH)/../include

AR_FLAGS := 
AR_FLAGS += crsD

all :  \
	$(BIN_DIR)/libfreetype.a

# Archive
$(BIN_DIR)/libfreetype.a: \
	$(OBJ_DIR)/freetype-autofit-autofit.o \
	$(OBJ_DIR)/freetype-base-ftbase.o \
	$(OBJ_DIR)/freetype-base-ftbbox.o \
	$(OBJ_DIR)/freetype-base-ftbdf.o \
	$(OBJ_DIR)/freetype-base-ftbitmap.o \
	$(OBJ_DIR)/freetype-base-ftcid.o \
	$(OBJ_DIR)/freetype-base-ftfstype.o \
	$(OBJ_DIR)/freetype-base-ftgasp.o \
	$(OBJ_DIR)/freetype-base-ftglyph.o \
	$(OBJ_DIR)/freetype-base-ftgxval.o \
	$(OBJ_DIR)/freetype-base-ftinit.o \
	$(OBJ_DIR)/freetype-base-ftmm.o \
	$(OBJ_DIR)/freetype-base-ftotval.o \
	$(OBJ_DIR)/freetype-base-ftpatent.o \
	$(OBJ_DIR)/freetype-base-ftpfr.o \
	$(OBJ_DIR)/freetype-base-ftstroke.o \
	$(OBJ_DIR)/freetype-base-ftsynth.o \
	$(OBJ_DIR)/freetype-base-ftsystem.o \
	$(OBJ_DIR)/freetype-base-fttype1.o \
	$(OBJ_DIR)/freetype-base-ftwinfnt.o \
	$(OBJ_DIR)/freetype-bdf-bdf.o \
	$(OBJ_DIR)/freetype-bzip2-ftbzip2.o \
	$(OBJ_DIR)/freetype-cache-ftcache.o \
	$(OBJ_DIR)/freetype-cff-cff.o \
	$(OBJ_DIR)/freetype-cid-type1cid.o \
	$(OBJ_DIR)/freetype-gzip-ftgzip.o \
	$(OBJ_DIR)/freetype-lzw-ftlzw.o \
	$(OBJ_DIR)/freetype-pcf-pcf.o \
	$(OBJ_DIR)/freetype-pfr-pfr.o \
	$(OBJ_DIR)/freetype-psaux-psaux.o \
	$(OBJ_DIR)/freetype-pshinter-pshinter.o \
	$(OBJ_DIR)/freetype-psnames-psnames.o \
	$(OBJ_DIR)/freetype-raster-raster.o \
	$(OBJ_DIR)/freetype-sfnt-sfnt.o \
	$(OBJ_DIR)/freetype-smooth-smooth.o \
	$(OBJ_DIR)/freetype-truetype-truetype.o \
	$(OBJ_DIR)/freetype-type1-type1.o \
	$(OBJ_DIR)/freetype-type42-type42.o \
	$(OBJ_DIR)/freetype-winfonts-winfnt.o
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) $(AR) $(AR_FLAGS) \
		$(BIN_DIR)/libfreetype.a \
		$(OBJ_DIR)/freetype-autofit-autofit.o \
		$(OBJ_DIR)/freetype-base-ftbase.o \
		$(OBJ_DIR)/freetype-base-ftbbox.o \
		$(OBJ_DIR)/freetype-base-ftbdf.o \
		$(OBJ_DIR)/freetype-base-ftbitmap.o \
		$(OBJ_DIR)/freetype-base-ftcid.o \
		$(OBJ_DIR)/freetype-base-ftfstype.o \
		$(OBJ_DIR)/freetype-base-ftgasp.o \
		$(OBJ_DIR)/freetype-base-ftglyph.o \
		$(OBJ_DIR)/freetype-base-ftgxval.o \
		$(OBJ_DIR)/freetype-base-ftinit.o \
		$(OBJ_DIR)/freetype-base-ftmm.o \
		$(OBJ_DIR)/freetype-base-ftotval.o \
		$(OBJ_DIR)/freetype-base-ftpatent.o \
		$(OBJ_DIR)/freetype-base-ftpfr.o \
		$(OBJ_DIR)/freetype-base-ftstroke.o \
		$(OBJ_DIR)/freetype-base-ftsynth.o \
		$(OBJ_DIR)/freetype-base-ftsystem.o \
		$(OBJ_DIR)/freetype-base-fttype1.o \
		$(OBJ_DIR)/freetype-base-ftwinfnt.o \
		$(OBJ_DIR)/freetype-bdf-bdf.o \
		$(OBJ_DIR)/freetype-bzip2-ftbzip2.o \
		$(OBJ_DIR)/freetype-cache-ftcache.o \
		$(OBJ_DIR)/freetype-cff-cff.o \
		$(OBJ_DIR)/freetype-cid-type1cid.o \
		$(OBJ_DIR)/freetype-gzip-ftgzip.o \
		$(OBJ_DIR)/freetype-lzw-ftlzw.o \
		$(OBJ_DIR)/freetype-pcf-pcf.o \
		$(OBJ_DIR)/freetype-pfr-pfr.o \
		$(OBJ_DIR)/freetype-psaux-psaux.o \
		$(OBJ_DIR)/freetype-pshinter-pshinter.o \
		$(OBJ_DIR)/freetype-psnames-psnames.o \
		$(OBJ_DIR)/freetype-raster-raster.o \
		$(OBJ_DIR)/freetype-sfnt-sfnt.o \
		$(OBJ_DIR)/freetype-smooth-smooth.o \
		$(OBJ_DIR)/freetype-truetype-truetype.o \
		$(OBJ_DIR)/freetype-type1-type1.o \
		$(OBJ_DIR)/freetype-type42-type42.o \
		$(OBJ_DIR)/freetype-winfonts-winfnt.o

# Compile
$(OBJ_DIR)/freetype-autofit-autofit.o: $(SOURCE_DIR)/autofit/autofit.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/autofit/autofit.c -MD -MF $(OBJ_DIR)/freetype-autofit-autofit.d -o $(OBJ_DIR)/freetype-autofit-autofit.o

$(OBJ_DIR)/freetype-base-ftbase.o: $(SOURCE_DIR)/base/ftbase.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftbase.c -MD -MF $(OBJ_DIR)/freetype-base-ftbase.d -o $(OBJ_DIR)/freetype-base-ftbase.o

$(OBJ_DIR)/freetype-base-ftbbox.o: $(SOURCE_DIR)/base/ftbbox.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftbbox.c -MD -MF $(OBJ_DIR)/freetype-base-ftbbox.d -o $(OBJ_DIR)/freetype-base-ftbbox.o

$(OBJ_DIR)/freetype-base-ftbdf.o: $(SOURCE_DIR)/base/ftbdf.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftbdf.c -MD -MF $(OBJ_DIR)/freetype-base-ftbdf.d -o $(OBJ_DIR)/freetype-base-ftbdf.o

$(OBJ_DIR)/freetype-base-ftbitmap.o: $(SOURCE_DIR)/base/ftbitmap.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftbitmap.c -MD -MF $(OBJ_DIR)/freetype-base-ftbitmap.d -o $(OBJ_DIR)/freetype-base-ftbitmap.o

$(OBJ_DIR)/freetype-base-ftcid.o: $(SOURCE_DIR)/base/ftcid.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftcid.c -MD -MF $(OBJ_DIR)/freetype-base-ftcid.d -o $(OBJ_DIR)/freetype-base-ftcid.o

$(OBJ_DIR)/freetype-base-ftfstype.o: $(SOURCE_DIR)/base/ftfstype.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftfstype.c -MD -MF $(OBJ_DIR)/freetype-base-ftfstype.d -o $(OBJ_DIR)/freetype-base-ftfstype.o

$(OBJ_DIR)/freetype-base-ftgasp.o: $(SOURCE_DIR)/base/ftgasp.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftgasp.c -MD -MF $(OBJ_DIR)/freetype-base-ftgasp.d -o $(OBJ_DIR)/freetype-base-ftgasp.o

$(OBJ_DIR)/freetype-base-ftglyph.o: $(SOURCE_DIR)/base/ftglyph.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftglyph.c -MD -MF $(OBJ_DIR)/freetype-base-ftglyph.d -o $(OBJ_DIR)/freetype-base-ftglyph.o

$(OBJ_DIR)/freetype-base-ftgxval.o: $(SOURCE_DIR)/base/ftgxval.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftgxval.c -MD -MF $(OBJ_DIR)/freetype-base-ftgxval.d -o $(OBJ_DIR)/freetype-base-ftgxval.o

$(OBJ_DIR)/freetype-base-ftinit.o: $(SOURCE_DIR)/base/ftinit.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftinit.c -MD -MF $(OBJ_DIR)/freetype-base-ftinit.d -o $(OBJ_DIR)/freetype-base-ftinit.o

$(OBJ_DIR)/freetype-base-ftmm.o: $(SOURCE_DIR)/base/ftmm.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftmm.c -MD -MF $(OBJ_DIR)/freetype-base-ftmm.d -o $(OBJ_DIR)/freetype-base-ftmm.o

$(OBJ_DIR)/freetype-base-ftotval.o: $(SOURCE_DIR)/base/ftotval.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftotval.c -MD -MF $(OBJ_DIR)/freetype-base-ftotval.d -o $(OBJ_DIR)/freetype-base-ftotval.o

$(OBJ_DIR)/freetype-base-ftpatent.o: $(SOURCE_DIR)/base/ftpatent.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftpatent.c -MD -MF $(OBJ_DIR)/freetype-base-ftpatent.d -o $(OBJ_DIR)/freetype-base-ftpatent.o

$(OBJ_DIR)/freetype-base-ftpfr.o: $(SOURCE_DIR)/base/ftpfr.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftpfr.c -MD -MF $(OBJ_DIR)/freetype-base-ftpfr.d -o $(OBJ_DIR)/freetype-base-ftpfr.o

$(OBJ_DIR)/freetype-base-ftstroke.o: $(SOURCE_DIR)/base/ftstroke.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftstroke.c -MD -MF $(OBJ_DIR)/freetype-base-ftstroke.d -o $(OBJ_DIR)/freetype-base-ftstroke.o

$(OBJ_DIR)/freetype-base-ftsynth.o: $(SOURCE_DIR)/base/ftsynth.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftsynth.c -MD -MF $(OBJ_DIR)/freetype-base-ftsynth.d -o $(OBJ_DIR)/freetype-base-ftsynth.o

$(OBJ_DIR)/freetype-base-ftsystem.o: $(SOURCE_DIR)/base/ftsystem.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftsystem.c -MD -MF $(OBJ_DIR)/freetype-base-ftsystem.d -o $(OBJ_DIR)/freetype-base-ftsystem.o

$(OBJ_DIR)/freetype-base-fttype1.o: $(SOURCE_DIR)/base/fttype1.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/fttype1.c -MD -MF $(OBJ_DIR)/freetype-base-fttype1.d -o $(OBJ_DIR)/freetype-base-fttype1.o

$(OBJ_DIR)/freetype-base-ftwinfnt.o: $(SOURCE_DIR)/base/ftwinfnt.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/base/ftwinfnt.c -MD -MF $(OBJ_DIR)/freetype-base-ftwinfnt.d -o $(OBJ_DIR)/freetype-base-ftwinfnt.o

$(OBJ_DIR)/freetype-bdf-bdf.o: $(SOURCE_DIR)/bdf/bdf.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/bdf/bdf.c -MD -MF $(OBJ_DIR)/freetype-bdf-bdf.d -o $(OBJ_DIR)/freetype-bdf-bdf.o

$(OBJ_DIR)/freetype-bzip2-ftbzip2.o: $(SOURCE_DIR)/bzip2/ftbzip2.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/bzip2/ftbzip2.c -MD -MF $(OBJ_DIR)/freetype-bzip2-ftbzip2.d -o $(OBJ_DIR)/freetype-bzip2-ftbzip2.o

$(OBJ_DIR)/freetype-cache-ftcache.o: $(SOURCE_DIR)/cache/ftcache.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/cache/ftcache.c -MD -MF $(OBJ_DIR)/freetype-cache-ftcache.d -o $(OBJ_DIR)/freetype-cache-ftcache.o

$(OBJ_DIR)/freetype-cff-cff.o: $(SOURCE_DIR)/cff/cff.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/cff/cff.c -MD -MF $(OBJ_DIR)/freetype-cff-cff.d -o $(OBJ_DIR)/freetype-cff-cff.o

$(OBJ_DIR)/freetype-cid-type1cid.o: $(SOURCE_DIR)/cid/type1cid.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/cid/type1cid.c -MD -MF $(OBJ_DIR)/freetype-cid-type1cid.d -o $(OBJ_DIR)/freetype-cid-type1cid.o

$(OBJ_DIR)/freetype-gzip-ftgzip.o: $(SOURCE_DIR)/gzip/ftgzip.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/gzip/ftgzip.c -MD -MF $(OBJ_DIR)/freetype-gzip-ftgzip.d -o $(OBJ_DIR)/freetype-gzip-ftgzip.o

$(OBJ_DIR)/freetype-lzw-ftlzw.o: $(SOURCE_DIR)/lzw/ftlzw.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/lzw/ftlzw.c -MD -MF $(OBJ_DIR)/freetype-lzw-ftlzw.d -o $(OBJ_DIR)/freetype-lzw-ftlzw.o

$(OBJ_DIR)/freetype-pcf-pcf.o: $(SOURCE_DIR)/pcf/pcf.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/pcf/pcf.c -MD -MF $(OBJ_DIR)/freetype-pcf-pcf.d -o $(OBJ_DIR)/freetype-pcf-pcf.o

$(OBJ_DIR)/freetype-pfr-pfr.o: $(SOURCE_DIR)/pfr/pfr.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/pfr/pfr.c -MD -MF $(OBJ_DIR)/freetype-pfr-pfr.d -o $(OBJ_DIR)/freetype-pfr-pfr.o

$(OBJ_DIR)/freetype-psaux-psaux.o: $(SOURCE_DIR)/psaux/psaux.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/psaux/psaux.c -MD -MF $(OBJ_DIR)/freetype-psaux-psaux.d -o $(OBJ_DIR)/freetype-psaux-psaux.o

$(OBJ_DIR)/freetype-pshinter-pshinter.o: $(SOURCE_DIR)/pshinter/pshinter.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/pshinter/pshinter.c -MD -MF $(OBJ_DIR)/freetype-pshinter-pshinter.d -o $(OBJ_DIR)/freetype-pshinter-pshinter.o

$(OBJ_DIR)/freetype-psnames-psnames.o: $(SOURCE_DIR)/psnames/psnames.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/psnames/psnames.c -MD -MF $(OBJ_DIR)/freetype-psnames-psnames.d -o $(OBJ_DIR)/freetype-psnames-psnames.o

$(OBJ_DIR)/freetype-raster-raster.o: $(SOURCE_DIR)/raster/raster.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/raster/raster.c -MD -MF $(OBJ_DIR)/freetype-raster-raster.d -o $(OBJ_DIR)/freetype-raster-raster.o

$(OBJ_DIR)/freetype-sfnt-sfnt.o: $(SOURCE_DIR)/sfnt/sfnt.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/sfnt/sfnt.c -MD -MF $(OBJ_DIR)/freetype-sfnt-sfnt.d -o $(OBJ_DIR)/freetype-sfnt-sfnt.o

$(OBJ_DIR)/freetype-smooth-smooth.o: $(SOURCE_DIR)/smooth/smooth.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/smooth/smooth.c -MD -MF $(OBJ_DIR)/freetype-smooth-smooth.d -o $(OBJ_DIR)/freetype-smooth-smooth.o

$(OBJ_DIR)/freetype-truetype-truetype.o: $(SOURCE_DIR)/truetype/truetype.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/truetype/truetype.c -MD -MF $(OBJ_DIR)/freetype-truetype-truetype.d -o $(OBJ_DIR)/freetype-truetype-truetype.o

$(OBJ_DIR)/freetype-type1-type1.o: $(SOURCE_DIR)/type1/type1.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/type1/type1.c -MD -MF $(OBJ_DIR)/freetype-type1-type1.d -o $(OBJ_DIR)/freetype-type1-type1.o

$(OBJ_DIR)/freetype-type42-type42.o: $(SOURCE_DIR)/type42/type42.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/type42/type42.c -MD -MF $(OBJ_DIR)/freetype-type42-type42.d -o $(OBJ_DIR)/freetype-type42-type42.o

$(OBJ_DIR)/freetype-winfonts-winfnt.o: $(SOURCE_DIR)/winfonts/winfnt.c
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/winfonts/winfnt.c -MD -MF $(OBJ_DIR)/freetype-winfonts-winfnt.d -o $(OBJ_DIR)/freetype-winfonts-winfnt.o

-include \
	$(OBJ_DIR)/freetype-autofit-autofit.d \
	$(OBJ_DIR)/freetype-base-ftbase.d \
	$(OBJ_DIR)/freetype-base-ftbbox.d \
	$(OBJ_DIR)/freetype-base-ftbdf.d \
	$(OBJ_DIR)/freetype-base-ftbitmap.d \
	$(OBJ_DIR)/freetype-base-ftcid.d \
	$(OBJ_DIR)/freetype-base-ftfstype.d \
	$(OBJ_DIR)/freetype-base-ftgasp.d \
	$(OBJ_DIR)/freetype-base-ftglyph.d \
	$(OBJ_DIR)/freetype-base-ftgxval.d \
	$(OBJ_DIR)/freetype-base-ftinit.d \
	$(OBJ_DIR)/freetype-base-ftmm.d \
	$(OBJ_DIR)/freetype-base-ftotval.d \
	$(OBJ_DIR)/freetype-base-ftpatent.d \
	$(OBJ_DIR)/freetype-base-ftpfr.d \
	$(OBJ_DIR)/freetype-base-ftstroke.d \
	$(OBJ_DIR)/freetype-base-ftsynth.d \
	$(OBJ_DIR)/freetype-base-ftsystem.d \
	$(OBJ_DIR)/freetype-base-fttype1.d \
	$(OBJ_DIR)/freetype-base-ftwinfnt.d \
	$(OBJ_DIR)/freetype-bdf-bdf.d \
	$(OBJ_DIR)/freetype-bzip2-ftbzip2.d \
	$(OBJ_DIR)/freetype-cache-ftcache.d \
	$(OBJ_DIR)/freetype-cff-cff.d \
	$(OBJ_DIR)/freetype-cid-type1cid.d \
	$(OBJ_DIR)/freetype-gzip-ftgzip.d \
	$(OBJ_DIR)/freetype-lzw-ftlzw.d \
	$(OBJ_DIR)/freetype-pcf-pcf.d \
	$(OBJ_DIR)/freetype-pfr-pfr.d \
	$(OBJ_DIR)/freetype-psaux-psaux.d \
	$(OBJ_DIR)/freetype-pshinter-pshinter.d \
	$(OBJ_DIR)/freetype-psnames-psnames.d \
	$(OBJ_DIR)/freetype-raster-raster.d \
	$(OBJ_DIR)/freetype-sfnt-sfnt.d \
	$(OBJ_DIR)/freetype-smooth-smooth.d \
	$(OBJ_DIR)/freetype-truetype-truetype.d \
	$(OBJ_DIR)/freetype-type1-type1.d \
	$(OBJ_DIR)/freetype-type42-type42.d \
	$(OBJ_DIR)/freetype-winfonts-winfnt.d

clean:
	$(HIDE) rm -f $(BIN_DIR)/libfreetype.a
	$(HIDE) rm -f $(OBJ_DIR)/freetype-autofit-autofit.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbase.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbbox.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbdf.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbitmap.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftcid.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftfstype.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftgasp.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftglyph.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftgxval.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftinit.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftmm.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftotval.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftpatent.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftpfr.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftstroke.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftsynth.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftsystem.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-fttype1.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftwinfnt.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-bdf-bdf.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-bzip2-ftbzip2.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cache-ftcache.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cff-cff.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cid-type1cid.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-gzip-ftgzip.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-lzw-ftlzw.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pcf-pcf.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pfr-pfr.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-psaux-psaux.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pshinter-pshinter.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-psnames-psnames.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-raster-raster.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-sfnt-sfnt.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-smooth-smooth.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-truetype-truetype.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-type1-type1.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-type42-type42.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-winfonts-winfnt.o
	$(HIDE) rm -f $(OBJ_DIR)/freetype-autofit-autofit.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbase.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbbox.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbdf.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftbitmap.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftcid.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftfstype.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftgasp.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftglyph.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftgxval.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftinit.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftmm.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftotval.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftpatent.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftpfr.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftstroke.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftsynth.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftsystem.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-fttype1.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-base-ftwinfnt.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-bdf-bdf.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-bzip2-ftbzip2.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cache-ftcache.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cff-cff.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-cid-type1cid.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-gzip-ftgzip.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-lzw-ftlzw.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pcf-pcf.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pfr-pfr.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-psaux-psaux.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-pshinter-pshinter.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-psnames-psnames.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-raster-raster.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-sfnt-sfnt.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-smooth-smooth.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-truetype-truetype.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-type1-type1.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-type42-type42.d
	$(HIDE) rm -f $(OBJ_DIR)/freetype-winfonts-winfnt.d

.PHONY : \
	all \
	clean