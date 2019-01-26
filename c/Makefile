# only for cleaning

MODULES = first_try second_try

all:
	@echo Cheers

clean:
	rm -f *~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean

.PHONY: clean all
