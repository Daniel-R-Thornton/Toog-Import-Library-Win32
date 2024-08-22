#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>
#include <guiddef.h>
#include <Windows.h>
#include <optional>
#include <future>
#include <nlohmann/json.hpp>

enum class MessageFormat
{
    None,
    utf8,
    image_bmp,
    image_png,
    image_jpg,
    raw,
};

enum class MessageType
{
    none = 0,
    startImport = 1,
    lineItem = 2,
    endImport = 3,
    queryAccounts = 4,
    queryJobs = 5,
    getImportTypes = 6,
};

class Message
{
public:
    MessageFormat MsgFormat;
    MessageType MsgType;
    GUID Id;
    int MsgLength;
    std::vector<uint8_t> MsgBody;

    Message(const std::vector<uint8_t>& message);

   
    template<typename T>
    bool Deserialize(T& outValue); 
    std::future<bool> SendMessage(HANDLE pipeHandle);

    Message(const std::string& bodyData, MessageType messageType, MessageFormat messageFormat);
};

#endif // MESSAGE_H
