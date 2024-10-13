all:
	cd parser && make
	CGO_ENABLED=0 go build -v
clean:
	rm -f luau-bundler
	cd parser && make trimclean
distclean:
	rm -f luau-bundler
	cd parser && make clean