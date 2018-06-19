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

  curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);

  return c;
}

void go_go_gadget_curl(CURLM *curlm) {
  while (true) {
    int running;
    auto mc = curl_multi_perform(curlm, &running);
    if (mc != CURLM_OK) {
      std::cerr << "unexpected error from curl_multi_perform: " << curl_multi_strerror(mc) << "\n";
      exit(1);
    }

    int left;
    CURLMsg *msg;
    while ((msg = curl_multi_info_read(curlm, &left))) {
      if (msg->msg == CURLMSG_DONE) {
        std::cout << "\nrequest finished!\n";
        auto *h = msg->easy_handle;
        curl_multi_remove_handle(curlm, h);
        curl_easy_cleanup(h);
      }
    }

    if (running == 0)
      return;

    mc = curl_multi_wait(curlm, nullptr, 0, 0, nullptr);
    if (mc != CURLM_OK) {
      std::cerr << "unexpected error from curl_multi_wait: " << curl_multi_strerror(mc) << "\n";
      exit(1);
    }
  }
}

std::vector<const char *> urls{
  "https://cache.nixos.org/0dmssn172jc1251l65sgxb5jcqmyfqnn.narinfo",
  "https://cache.nixos.org/0r3xw18pgw740xpcr0wp9xvp0sjin1wl.narinfo",
  "https://cache.nixos.org/20dkfwzggq1yd34pmx13bqahi27byw2r.narinfo",
  "https://cache.nixos.org/2rvy5id3k2jq7k76fln3sky476lnf5q2.narinfo",
  "https://cache.nixos.org/33j9kxbwpkwjnp3a8jrbvdhac1xadmpb.narinfo",
  "https://cache.nixos.org/397xjvpbasaz0jlmcj3wza29fdq66abf.narinfo",
  "https://cache.nixos.org/3cjpslnfh6v1yq28vzw09i313v0wk0d2.narinfo",
  "https://cache.nixos.org/3fhybx67vj1axg3h605xhfaqrrkqv0md.narinfo",
  "https://cache.nixos.org/4a1bx9lmbdyfnsv78wx3hj93xhjm4lm0.narinfo",
  "https://cache.nixos.org/4bm37qykqay35xdz1m6ic4rv67igmm36.narinfo",
  "https://cache.nixos.org/4bw3cxcsr90k9abnba5brwmq3yrc1jss.narinfo",
  "https://cache.nixos.org/4p66fbi78b0gahvbhmpv56hacg3kmj5g.narinfo",
  "https://cache.nixos.org/6923y46zgvqs1xd3c62pllpbzq0c6cmq.narinfo",
  "https://cache.nixos.org/6aj2nxgq35vjk1r4h2j8ll4b3iic5qc4.narinfo",
  "https://cache.nixos.org/78svwh3h1b7wl5dvrnwrlfpvpdw357ff.narinfo",
  "https://cache.nixos.org/8jq5ww0kiwlxxxfkv0l5mgkb0hfadyin.narinfo",
  "https://cache.nixos.org/9g62blmkj1zxkjmbdm6zk8zikg2jaxnj.narinfo",
  "https://cache.nixos.org/9k6f2b4a9hgxgbl6lfdp5fw226dylfb8.narinfo",
  "https://cache.nixos.org/9yq0g8y0jk435s1igpah29ilirvxxl77.narinfo",
  "https://cache.nixos.org/9z7lyrzk15lbbrnzqdzj9ijq1zh8n9cx.narinfo",
  "https://cache.nixos.org/a0c6qb1ix9wm4jn1s0cykcscnky8wgin.narinfo",
  "https://cache.nixos.org/b9h3f6z1qxbcvac1c32afl8dvvxgd2mw.narinfo",
  "https://cache.nixos.org/bl7x3mlbk4lwlvlnr6s8rlmpvy74mbzs.narinfo",
  "https://cache.nixos.org/c18l1xsfc8fq7xangwxbxhl3wd50jlgv.narinfo",
  "https://cache.nixos.org/cgrz9ikn0p032i6by2rck62nm6ndq6qm.narinfo",
  "https://cache.nixos.org/ciwafxy9byxg0475qj8n9dam3a1ad5d6.narinfo",
  "https://cache.nixos.org/d42hcbpkawwdf8z1i9hqnq8by7l1wbn5.narinfo",
  "https://cache.nixos.org/d77sc37l0n64rkfs3lqfqrjzzyqbh2wy.narinfo",
  "https://cache.nixos.org/dfji5lh1pd7ccb3g8z76rjcnb4a7jd4a.narinfo",
  "https://cache.nixos.org/gbz34vqzadszchfkscbscahszp4zby6c.narinfo",
  "https://cache.nixos.org/gc85i0ls59drh2a7dxh8vwpiggfdxnkd.narinfo",
  "https://cache.nixos.org/j9dnx79ljfrkp4lwn1aa756xnkhw230a.narinfo",
  "https://cache.nixos.org/jcf3gnnv3jagikjbi3c3p79d1vj7y9kv.narinfo",
  "https://cache.nixos.org/k00khfndbiljmai9cq4001di8yk077pl.narinfo",
  "https://cache.nixos.org/k5nd65fp61zs5zfg9bh5vbpzd21gki9x.narinfo",
  "https://cache.nixos.org/lav10hhcnb37f4g4w0s83428hrg37b35.narinfo",
  "https://cache.nixos.org/n3x8iq4k11vc7b4wzjiz108494mw22s7.narinfo",
  "https://cache.nixos.org/nd7p91cl96fgjqgn1ssa6d24qyqnbkyw.narinfo",
  "https://cache.nixos.org/pn4mvcq0qiif12ay7aj9imbk31wa4zyq.narinfo",
  "https://cache.nixos.org/r4h24d8afw7n0vsh0sfg2ha0cfkb5cyv.narinfo",
  "https://cache.nixos.org/ri4p8x84sq8jkg20cd2qrk8rwpgq6yxd.narinfo",
  "https://cache.nixos.org/rxlf5v1isil4kmwy5z38mb6n67p2vwzy.narinfo",
  "https://cache.nixos.org/s0yaqb0sx5h9y22mwlj2vlwlp75mid9s.narinfo",
  "https://cache.nixos.org/s3b0zxhvwc02h9kndbw6ni7kgn0lcwgx.narinfo",
  "https://cache.nixos.org/s7wpxinyb8l8qhpmlzpzkk1q3flzf58a.narinfo",
  "https://cache.nixos.org/sci34dw0p3l16rf4dvm61mbcyv3i1csg.narinfo",
  "https://cache.nixos.org/wgjcqgva24xsw5mzpi6y9hha4qvxpj1b.narinfo",
  "https://cache.nixos.org/ywszkz3c7mslkbnv672krq2f3vd9y1z0.narinfo",
};

int main() {

  auto curlm = curl_multi_init();

  curl_multi_setopt(curlm, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
  curl_multi_setopt(curlm, CURLMOPT_MAX_TOTAL_CONNECTIONS, 25L);

  // Process urls in chunks of 5
  while (!urls.empty()) {

    for (size_t n = 0; n < 5 && !urls.empty(); ++n) {
      curl_multi_add_handle(curlm, makeHandle(urls.back()));
      urls.pop_back();
    }

    go_go_gadget_curl(curlm);
  }

  curl_multi_cleanup(curlm);

  return 0;
}

