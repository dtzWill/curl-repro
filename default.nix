{ nixpkgs ? 
  builtins.fetchTarball https://github.com/NixOS/nixpkgs/archive/f9ad1cae78b5fc27a5bf2f17b3f9ebf7b239b3ca.tar.gz
}:

with import nixpkgs {};

let

  extraCFLAGS = [
    "-g"
    "-O1"
    "-fsanitize=address"
  ];

  curl_asan = attrs: curl.overrideAttrs(o: {
    NIX_CFLAGS_COMPILE = (o.NIX_CFLAGS_COMPILE or []) ++ extraCFLAGS;
    separateDebugInfo = false;
    dontStrip = true;

    configureFlags = (o.configureFlags or []) ++ [
      "--enable-debug"
      "--enable-curldebug"
    ];
  });

  buildTests = { name, curl ? curl }:
    stdenv.mkDerivation {
      inherit name;

      nativeBuildInputs = [ pkgconfig ];
      buildInputs = [ curl ];

      src = builtins.fetchGit ./.;

      NIX_CFLAGS_COMPILE = extraCFLAGS;

      dontStrip = true;

      installPhase = ''
        install -D -t $out/bin test multi-stress
      '';
    };

in {
  use_curl_release = buildTests { name = "curl-release-tests"; curl = curl; };
  use_curl_fix1 = buildTests {
    name = "curl-fix1-tests";
    curl = curl.overrideAttrs (o:{
      patches = (o.patches or []) ++ [
        ./fix1.patch
      #  ./fix2.patch
      ];
    });
  };
  use_curl_git = buildTests {
    name = "curl-git-tests";
    curl = curl.overrideAttrs (o:{
      src = builtins.fetchGit https://github.com/curl/curl;
      nativeBuildInputs = (o.nativeBuildInputs or []) ++ [ autoreconfHook ];
      preConfigure = ":"; # disable fixups we do that are only needed on release tarballs
    });
  };
}
