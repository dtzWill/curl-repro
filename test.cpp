#include <iostream>
#include <vector>

#include <cstdlib>

#include <curl/curl.h>

static struct CurlGlobal {
 CurlGlobal() { curl_global_init(CURL_GLOBAL_ALL); }
 ~CurlGlobal() { curl_global_cleanup(); }
} init;

CURL * makeHandle(const std::string & URL) {
  CURL *c = curl_easy_init();

  // curl_easy_reset(c);

  curl_easy_setopt(c, CURLOPT_URL, URL.c_str());
  curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(c, CURLOPT_MAXREDIRS, 10L);
  curl_easy_setopt(c, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt(c, CURLOPT_USERAGENT, "curl/" LIBCURL_VERSION " NixRepro");
  curl_easy_setopt(c, CURLOPT_PIPEWAIT, 1L);
  curl_easy_setopt(c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

  curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0);

  curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT, 0L);

  curl_easy_setopt(c, CURLOPT_LOW_SPEED_LIMIT, 1L);
  curl_easy_setopt(c, CURLOPT_LOW_SPEED_TIME, 300L);

  return c;
}

int main() {

  auto curlm = curl_multi_init();

  curl_multi_setopt(curlm, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
  curl_multi_setopt(curlm, CURLMOPT_MAX_TOTAL_CONNECTIONS, 25L);



  std::vector<CURL *> handles;

  handles.push_back(makeHandle("https://cache.nixos.org/3cjpslnfh6v1yq28vzw09i313v0wk0d2.narinfo"));
  handles.push_back(makeHandle("https://cache.nixos.org/r4h24d8afw7n0vsh0sfg2ha0cfkb5cyv.narinfo'"));
  handles.push_back(makeHandle("https://cache.nixos.org/4p66fbi78b0gahvbhmpv56hacg3kmj5g.narinfo"));

  for (auto *c: handles)
    curl_multi_add_handle(curlm, c);

  while (true) {
    int running;
    auto mc = curl_multi_perform(curlm, &running);
    if (mc != CURLM_OK) {
      std::cerr << "unexpected error from curl_multi_perform: " << curl_multi_strerror(mc) << "\n";
      exit(1);
    }

    mc = curl_multi_wait(curlm, nullptr, 0, 0, nullptr);
    if (mc != CURLM_OK) {
      std::cerr << "unexpected error from curl_multi_wait: " << curl_multi_strerror(mc) << "\n";
      exit(1);
    }
  }


  curl_multi_cleanup(curlm);

  return 0;
}