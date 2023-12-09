#pragma once
#include <Essentials/Unicode.h>
#include <Network/Client/TCPClient.h>
#include "Snowflake.h"
#include <json.h>
#include <curl/curl.h>

class Bot
{
private:
    std::string sToken;
    Gum::Unicode sName;
    Gum::Unicode sDescription;
    Snowflake iID;
    
    nlohmann::json apiRequest(std::string endpoint, Gum::Unicode payload);

public:
    Bot(std::string token);
    ~Bot();

    Snowflake getID() const;
    Gum::Unicode getUsername() const;
    Gum::Unicode getDescription() const;
};