

all: bin
	/home/ondra/vscode/modbuilder/build/bin/cairn -j16 -ccompile_commands.json bin/cairn=src/cairn/main.cpp clang++-18 --compile: -std=c++20 -O0 -ggdb

clean:
	rm -rf .build


bin: 
	mkdir bin
