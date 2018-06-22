#include <curl/curl.h>

/* the default readfunc os fread which can cause R to freeze */
size_t dontread(char *buffer, size_t size, size_t nitems, void *instream){
  return 0;
}

CURL * make_handle(){
  CURL * handle = curl_easy_init();

  /* Important: use HTTP2 over HTTPS */
  curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(handle, CURLOPT_URL, "https://cloud.r-project.org/");

  /* For completeness */
  curl_easy_setopt(handle, CURLOPT_ENCODING, "gzip, deflate");
  curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);
  curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 10L);
  curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
  curl_easy_setopt(handle, CURLOPT_FILETIME, 1L);
  curl_easy_setopt(handle, CURLOPT_USERAGENT, "agent smith");
  curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
  curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
  curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
  curl_easy_setopt(handle, CURLOPT_READFUNCTION, dontread);
  curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
  return handle;
}


int main(void){
  //setup multi pool
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURLM *multi_handle = curl_multi_init();
  FILE *dest = fopen("/dev/null", "wb");

  for(int round = 0; round < 20; round++){
    printf("Round %d: adding 100 handles...\n", round);
    for(int i = 0; i < 100; i++){
      CURL *handle = make_handle();
      curl_multi_add_handle(multi_handle, handle);
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, dest);
    }

    /* we start some action by calling perform right away */
    int msgs_left;
    int total = 0;
    int still_running = 1;
    while(still_running) {
      int numfds;
      CURLMcode mc = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
      if(mc != CURLM_OK) {
        fprintf(stderr, "curl_multi_wait() failed, code %d.\n", mc);
        break;
      }
      curl_multi_perform(multi_handle, &still_running);

      /* See how the transfers went */
      CURLMsg *m = NULL;
      while((m = curl_multi_info_read(multi_handle, &msgs_left))) {
        if(m->msg == CURLMSG_DONE) {
          CURL *handle = m->easy_handle;
          if(m->data.result == CURLE_OK){
            long res_status;
            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
            if(res_status != 200)
              printf("Failed: HTTP %d!\n", (int) res_status);
            else {
              printf("\rCompleted: %d", ++total);
              fflush(stdout);
            }
          } else {
            printf("Connection failure");
          }
          curl_multi_remove_handle(multi_handle, handle);
          curl_easy_cleanup(handle);
        }
      }
    }
    printf(" OK!\n");
  }

  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
  return 0;
}

