#include "Message.h"
#include <array>
#include <iostream>
#include <cstring>

Message::Message(const std::vector<uint8_t>& message)
{
    std::vector<uint8_t> bytes(message);

    uint8_t formatByte = bytes[0];
    uint8_t typeByte = bytes[1];

    std::array<uint8_t, 16> guidBytes;
    std::copy(bytes.begin() + 3, bytes.begin() + 19, guidBytes.begin());
    Id = *reinterpret_cast<GUID*>(guidBytes.data());

    std::array<uint8_t, 4> messageLengthBytes;
    std::copy(bytes.begin() + 19, bytes.begin() + 23, messageLengthBytes.begin());
    MsgLength = *reinterpret_cast<int*>(messageLengthBytes.data());

    MsgFormat = static_cast<MessageFormat>(formatByte);
    MsgType = static_cast<MessageType>(typeByte);

    MsgBody.assign(bytes.begin() + 22, bytes.end());
}

template <typename T>
bool Message::Deserialize(T& outValue)
{
    try
    {
        std::string jsonStr(MsgBody.begin(), MsgBody.end());
        outValue = nlohmann::json::parse(jsonStr).get<T>();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "JSON deserialization error: " << e.what() << std::endl;
        return false;
    }
}

std::future<bool> Message::SendMessage(HANDLE pipeHandle)
{
    return std::async(std::launch::async, [this, pipeHandle]() {
        std::vector<uint8_t> headerBytes(22);

        headerBytes[0] = static_cast<uint8_t>(MsgFormat);
        headerBytes[1] = static_cast<uint8_t>(MsgType);

        std::array<uint8_t, 16> guidBytes;
        memcpy(guidBytes.data(), &Id, 16);
        std::copy(guidBytes.begin(), guidBytes.end(), headerBytes.begin() + 2);

        std::array<uint8_t, 4> messageLengthBytes;
        memcpy(messageLengthBytes.data(), &MsgLength, 4);
        std::copy(messageLengthBytes.begin(), messageLengthBytes.end(), headerBytes.begin() + 18);

        std::vector<uint8_t> messageBytes;
        messageBytes.insert(messageBytes.end(), headerBytes.begin(), headerBytes.end());
        messageBytes.insert(messageBytes.end(), MsgBody.begin(), MsgBody.end());

        DWORD bytesWritten;
        BOOL result = WriteFile(pipeHandle, messageBytes.data(), messageBytes.size(), &bytesWritten, nullptr);
        return result == TRUE;
        });
}

Message::Message(const std::string& bodyData, MessageType messageType, MessageFormat messageFormat)
{
    MsgBody.assign(bodyData.begin(), bodyData.end());
    MsgType = messageType;
    MsgFormat = messageFormat;
    CoCreateGuid(&Id);
    MsgLength = static_cast<int>(MsgBody.size());
}
