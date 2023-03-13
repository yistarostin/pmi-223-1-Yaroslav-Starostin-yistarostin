#include "cow_string.h"

#include <cstring>

Character::operator char() const {
    return (**master_buffer_)[offset_];
}

ConstCharacter::operator char() const {
    return (**master_buffer_)[offset_];
}

char* CowString::GetData() const {
    return buffer_->data;
}

CowString::CowString(const CowString& other) {
    buffer_ = other.buffer_;
    ++buffer_->ptr_count;
}

CowString::CowString(CowString&& other) {
    buffer_ = other.buffer_;
    // TODO: somehow destroy Buffer somehow (mb just with automaticall call ~)
    //--other.data.ptr_count;
}

CowString& CowString::operator=(const CowString& other) {
    delete buffer_;
    buffer_ = other.buffer_;  // TODO ensure Buffer::operator= is not dogshit
    ++buffer_->ptr_count;
    return *this;
}

CowString& CowString::operator=(CowString&& other) {
    // TODO: is delete buffer_; needed
    buffer_ = other.buffer_;
    // other.data = nullptr; TODO: same as CowString::CowString(CowString&& other)
    return *this;
}

Buffer::Buffer() : data(nullptr), size_(0), ptr_count(1) {
}

Buffer::Buffer(const char* str) {
    size_ = std::strlen(str);
    data = new char[size_ + 1];
    ptr_count = 1;
    std::strcpy(data, str);
}

Buffer::~Buffer() {
    --ptr_count;
    if (ptr_count == 0) {
        delete[] data;
    }
}

char& Buffer::operator[](size_t index) {
    return data[index];
}

const char& Buffer::operator[](size_t index) const {
    return data[index];
}

Buffer& Buffer::operator=(const Buffer& other) {
    data = other.data;
    ++ptr_count;
    size_ = other.size_;
    return *this;
}

Character CowStringIterator::operator*() {
    return Character{.offset_ = offset, .master_buffer_ = buffer};
}

CowStringIterator& CowStringIterator::operator++() {
    ++offset;
    return *this;
}

bool CowStringIterator::operator!=(const CowStringIterator& other) const {
    return other.buffer != this->buffer || other.offset != this->offset;
}

Character& Character::operator=(char c) {  // FIXME: definitely contains a bug
    if ((*master_buffer_)->data[offset_] == c) {
        return *this;
    }
    if ((*master_buffer_)->ptr_count != 1) {
        (*master_buffer_)->~Buffer();
        *master_buffer_ = new Buffer((*master_buffer_)->data);
    }
    (*master_buffer_)->data[offset_] = c;
    return *this;
}

CowStringIterator CowString::begin() {
    return CowStringIterator{.offset = 0, .buffer = &buffer_};
}

CowStringIterator CowString::end() {
    return CowStringIterator{.offset = buffer_->size_, .buffer = &buffer_};
}

const ConstCowStringIterator CowString::begin() const {
    return ConstCowStringIterator{.offset = 0, .buffer = &buffer_};
}

const ConstCowStringIterator CowString::end() const {
    return ConstCowStringIterator{.offset = buffer_->size_, .buffer = &buffer_};
}

CowString& CowString::operator+=(const CowString& other) {
    char* temp = new char[buffer_->size_ + other.buffer_->size_];
    std::strcpy(temp, buffer_->data);
    std::strcpy(temp + buffer_->size_, other.buffer_->data);
    delete buffer_;
    buffer_ = new Buffer(temp);
    return *this;
}

CowString CowString::operator+(const CowString& other) const {
    CowString res{*this};
    res += other;
    return res;
}

bool CowString::operator==(const CowString& other) const {
    return std::strcmp(GetData(), other.GetData()) == 0;
}

bool CowString::operator!=(const CowString& other) const {
    return std::strcmp(GetData(), other.GetData()) != 0;
}

const CowString::ConstStringCharacter CowString::At(std::size_t index) const {
    return ConstStringCharacter{.offset_ = index, .master_buffer_ = &buffer_};
}

CowString::StringCharacter CowString::operator[](std::size_t index) {
    return StringCharacter{.offset_ = index, .master_buffer_ = &buffer_};
}

CowString::CowString(std::string_view str) : buffer_(new Buffer(str.data())) {
}

CowString::operator std::string_view() {
    return std::string_view(buffer_->data);
}

CowString& CowString::operator+=(std::string_view other) {
    char* temp = new char[buffer_->size_ + other.size() + 1];
    std::strcpy(temp, buffer_->data);
    std::strcat(temp, other.data());
    size_t count = buffer_->ptr_count;
    if (count == 1) {
        delete buffer_;
    } else {
        buffer_->~Buffer();
    }
    buffer_ = new Buffer(temp);
    delete[] temp;
    return *this;
}

CowString CowString::operator+(std::string_view other) const {
    CowString res{*this};
    res += other;
    return res;
}

bool CowString::operator==(std::string_view other) const {
    auto res = strcmp(buffer_->data, other.data()) == 0;
    return res;
}

bool CowString::operator!=(std::string_view other) const {
    return strcmp(buffer_->data, other.data()) != 0;
}

bool CowString::ConstCowStringIterator::operator!=(const ConstCowStringIterator& other) const {
    return other.buffer != this->buffer || other.offset != this->offset;
}

ConstCowStringIterator& CowString::ConstCowStringIterator::operator++() {
    ++offset;
    return *this;
}

const ConstCharacter ConstCowStringIterator::operator*() const {
    return ConstStringCharacter{.offset_ = offset, .master_buffer_ = buffer};
}

CowString::~CowString() {
    size_t count = buffer_->ptr_count;
    if (count == 1) {
        delete buffer_;
    } else {
        buffer_->~Buffer();
    }
}
