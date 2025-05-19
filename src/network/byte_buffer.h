#ifndef BYTE_BUFFER
#define BYTE_BUFFER

#include <vector>
#include <cstring>

class ByteBuffer {
    public:
        std::vector<char> data;
        size_t read_offset = 0;
    
        // inserts data into the vector in the form of bytes
        template<typename T>
        void put(T value) { 
            static_assert(std::is_trivially_copyable_v<T>, "Only trivial types allowed");
            char* p = reinterpret_cast<char*>(&value); // reinterprets the values bytes to be the type specified (so as a char*)
            data.insert(data.end(), p, p + sizeof(T));
        }
    
        // copies data from vector and puts it into specified type
        template<typename T>
        T read() { 
            static_assert(std::is_trivially_copyable_v<T>, "Only trivial types allowed");
            if (read_offset + sizeof(T) > data.size()) {
                throw "Read past end of buffer";
            }
    
            T value;
            std::memcpy(&value, &data[read_offset], sizeof(T));
            read_offset += sizeof(T);
            return value;
        }
    
        void reset();
        const char* raw() const;
        size_t size() const;
    };

// Inline trivial methods
inline void ByteBuffer::reset() { read_offset = 0; }
inline const char* ByteBuffer::raw() const { return data.data(); }
inline size_t ByteBuffer::size() const { return data.size(); }

#endif
