#include <curl/curl.h>

static struct CurlGlobal {
 CurlGlobal() { curl_global_init(CURL_GLOBAL_ALL); }
 ~CurlGlobal() { curl_global_cleanup(); }
} init;

int main() {

  return 0;
}
