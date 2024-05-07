#pragma once

#include <random>
#include "complexities/complexity_analyzer.h"
#include "libds/adt/priority_queue.h"

namespace ds::utils {

	template <class Container>
	class MyAnalyzer : public ComplexityAnalyzer<Container>
	{
	protected:
		explicit MyAnalyzer(const std::string& name);

		void growToSize(Container& container, size_t size) override;

		[[nodiscard]] auto getRandomData() const -> int;

		const int DEFAULT_SIZE = 100;

		Container create_prototype() override;

	private:
		std::default_random_engine rngData_{144};
		int data_;
	};

	template <class Container>
	class MyAnalyzerPush final : public MyAnalyzer<Container>
	{
	public:
		explicit MyAnalyzerPush(const std::string& name);

	protected:
		void executeOperation(Container& container) override;
	};

	//-------------

	class MyAnalyzers final : public CompositeAnalyzer
	{
	public:
		MyAnalyzers();
	};

	//-------------

	template <class Container>
	MyAnalyzer<Container>::MyAnalyzer(const std::string& name) :
	ComplexityAnalyzer<Container>(name),
	data_(0)
	{
		ComplexityAnalyzer<Container>::registerBeforeOperation(
			[this](Container& container) {
				data_ = rngData_();
			}
		);
	}

	template <class Container>
	void MyAnalyzer<Container>::growToSize(Container& container, size_t size)
	{
		const size_t howMany = size - container.size();
		for (size_t i = 0; i < howMany; i++)
		{
			container.push(rngData_(),rngData_());
		}
	}

	template<class Container>
	int MyAnalyzer<Container>::getRandomData() const
	{
		return data_;
	}

	template <class Container>
	Container MyAnalyzer<Container>::create_prototype()
	{
		if constexpr (std::is_default_constructible_v<Container>) 
		{
			return Container();
		}
		return Container(DEFAULT_SIZE);
	}

	//------

	template <class Container>
	MyAnalyzerPush<Container>::MyAnalyzerPush(const std::string& name) :
	MyAnalyzer<Container>(name)
	{
	}

	template <class Container>
	void MyAnalyzerPush<Container>::executeOperation(Container& container)
	{
		auto data = this->getRandomData();
		container.push(data, data);
	}

	//---------

	inline MyAnalyzers::MyAnalyzers() : CompositeAnalyzer("IDK")
	{
		this->addAnalyzer(std::make_unique<MyAnalyzerPush<adt::TwoLists<int, int>>>("twoLists-push"));
	}

}
