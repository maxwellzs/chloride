#ifndef RenderManager_h
#define RenderManager_h

#include <string>
#include <map>
#include <functional>

#include "RenderHandler.h"
#include "utils/Spinlock.h"
#include "utils/Singleton.h"

namespace chloride {

	/**
	 * @brief a render manager manages the handlers
	 */
	class RenderManager {
		friend class Singleton<RenderManager>;

	private:
		std::atomic_flag flag;

		RenderManager() = default;

		std::map<std::string, std::unique_ptr<RenderHandler>> handlerMap;

	public:
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
		 * @brief execute all handler in this manager for once
		 */
		void executeAll();
	};

};

#endif
