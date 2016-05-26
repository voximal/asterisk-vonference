# $Id: Makefile,v 1.1 2016/05/26 08:45:43 borja.sixto Exp $

#
# Makefile, based on the Asterisk Makefile, Coypright (C) 1999, Mark Spencer
#
# Copyright (C) 2002,2003 Junghanns.NET GmbH
#
# Klaus-Peter Junghanns <kapejod@ns1.jnetdns.de>
#
# This program is free software and may be modified and
# distributed under the terms of the GNU Public License.
#

.EXPORT_ALL_VARIABLES:

#
# app_vonference defines which can be passed on the command-line
#

INSTALL_PREFIX :=
INSTALL_MODULES_DIR := $(INSTALL_PREFIX)/usr/lib/asterisk/modules

#ASTERISK_INCLUDE_DIR ?= ../asterisk/include
ASTERISK_INCLUDE_DIR ?= $(ASTERISKDIR)/include
ASTERISK_MACROS_INCLUDE_DIR := $(ASTERISKMACROSDIR)/include

RELEASE = 1
REVISION = 0

# turn app_vonference debugging on or off ( 0 == OFF, 1 == ON )
APP_VONFERENCE_DEBUG ?= 1

# turn cache_control_blocks on or off ( 0 == OFF, 1 == ON )
CACHE_CONTROL_BLOCKS ?= 1

# channel table size default
CHANNEL_TABLE_SIZE ?= 997

# conference table size default
CONFERENCE_TABLE_SIZE ?= 199

# schedule conference threads fifo ( 0 == OFF, 1 == ON )
REALTIME ?= 1

# turn app_vonference texting on of off ( 0 == OFF, 1 == ON )
TEXT ?= 1

# turn app_vonference video on of off ( 0 == OFF, 1 == ON )
VIDEO ?= 1

# turn app_vonference dtmf on of off ( 0 == OFF, 1 == ON )
DTMF ?= 1

# turn app_vonference manager commands on of off ( 0 == OFF, 1 == ON )
MANAGER ?= 1

# 0 = OFF 1 = astdsp 2 = speex
SILDET := 2

# turn Ulex modifications on of off ( 0 == OFF, 1 == ON )
ULEX ?= 1

#
# app_vonference objects to build
#

OBJS = app_vonference.o conference.o member.o frame.o cli.o
TARGET = app_vonference.so


#
# standard compile settings
#

PROC = $(shell uname -m)
INSTALL = install

INCLUDE = -I$(ASTERISK_MACROS_INCLUDE_DIR) -I$(ASTERISK_INCLUDE_DIR) -I.
DEBUG := -g

CFLAGS  += -march=$(LINUX_BUILD) -pipe -Wall -Wmissing-prototypes -Wmissing-declarations -MD -MP $(DEBUG)
CPPFLAGS += $(INCLUDE) -D_REENTRANT -D_GNU_SOURCE -DRELEASE=\"$(RELEASE)\" -DREVISION=\"$(REVISION)\"
#CFLAGS += -O2
#CFLAGS += -O3 -march=pentium3 -msse -mfpmath=sse,387 -ffast-math
# PERF: below is 10% faster than -O2 or -O3 alone.
#CFLAGS += -O3 -ffast-math -funroll-loops
# below is another 5% faster or so.
#CFLAGS += -O3 -ffast-math -funroll-all-loops -fsingle-precision-constant
#CFLAGS += -mcpu=7450 -faltivec -mabi=altivec -mdynamic-no-pic
# adding -msse -mfpmath=sse has little effect.
#CFLAGS += -O3 -msse -mfpmath=sse
#CFLAGS += $(shell if $(CC) -march=$(PROC) -S -o /dev/null -xc /dev/null >/dev/null 2>&1; then echo "-march=$(PROC)"; fi)
CFLAGS += $(shell if uname -m | grep -q ppc; then echo "-fsigned-char"; fi)
CFLAGS += -fPIC
CPPFLAGS += -DCRYPTO
CPPFLAGS += -DCHANNEL_TABLE_SIZE=$(CHANNEL_TABLE_SIZE)
CPPFLAGS += -DCONFERENCE_TABLE_SIZE=$(CONFERENCE_TABLE_SIZE)
CPPFLAGS += -DONEMIXTHREAD

#
# Uncomment this if you want G.729A support (need to have the actual codec installed
#
CPPFLAGS += -DAC_USE_G729A

#
# Uncomment this if you want G.722 support (requires asterisk 1.6)
#
# CPPFLAGS += -DAC_USE_G722

#
# Uncomment this if you want SPEEX support (need to have the actual codec installed)
#
CPPFLAGS += -DAC_USE_SPEEX

ifeq ($(APP_VONFERENCE_DEBUG), 1)
CPPFLAGS += -DAPP_VONFERENCE_DEBUG
endif

ifeq ($(CACHE_CONTROL_BLOCKS), 1)
CPPFLAGS += -DCACHE_CONTROL_BLOCKS
endif

ifeq ($(REALTIME), 1)
CPPFLAGS += -DREALTIME
endif

ifeq ($(TEXT), 1)
CPPFLAGS += -DTEXT
endif

ifeq ($(VIDEO), 1)
CPPFLAGS += -DVIDEO
endif

ifeq ($(DTMF), 1)
CPPFLAGS += -DDTMF
endif

ifeq ($(MANAGER), 1)
CPPFLAGS += -DMANAGER_COMMANDS
endif

ifeq ($(ULEX), 1)
CPPFLAGS += -DULEX -Dulex
endif

#
# additional flag values for silence detection
#

ifeq ($(SILDET), 2)
OBJS += libspeex/preprocess.o libspeex/misc.o libspeex/smallft.o
CPPFLAGS += -Ilibspeex -DSILDET=2
endif

ifeq ($(SILDET), 1)
CPPFLAGS += -DSILDET=1
endif

OSARCH=$(shell uname -s)
ifeq (${OSARCH},Darwin)
SOLINK=-dynamic -bundle -undefined suppress -force_flat_namespace
else
SOLINK=-shared -Xlinker -x
endif

DEPS += $(subst .o,.d,$(OBJS))

#
# targets
#

all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS)

.PHONY: distclean
distclean: clean
	$(RM) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -pg $(SOLINK) -o $@ $(OBJS)

vad_test: vad_test.o libspeex/preprocess.o libspeex/misc.o libspeex/smallft.o
	$(CC) $(PROFILE) -o $@ $^ -lm

install:
	$(INSTALL) -m 755 $(TARGET) $(INSTALL_MODULES_DIR)


# config: all
# 	cp conf.conf /etc/asterisk/

-include $(DEPS)
