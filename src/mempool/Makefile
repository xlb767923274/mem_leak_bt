TARGET=libmempool.a
OBJS=ncx_slab.o
OBJSO=libmempool.so

CFLAGS+=$(EXTRA_FLAGS)

all:$(TARGET)

$(TARGET):$(OBJS)
	$(AR) -rcs $@ $^
	$(RANLIB) $@
	cp $@ ../../lib
	cp *.h ../../include

%.o:%.c
	$(CXX) $(CFLAGS) -c -fPIC -o $@ $< $(INC)
	$(CXX) -shared $(OBJS) -o $(OBJSO) -g
	cp $(OBJSO) ../../lib/

.PHONY:clean

clean:
	rm -rf $(TARGET) $(OBJS) *.so
