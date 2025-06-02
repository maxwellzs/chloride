#ifndef Interface_h
#define Interface_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <atomic>
#include <functional>
#include <ctime>

#include "RenderManager.h"

namespace chloride {

	using DrawFunction = std::function<void ()>;

	class InterfaceClosedException : public std::exception {
	public:
		virtual char const* what() const
		{
			return "interface closed by user";
		}
	};

	class InputHandler {
	public:
		virtual void onClick(int x, int y) {};
	};

	/**
	 * @brief a interface holds a window surface and handle all its events
	 */
	class Interface {
	private:
		GLFWwindow* window;
		
		static std::atomic_int ref;
		static void initialize();
		static void shutdown();
		std::string title;

		bool stopped = false;

		size_t fpsLimit = 60;
		double previousFrame = glfwGetTime();

		inline bool shouldRender(double now) const {
			size_t currentRate = 1 / (now - previousFrame);
			return fpsLimit >= currentRate;
		}

		void displayFPS(double delta);

	public:
		~Interface();
		Interface(size_t width, size_t height, const std::string& title);
		void limitFramerate(size_t rate);

		bool loopOnce();
	};

};

#endif
