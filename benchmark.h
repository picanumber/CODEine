#ifndef I_BMRK_7K45G66H7JI
#define I_BMRK_7K45G66H7JI

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <locale>
#include <thread>
#include <cstddef>
#include <utility>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <type_traits>

#define fw(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace bmk
{

	using std::map;
	using std::pair;
	using std::vector;
	using std::string;
	using std::size_t;
	using std::ostream;
	using std::is_same;
	using std::forward;
	using std::ofstream;
	using std::enable_if;
	using std::unique_ptr;
	using std::result_of_t;
	using std::make_unique;
	using std::initializer_list;
	using std::integral_constant;
	using std::remove_reference_t;
	using std::chrono::time_point;
	using std::chrono::duration_cast;

/// folly function to avoid optimizing code away
#if defined(_MSC_VER) && !defined(__clang__)
#pragma optimize("", off)
	template <class T>
	void doNotOptimizeAway(T&& datum)
	{
		(void)datum;
	}
#pragma optimize("", on)
#else
	template <class T>
	void doNotOptimizeAway(T&& datum)
	{
		asm volatile("" : "+r"(datum));
	}
#endif

	enum class central_location { mean, median, mode, min, none };

	/// calculate the mean value
	template <typename C>
	auto mean(C&& data)
	{
		return std::accumulate(
		           begin(data), end(data),
		           (remove_reference_t<decltype(*begin(data))>) 0) /
		       data.size();
	}

	template <class T>
	auto median(std::vector<T> const& v)
	{
		bool isEven = !(v.size() % 2);
		std::size_t n = v.size() / 2;

		std::vector<size_t> vi(v.size());
		std::iota(vi.begin(), vi.end(), 0);

		std::partial_sort(vi.begin(), vi.begin() + n + 1, vi.end(),
			[&](size_t lhs, size_t rhs) { return v[lhs] < v[rhs]; });

		return isEven ? 0.5 * (v[vi.at(n - 1)] + v[vi.at(n)]) : v[vi.at(n)];
	}

	namespace detail
	{
		using std::chrono::duration; 

		template <class duration_t>
		struct impl_time_t
		{
			static string apply() { return "unknown"; }
		};

		template <class Rep>
		struct impl_time_t<duration<Rep, std::nano>>
		{
			static string apply() { return "nanoseconds"; }
		}; 

		template <class Rep>
		struct impl_time_t<duration<Rep, std::micro>>
		{
			static string apply() { return "microseconds"; }
		}; 

		template <class Rep>
		struct impl_time_t<duration<Rep, std::milli>>
		{
			static string apply() { return "milliseconds"; }
		}; 

		template <class Rep>
		struct impl_time_t<duration<Rep>>
		{
			static string apply() { return "seconds"; }
		}; 

		template <class Rep>
		struct impl_time_t<duration<Rep, std::ratio<60>>>
		{
			static string apply() { return "minutes"; }
		}; 

		template <class Rep>
		struct impl_time_t<duration<Rep, std::ratio<3600>>>
		{
			static string apply() { return "hours"; }
		}; 
	}

	/// get the name of the chrono time type
	template <class T>
	inline string time_type()
	{
		return detail::impl_time_t<T>::apply(); 
	}

	/**
	* @struct ccleaner
	* @brief  cache cleaner
	* @detail Use the following sizes as reference:
	*	L1 Data Cache        : 32 KB - 256 KB, Line Size 64
	*	L1 Instruction Cache :          32 KB, Line Size 64
	*	L2 Unified Cache     :   256 KB - 1MB, Line Size 64
	*	L3 Unified Cache     :           6 MB, Line Size 64
	*/
	struct ccleaner
	{
		std::size_t _megs; // number of megabytes 
		std::size_t _dumy; // dummy value to hinter optimization

		explicit ccleaner(std::size_t megs = 20)
			: _megs(megs)
			, _dumy(0)
		{}

		std::size_t operator()()
		{
			const std::size_t size = _megs * 1024 * 1024;
			char *c = (char *)malloc(size);
			memset(c, 1 * ((int)_megs), size);

			for (std::size_t j = 0; j < size; j++)
				_dumy += (c[j] + _megs);

			free(c);
			return _dumy;
		}

		std::size_t rest()
		{
			std::size_t ret = _dumy; 
			std::this_thread::sleep_for(std::chrono::milliseconds{ _megs }); 
			return ((_dumy = 0), ret); 
		}
	};

	template <class TimeT  = std::chrono::milliseconds,
	          class ClockT = std::chrono::steady_clock>
	class timeout
	{
		TimeT _total{0};
		decltype(ClockT::now()) _start;

	public:
		void tic() { _start = ClockT::now(); }
		void toc() { _total += duration_cast<TimeT>(ClockT::now() - _start); }
		TimeT duration() const { return _total; }
	};

	template <class TimeT  = std::chrono::milliseconds,
	          class ClockT = std::chrono::steady_clock>
	using timeout_ptr      = unique_ptr<timeout<TimeT, ClockT>>;

	namespace detail
	{
		template <central_location CL>
		struct measure_cl; 

		template <>
		struct measure_cl<central_location::mean>
		{
			template <typename C>
			static auto apply(C&& input)
			{
				return mean(fw(input)); 
			}
		};

		template <>
		struct measure_cl<central_location::median>
		{
			template <typename C>
			static auto apply(C&& input)
			{
				return median(fw(input));
			}
		};

		template <>
		struct measure_cl<central_location::min>
		{
			template <typename C>
			static auto apply(C&& input)
			{
				return *std::min_element(std::begin(fw(input)), std::end(fw(input)));
			}
		};

		/**
		* @ class experiment_impl
		* @ brief implementation details of an experiment
		*/
		template <class TimeT, class FactorT, central_location CL>
		struct experiment_impl
		{
			string _fctName;
			map<FactorT, vector<TimeT>> _timings;

			experiment_impl(string const& factorName) : _fctName(factorName) {}

			// implementation of forwarded functions --------------------
			void print(ostream& os) const
			{
				string token{""};

				os << ", 'factor_name' : '" << _fctName << "'";
				// print the factor list
				os << ", 'factors' : [ ";
				for (auto&& Pair : _timings)
				{
					os << token;
					os << Pair.first;
					token = ", ";
				}
				os << " ]";
				// print the timings
				token.clear();
				os << ", 'timings' : [ ";
				for (auto&& Pair : _timings)
				{
					os << token;
					os << measure_cl<CL>::apply(Pair.second).count();
					token = ", ";
				}
				os << " ]";
			}

		protected:
			~experiment_impl() = default;
		};

		template <class TimeT, central_location CL>
		struct experiment_impl<TimeT, void, CL>
		{
			vector<TimeT> _timings;

			experiment_impl(size_t nSample) : _timings(nSample) {}

			// implementation of forwarded functions --------------------
			void print(ostream& os) const
			{
				string token{""};
				// print the timings
				os << ", 'timings' : [ ";
				for (auto&& elem : _timings)
				{
					os << token;
					os << elem.count();
					token = ", ";
				}
				os << " ]";
			}

		protected:
			~experiment_impl() = default;
		};

		/**
		* @ class experiment
		* @ brief base class to serve as an experiment concept
		*/
		struct experiment
		{
			virtual ~experiment() {}

			// forwarded functions --------------------------------------
			virtual void print(ostream& os) const = 0;
		};

		template <class TimeT, class ClockT>
		struct measure
		{
			template <class F>
			inline static auto duration(F&& callable) ->
			    typename enable_if<!is_same<decltype(callable()),
			                                timeout_ptr<TimeT, ClockT>>::value,
			                       TimeT>::type
			{
				auto start = ClockT::now();
				fw(callable)();
				return duration_cast<TimeT>(ClockT::now() - start);
			}

			template <class F>
			inline static auto duration(F&& callable) ->
			    typename enable_if<is_same<decltype(callable()),
			                               timeout_ptr<TimeT, ClockT>>::value,
			                       TimeT>::type
			{
				auto start = ClockT::now();
				auto tOut = fw(callable)();
				return (duration_cast<TimeT>(ClockT::now() - start)) -
				       tOut->duration();
			}

			template <class F, typename FactorT>
			inline static auto duration(F&& callable, FactorT&& factor) ->
			    typename enable_if<
			        !is_same<decltype(callable(forward<FactorT>(factor))),
			                 timeout_ptr<TimeT, ClockT>>::value,
			        TimeT>::type
			{
				auto start = ClockT::now();
				fw(callable)(forward<FactorT>(factor));
				return duration_cast<TimeT>(ClockT::now() - start);
			}

			template <class F, typename FactorT>
			inline static auto duration(F&& callable, FactorT&& factor) ->
			    typename enable_if<
			        is_same<decltype(callable(forward<FactorT>(factor))),
			                timeout_ptr<TimeT, ClockT>>::value,
			        TimeT>::type
			{
				auto start = ClockT::now();
				auto tOut = fw(callable)(forward<FactorT>(factor));
				return (duration_cast<TimeT>(ClockT::now() - start)) -
				       tOut->duration();
			}
		};

		/**
		* @ class experiment_model
		* @ brief abrastraction for a single sampling process
		*/
		template <
			class TimeT, 
			class ClockT, 
			central_location CL, 
			class FactorT = void
		>
		struct experiment_model final : experiment,
		                                experiment_impl<TimeT, FactorT, CL>
		{
			ccleaner cache_cleaner; 

			// construction - destruction -------------------------------
			template <class F>
			experiment_model(size_t nSample, F&& callable)
			    : experiment_impl<TimeT, void>(nSample)
			{
				for (size_t i = 0; i < nSample; i++)
				{
					cache_cleaner(); 

					experiment_impl<TimeT, FactorT>::_timings[i] =
					    measure<TimeT, ClockT>::duration(fw(callable));
					
					cache_cleaner.rest();
				}
			}

			template <class F>
			experiment_model(size_t nSample, F&& callable,
			                 string const& factorName,
			                 initializer_list<FactorT>&& factors)
			    : experiment_impl<TimeT, FactorT>(factorName)
			{
				for (auto&& factor : factors)
				{
					experiment_impl<TimeT, FactorT>::_timings[factor].reserve(nSample);
					for (size_t i = 0; i < nSample; i++)
					{
						cache_cleaner();

						experiment_impl<TimeT, FactorT>::_timings[factor]
						    .push_back(measure<TimeT, ClockT>::duration(
						        fw(callable), factor));
						
						cache_cleaner.rest();
					}
				}
			}

			template <class F, class It>
			experiment_model(size_t nSample, F&& callable,
			                 string const& factorName, It beg, It fin)
			    : experiment_impl<
			          TimeT, remove_reference_t<decltype(*beg)>, CL>(
			          factorName)
			{
				while (beg != fin)
				{
					experiment_impl<TimeT, FactorT, CL>::_timings[*beg].reserve(nSample);
					for (size_t i = 0; i < nSample; i++)
					{
						cache_cleaner();
					
						experiment_impl<TimeT, FactorT, CL>::_timings[*beg]
						    .push_back(measure<TimeT, ClockT>::duration(
						        fw(callable), *beg));

						cache_cleaner.rest();
					}
					++beg;
				}
			}

			// forwarded functions --------------------------------------
			void print(ostream& os) const override
			{
				experiment_impl<TimeT, FactorT, CL>::print(os);
			}
		};
	} // ~ namespace detail

	/**
	* @ class benchmark
	* @ brief organizes the execution and serialization of timing experiments
	*/
	template <
		class TimeT  = std::chrono::milliseconds,
		class ClockT = std::chrono::steady_clock,
		central_location CL = central_location::median
	>
	class benchmark
	{
		vector<pair<string, unique_ptr<detail::experiment>>> _data;

	public:
		// typedefs -----------------------------------------------------
		using time_t = TimeT; 
		using clock_t = ClockT; 
		using timeout_t = timeout<time_t, clock_t>;
		// construction - destruction -----------------------------------
		benchmark() = default;
		benchmark(benchmark const&) = delete;

		// run experiments ----------------------------------------------
		template <class F>
		void run(string const& name, size_t nSample, F&& callable)
		{
			_data.emplace_back(
			    name, make_unique<detail::experiment_model<TimeT, ClockT>>(
			              nSample, fw(callable)));
		}

		template <class FactorT, class F>
		void run(string const& name, size_t nSample, F&& callable,
		         string const& factorName, initializer_list<FactorT>&& factors)
		{
			_data.emplace_back(
			    name,
			    make_unique<detail::experiment_model<TimeT, ClockT, FactorT, CL>>(
			        nSample, fw(callable), factorName,
			        forward<initializer_list<FactorT>&&>(factors)));
		}

		template <class F, class It>
		void run(string const& name, size_t nSample, F&& callable,
		         string const& factorName, It beg, It fin)
		{
			_data.emplace_back(
			    name, make_unique<detail::experiment_model<
			              TimeT, ClockT, CL,
			              remove_reference_t<decltype(*beg)>>>(
			              nSample, fw(callable), factorName, beg, fin));
		}

		// utilities ----------------------------------------------------
		void print(const char* benchmarkName, ostream& os) const
		{
			for (auto&& Pair : _data)
			{
				os << "{ 'benchmark_name' : '" << benchmarkName << "'";
				os << ", 'experiment_name' : '" << Pair.first << "'";
				os << ", 'time_type' : '" << time_type<TimeT>() << "'";
				Pair.second->print(os);
				os << " } \n";
			}
		}

		void serialize(
			const char* benchmarkName, const char* filename,
			std::ios_base::openmode mode = ofstream::out, 
			std::locale punct = std::locale::classic()) const
		{
			ofstream os;
			os.imbue(punct); 
			os.open(filename, mode);
			for (auto&& Pair : _data)
			{
				os << "{ 'benchmark_name' : '" << benchmarkName << "'";
				os << ", 'experiment_name' : '" << Pair.first << "'";
				os << ", 'time_type' : '" << time_type<TimeT>() << "'";
				Pair.second->print(os);
				os << " } \n";
			}
			os.close();
		}
	};

} // ~ namespace bmk

#undef fw

#endif

