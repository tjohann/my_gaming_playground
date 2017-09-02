# only for cleaning

LIB = libgame
MODULES = $(shell ls -d */ | cut -f1 -d'/')

.PHONY: all
all:
	for dir in $(LIB); do (cd $$dir && $(MAKE)); done
	for dir in $(MODULES); do (cd $$dir && $(MAKE)); done

.PHONY: clean
clean:
	rm -f *~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean
