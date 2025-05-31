#ifndef Object_h
#define Object_h

namespace chloride {

	/**
	 * @brief base class of all engine objects
	 */
	class Object {
	public:
		Object() = default;
		virtual ~Object() = default;
	};

};

#endif