all: target-linux/pixview compile_commands.json

target-linux:
	mkdir target-linux
target-linux/build.ninja: target-linux CMakeLists.txt
	cd target-linux && cmake .. -GNinja

target-linux/pixview: target-linux/build.ninja
	cd target-linux && ninja

run:target-linux/pixview
	./target-linux/pixview

compile_commands.json: target-linux/build.ninja
	ln -s ./target-linux/compile_commands.json

clean:
	rm -r compile_commands.json target-linux
