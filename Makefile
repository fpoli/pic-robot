
all:
	@cd test_lib && make all

test:
	@cd test_lib && make test

clean:
	@rm -f */nbproject/Makefile-*
	@rm -f */nbproject/Package-default.bash
	@rm -rf */nbproject/private/
	@rm -rf */build/
	@rm -rf */debug/
	@rm -rf */dist/
	@cd test_lib && make clean
