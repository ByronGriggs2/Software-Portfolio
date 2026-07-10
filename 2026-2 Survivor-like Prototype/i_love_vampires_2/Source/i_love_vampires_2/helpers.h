#pragma once

namespace helpers {
    bool nearEq(float left, float right);
    inline bool isInvalidData(float data) { return nearEq(data, -999); }
    inline bool isInvalidData(int data) { return data == -999; }

    struct MemberPtrHash {
        template<typename T>
        size_t operator()(T ptr) const noexcept {
            const unsigned char* data =
                reinterpret_cast<const unsigned char*>(&ptr);

            size_t hash = 0;
            for (size_t i = 0; i < sizeof(T); ++i) {
                hash = hash * 131 + data[i];
            }
            return hash;
        }
    };
}