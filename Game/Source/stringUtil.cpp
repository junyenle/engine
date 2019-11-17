#include "stdafx.h"
#include "stringUtil.h"
#include <locale>
#include <codecvt>

namespace StringUtil
{
    void String2WString(std::wstring &output, const std::string &input)
    {
        output = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(input.data());
    }
}