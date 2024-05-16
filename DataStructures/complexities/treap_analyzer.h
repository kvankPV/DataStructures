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
        size_t randomizedKey_;
        size_t index_;
        std::default_random_engine rngIndex_{ static_cast<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(std::chrono::system_clock::now().time_since_epoch().count()) };
        bool keyExists_;

    private:
        std::unordered_set<size_t> usedKeys_;
    	std::default_random_engine rngData_{ static_cast<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(std::chrono::system_clock::now().time_since_epoch().count()) };
        size_t data_;
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
        : ComplexityAnalyzer<Container>(name), index_(0), data_(0)
    {
        ComplexityAnalyzer<Container>::registerBeforeOperation(
            [this](Container&)
            {
                data_ = rngData_();
            }
        );
    }

    template <class Container>
    void TreapAnalyzer<Container>::growToSize(Container& container, const size_t size)
    {
        if (this->usedKeys_.size() > size)
        {
            this->usedKeys_.clear();
        }
        while (container.size() < size)
        {
            size_t key = this->rngData_();
            if (auto [_, inserted] = this->usedKeys_.emplace(key); inserted)
            {
                container.insert(key, key);
            }
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
        TreapAnalyzer<Container>::registerBeforeOperation([this](Container& container)
            {
                size_t key = this->getRandomData();
                if (auto [_, inserted] = this->getUsedKeys().emplace(key); inserted)
                {
                    this->keyExists_ = true;
                }
            });
        TreapAnalyzer<Container>::registerAfterOperation([this](Container&)
        {
                this->keyExists_ = false;
        });
    }

    template <class Container>
    void TreapAnalyzerInsert<Container>::executeOperation(Container& container)
    {
        if (this->keyExists_) {
            container.insert(this->getRandomData(), this->getRandomData());
        }
    }

    template <class Container>
    TreapAnalyzerFind<Container>::TreapAnalyzerFind(const std::string& name)
        : TreapAnalyzer<Container>(name)
    {
        TreapAnalyzer<Container>::registerBeforeOperation(
            [&](Container& container)
            {
                std::uniform_int_distribution<size_t> indexDist(0, container.size() - 1);
                this->index_ = indexDist(this->rngIndex_);
                this->randomizedKey_ = *std::next(this->getUsedKeys().begin(), this->index_);
            });
    }

    template <class Container>
    void TreapAnalyzerFind<Container>::executeOperation(Container& container)
    {
        if (const bool found = container.find(this->randomizedKey_); !found)
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