#ifndef Stage_h
#define Stage_h

#include <functional>
#include "Actor.h"
#include "Object.h"

namespace chloride {
	
	/**
	 * @brief a stage object manage the actors belongs to it
	 */
	class Stage : public Object {
	private:

		std::string stageName;
		std::vector<std::unique_ptr<Actor>> actorList;

	public:
		
		Stage() : stageName("anonymous stage") {};
		Stage(const std::string& name) : stageName(name) {}

		const std::string& getStageName() {
			return stageName;
		}

		void addActor(std::unique_ptr<Actor> actor);
		void forEach(std::function<void (const std::unique_ptr<Actor>&)> callback);
	};

};

#endif
