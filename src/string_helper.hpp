
#pragma once
#include <memory>


namespace string {

class MBStringHelperPrivate;

class MBStringHelper {
public:

    MBStringHelper();

    MBStringHelper(MBStringHelper&&) = delete;
    MBStringHelper(const MBStringHelper&) = delete;
    MBStringHelper& operator=(const MBStringHelper&) = delete;
    MBStringHelper& operator=(MBStringHelper&&) = delete;

    ~MBStringHelper() = default;

    /* ### PUBLIC APIs ### */
    int32_t getLength() const;

private:
    std::shared_ptr<MBStringHelperPrivate> m_mbstringhelper;
};

} // namespace string

