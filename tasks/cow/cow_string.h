#pragma once

#include <string_view>

class CowString {
public:
    struct Buffer {
        char* data;
        std::size_t size_;
        std::size_t ptr_count;

    public:
        Buffer();
        explicit Buffer(const char* str);
        Buffer& operator=(const Buffer& other);
        char& operator[](size_t index);
        const char& operator[](size_t index) const;
        ~Buffer();
    };

    struct StringCharacter {
        std::size_t offset_;
        Buffer** master_buffer_;

    public:
        StringCharacter& operator=(char c);
        operator char() const;
        //~StringCharacter();
    };

    struct ConstStringCharacter {
        std::size_t offset_;
        Buffer* const* master_buffer_;

    public:
        // StringCharacter& operator=(char c);
        operator char() const;
        //~StringCharacter();
    };

    struct CowStringIterator {
        std::size_t offset;
        Buffer** buffer;

    public:
        StringCharacter operator*();
        CowStringIterator& operator++();
        bool operator!=(const CowStringIterator& other) const;
        //~CowStringIterator();
    };

    struct ConstCowStringIterator {
        std::size_t offset;
        Buffer* const* buffer;

    public:
        const ConstStringCharacter operator*() const;
        ConstCowStringIterator& operator++();
        bool operator!=(const ConstCowStringIterator& other) const;
        //~ConstCowStringIterator();
    };

    explicit CowString(std::string_view str);
    CowString(const CowString& other);
    CowString(CowString&& other);
    CowString& operator=(const CowString& other);
    CowString& operator=(CowString&& other);
    char* GetData() const;
    StringCharacter operator[](std::size_t index);
    // const StringCharacter operator[](std::size_t index) ;
    const ConstStringCharacter At(std::size_t index) const;
    CowString& operator+=(const CowString& other);
    CowString operator+(const CowString& other) const;
    bool operator==(const CowString& other) const;
    bool operator!=(const CowString& other) const;
    CowString& operator+=(std::string_view other);
    CowString operator+(std::string_view other) const;
    bool operator==(std::string_view other) const;
    bool operator!=(std::string_view other) const;
    CowStringIterator begin();
    CowStringIterator end();
    const ConstCowStringIterator begin() const;
    const ConstCowStringIterator end() const;
    // const ConstCowStringIterator begin() const;
    // const ConstCowStringIterator end() const;
    operator std::string_view();
    ~CowString();

private:
    Buffer* buffer_;
};

using Buffer = CowString::Buffer;
using Character = CowString::StringCharacter;
using ConstCharacter = CowString::ConstStringCharacter;
using CowStringIterator = CowString::CowStringIterator;
using ConstCowStringIterator = CowString::ConstCowStringIterator;
