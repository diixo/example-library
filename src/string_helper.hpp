
#pragma once
#include <memory>


namespace string {

class StringHelperPrivate;

class StringHelper {
public:

    StringHelper();

    StringHelper(StringHelper&&) = delete;
    StringHelper(const StringHelper&) = delete;
    StringHelper& operator=(const StringHelper&) = delete;
    StringHelper& operator=(StringHelper&&) = delete;

    ~StringHelper() = default;

    /* ### PUBLIC APIs ### */
    int32_t getLength() const;

private:
    std::shared_ptr<StringHelperPrivate> m_stringhelper;
};

} // namespace string

