src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

CFLAGS = -Wall -D TESTING
LDFLAGS = -shared

all: cordlib

cordlib: $(obj)
	$(CC) -o $@.so $^ $(LDFLAGS)

-include $(dep)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) cordlib.so

.PHONY: cleandep
cleandep:
	rm -f $(dep)

test:
	LD_LIBRARY_PATH=. prove --exec perl6 t
