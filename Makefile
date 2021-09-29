CURRENT_DIR = $(shell pwd)
TOP_DIR = $(CURRENT_DIR)
SRC_DIR = $(TOP_DIR)/src
BIN_DIR = $(TOP_DIR)/bin
LIB_DIR = $(TOP_DIR)/lib
INC_DIR = -I$(TOP_DIR)/include -I$(SRC_DIR)/mempool -I$(SRC_DIR)/tbacktrace

CFLAGS += $(INC_DIR)

ifeq ($(asan), 1)
	ASAN_CFLAGS += -fsanitize=address -fsanitize-recover=address -fno-stack-protector -fno-omit-frame-pointer -fno-var-tracking
	EXTRA_FLAGS += -lasan $(ASAN_CFLAGS)
endif

ifeq ($(arm), 1)
	host=arm-linux-gnueabihf-
else
	host=
endif

CC = $(host)gcc 
CXX = $(host)gcc
AR = $(host)ar
RANLIB = $(host)ranlib
STRIP = $(host)strip
LD = $(host)ld
EXTRA_FLAGS += -O0 -g -Wall -rdynamic -no-pie #position independent executable(PIE)
EXTRA_FLAGS += -D_GNU_SOURCE -fno-common -Wno-unused-parameter -Wno-unused-function \
	       -Wextra -fno-omit-frame-pointer -Wno-unused-variable 
		#-Wno-missing-field-initializers -Wmissing-declarations -Wno-comment -Wformat
		#-Wno-unused-parameter -Wno-pointer-sign -Wstrict-prototype -Wold-style-definitio -fstack-protectorn
LDFLAGS += -lpthread -rdynamic

STATIC_LIB+=$(LIB_DIR)/libbt.a $(LIB_DIR)/libmempool.a 

export CC CXX AR RANLIB STRIP LD CFLAGS INC_DIR MIPS_INC LDFLAGS EXTRA_FLAGS

TARGET = test_main

SUBDIRS = src/mempool src/tbacktrace

OBJECTS = test_main.o

$(TARGET):$(SUBDIRS) $(OBJECTS)
	echo $(OBJECTS)
	$(CXX) -o $@ $(CFLAGS) $(EXTRA_FLAGS) $(OBJECTS) $(STATIC_LIB) $(LDFLAGS)
	#$(CXX) -o test $(CFLAGS) $(EXTRA_FLAGS) $(OBJECTS) -L$(LIB_DIR) -lbt -lmempool $(LDFLAGS) -O0 -g -Wall -rdynamic -no-pie
	mv $@ $(BIN_DIR)

$(OBJECTS):%.o:%.c
	$(CXX) -c $< -o $@ $(EXTRA_FLAGS) $(CFLAGS)

$(SUBDIRS):ECHO
	+$(MAKE) -C $@ $(CFLAGS) $1
ECHO:
	@echo $(SUBDIRS)
	@echo begin compile

.PHONY: clean

DEL_BIN = $(BIN_DIR)/test_main

clean:
	rm -rf $(LIB_DIR)/*.a $(LIB_DIR)/*.so $(TOP_DIR)/include/*.h $(OBJECTS) $(DEL_BIN)
	for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir clean||exit 1;\
	done

