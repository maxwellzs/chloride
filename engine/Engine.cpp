#include "Engine.h"

chloride::Engine& chloride::Engine::bindInterface(std::unique_ptr<Interface> interface) {
	graphicInterface = std::move(interface);
	return *this;
}

void chloride::Engine::updateActors(clock_t delta)
{
	if (currentStage.get()) {
		currentStage->forEach([&](const std::unique_ptr<Actor>& actor) {
			actor->update(*this, delta);
			});
	}
}

chloride::Engine& chloride::Engine::setTickInterval(size_t tps)
{
	// TODO: insert return statement here
	tickInterval = CLOCKS_PER_SEC / tps;
	return *this;
}

std::unique_ptr<chloride::Stage> chloride::Engine::setCurrentStage(std::unique_ptr<Stage> stage)
{
	std::unique_ptr<Stage> previous = std::move(currentStage);
	currentStage = std::move(stage);
	return previous;
}

const std::unique_ptr<chloride::Stage>& chloride::Engine::getCurrentStage()
{
	// TODO: insert return statement here
	return currentStage;
}

void chloride::Engine::run()
{
	bool executed = false;

	while (!stopped) {

		try
		{
			// render
			executed = graphicInterface->loopOnce();
			clock_t now = clock();
			
			if (nextTick(now)) {
				executed = true;
				// game logic
				clock_t delta = now - previousTick;
				updateActors(delta);

				previousTick = now;
			}

			if (!executed) std::this_thread::yield();
		}
		catch (const InterfaceClosedException& e)
		{
			// windows closed 
			LOG(INFO) << "engine received quit signal, stopping...";
			break;
		}
		catch (const std::exception& e) 
		{
			// general exceptions not properly caught
			LOG(WARNING) << "uncaught exception : " << e.what();
		}

	}
}

void chloride::Engine::stop()
{
	stopped = true;
}
