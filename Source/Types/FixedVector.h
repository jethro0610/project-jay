#pragma once
#include <deque>

template <typename T, int size>
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
    int Append(T element) { 
        assert(count_ < size);
        data_[count_++] = element; 
        return count_ - 1;
    }
    // Returns new value at the deleted index
    T Remove(int index) { 
        assert(index < count_);
        data_[index] = data_[--count_]; 
        return data_[index];
    }
    void Clear() {
        count_ = 0;
    }
    int GetCount() const { return count_; }
    T& operator[](int index) const { return data_[index]; };
    T* GetData() const { return data_; };
    void CopyFrom(FixedVector& source) {
        memcpy(data_, source.data_, sizeof(T) * source.count_);
        count_ = source.count_;
    };

private:
    T* data_; 
    int count_;
};
