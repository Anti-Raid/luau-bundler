all:
	cd luau && make && cd ..
	g++ main.cpp luau/CLI/FileUtils.cpp -I luau/Ast/include/ -I luau/Common/include -I luau/Analysis/include luau/build/debug/*.a