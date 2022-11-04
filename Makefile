all:
	@make --directory=src

install:
	@make --directory=src install

uninstall:
	@make --directory=src uninstall

debug:
	@make --directory=src debug

build:
	@make --directory=src build

doc:
	@make --directory=src doc

clean:
	@make --directory=src clean

format:
	@make --directory=src format
