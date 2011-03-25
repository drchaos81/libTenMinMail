#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/types.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "/home/bert/Source/myString/libMyString.h"

#define mailurl "10minutemail.com"

CURL      *tenminmailcurl;
CURLcode  rescurl;
static int connected;
struct MemoryStruct
    {
    char *memory;
  size_t size;
};

static struct MemoryStruct chunk;

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }

  memcpy(&(mem->memory[mem->size]), ptr, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int tenmail_init()
{
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
 chunk.size = 0;    /* no data at this point */

tenminmailcurl = curl_easy_init();
if (tenminmailcurl)
{
rescurl = curl_easy_setopt(tenminmailcurl,CURLOPT_URL,mailurl);
curl_easy_setopt(tenminmailcurl,CURLOPT_FOLLOWLOCATION,1);
curl_easy_setopt(tenminmailcurl,CURLOPT_AUTOREFERER,0);
struct curl_slist *httpheadertenminemail = NULL;
    httpheadertenminemail = curl_slist_append(httpheadertenminemail, "User-Agent: libTenMail v0.001");
    httpheadertenminemail = curl_slist_append(httpheadertenminemail, "Accept-Language: de-de,en-us");
    httpheadertenminemail = curl_slist_append(httpheadertenminemail, "Accept: text");
    curl_easy_setopt(tenminmailcurl,CURLOPT_HTTPHEADER,httpheadertenminemail);
  /* send all data to this function  */
  curl_easy_setopt(tenminmailcurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(tenminmailcurl, CURLOPT_WRITEDATA, (void *)&chunk);

   return EXIT_SUCCESS;
}
    return EXIT_FAILURE;
}

int tenmail_cleanup()
{
    if(chunk.memory)
free(chunk.memory);
if (tenminmailcurl)
    curl_easy_cleanup(tenminmailcurl);
return EXIT_SUCCESS;
}

int tenmail_connect()
{

    rescurl = curl_easy_perform(tenminmailcurl);
    if(CURLE_OK == rescurl)
    {
        printf("CURL CONNECTED\n");

  printf("%lu bytes retrieved\n", (long)chunk.size);

          connected = 1;
        return EXIT_SUCCESS;
    }
    printf("CURL CONNECT ERROR");
    connected = 0;
    return EXIT_FAILURE;
}
