LUAU_MODE := release

all:
	cd luau && make config=$(LUAU_MODE)
	g++ -O2 -o parser main.cpp luau/CLI/FileUtils.cpp -I luau/Ast/include/ -I luau/Common/include -I luau/Analysis/include luau/build/$(LUAU_MODE)/*.a
clean:
	rm -f parser
	cd luau && make clean && make clean config=$(LUAU_MODE)
	cd luau && rm -f luau-bytecode # If it still exists
trimclean:
	rm -f parser
	cd luau && rm -f luau-bytecode # If it still exists