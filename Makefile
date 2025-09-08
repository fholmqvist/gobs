.PHONY: run cloc

run:
	sh run.sh

cloc:
	cloc * --exclude-dir=build,external
