#include <iostream>
#include "Engine.h"

int main() {
	Engine engine;

	try {
		engine.Run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}