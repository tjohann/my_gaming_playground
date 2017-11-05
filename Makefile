# only for cleaning

LIB = libgame
MODULES = simple_ai

all:
	for dir in $(LIB); do (cd $$dir && $(MAKE)); done
	for dir in $(MODULES); do (cd $$dir && $(MAKE)); done

run:
	(cd menu && ./menu)

clean:
	rm -f *~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean

.PHONY: clean run all
