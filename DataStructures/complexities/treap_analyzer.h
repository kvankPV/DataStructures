#pragma once
#include <random>
#include <unordered_set>

#include "complexities/complexity_analyzer.h"
#include "libds/adt/table.h"

namespace ds::utils {

    template <class Container>
    class TreapAnalyzer : public ComplexityAnalyzer<Container>
    {
    protected:
        explicit TreapAnalyzer(const std::string& name);

        void growToSize(Container& container, size_t size) override;

        [[nodiscard]] auto getRandomData() const -> size_t;
        [[nodiscard]] size_t getRandomIndex() const;
        std::unordered_set<size_t>& getUsedKeys() { return this->usedKeys_; }

    private:
        std::unordered_set<size_t> usedKeys_;
    	std::default_random_engine rngData_{ static_cast<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(std::chrono::system_clock::now().time_since_epoch().count()) };
        size_t data_;
        std::default_random_engine rngIndex_{ static_cast<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(std::chrono::system_clock::now().time_since_epoch().count()) };
        size_t index_;
    };

    template <class Container>
    class TreapAnalyzerInsert final : public TreapAnalyzer<Container>
    {
    public:
        explicit TreapAnalyzerInsert(const std::string& name);

    protected:
        void executeOperation(Container& container) override;
    };

    template <class Container>
    class TreapAnalyzerFind final : public TreapAnalyzer<Container>
    {
    public:
        explicit TreapAnalyzerFind(const std::string& name);

    protected:
        void executeOperation(Container& container) override;
    };

    class TreapAnalyzers final : public CompositeAnalyzer
    {
    public:
        TreapAnalyzers();
    };

    template <class Container>
    TreapAnalyzer<Container>::TreapAnalyzer(const std::string& name)
        : ComplexityAnalyzer<Container>(name), data_(0), index_(0)
    {
        ComplexityAnalyzer<Container>::registerBeforeOperation(
            [this](Container& container)
            {
                std::uniform_int_distribution<size_t> indexDist(0, container.size() - 1);
                index_ = indexDist(rngIndex_);
                data_ = rngData_();
            }
        );
    }

    template <class Container>
    void TreapAnalyzer<Container>::growToSize(Container& container, size_t size)
    {
        std::unordered_set<size_t> tempSet;
        tempSet.reserve(size);
        const size_t containerSize = container.size();
        const size_t usedKeysSize = usedKeys_.size();
        while (container.size() < size)
        {
            if (size_t key = rngData_(); this->usedKeys_.find(key) == this->usedKeys_.end()
                && tempSet.emplace(key).second)
            {
                container.insert(key, key);
            }
        }

        if (containerSize != usedKeysSize)
        {
            this->usedKeys_ = std::move(tempSet);
        } else
        {
            this->usedKeys_.insert(tempSet.begin(), tempSet.end());
        }
    }

    template <class Container>
    size_t TreapAnalyzer<Container>::getRandomData() const
    {
        return data_;
    }

    template <class Container>
    size_t TreapAnalyzer<Container>::getRandomIndex() const
    {
        return index_;
    }

    template <class Container>
    TreapAnalyzerInsert<Container>::TreapAnalyzerInsert(const std::string& name)
        : TreapAnalyzer<Container>(name)
    {
    }

    template <class Container>
    void TreapAnalyzerInsert<Container>::executeOperation(Container& container)
    {
	    if (size_t key = this->getRandomData(); this->getUsedKeys().emplace(key).second)
        {
            container.insert(key, key);
        }
    }

    template <class Container>
    TreapAnalyzerFind<Container>::TreapAnalyzerFind(const std::string& name)
        : TreapAnalyzer<Container>(name)
    {
    }

    template <class Container>
    void TreapAnalyzerFind<Container>::executeOperation(Container& container)
    {
        auto& usedKeys = this->getUsedKeys();
        if (usedKeys.empty())
        {
            return;
        }

        auto vi = std::next(usedKeys.begin(), this->getRandomIndex());
        if (const bool found = container.find(*vi); !found)
        {
            throw std::runtime_error("No such key!");
        }
    }


    inline TreapAnalyzers::TreapAnalyzers() : CompositeAnalyzer("Treap")
    {
        this->addAnalyzer(std::make_unique<TreapAnalyzerInsert<adt::Treap<size_t, size_t>>>("treap-insert"));
        this->addAnalyzer(std::make_unique<TreapAnalyzerFind<adt::Treap<size_t, size_t>>>("treap-find"));
    }
}