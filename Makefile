
all:
	@cd test_lib && make all

check:
	@cppcheck . --std=c99 --enable=all --suppress=unusedFunction

linter:
	@cpplint --linelength=100 --filter=-legal/copyright,-build/include_subdir lib/* */main.c

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
