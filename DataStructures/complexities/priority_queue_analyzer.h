#pragma once

#include <random>
#include "complexities/complexity_analyzer.h"
#include "libds/adt/priority_queue.h"

namespace ds::utils {

	template <class Container>
	class PriorityQueueAnalyzer : public ComplexityAnalyzer<Container>
	{
	protected:
		explicit PriorityQueueAnalyzer(const std::string& name);

		void growToSize(Container& container, size_t size) override;

		[[nodiscard]] auto getRandomData() const -> int;

		static constexpr int DEFAULT_SIZE = 1000000;

		Container create_prototype() override;

	private:
		std::default_random_engine rngData_{ static_cast<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(std::chrono::system_clock::now().time_since_epoch().count()) };
		int data_;
	};

	//---

	template <class Container>
	class PriorityQueueAnalyzerPush final : public PriorityQueueAnalyzer<Container>
	{
	public:
		explicit PriorityQueueAnalyzerPush(const std::string& name);

	protected:
		void executeOperation(Container& container) override;
	};

	//---

	template <class Container>
	class PriorityQueueAnalyzerPop final : public PriorityQueueAnalyzer<Container>
	{
	public:
		explicit PriorityQueueAnalyzerPop(const std::string& name);

	protected:
		void executeOperation(Container& container) override;
	};

	//---

	//-------------

	class PriorityQueueAnalyzers final : public CompositeAnalyzer
	{
	public:
		PriorityQueueAnalyzers();
	};

	//-------------

	template <class Container>
	PriorityQueueAnalyzer<Container>::PriorityQueueAnalyzer(const std::string& name) :
	ComplexityAnalyzer<Container>(name),
	data_(0)
	{
		ComplexityAnalyzer<Container>::registerBeforeOperation(
			[this](Container&) {
				data_ = rngData_();
			}
		);
	}

	template <class Container>
	void PriorityQueueAnalyzer<Container>::growToSize(Container& container, size_t size)
	{
		const size_t howMany = size - container.size();
		for (size_t i = 0; i < howMany; i++)
		{
			container.push(rngData_(),rngData_());
		}
	}

	template<class Container>
	int PriorityQueueAnalyzer<Container>::getRandomData() const
	{
		return data_;
	}

	template <class Container>
	Container PriorityQueueAnalyzer<Container>::create_prototype()
	{
		if constexpr (std::is_default_constructible_v<Container>) 
		{
			return Container();
		}
		return Container(DEFAULT_SIZE);
	}

	//------

	template <class Container>
	PriorityQueueAnalyzerPush<Container>::PriorityQueueAnalyzerPush(const std::string& name) :
	PriorityQueueAnalyzer<Container>(name)
	{
	}

	template <class Container>
	void PriorityQueueAnalyzerPush<Container>::executeOperation(Container& container)
	{
		auto data = this->getRandomData();
		container.push(data, data);
	}

	//---------

	//--
	template <class Container>
	PriorityQueueAnalyzerPop<Container>::PriorityQueueAnalyzerPop(const std::string& name) :
	PriorityQueueAnalyzer<Container>(name)
	{
	}

	template <class Container>
	void PriorityQueueAnalyzerPop<Container>::executeOperation(Container& container)
	{
		container.pop();
	}

	//--

	inline PriorityQueueAnalyzers::PriorityQueueAnalyzers() : CompositeAnalyzer("Priority-Queue")
	{
		this->addAnalyzer(std::make_unique<PriorityQueueAnalyzerPush<adt::TwoLists<int, int>>>("twoLists-push"));
		this->addAnalyzer(std::make_unique<PriorityQueueAnalyzerPop<adt::TwoLists<int, int>>>("twoLists-pop"));
	}
}
