# only for cleaning

MODULES = c cpp go assets Documentation

all:
	@echo Cheers

clean:
	rm -f *~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean

.PHONY: clean all
