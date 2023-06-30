#pragma once
#include <deque>

template <typename T, uint32_t size>
class FixedVector {
public:
    FixedVector() {
        data_ = new T[size];
        count_ = 0;
    }
    ~FixedVector() {
        delete[] data_;
    }
    FixedVector(const FixedVector&) = delete;

    // Returns the index of the newly added element
    uint32_t Append(T element) { 
        assert(count_ < size);
        data_[count_++] = element; 
        return count_ - 1;
    }
    // Returns new value at the deleted index
    T Remove(uint32_t index) { 
        assert(index < count_);
        data_[index] = data_[--count_]; 
        return data_[index];
    }
    void Clear() {
        count_ = 0;
    }
    uint32_t GetCount() const { return count_; }
    T& operator[](uint32_t index) const { return data_[index]; };
    T* GetData() const { return data_; };
    void CopyFrom(FixedVector& source) {
        memcpy(data_, source.data_, sizeof(T) * source.count_);
        count_ = source.count_;
    };

private:
    T* data_; 
    uint32_t count_;
};
