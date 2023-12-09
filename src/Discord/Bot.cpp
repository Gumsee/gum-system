#include "Bot.h"
#include <System/Output.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <Essentials/Tools.h>
#include <unistd.h>

using namespace nlohmann;

size_t CurlWriteString(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size * nmemb;
    try                      { s->append((char*)contents, newLength); }
    catch(std::bad_alloc &e) { return 0; }
    return newLength;
}

static int ping(CURL *curl, const char *send_payload)
{
  size_t sent;
  CURLcode result =
    curl_ws_send(curl, send_payload, strlen(send_payload), &sent, 0,
                 CURLWS_PING);
  return (int)result;
}
 
static int recv_pong(CURL *curl, const char *expected_payload)
{
  size_t rlen;
  const struct curl_ws_frame *meta;
  char buffer[256];
  CURLcode result = curl_ws_recv(curl, buffer, sizeof(buffer), &rlen, &meta);
  if(!result) {
    if(meta->flags & CURLWS_PONG) {
      int same = 0;
      fprintf(stderr, "ws: got PONG back\n");
      if(rlen == strlen(expected_payload)) {
        if(!memcmp(expected_payload, buffer, rlen)) {
          fprintf(stderr, "ws: got the same payload back\n");
          same = 1;
        }
      }
      if(!same)
        fprintf(stderr, "ws: did NOT get the same payload back\n");
    }
    else {
      fprintf(stderr, "recv_pong: got %u bytes rflags %x\n", (int)rlen,
              meta->flags);
    }
  }
  fprintf(stderr, "ws: curl_ws_recv returned %u, received %u\n",
          (unsigned int)result, (unsigned int)rlen);
  return (int)result;
}
 
static int recv_any(CURL *curl)
{
  size_t rlen;
  const struct curl_ws_frame *meta;
  char buffer[256];
  CURLcode result = curl_ws_recv(curl, buffer, sizeof(buffer), &rlen, &meta);
  if(result)
    return result;
 
  return 0;
}

Bot::Bot(std::string token)
    : sToken(token)
{
    json appinfo = apiRequest("/applications/@me", "");

    if(appinfo.contains("description"))
        sDescription = appinfo["description"].get<std::string>();

    if(appinfo.contains("bot"))
    {
        json botinfo = appinfo["bot"];

        if(botinfo.contains("username"))
            sName = botinfo["username"].get<std::string>();

        if(botinfo.contains("id"))
            iID = Tools::StringToLong(botinfo["id"].get<std::string>());
    }



    std::string wsuri = "wss://gateway.discord.gg/?v=10&encoding=json";
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, wsuri.c_str());
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); /* websocket style */
    
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else 
        {
            int i = 0;
            do {
                recv_any(curl);
                if(ping(curl, "foobar"))
                    return;

                if(recv_pong(curl, "foobar")) 
                    return;
                sleep(2);
            } while(i++ < 10);


            size_t sent;
            (void)curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
        }
    
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

Bot::~Bot()
{
}

json Bot::apiRequest(std::string endpoint, Gum::Unicode payload)
{
    std::string url = "https://discord.com/api/v10/" + endpoint;

    struct curl_slist *slist1 = NULL;
    slist1 = curl_slist_append(slist1, ("Authorization: Bot " + sToken).c_str());
    slist1 = curl_slist_append(slist1, "Content-Type: application/json; charset=UTF-8");
    slist1 = curl_slist_append(slist1, "Accept: application/json");
    slist1 = curl_slist_append(slist1, "User-Agent: DiscordBot");
    
    std::string retString;
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if(!payload.isEmpty())
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.toString().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &retString);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    CURLcode retCode = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    // throw API errors
    if(retCode != CURLE_OK)
    {
        Gum::Output::error(curl_easy_strerror(retCode));
        return json::parse("");
    }

    return json::parse(retString);
}


Snowflake Bot::getID() const             { return this->iID; }
Gum::Unicode Bot::getUsername() const    { return this->sName; }
Gum::Unicode Bot::getDescription() const { return this->sDescription; }