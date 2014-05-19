include config.mk

default: rmdirs all done

all: mkdirs
	@echo "ENTERING --> src"
	@make -s -C src
	@echo "ENTERING --> tests!"
	@make -s -C tests

debug: mkdirs
	@echo "ENTERING --> src"
	@make -C src
	@echo "ENTERING --> tests"
	@make -C tests

mkdirs: rmdirs
	@mkdir bin
	@mkdir obj
	@mkdir lib

rmdirs:
	@rm -rf bin
	@rm -rf obj
	@rm -rf lib
	@rm -rf tests/*.dSYM

run_tests: scripts/unittest_runner.py
	@./$<

clean: rmdirs
	@echo "cleaning ..."
	@echo "done! :)"

done:
	@echo "done! :)"
