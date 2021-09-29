TARGET=libbt.a
OBJS=bt.o
OBJSO=libbt.so

CFLAGS+=$(EXTRA_FLAGS)

all:$(TARGET)
	$(CXX) -shared $(OBJS) -o $(OBJSO)
	cp $(OBJSO) $^ ../../lib/
	cp *.h ../../include

$(TARGET):$(OBJS)
	$(AR) -rcs $@ $^
	$(RANLIB) $@

%.o:%.c
	$(CXX) $(CFLAGS) -fPIC -c -o $@ $< $(INC_DIR)

.PHONY:clean

clean:
	rm -rf $(TARGET) $(OBJS) *.so
