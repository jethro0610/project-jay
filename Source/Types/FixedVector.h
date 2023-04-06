#include <integer.hpp>
#include <deque>

template <typename T, uint32_t size>
class FixedVector {
public:
    FixedVector() {
        data_ = new T[size];
        count_ = 0;
    }

    void Append(T element) { 
        if (count_ == size)
            return;
        data_[count_++] = element; 
    }
    void Remove(uint32_t index) { 
        if (index >= count_)
            return;
        data_[index] = data_[count_--]; 
    }
    uint32_t GetCount() const { return count_; }
    T operator[](uint32_t index) const { return data_[index]; };
    T* GetData() { return data_; }

private:
    T* data_; 
    uint32_t count_;
};
