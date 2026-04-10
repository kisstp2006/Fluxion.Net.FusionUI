#pragma once

namespace Fusion
{
    
    template<typename T, SizeT GrowthIncrement = 128>
    class FStableGrowthArray
    {
        static_assert(GrowthIncrement > 0);
    public:

        static constexpr SizeT IncrementSize = GrowthIncrement;

        FStableGrowthArray()
        {}

        ~FStableGrowthArray()
        {
            Free();
        }

        FStableGrowthArray(const FStableGrowthArray& copy)
        {
            CopyFrom(copy);
        }

        FStableGrowthArray& operator=(const FStableGrowthArray& copy)
        {
            if (this == &copy)
                return *this;

            CopyFrom(copy);
            return *this;
        }

        FStableGrowthArray(FStableGrowthArray&& move) noexcept
        {
            data = move.data;
            count = move.count;
            capacity = move.capacity;

            move.data = nullptr;
            move.count = move.capacity = 0;
        }

        FStableGrowthArray& operator=(FStableGrowthArray&& move) noexcept
        {
            if (this == &move)
                return *this;

            Free();

            data = move.data;
            count = move.count;
            capacity = move.capacity;

            move.data = nullptr;
            move.count = move.capacity = 0;

            return *this;
        }

        typedef T* iterator;
        typedef const T* const_iterator;

        iterator begin() { return data; }
        const_iterator begin() const { return data; }
        iterator end() { return data + count; }
        const_iterator end() const { return data + count; }

        const T& operator[](SizeT index) const
        {
            return data[index];
        }

        T& operator[](SizeT index)
        {
            return data[index];
        }

        const T& First() const
        {
            return data[0];
        }

        T& First()
        {
            return data[0];
        }

        const T& Last() const
        {
            return data[count - 1];
        }

        T& Last()
        {
            return data[count - 1];
        }

        void Free()
        {
            ZoneScoped;

	        if (data != nullptr)
	        {
                delete[] data;
	        }

            data = nullptr;
            capacity = count = 0;
        }

        inline void Grow()
        {
            ZoneScoped;

            Reserve(capacity + GrowthIncrement);
        }

        void Insert(const T& item)
        {
            ZoneScoped;

            T copy = item; // capture before potential reallocation invalidates a self-reference
	        if (data == nullptr || count >= capacity)
	        {
                Grow();
	        }

            data[count++] = std::move(copy);
        }

        void InsertRange(SizeT numItems, const T& value = {})
        {
            ZoneScoped;

            if (data == nullptr || this->count + numItems > capacity)
	        {
                Reserve(capacity + std::max<SizeT>(this->count + numItems, GrowthIncrement));
	        }

            for (SizeT i = 0; i < numItems; i++)
            {
                data[count++] = value;
            }
        }

        void Insert(const T* values, SizeT numItems)
        {
            ZoneScoped;

            if (data == nullptr || this->count + numItems > capacity)
            {
                Reserve(capacity + std::max<SizeT>(this->count + numItems, GrowthIncrement));
            }

            if constexpr (std::is_trivially_copyable_v<T>)
            {
                memcpy(data + count, values, numItems * sizeof(T));
                count += numItems;
            }
            else
            {
                for (SizeT i = 0; i < numItems; i++)
                {
                    data[count++] = values[i];
                }
            }
        }

        void RemoveAll()
        {
            ZoneScoped;

            count = 0;
        }

        void RemoveAt(SizeT index)
        {
            ZoneScoped;

            if (count == 0 || index >= count)
                return;

            if constexpr (std::is_trivially_copyable_v<T>)
                memmove(data + index, data + index + 1, (count - index - 1) * sizeof(T));
            else
                for (SizeT i = index; i < count - 1; ++i)
                    data[i] = std::move(data[i + 1]);

            count--;
        }

        void RemoveLast()
        {
            if (count > 0)
                RemoveAt(count - 1);
        }

        bool IsEmpty() const
        {
	        return count == 0 || data == nullptr;
        }

        SizeT GetCapacity() const
        {
            return capacity;
        }

        SizeT GetCount() const
        {
            return count;
        }

        SizeT GetByteSize() const
        {
            return count * sizeof(T);
        }

        T* GetData() const
        {
            return data;
        }
        
    private:

        void Reserve(SizeT totalElementCapacity)
        {
            ZoneScoped;

            totalElementCapacity = ((totalElementCapacity + GrowthIncrement - 1) / GrowthIncrement) * GrowthIncrement;

            if (capacity < totalElementCapacity)
            {
                T* newData = new T[totalElementCapacity];
                if (data != nullptr)
                {
                    if constexpr (std::is_trivially_copyable_v<T>)
                        memcpy(newData, data, sizeof(T) * count);
                    else
                        for (SizeT i = 0; i < count; ++i)
                            newData[i] = std::move(data[i]);
                    delete[] data;
                }
                data = newData;

                capacity = totalElementCapacity;
            }
        }

        void CopyFrom(const FStableGrowthArray& copy)
        {
            Free();

            Reserve(std::max(copy.capacity, copy.count));

            count = copy.count;
            
            if (count > 0)
            {
                if constexpr (std::is_trivially_copyable_v<T>)
                    memcpy(data, copy.data, count * sizeof(T));
                else
                    for (SizeT i = 0; i < count; ++i)
                        data[i] = copy.data[i];
            }
        }

        T* data = nullptr;
        SizeT capacity = 0;
        SizeT count = 0;
    };

} // namespace Fusion

