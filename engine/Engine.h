#ifndef Engine_h
#define Engine_h

#include "graphic/Interface.h"
#include "Stage.h"
#include "Object.h"
#include "Camera.h"

#include <ng-log/logging.h>
#include <memory>
#include <thread>
#include <vector>

namespace chloride {

	

	/**
	 * @brief an Engine object is a singleton object that 
	 * handles the translation of objects, user input, scripts
	 */
	class Engine {

	private:
		clock_t tickInterval;
		clock_t previousTick = clock();
		bool stopped = false;

		inline bool nextTick(clock_t now) const {
			return now - previousTick >= tickInterval;
		}

		Engine() = default;
	
		std::unique_ptr<Interface> graphicInterface;
		
		std::unique_ptr<Stage> currentStage;

		std::unique_ptr<Camera> defaultCamera;

		void updateActors(clock_t delta);

	public:
		static Engine& get();
		/**
		 * @brief set logical frame time
		 * @param tps tick per second
		 * @return this ref
		 */
		Engine& setTickInterval(size_t tps);
		Engine& bindInterface(std::unique_ptr<Interface> interface);

		/**
		 * @brief set current stage, all actor in current stage will be 
		 * updated by the engine
		 * @param stage current stage
		 * @return the previous stage
		 */
		std::unique_ptr<Stage> setCurrentStage(std::unique_ptr<Stage> stage);
		/**
		 * @brief get current stage
		 * @return stage pointer
		 */
		const std::unique_ptr<Stage>& getCurrentStage();


		/**
		 * @brief dispatch the engine and occupy the current thread
		 */
		void run();
		void stop();
	};

};

#endif
