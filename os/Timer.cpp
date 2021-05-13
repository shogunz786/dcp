namespace Bosma {
	using Clock = std::chrono::system_clock;

	inline void add(std::tm &tm, Clock::duration time) {
		auto tp = Clock::from_time_t(std::mktime(&tm));
		auto tp_adjusted = tp + time;
		auto tm_adjusted = Clock::to_time_t(tp_adjusted);
		tm = *std::localtime(&tm_adjusted);
	}

	class BadCronExpression : public std::exception {
		public:
			explicit BadCronExpression(std::string msg) : msg_(std::move(msg)) {}

			const char *what() const noexcept override { return (msg_.c_str()); }

		private:
			std::string msg_;
	};

	inline void
		verify_and_set(const std::string &token, const std::string &expression, int &field, const int lower_bound,
				const int upper_bound, const bool adjust = false) {
			if (token == "*")
				field = -1;
			else {
				try {
					field = std::stoi(token);
				} catch (const std::invalid_argument &) {
					throw BadCronExpression("malformed cron string (`" + token + "` not an integer or *): " + expression);
				} catch (const std::out_of_range &) {
					throw BadCronExpression("malformed cron string (`" + token + "` not convertable to int): " + expression);
				}
				if (field < lower_bound || field > upper_bound) {
					std::ostringstream oss;
					oss << "malformed cron string ('" << token << "' must be <= " << upper_bound << " and >= " << lower_bound
						<< "): " << expression;
					throw BadCronExpression(oss.str());
				}
				if (adjust)
					field--;
			}
		}

	class Cron {
		public:
			explicit Cron(const std::string &expression) {
				std::istringstream iss(expression);
				std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
					std::istream_iterator<std::string>{}};

				if (tokens.size() != 5) throw BadCronExpression("malformed cron string (must be 5 fields): " + expression);

				verify_and_set(tokens[0], expression, minute, 0, 59);
				verify_and_set(tokens[1], expression, hour, 0, 23);
				verify_and_set(tokens[2], expression, day, 1, 31);
				verify_and_set(tokens[3], expression, month, 1, 12, true);
				verify_and_set(tokens[4], expression, day_of_week, 0, 6);
			}

			// http://stackoverflow.com/a/322058/1284550
			Clock::time_point cron_to_next(const Clock::time_point from = Clock::now()) const {
				// get current time as a tm object
				auto now = Clock::to_time_t(from);
				std::tm next(*std::localtime(&now));
				// it will always at least run the next minute
				next.tm_sec = 0;
				add(next, std::chrono::minutes(1));
				while (true) {
					if (month != -1 && next.tm_mon != month) {
						// add a month
						// if this will bring us over a year, increment the year instead and reset the month
						if (next.tm_mon + 1 > 11) {
							next.tm_mon = 0;
							next.tm_year++;
						} else
							next.tm_mon++;

						next.tm_mday = 1;
						next.tm_hour = 0;
						next.tm_min = 0;
						continue;
					}
					if (day != -1 && next.tm_mday != day) {
						add(next, std::chrono::hours(24));
						next.tm_hour = 0;
						next.tm_min = 0;
						continue;
					}
					if (day_of_week != -1 && next.tm_wday != day_of_week) {
						add(next, std::chrono::hours(24));
						next.tm_hour = 0;
						next.tm_min = 0;
						continue;
					}
					if (hour != -1 && next.tm_hour != hour) {
						add(next, std::chrono::hours(1));
						next.tm_min = 0;
						continue;
					}
					if (minute != -1 && next.tm_min != minute) {
						add(next, std::chrono::minutes(1));
						continue;
					}
					break;
				}

				// telling mktime to figure out dst
				next.tm_isdst = -1;
				return Clock::from_time_t(std::mktime(&next));
			}

			int minute, hour, day, month, day_of_week;
	};
	class InterruptableSleep {

		using Clock = std::chrono::system_clock;

		// InterruptableSleep offers a sleep that can be interrupted by any thread.
		// It can be interrupted multiple times
		// and be interrupted before any sleep is called (the sleep will immediately complete)
		// Has same interface as condition_variables and futures, except with sleep instead of wait.
		// For a given object, sleep can be called on multiple threads safely, but is not recommended as behaviour is undefined.

		public:
		InterruptableSleep() : interrupted(false) {
		}

		InterruptableSleep(const InterruptableSleep &) = delete;

		InterruptableSleep(InterruptableSleep &&) noexcept = delete;

		~InterruptableSleep() noexcept = default;

		InterruptableSleep &operator=(const InterruptableSleep &) noexcept = delete;

		InterruptableSleep &operator=(InterruptableSleep &&) noexcept = delete;

		void sleep_for(Clock::duration duration) {
			std::unique_lock<std::mutex> ul(m);
			cv.wait_for(ul, duration, [this] { return interrupted; });
			interrupted = false;
		}

		void sleep_until(Clock::time_point time) {
			std::unique_lock<std::mutex> ul(m);
			cv.wait_until(ul, time, [this] { return interrupted; });
			interrupted = false;
		}

		void sleep() {
			std::unique_lock<std::mutex> ul(m);
			cv.wait(ul, [this] { return interrupted; });
			interrupted = false;
		}

		void interrupt() {
			std::lock_guard<std::mutex> lg(m);
			interrupted = true;
			cv.notify_one();
		}

		private:
		bool interrupted;
		std::mutex m;
		std::condition_variable cv;
	};
	using Clock = std::chrono::system_clock;

	class Task {
		public:
			explicit Task(std::function<void()> &&f, bool recur = false, bool interval = false) :
				f(std::move(f)), recur(recur), interval(interval) {}

			virtual Clock::time_point get_new_time() const = 0;

			void* (*f)(void*);

			bool recur;
			bool interval;
	};

	class InTask : public Task {
		public:
			explicit InTask(std::function<void()> &&f) : Task(std::move(f)) {}

			// dummy time_point because it's not used
			Clock::time_point get_new_time() const override { return Clock::time_point(Clock::duration(0)); }
	};

	class EveryTask : public Task {
		public:
			EveryTask(Clock::duration time, std::function<void()> &&f, bool interval = false) :
				Task(std::move(f), true, interval), time(time) {}

			Clock::time_point get_new_time() const override {
				return Clock::now() + time;
			};
			Clock::duration time;
	};

	class CronTask : public Task {
		public:
			CronTask(const std::string &expression, std::function<void()> &&f) : Task(std::move(f), true),
			cron(expression) {}

			Clock::time_point get_new_time() const override {
				return cron.cron_to_next();
			};
			Cron cron;
	};

	inline bool try_parse(std::tm &tm, const std::string &expression, const std::string &format) {
		std::stringstream ss(expression);
		return !(ss >> std::get_time(&tm, format.c_str())).fail();
	}

	class Scheduler {
		public:
			explicit Scheduler(unsigned int max_n_tasks = 4) : done(false), threads(max_n_tasks + 1) {
				threads.push([this](int) {
						while (!done) {
						if (tasks.empty()) {
						sleeper.sleep();
						} else {
						auto time_of_first_task = (*tasks.begin()).first;
						sleeper.sleep_until(time_of_first_task);
						}
						manage_tasks();
						}
						});
			}

			Scheduler(const Scheduler &) = delete;

			Scheduler(Scheduler &&) noexcept = delete;

			Scheduler &operator=(const Scheduler &) = delete;

			Scheduler &operator=(Scheduler &&) noexcept = delete;

			~Scheduler() {
				done = true;
				sleeper.interrupt();
			}

			template<typename _Callable, typename... _Args>
				void in(const Clock::time_point time, _Callable &&f, _Args &&... args) {
					std::shared_ptr<Task> t = std::make_shared<InTask>(
							std::bind(std::forward<_Callable>(f), std::forward<_Args>(args)...));
					add_task(time, std::move(t));
				}

			template<typename _Callable, typename... _Args>
				void in(const Clock::duration time, _Callable &&f, _Args &&... args) {
					in(Clock::now() + time, std::forward<_Callable>(f), std::forward<_Args>(args)...);
				}

			template<typename _Callable, typename... _Args>
				void at(const std::string &time, _Callable &&f, _Args &&... args) {
					// get current time as a tm object
					auto time_now = Clock::to_time_t(Clock::now());
					std::tm tm = *std::localtime(&time_now);

					// our final time as a time_point
					Clock::time_point tp;

					if (try_parse(tm, time, "%H:%M:%S")) {
						// convert tm back to time_t, then to a time_point and assign to final
						tp = Clock::from_time_t(std::mktime(&tm));

						// if we've already passed this time, the user will mean next day, so add a day.
						if (Clock::now() >= tp)
							tp += std::chrono::hours(24);
					} else if (try_parse(tm, time, "%Y-%m-%d %H:%M:%S")) {
						tp = Clock::from_time_t(std::mktime(&tm));
					} else if (try_parse(tm, time, "%Y/%m/%d %H:%M:%S")) {
						tp = Clock::from_time_t(std::mktime(&tm));
					} else {
						// could not parse time
						throw std::runtime_error("Cannot parse time string: " + time);
					}

					in(tp, std::forward<_Callable>(f), std::forward<_Args>(args)...);
				}

			template<typename _Callable, typename... _Args>
				void every(const Clock::duration time, _Callable &&f, _Args &&... args) {
					std::shared_ptr<Task> t = std::make_shared<EveryTask>(time, std::bind(std::forward<_Callable>(f),
								std::forward<_Args>(args)...));
					auto next_time = t->get_new_time();
					add_task(next_time, std::move(t));
				}

			// expression format:
			// from https://en.wikipedia.org/wiki/Cron#Overview
			//    +------------- minute (0 - 59)
			//    ¦ +------------- hour (0 - 23)
			//    ¦ ¦ +------------- day of month (1 - 31)
			//    ¦ ¦ ¦ +------------- month (1 - 12)
			//    ¦ ¦ ¦ ¦ +------------- day of week (0 - 6) (Sunday to Saturday)
			//    ¦ ¦ ¦ ¦ ¦
			//    ¦ ¦ ¦ ¦ ¦
			//    * * * * *
			template<typename _Callable, typename... _Args>
				void cron(const std::string &expression, _Callable &&f, _Args &&... args) {
					std::shared_ptr<Task> t = std::make_shared<CronTask>(expression, std::bind(std::forward<_Callable>(f),
								std::forward<_Args>(args)...));
					auto next_time = t->get_new_time();
					add_task(next_time, std::move(t));
				}

			template<typename _Callable, typename... _Args>
				void interval(const Clock::duration time, _Callable &&f, _Args &&... args) {
					std::shared_ptr<Task> t = std::make_shared<EveryTask>(time, std::bind(std::forward<_Callable>(f),
								std::forward<_Args>(args)...), true);
					add_task(Clock::now(), std::move(t));
				}

		private:
			std::atomic<bool> done;

			Bosma::InterruptableSleep sleeper;

			std::multimap<Clock::time_point, std::shared_ptr<Task>> tasks;
			std::mutex lock;
			ctpl::thread_pool threads;

			void add_task(const Clock::time_point time, std::shared_ptr<Task> t) {
				std::lock_guard<std::mutex> l(lock);
				tasks.emplace(time, std::move(t));
				sleeper.interrupt();
			}

			void manage_tasks() {
				std::lock_guard<std::mutex> l(lock);

				auto end_of_tasks_to_run = tasks.upper_bound(Clock::now());

				// if there are any tasks to be run and removed
				if (end_of_tasks_to_run != tasks.begin()) {
					// keep track of tasks that will be re-added
					decltype(tasks) recurred_tasks;

					// for all tasks that have been triggered
					for (auto i = tasks.begin(); i != end_of_tasks_to_run; ++i) {

						auto &task = (*i).second;

						if (task->interval) {
							// if it's an interval task, only add the task back after f() is completed
							threads.push([this, task](int) {
									task->f();
									// no risk of race-condition,
									// add_task() will wait for manage_tasks() to release lock
									add_task(task->get_new_time(), task);
									});
						} else {
							threads.push([task](int) {
									task->f();
									});
							// calculate time of next run and add the new task to the tasks to be recurred
							if (task->recur)
								recurred_tasks.emplace(task->get_new_time(), std::move(task));
						}
					}

					// remove the completed tasks
					tasks.erase(tasks.begin(), end_of_tasks_to_run);

					// re-add the tasks that are recurring
					for (auto &task : recurred_tasks)
						tasks.emplace(task.first, std::move(task.second));
				}
			}
	};
}

