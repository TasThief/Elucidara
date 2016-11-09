#include "RenderEngine.h"

//main function
int main()
{
	RenderingEngine renderingEngine;

	//initialize rendering engine
	renderingEngine.initialize();

	renderingEngine.update();


	//finalize rendering engine
	renderingEngine.finalize();

	return 0;
}
