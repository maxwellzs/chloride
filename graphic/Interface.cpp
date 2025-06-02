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


void chloride::Interface::displayFPS(double delta)
{
	size_t fps = 1 / delta;
	std::string newTitle = title + " fps=" + std::to_string(fps);
	glfwSetWindowTitle(window, newTitle.c_str());
}

chloride::Interface::~Interface()
{
	glfwDestroyWindow(window);
	shutdown();
}

chloride::Interface::Interface(size_t width, size_t height, const std::string& title)
	: title(title)
{
	initialize();
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("cannot load glad");

	glViewport(0, 0, width, height);
	mat4 currentPerspective = mat4::perspective(0.1f, 100.0f, 80, static_cast<float>(width) / height);
	LOG(INFO) << "current perspective : \r\n" << currentPerspective;
	RenderManager::get().setPerspective(currentPerspective);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		RenderManager::get().setPerspective(mat4::perspective(0.1f, 100.0f, 80, static_cast<float>(w) / h));
		});
	previousFrame = clock();

}

void chloride::Interface::limitFramerate(size_t rate)
{
	fpsLimit = rate;
}

bool chloride::Interface::loopOnce()
{
	if (glfwWindowShouldClose(window)) throw InterfaceClosedException();

	double now = glfwGetTime();
	bool rendered = false;

	if (shouldRender(now)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		// draw calls
		RenderManager::get().executeAll();
		displayFPS(now - previousFrame);

		previousFrame = now;
		rendered = true;

		glfwSwapBuffers(window);
	}

	glfwPollEvents();
	return rendered;
}
