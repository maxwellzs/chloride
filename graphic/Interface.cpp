#include "Interface.h"

std::atomic_int chloride::Interface::ref(0);

void chloride::Interface::initialize()
{
	static bool initialized = false;
	if (!initialized) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		ref.fetch_add(1);
		initialized = true;
	}
}

void chloride::Interface::shutdown()
{
	int oldRef = ref.fetch_sub(1);
	if (oldRef == 1) {
		glfwTerminate();
	}
}

chloride::Interface::~Interface()
{
	glfwDestroyWindow(window);
	shutdown();
}

chloride::Interface::Interface(size_t width, size_t height, const std::string& title)
{
	initialize();
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("cannot load glad");

	glViewport(0, 0, width, height);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		});
	previousFrame = clock();

}

void chloride::Interface::limitFramerate(size_t rate)
{
	renderInterval = CLOCKS_PER_SEC / rate;
}

bool chloride::Interface::loopOnce()
{
	if (glfwWindowShouldClose(window)) throw std::exception();

	clock_t now = clock();
	bool rendered = false;

	if (shouldRender(now - previousFrame)) {
		// draw calls
		Singleton<RenderManager>::get().executeAll();

		previousFrame = now;
		rendered = true;
	}

	glfwPollEvents();
	glfwSwapBuffers(window);
	return rendered;
}
