#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>

namespace ds::amt {

    template<typename BlockType>
    class ExplicitSequence :
        public Sequence<BlockType>,
        public ExplicitAMS<BlockType>
    {
    public:
        ExplicitSequence();
        ExplicitSequence(const ExplicitSequence& other);
        ~ExplicitSequence() override;

        AMT& assign(const AMT& other) override;
        void clear() override;
        bool equals(const AMT& other) override;

        size_t calculateIndex(BlockType& data) override;

        BlockType* accessFirst() const override;
        BlockType* accessLast() const override;
        BlockType* access(size_t index) const override;
        BlockType* accessNext(const BlockType& block) const override;
        BlockType* accessPrevious(const BlockType& block) const override;

        BlockType& insertFirst() override;
        BlockType& insertLast() override;
        BlockType& insert(size_t index) override;
        BlockType& insertAfter(BlockType& block) override;
        BlockType& insertBefore(BlockType& block) override;

        void removeFirst() override;
        void removeLast() override;
        void remove(size_t index) override;
        void removeNext(const BlockType& block) override;
        void removePrevious(const BlockType& block) override;

    protected:
        virtual void connectBlocks(BlockType* previous, BlockType* next);
        virtual void disconnectBlock(BlockType* block);

        BlockType* first_;
        BlockType* last_;

    public:
        using DataType = typename BlockType::DataT;

        class ExplicitSequenceIterator
        {
        public:
            explicit ExplicitSequenceIterator(BlockType* position);
            ExplicitSequenceIterator(const ExplicitSequenceIterator& other);
            ExplicitSequenceIterator& operator++();
            ExplicitSequenceIterator operator++(int);
            bool operator==(const ExplicitSequenceIterator& other) const;
            bool operator!=(const ExplicitSequenceIterator& other) const;
            DataType& operator*();

        private:
            BlockType* position_;
        };

        ExplicitSequenceIterator begin();
        ExplicitSequenceIterator end();

        using IteratorType = ExplicitSequenceIterator;
    };

    template<typename BlockType>
    using ES = ExplicitSequence<BlockType>;

    //----------

    template<typename DataType>
    struct SinglyLinkedSequenceBlock :
        public MemoryBlock<DataType>
    {

        SinglyLinkedSequenceBlock() : next_(nullptr) {}
        ~SinglyLinkedSequenceBlock() { next_ = nullptr; }

        SinglyLinkedSequenceBlock<DataType>* next_;
    };
    template<typename DataType>
    using SLSBlock = SinglyLinkedSequenceBlock<DataType>;

    template<typename DataType>
    class SinglyLinkedSequence :
        public ES<SLSBlock<DataType>>
    {
    public:
        using BlockType = SinglyLinkedSequenceBlock<DataType>;
    };

    template<typename DataType>
    using SinglyLS = SinglyLinkedSequence<DataType>;

    template<typename DataType>
    class SinglyCyclicLinkedSequence :
        public SinglyLS<DataType>
    {
    };

    template<typename DataType>
    using SinglyCLS = SinglyCyclicLinkedSequence<DataType>;

    //----------

    template<typename DataType>
    struct DoublyLinkedSequenceBlock :
        public SLSBlock<DataType>
    {

        DoublyLinkedSequenceBlock() : previous_(nullptr) {}
        ~DoublyLinkedSequenceBlock() { previous_ = nullptr; }

        DoublyLinkedSequenceBlock<DataType>* previous_;
    };

    template<typename DataType>
    using DLSBlock = DoublyLinkedSequenceBlock<DataType>;

    template<typename DataType>
    class DoublyLinkedSequence :
        public ES<DLSBlock<DataType>>
    {
    public:
        using BlockType = DLSBlock<DataType>;

        BlockType* access(size_t index) const override;
    	BlockType* accessPrevious(const BlockType& block) const override;

        void removeFirst() override;

    protected:
        void connectBlocks(BlockType* previous, BlockType* next) override;
        void disconnectBlock(BlockType* block) override;
    };

    template<typename DataType>
    using DoublyLS = DoublyLinkedSequence<DataType>;

    //----------

    template<typename DataType>
    class DoublyCyclicLinkedSequence :
        public DoublyLS<DataType>
    {
    };

