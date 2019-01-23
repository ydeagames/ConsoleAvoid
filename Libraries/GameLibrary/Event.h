#pragma once
#include "Component.h"

class Event
{
public:
	Event() = default;
	virtual ~Event() = default;
};

class EventBus : public Component
{
private:
	// Event Dispatcher class from https://stackoverflow.com/a/42350751
	class event_dispatcher
	{
	public:
		template <typename F>
		struct function_traits : public function_traits<decltype(&F::operator())>
		{};

		template <typename T, typename R, typename... Args>
		struct function_traits<R(T::*)(Args...) const>
		{
			typedef R(*pointer)(Args...);
			typedef R return_type;
			static constexpr std::size_t arg_count = sizeof...(Args);
			typedef std::tuple<Args...> args_tuple;
			typedef const std::function<R(Args...)> function;
		};

	private:
		struct function_wrapper
		{
			virtual ~function_wrapper() {}
			virtual const void* get_ptr() const = 0;
		};

		template<typename F>
		class function_wrapper_t : public function_wrapper
		{
		public:
			function_wrapper_t(F&& f) : _function(f) {}
			~function_wrapper_t() {}
			const void* get_ptr() const { return &_function; }

		private:
			typename function_traits<F>::function _function;
		};

		template <typename F>
		std::unique_ptr<function_wrapper> create_wrapper(F f)
		{
			return std::unique_ptr<function_wrapper_t<decltype(f)>>(new function_wrapper_t<decltype(f)>(std::forward<F>(f)));
		}

	private:
		std::unordered_map<std::type_index, std::vector<std::unique_ptr<function_wrapper>>> _list;

	public:
		template<typename F>
		void connect(const std::type_index& name, F f)
		{
			static_assert(std::is_same<void, typename function_traits<F>::return_type>::value,
				"Signals cannot have a return type different from void");

			_list[name].emplace_back(create_wrapper(std::forward<F>(f)));
		}

		template<typename ... Args>
		void dispatch(const std::type_index& name, Args... args)
		{
			auto& funcs = _list[name];

			for (auto& func : funcs)
			{
				auto& f = *reinterpret_cast<const std::function<void(Args...)>*>(func->get_ptr());
				f(std::forward<Args>(args) ...); // is this undefined behavior?
			}
		}
	} dispatcher;

public:
	template<class F>
	void Register(F listener)
	{
		dispatcher.connect(typeid(event_dispatcher::function_traits<F>::args_tuple), listener);
	}

	template<class T>
	void Post(T& eventObj)
	{
		dispatcher.dispatch(typeid(T), eventObj);
	}
};

