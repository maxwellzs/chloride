#ifndef RenderManager_h
#define RenderManager_h

#include <string>
#include <map>
#include <functional>
#include <ng-log/logging.h>

#include "RenderHandler.h"
#include "utils/Spinlock.h"

namespace chloride {

	/**
	 * @brief a render manager manages the handlers
	 */
	class RenderManager {

	private:
		std::atomic_flag flag;

		RenderManager() = default;
		~RenderManager();

		mat4 currentPerspective = mat4::eyes();
		mat4 currentCameraPos = mat4::eyes();
		mat4 currentCameraRotation = mat4::eyes();
		std::map<std::string, std::unique_ptr<RenderHandler>> handlerMap;

	public:
		static RenderManager& get();

		/**
		 * @brief check if a handler with the name exist
		 * @param id handler id
		 * @return true if exist
		 */
		bool handlerExist(const std::string& id);

		/**
		 * @brief get the handler with the id
		 * @param id handler id
		 * @return handler, if not exist, raise an exception
		 */
		const std::unique_ptr<RenderHandler>& getHandlerById(const std::string& id);

		/**
		 * @brief add a handler to this manager, if the id already existed this handler will
		 * be discard 
		 * @param id the id
		 * @param handler handler 
		 */
		void addHandler(const std::string& id, std::unique_ptr<RenderHandler> handler);
		/**
		 * @brief set current render perspective
		 * @param perspective matrix
		 */
		void setPerspective(const mat4& perspective);
		/**
		 * @brief set current camera location
		 * @param location 
		 */
		void setCameraLocation(const mat4& location);
		/**
		 * @brief set current camera rotation
		 * @param rotation 
		 */
		void setCameraRotation(const mat4& rotation);

		/**
		 * @brief execute all handler in this manager for once
		 */
		void executeAll();
	};

};

#endif