    template<typename DataType>
    using DoublyCLS = DoublyCyclicLinkedSequence<DataType>;

    //----------

    template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence() :
        first_(nullptr),
        last_(nullptr)
    {
    }

    template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence(const ExplicitSequence& other) :
        ExplicitSequence()
    {
        this->assign(other);
    }

    template<typename BlockType>
    ExplicitSequence<BlockType>::~ExplicitSequence()
    {
	    ExplicitSequence<BlockType>::clear();
    }

    template<typename BlockType>
    AMT& ExplicitSequence<BlockType>::assign(const AMT& other)
    {
        if (this != &other)
        {
            this->clear();

            const ExplicitSequence<BlockType>& otherExplicitSequence = dynamic_cast<const ExplicitSequence<BlockType>&>(other);
            otherExplicitSequence.processAllBlocksForward([&](const BlockType* b)
            {
                this->insertLast().data_ = b->data_;
            });
        }

        return *this;
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::clear()
    {
        this->last_ = this->first_;
        while (this->first_ != nullptr)
        {
            this->first_ = accessNext(*first_);
            this->memoryManager_->releaseMemory(this->last_);
            this->last_ = this->first_;
        }

        /*
         * while (akt != nullptr) {
         *  auto p = accessNext(akt);
         *  mm::MemoryManager<BlockType>::releaseMemory(akt);
         *  akt = p;
         * }
         */
    }

    template<typename BlockType>
    bool ExplicitSequence<BlockType>::equals(const AMT& other)
    {
        const ES<BlockType>* other_es = dynamic_cast<const ES<BlockType>*>(&other);
        if (other_es == nullptr)
        {
            return false;
        }
        if (this == other_es)
        {
            return true;
        }
        if (this->size() != other_es->size())
        {
            return false;
        }

        auto this_first = this->first_;
        auto other_first = other_es->first_;

        while (this_first != nullptr)
        {
	        if (this_first->data_ != other_first->data_)
	        {
                return false;
	        }

        	this_first = accessNext(*this_first);
        	other_first = other_es->accessNext(*other_first);
        }
        return true;
    }

    template<typename BlockType>
    size_t ExplicitSequence<BlockType>::calculateIndex(BlockType& data)
    {
        size_t result = 0;

        auto block = this->findBlockWithProperty(
            [&](BlockType* b)
            {
                result++;
                return b == &data;
            }
    	);

        if (block != nullptr)
        {
            return result - 1;
        }
    	return INVALID_INDEX;
        
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessFirst() const
    {
        return first_;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessLast() const
    {
        return last_;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::access(size_t index) const
    {
        BlockType* result = nullptr;
        if (index < this->size())
        {
            result = this->first_;
			for (size_t i = 0; i < index; ++i)
			{
                result = static_cast<BlockType*>(result->next_);
			}
        }
        return result;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessNext(const BlockType& block) const
    {
        return static_cast<BlockType*>(block.next_);
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessPrevious(const BlockType& block) const
    {
        return this->findBlockWithProperty([&](BlockType* b)
            {
                return b->next_ == &block;
            });
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertFirst()
    {
        if (this->size() == 0)
        {
            this->first_ = this->last_ = this->memoryManager_->allocateMemory();
            return *this->first_;
        }
        return this->insertBefore(*this->first_);
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertLast()
    {
        if (this->size() == 0)
        {
            this->first_ = this->last_ = this->memoryManager_->allocateMemory();
            return *this->last_;
        }
        return this->insertAfter(*this->last_);
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insert(size_t index)
    {
        return index == 0
            ? this->insertFirst()
            : index == this->size()
                ? this->insertLast()
                : this->insertAfter(*this->access(index - 1));
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertAfter(BlockType& block)
    {
        auto next_block = this->accessNext(block);
        auto new_block = this->memoryManager_->allocateMemory();
        this->connectBlocks(&block, new_block);
        this->connectBlocks(new_block, next_block);
        if (this->last_ == &block)
        {
            this->last_ = new_block;
        }
        return *new_block;
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertBefore(BlockType& block)
    {
        auto previous_block = this->accessPrevious(block);
        auto new_block = this->memoryManager_->allocateMemory();
        this->connectBlocks(previous_block, new_block);
        this->connectBlocks(new_block, &block);
        if (this->first_ == &block)
        {
            this->first_ = new_block;
        }
        return *new_block;
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeFirst()
    {
        if (this->first_ == this->last_)
        {
            this->memoryManager_->releaseMemory(this->first_);
            this->first_ = this->last_ = nullptr;
        } else
        {
            auto new_first = this->accessNext(*this->first_);
            this->memoryManager_->releaseMemory(this->first_);
            this->first_ = new_first;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeLast()
    {
        if (this->first_ == this->last_)
        {
            this->memoryManager_->releaseMemory(this->last_);
            this->first_ = this->last_ = nullptr;
        } else
        {
            auto new_last = this->accessPrevious(*this->last_);
            this->memoryManager_->releaseMemory(this->last_);
            this->last_ = new_last;
            this->last_->next_ = nullptr;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::remove(const size_t index)
    {
        if (index == 0)
        {
            this->removeFirst();
        }
        else
        {
            this->removeNext(*this->access(index - 1));
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeNext(const BlockType& block)
    {
        auto removed_block = this->accessNext(block);
        if (removed_block == this->last_)
        {
            this->removeLast();
        } else
        {
            this->disconnectBlock(removed_block);
            this->memoryManager_->releaseMemory(removed_block);
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removePrevious(const BlockType& block)
    {
        auto removed_block = this->accessPrevious(block);
        if (removed_block == this->first_)
        {
            this->removeFirst();
        } else
        {
            this->disconnectBlock(removed_block);
            this->memoryManager_->releaseMemory(removed_block);
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::connectBlocks(BlockType* previous, BlockType* next)
    {
        if (previous != nullptr)
        {
            previous->next_ = next;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::disconnectBlock(BlockType* block)
    {
        this->connectBlocks(this->accessPrevious(*block), this->accessNext(*block));
        block->next_ = nullptr;
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(BlockType* position) :
        position_(position)
    {
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(
        const ExplicitSequenceIterator& other
    ) :
        position_(other.position_)
    {
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++()
    {
        position_ = static_cast<BlockType*>(position_->next_);
        return *this;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++(int)
    {
        ExplicitSequenceIterator tmp(*this);
    	this->operator++();
        return tmp;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator==(const ExplicitSequenceIterator& other) const
    {
        return position_ == other.position_;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator!=(const ExplicitSequenceIterator& other) const
    {
        return !(*this == other);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::DataType& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*()
    {
        return position_->data_;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::begin()
    {
        return ExplicitSequenceIterator(first_);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::end()
    {
        return ExplicitSequenceIterator(nullptr);
    }

    template<typename DataType>
    typename DoublyLinkedSequence<DataType>::BlockType* DoublyLinkedSequence<DataType>::access(size_t index) const
    {
        BlockType* result = nullptr;
        if (index < this->size())
        {
	        if (index < this->size()/2)
	        {
                result = this->first_;
                for (size_t i = 0; i < index; ++i)
                {
                    result = static_cast<BlockType*>(result->next_);
                }
	        } else
	        {
                result = this->last_;
                for (size_t i = this->size() - index; i > 1; --i)
                {
                    result = static_cast<BlockType*>(result->previous_);
                } 
	        }
        }
        return result;
    }

    template<typename DataType>
    typename DoublyLinkedSequence<DataType>::BlockType* DoublyLinkedSequence<DataType>::accessPrevious(const BlockType& block) const
    {
        return block.previous_;
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::removeFirst()
    {
        ExplicitSequence<BlockType>::removeFirst();

        if (ExplicitSequence<BlockType>::first_ != nullptr)
        {
            ExplicitSequence<BlockType>::first_->previous_ = nullptr;
        }
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::connectBlocks(BlockType* previous, BlockType* next)
    {
        ExplicitSequence<BlockType>::connectBlocks(previous, next);

        if (next != nullptr)
        {
            next->previous_ = const_cast<BlockType*>(previous);
        }
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::disconnectBlock(BlockType* block)
    {
        ExplicitSequence<BlockType>::disconnectBlock(block);
        block->previous_ = nullptr;
    }

}