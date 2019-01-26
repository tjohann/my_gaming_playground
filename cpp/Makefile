# only for cleaning

MODULES = sdl_book

all:
	@echo Cheers

clean:
	rm -f *~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean

.PHONY: clean all