void message(const std::string &s) {
  std::cout << s << std::endl;
}

int main() {
  // number of tasks that can run simultaneously
  // Note: not the number of tasks that can be added,
  //       but number of tasks that can be run in parallel
  unsigned int max_n_threads = 12;

  // Make a new scheduling object.
  // Note: s cannot be moved or copied
  Bosma::Scheduler s(max_n_threads);

  // every second call message("every second")
  s.every(std::chrono::seconds(1), message, "every second");

  // in one minute
  s.in(std::chrono::minutes(1), []() { std::cout << "in one minute" << std::endl; });

  // run lambda, then wait a second, run lambda, and so on
  // different from every in that multiple instances of the function will never be run concurrently
  s.interval(std::chrono::seconds(1), []() {
      std::cout << "right away, then once every 6s" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(5));
  });

  // https://en.wikipedia.org/wiki/Cron
  s.cron("* * * * *", []() { std::cout << "top of every minute" << std::endl; });

  // Time formats supported:
  // %Y/%m/%d %H:%M:%S, %Y-%m-%d %H:%M:%S, %H:%M:%S
  // With only a time given, it will run tomorrow if that time has already passed.
  // But with a date given, it will run immediately if that time has already passed.
  s.at("2017-04-19 12:31:15", []() { std::cout << "at a specific time." << std::endl; });

  s.cron("5 0 * * *", []() { std::cout << "every day 5 minutes after midnight" << std::endl; });

  // destructor of Bosma::Scheduler will cancel all schedules but finish any tasks currently running
  std::this_thread::sleep_for(std::chrono::minutes(10));
}
