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
  "https://cache.allvm.org/00cpg666l7gfnq4657f7gmk7mvd7x15p.narinfo",
  "https://cache.allvm.org/03flbqqgvr73i6h9bp3ssikvq718q72p.narinfo",
  "https://cache.allvm.org/04nczmckhi6626ffgrsm7jbr7hmbh53w.narinfo",
  "https://cache.allvm.org/05q2ipvkvnr0zzwwinvaichm6yhckx2s.narinfo",
  "https://cache.allvm.org/0a1bn76413xvy3idrrd82scxy4xbfh1g.narinfo",
  "https://cache.allvm.org/0cqvkv09y5gl6jzb5annrc90q00lvsgi.narinfo",
  "https://cache.allvm.org/0dmssn172jc1251l65sgxb5jcqmyfqnn.narinfo",
  "https://cache.allvm.org/0g13akad2m1ml6pa6sz9gff3mrjszv8l.narinfo",
  "https://cache.allvm.org/0gd0rj3s6qx31a7lh4i46rhba2j0vbrm.narinfo",
  "https://cache.allvm.org/0jzqkgbc3jvr2fpg932520dch9lws4fd.narinfo",
  "https://cache.allvm.org/0r3xw18pgw740xpcr0wp9xvp0sjin1wl.narinfo",
  "https://cache.allvm.org/0r65zn075qpcm1cggxl0n8qjj30gjpbl.narinfo",
  "https://cache.allvm.org/0v82ii1mdrcnd29is3sn3q7ahsgn91d2.narinfo",
  "https://cache.allvm.org/0wckw380dkhvqls4isi9kaang6cwnwxk.narinfo",
  "https://cache.allvm.org/0z6nvlnnbjd56j6dnxady0ia94hyx82q.narinfo",
  "https://cache.allvm.org/13fqjfas6xgr8yi9b8ligypwq0fh94zx.narinfo",
  "https://cache.allvm.org/16sg6965v12aaal13wwnl559683fhmkw.narinfo",
  "https://cache.allvm.org/16yh9vph1qmyaqxd9lb6ygjh16inch37.narinfo",
  "https://cache.allvm.org/17xvwkgd1a5yxy20n26iy7vri523m5mv.narinfo",
  "https://cache.allvm.org/18w4qzfmg80mh8i64k6h1xxbzav6wcdq.narinfo",
  "https://cache.allvm.org/19kgsdxqgl5hym3ly3sz5gmjbfw744pi.narinfo",
  "https://cache.allvm.org/1a9xc5gjyc6ckwyz5gvykp5bjphjcsd5.narinfo",
  "https://cache.allvm.org/1af118qc83g50ky2rywapdbg2yg1d59d.narinfo",
  "https://cache.allvm.org/1agjwyj7zmakwdhx1j6bx75q6axr2s7i.narinfo",
  "https://cache.allvm.org/1arizj7n4na9a5sa0v51k4jwisk011fg.narinfo",
  "https://cache.allvm.org/1aw6yh1a3wxzv0cmy3bxkd326xj66mjq.narinfo",
  "https://cache.allvm.org/1k2mnrdkqd4fmh8w8sl2qwsadmn6972n.narinfo",
  "https://cache.allvm.org/1myp35a4qx3535w2gvyph61dym5c20w9.narinfo",
  "https://cache.allvm.org/1qwk8l831wwzf9xc7bj6fwndx9cnfx4k.narinfo",
  "https://cache.allvm.org/1qwr0f09nm6c0xvymz7qvzswxy4wyvfq.narinfo",
  "https://cache.allvm.org/1s5gqymfx82s691fzqvm7ykb60d2nxfi.narinfo",
  "https://cache.allvm.org/1spy230qrhxgic41hlxb5sxbpwcj3zs1.narinfo",
  "https://cache.allvm.org/1vcmdxpd801wks5cs05ka5f44b3ynykf.narinfo",
  "https://cache.allvm.org/1vw0qx9pg7bi55cjqchfqbqljiaqgabh.narinfo",
  "https://cache.allvm.org/1vwm4glc87naqd41c8pci5jqr4dphl3c.narinfo",
  "https://cache.allvm.org/1yqzgz4k8py4d7vq70nwrz3zd9fvw1nn.narinfo",
  "https://cache.allvm.org/1zcpkiq84xb1g33rjv0a2yv4mk0r7l6v.narinfo",
  "https://cache.allvm.org/1zyvqvc65r2fnqx9zg1w5nv5jpjn91w8.narinfo",
  "https://cache.allvm.org/20b4aal4w45sskrhxcyjdy3nvzj0vh06.narinfo",
  "https://cache.allvm.org/20dkfwzggq1yd34pmx13bqahi27byw2r.narinfo",
  "https://cache.allvm.org/20ww8v6awr4890wrd34rnk94jsh6kj17.narinfo",
  "https://cache.allvm.org/25czxi8q5k0xriq2lwdxlsqprk17hkn3.narinfo",
  "https://cache.allvm.org/26s2n93qy2cq5gch4xgr07gyrc2k86fg.narinfo",
  "https://cache.allvm.org/27vdwhhbmp8gnbg6d1h0f0h0wbbpzrm9.narinfo",
  "https://cache.allvm.org/2fwm91pz3w9b6jqcvnfb829mpqkx8zl0.narinfo",
  "https://cache.allvm.org/2g6l6ff715jkz01shri0sz3qdrk0m3yi.narinfo",
  "https://cache.allvm.org/2gikpnaq38764fv7h4mkq1ww95fzmlr7.narinfo",
  "https://cache.allvm.org/2ii8a0aagdxnzld1xbi39ap4pd32assy.narinfo",
  "https://cache.allvm.org/2imh0k4ddvzmnaqnjhspg23nyfd1za8i.narinfo",
  "https://cache.allvm.org/2qk89fnkpp7qyrlpll5nxjjk6hfllyp0.narinfo",
  "https://cache.allvm.org/2ql1kir3v7srq7rzvh825m0zyhkwwx7y.narinfo",
  "https://cache.allvm.org/2rpvwmhvgc1lq473vn6gra7pwx9w9nrd.narinfo",
  "https://cache.allvm.org/2rvy5id3k2jq7k76fln3sky476lnf5q2.narinfo",
  "https://cache.allvm.org/2vh194999cnr5pvnkchcpn7p7cgyxqjb.narinfo",
  "https://cache.allvm.org/2xr9d9ksgvz7vsk8v3qgyvrz6jkpjhl0.narinfo",
  "https://cache.allvm.org/2y57dgb0fwspdm0nq2kryks28dscllmy.narinfo",
  "https://cache.allvm.org/2zbmhz34icr3k8x0xq8z0x1cbyyq8c8m.narinfo",
  "https://cache.allvm.org/303fd5nhng8pm98bm1fanbbq3rhafqk3.narinfo",
  "https://cache.allvm.org/3141rb278c9wf25g3yriqlzmziv3k0g3.narinfo",
  "https://cache.allvm.org/32z54r78nfgwbz5ww79r1gra17hg4w09.narinfo",
  "https://cache.allvm.org/33j9kxbwpkwjnp3a8jrbvdhac1xadmpb.narinfo",
  "https://cache.allvm.org/36bz8fnz8598r6bicxnsfdbs1hwh81qb.narinfo",
  "https://cache.allvm.org/397xjvpbasaz0jlmcj3wza29fdq66abf.narinfo",
  "https://cache.allvm.org/3cjpslnfh6v1yq28vzw09i313v0wk0d2.narinfo",
  "https://cache.allvm.org/3dlrvy2wvphi3jvvx9m7x0n10s6ikyhx.narinfo",
  "https://cache.allvm.org/3fhybx67vj1axg3h605xhfaqrrkqv0md.narinfo",
  "https://cache.allvm.org/3g7k9ipykxd2r0gw9pnc8c3x78b6qwnj.narinfo",
  "https://cache.allvm.org/3jikxpap5ggycmixkg2lisz2a7psqwzi.narinfo",
  "https://cache.allvm.org/3l1mbylrish7l95zvdvff51v6b5damq3.narinfo",
  "https://cache.allvm.org/3lmm1j6gzfzdfidp7f2ln1wzq1ybsgjj.narinfo",
  "https://cache.allvm.org/3mhginjn8mbjbrjb41v1zy3mbb47lv4l.narinfo",
  "https://cache.allvm.org/3n4ccwwacagc7abcrhpjh8xprp41dzc6.narinfo",
  "https://cache.allvm.org/3pvybp6gbmfq4r8mgi4nq46rr5qv6i6g.narinfo",
  "https://cache.allvm.org/3v5hw2xqfryvd7nj9ggas9hfkisd2vkw.narinfo",
  "https://cache.allvm.org/3yhiq1hvh7s6zjk73wz0ykacakn6n46g.narinfo",
  "https://cache.allvm.org/3zcxmxr093n7b8lhffjhzzhrr0243v9f.narinfo",
  "https://cache.allvm.org/3zrlpx4z9rl8hqf4a70b93qlj9v0i4n4.narinfo",
  "https://cache.allvm.org/4037yj7nfih4zkw20v36fsq3gqs0g3vv.narinfo",
  "https://cache.allvm.org/446kr16vqnjan39iy1j4dfjjn2k7j2zy.narinfo",
  "https://cache.allvm.org/4583nc4kvkbyvpq2ih8li2pwx15wc9ph.narinfo",
  "https://cache.allvm.org/479yivqyfh3zy4w0ylhr9br357qffw5i.narinfo",
  "https://cache.allvm.org/4a1bx9lmbdyfnsv78wx3hj93xhjm4lm0.narinfo",
  "https://cache.allvm.org/4b9q1paivkkg1jkdfpmvnzfm0a43pdjz.narinfo",
  "https://cache.allvm.org/4bkfqpm3d9fjwdgp20ag4qlfx1kp7q91.narinfo",
  "https://cache.allvm.org/4bm37qykqay35xdz1m6ic4rv67igmm36.narinfo",
  "https://cache.allvm.org/4bw3cxcsr90k9abnba5brwmq3yrc1jss.narinfo",
  "https://cache.allvm.org/4cdlqf904nzm0cymnah421grjyxpmg2b.narinfo",
  "https://cache.allvm.org/4d8cdy5f4yvrw1b6qjx2s7lfdk0xbfwp.narinfo",
  "https://cache.allvm.org/4gps6snvl9qcynchjawv2cikksqpd65a.narinfo",
  "https://cache.allvm.org/4hk26942kqilhrmiz2fkbz7mlb93ls5y.narinfo",
  "https://cache.allvm.org/4j4m4v9icpqkvzznzghxglqa3c12w39m.narinfo",
  "https://cache.allvm.org/4l7mhgh9jy71rydvvmh284ggvfrjvvsb.narinfo",
  "https://cache.allvm.org/4lsbb26dar4rvbiaxddyry8jw7clm3l4.narinfo",
  "https://cache.allvm.org/4mm6pjxk2srzwm6nldy3sz5fb1m3bn4s.narinfo",
  "https://cache.allvm.org/4n0sclbgcdrdjf6gp97mxj9f94jh50zi.narinfo",
  "https://cache.allvm.org/4p66fbi78b0gahvbhmpv56hacg3kmj5g.narinfo",
  "https://cache.allvm.org/53q3x56bpajlfr9d3ap1qj6cd07sw7mx.narinfo",
  "https://cache.allvm.org/53qpb2zj70r50l1v8y1jc856hs1h57pg.narinfo",
  "https://cache.allvm.org/544pq6rl0mgfnqj61wyh5l9arp1zki2k.narinfo",
  "https://cache.allvm.org/5dwk2na98265sfiw27gqml9nic1yfxiv.narinfo",
  "https://cache.allvm.org/5gjg0g3id9lf85q5yxg3d5k7b0dqfgpv.narinfo",
  "https://cache.allvm.org/6923y46zgvqs1xd3c62pllpbzq0c6cmq.narinfo",
  "https://cache.allvm.org/6aj2nxgq35vjk1r4h2j8ll4b3iic5qc4.narinfo",
  "https://cache.allvm.org/78svwh3h1b7wl5dvrnwrlfpvpdw357ff.narinfo",
  "https://cache.allvm.org/9g62blmkj1zxkjmbdm6zk8zikg2jaxnj.narinfo",
  "https://cache.allvm.org/9k6f2b4a9hgxgbl6lfdp5fw226dylfb8.narinfo",
  "https://cache.allvm.org/9yq0g8y0jk435s1igpah29ilirvxxl77.narinfo",
  "https://cache.allvm.org/9z7lyrzk15lbbrnzqdzj9ijq1zh8n9cx.narinfo",
  "https://cache.allvm.org/a0c6qb1ix9wm4jn1s0cykcscnky8wgin.narinfo",
  "https://cache.allvm.org/b9h3f6z1qxbcvac1c32afl8dvvxgd2mw.narinfo",
  "https://cache.allvm.org/c18l1xsfc8fq7xangwxbxhl3wd50jlgv.narinfo",
  "https://cache.allvm.org/d42hcbpkawwdf8z1i9hqnq8by7l1wbn5.narinfo",
  "https://cache.allvm.org/gbz34vqzadszchfkscbscahszp4zby6c.narinfo",
  "https://cache.allvm.org/k5nd65fp61zs5zfg9bh5vbpzd21gki9x.narinfo",
  "https://cache.allvm.org/lav10hhcnb37f4g4w0s83428hrg37b35.narinfo",
  "https://cache.allvm.org/nd7p91cl96fgjqgn1ssa6d24qyqnbkyw.narinfo",
  "https://cache.allvm.org/pn4mvcq0qiif12ay7aj9imbk31wa4zyq.narinfo",
  "https://cache.allvm.org/r4h24d8afw7n0vsh0sfg2ha0cfkb5cyv.narinfo",
  "https://cache.allvm.org/ribfvqxpy4hzaiscp0arjf7qahy83akz.narinfo",
  "https://cache.allvm.org/s7wpxinyb8l8qhpmlzpzkk1q3flzf58a.narinfo",
  "https://cache.allvm.org/sci34dw0p3l16rf4dvm61mbcyv3i1csg.narinfo",
  "https://cache.allvm.org/ywszkz3c7mslkbnv672krq2f3vd9y1z0.narinfo",
  "https://cache.nixos.org/00cpg666l7gfnq4657f7gmk7mvd7x15p.narinfo",
  "https://cache.nixos.org/03flbqqgvr73i6h9bp3ssikvq718q72p.narinfo",
  "https://cache.nixos.org/04nczmckhi6626ffgrsm7jbr7hmbh53w.narinfo",
  "https://cache.nixos.org/05q2ipvkvnr0zzwwinvaichm6yhckx2s.narinfo",
  "https://cache.nixos.org/0a1bn76413xvy3idrrd82scxy4xbfh1g.narinfo",
  "https://cache.nixos.org/0cqvkv09y5gl6jzb5annrc90q00lvsgi.narinfo",
  "https://cache.nixos.org/0dmssn172jc1251l65sgxb5jcqmyfqnn.narinfo",
  "https://cache.nixos.org/0g13akad2m1ml6pa6sz9gff3mrjszv8l.narinfo",
  "https://cache.nixos.org/0gd0rj3s6qx31a7lh4i46rhba2j0vbrm.narinfo",
  "https://cache.nixos.org/0jzqkgbc3jvr2fpg932520dch9lws4fd.narinfo",
  "https://cache.nixos.org/0r3xw18pgw740xpcr0wp9xvp0sjin1wl.narinfo",
  "https://cache.nixos.org/0r65zn075qpcm1cggxl0n8qjj30gjpbl.narinfo",
  "https://cache.nixos.org/0v82ii1mdrcnd29is3sn3q7ahsgn91d2.narinfo",
  "https://cache.nixos.org/0wckw380dkhvqls4isi9kaang6cwnwxk.narinfo",
  "https://cache.nixos.org/0z6nvlnnbjd56j6dnxady0ia94hyx82q.narinfo",
  "https://cache.nixos.org/13fqjfas6xgr8yi9b8ligypwq0fh94zx.narinfo",
  "https://cache.nixos.org/16sg6965v12aaal13wwnl559683fhmkw.narinfo",
  "https://cache.nixos.org/16yh9vph1qmyaqxd9lb6ygjh16inch37.narinfo",
  "https://cache.nixos.org/17xvwkgd1a5yxy20n26iy7vri523m5mv.narinfo",
  "https://cache.nixos.org/18w4qzfmg80mh8i64k6h1xxbzav6wcdq.narinfo",
  "https://cache.nixos.org/19kgsdxqgl5hym3ly3sz5gmjbfw744pi.narinfo",
  "https://cache.nixos.org/1a9xc5gjyc6ckwyz5gvykp5bjphjcsd5.narinfo",
  "https://cache.nixos.org/1af118qc83g50ky2rywapdbg2yg1d59d.narinfo",
  "https://cache.nixos.org/1agjwyj7zmakwdhx1j6bx75q6axr2s7i.narinfo",
  "https://cache.nixos.org/1arizj7n4na9a5sa0v51k4jwisk011fg.narinfo",
  "https://cache.nixos.org/1aw6yh1a3wxzv0cmy3bxkd326xj66mjq.narinfo",
  "https://cache.nixos.org/1iysq6vq2541fxmcw0417sk9h0grb73c.narinfo",
  "https://cache.nixos.org/1k2mnrdkqd4fmh8w8sl2qwsadmn6972n.narinfo",
  "https://cache.nixos.org/1myp35a4qx3535w2gvyph61dym5c20w9.narinfo",
  "https://cache.nixos.org/1qwk8l831wwzf9xc7bj6fwndx9cnfx4k.narinfo",
  "https://cache.nixos.org/1qwr0f09nm6c0xvymz7qvzswxy4wyvfq.narinfo",
  "https://cache.nixos.org/1s5gqymfx82s691fzqvm7ykb60d2nxfi.narinfo",
  "https://cache.nixos.org/1spy230qrhxgic41hlxb5sxbpwcj3zs1.narinfo",
  "https://cache.nixos.org/1vcmdxpd801wks5cs05ka5f44b3ynykf.narinfo",
  "https://cache.nixos.org/1vhgkmby6lkm6qz3kv6pmzylv1599z7y.narinfo",
  "https://cache.nixos.org/1vw0qx9pg7bi55cjqchfqbqljiaqgabh.narinfo",
  "https://cache.nixos.org/1vwm4glc87naqd41c8pci5jqr4dphl3c.narinfo",
  "https://cache.nixos.org/1yqzgz4k8py4d7vq70nwrz3zd9fvw1nn.narinfo",
  "https://cache.nixos.org/1zcpkiq84xb1g33rjv0a2yv4mk0r7l6v.narinfo",
  "https://cache.nixos.org/1zyvqvc65r2fnqx9zg1w5nv5jpjn91w8.narinfo",
  "https://cache.nixos.org/20b4aal4w45sskrhxcyjdy3nvzj0vh06.narinfo",
  "https://cache.nixos.org/20dkfwzggq1yd34pmx13bqahi27byw2r.narinfo",
  "https://cache.nixos.org/20ww8v6awr4890wrd34rnk94jsh6kj17.narinfo",
  "https://cache.nixos.org/25czxi8q5k0xriq2lwdxlsqprk17hkn3.narinfo",
  "https://cache.nixos.org/26s2n93qy2cq5gch4xgr07gyrc2k86fg.narinfo",
  "https://cache.nixos.org/27vdwhhbmp8gnbg6d1h0f0h0wbbpzrm9.narinfo",
  "https://cache.nixos.org/2fwm91pz3w9b6jqcvnfb829mpqkx8zl0.narinfo",
  "https://cache.nixos.org/2g6l6ff715jkz01shri0sz3qdrk0m3yi.narinfo",
  "https://cache.nixos.org/2gikpnaq38764fv7h4mkq1ww95fzmlr7.narinfo",
  "https://cache.nixos.org/2ii8a0aagdxnzld1xbi39ap4pd32assy.narinfo",
  "https://cache.nixos.org/2imh0k4ddvzmnaqnjhspg23nyfd1za8i.narinfo",
  "https://cache.nixos.org/2qk89fnkpp7qyrlpll5nxjjk6hfllyp0.narinfo",
  "https://cache.nixos.org/2ql1kir3v7srq7rzvh825m0zyhkwwx7y.narinfo",
  "https://cache.nixos.org/2rpvwmhvgc1lq473vn6gra7pwx9w9nrd.narinfo",
  "https://cache.nixos.org/2rvy5id3k2jq7k76fln3sky476lnf5q2.narinfo",
  "https://cache.nixos.org/2vh194999cnr5pvnkchcpn7p7cgyxqjb.narinfo",
  "https://cache.nixos.org/2xr9d9ksgvz7vsk8v3qgyvrz6jkpjhl0.narinfo",
  "https://cache.nixos.org/2y57dgb0fwspdm0nq2kryks28dscllmy.narinfo",
  "https://cache.nixos.org/2zbmhz34icr3k8x0xq8z0x1cbyyq8c8m.narinfo",
  "https://cache.nixos.org/303fd5nhng8pm98bm1fanbbq3rhafqk3.narinfo",
  "https://cache.nixos.org/3141rb278c9wf25g3yriqlzmziv3k0g3.narinfo",
  "https://cache.nixos.org/32z54r78nfgwbz5ww79r1gra17hg4w09.narinfo",
  "https://cache.nixos.org/33j9kxbwpkwjnp3a8jrbvdhac1xadmpb.narinfo",
  "https://cache.nixos.org/36bz8fnz8598r6bicxnsfdbs1hwh81qb.narinfo",
  "https://cache.nixos.org/397xjvpbasaz0jlmcj3wza29fdq66abf.narinfo",
  "https://cache.nixos.org/3cjpslnfh6v1yq28vzw09i313v0wk0d2.narinfo",
  "https://cache.nixos.org/3dlrvy2wvphi3jvvx9m7x0n10s6ikyhx.narinfo",
  "https://cache.nixos.org/3fhybx67vj1axg3h605xhfaqrrkqv0md.narinfo",
  "https://cache.nixos.org/3g7k9ipykxd2r0gw9pnc8c3x78b6qwnj.narinfo",
  "https://cache.nixos.org/3jikxpap5ggycmixkg2lisz2a7psqwzi.narinfo",
  "https://cache.nixos.org/3l1mbylrish7l95zvdvff51v6b5damq3.narinfo",
  "https://cache.nixos.org/3lmm1j6gzfzdfidp7f2ln1wzq1ybsgjj.narinfo",
  "https://cache.nixos.org/3mhginjn8mbjbrjb41v1zy3mbb47lv4l.narinfo",
  "https://cache.nixos.org/3n4ccwwacagc7abcrhpjh8xprp41dzc6.narinfo",
  "https://cache.nixos.org/3pvybp6gbmfq4r8mgi4nq46rr5qv6i6g.narinfo",
  "https://cache.nixos.org/3v5hw2xqfryvd7nj9ggas9hfkisd2vkw.narinfo",
  "https://cache.nixos.org/3yhiq1hvh7s6zjk73wz0ykacakn6n46g.narinfo",
  "https://cache.nixos.org/3zcxmxr093n7b8lhffjhzzhrr0243v9f.narinfo",
  "https://cache.nixos.org/3zrlpx4z9rl8hqf4a70b93qlj9v0i4n4.narinfo",
  "https://cache.nixos.org/4037yj7nfih4zkw20v36fsq3gqs0g3vv.narinfo",
  "https://cache.nixos.org/446kr16vqnjan39iy1j4dfjjn2k7j2zy.narinfo",
  "https://cache.nixos.org/4583nc4kvkbyvpq2ih8li2pwx15wc9ph.narinfo",
  "https://cache.nixos.org/479yivqyfh3zy4w0ylhr9br357qffw5i.narinfo",
  "https://cache.nixos.org/4a1bx9lmbdyfnsv78wx3hj93xhjm4lm0.narinfo",
  "https://cache.nixos.org/4b9q1paivkkg1jkdfpmvnzfm0a43pdjz.narinfo",
  "https://cache.nixos.org/4bkfqpm3d9fjwdgp20ag4qlfx1kp7q91.narinfo",
  "https://cache.nixos.org/4bm37qykqay35xdz1m6ic4rv67igmm36.narinfo",
  "https://cache.nixos.org/4bw3cxcsr90k9abnba5brwmq3yrc1jss.narinfo",
  "https://cache.nixos.org/4cdlqf904nzm0cymnah421grjyxpmg2b.narinfo",
  "https://cache.nixos.org/4d8cdy5f4yvrw1b6qjx2s7lfdk0xbfwp.narinfo",
  "https://cache.nixos.org/4gps6snvl9qcynchjawv2cikksqpd65a.narinfo",
  "https://cache.nixos.org/4hk26942kqilhrmiz2fkbz7mlb93ls5y.narinfo",
  "https://cache.nixos.org/4j4m4v9icpqkvzznzghxglqa3c12w39m.narinfo",
  "https://cache.nixos.org/4l7mhgh9jy71rydvvmh284ggvfrjvvsb.narinfo",
  "https://cache.nixos.org/4lsbb26dar4rvbiaxddyry8jw7clm3l4.narinfo",
  "https://cache.nixos.org/4mm6pjxk2srzwm6nldy3sz5fb1m3bn4s.narinfo",
  "https://cache.nixos.org/4n0sclbgcdrdjf6gp97mxj9f94jh50zi.narinfo",
  "https://cache.nixos.org/4p66fbi78b0gahvbhmpv56hacg3kmj5g.narinfo",
  "https://cache.nixos.org/53q3x56bpajlfr9d3ap1qj6cd07sw7mx.narinfo",
  "https://cache.nixos.org/53qpb2zj70r50l1v8y1jc856hs1h57pg.narinfo",
  "https://cache.nixos.org/544pq6rl0mgfnqj61wyh5l9arp1zki2k.narinfo",
  "https://cache.nixos.org/5641x9c7v9l3irym3rvcxpd82f7b4han.narinfo",
  "https://cache.nixos.org/5dwk2na98265sfiw27gqml9nic1yfxiv.narinfo",
  "https://cache.nixos.org/5fcdm6jmvbikd3bdn1jyblmpz8a8mw16.narinfo",
  "https://cache.nixos.org/5gidzp0k0dx2yyx3ngfcask8dhnv0bb8.narinfo",
  "https://cache.nixos.org/5gjg0g3id9lf85q5yxg3d5k7b0dqfgpv.narinfo",
  "https://cache.nixos.org/5jhp5nvd6r0a812yhcs5sn6abd6rbpk8.narinfo",
  "https://cache.nixos.org/5k2pjs7mdvy9fj9sfin8zfwqj6xrhx04.narinfo",
  "https://cache.nixos.org/5l65vn92jawknxf0jj3hagb41zz86wv1.narinfo",
  "https://cache.nixos.org/5lzb02cylsczvckfmcdcb5marr6iw4py.narinfo",
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
  "https://cache.nixos.org/ribfvqxpy4hzaiscp0arjf7qahy83akz.narinfo",
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

