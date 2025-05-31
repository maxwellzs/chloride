#ifndef Singleton_h
#define Singleton_h

namespace chloride {

	template<typename T>
	class Singleton {
	private:
		static T* instance;

		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		void operator=(const Singleton&) = delete;

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	public:
		static T& get() {
			return instance ? *instance : *(instance = new T);
		}
	};

	template<typename T>
	T* Singleton<T>::instance = nullptr;

};

#endif
