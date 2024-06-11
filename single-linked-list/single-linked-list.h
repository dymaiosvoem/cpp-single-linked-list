#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
    friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);

            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator old_copy(node_);
            ++(*this);
            return old_copy;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);

            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);

            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);

        Node* current = pos.node_;
        Node* next_node = current->next_node;
        Node* new_node = new Node(value, next_node);
        current->next_node = new_node;
        ++size_;
        return Iterator{new_node};
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);

        Node* current = pos.node_;
        Node* to_delete = current->next_node;
        Node* next_node = current->next_node->next_node;
        delete to_delete;
        --size_;
        current->next_node = next_node;
        return Iterator{current->next_node};
    }

    SingleLinkedList() : size_(0) {
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        CopyAndSwap(values);
    }

    SingleLinkedList(const SingleLinkedList& other)  {
        CopyAndSwap(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList copy_rhs(rhs);
            swap(copy_rhs);
        }
        return *this;
    }
   
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }


    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0 ? true : false;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        if (size_ != 0) {
            Node* next_node = head_.next_node->next_node;
            delete head_.next_node;
            --size_;
            head_.next_node = next_node;
        }
    }

    void Clear() noexcept {
        while (head_.next_node) {
            Node* next_node = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next_node;
            --size_;
        }
    }

    ~SingleLinkedList() {
        Clear();
    }
private: 
    template <typename Container>
    void CopyAndSwap(Container& elements) {
        SingleLinkedList result;
        auto insert_after_it = before_begin();

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            result.InsertAfter(insert_after_it, *it);
            ++insert_after_it;
        }
        swap(result);
    }

    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
} 