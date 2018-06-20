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

  curl_asan = curl.overrideAttrs(o: {
    NIX_CFLAGS_COMPILE = (o.NIX_CFLAGS_COMPILE or []) ++ extraCFLAGS;
    separateDebugInfo = false;
    dontStrip = true;

    configureFlags = (o.configureFlags or []) ++ [
      "--enable-debug"
      "--enable-curldebug"
    ];

    patches = (o.patches or []) ++ [
      ./fix1.patch
      ./fix2.patch
    ];
  });
in
stdenv.mkDerivation {
  name = "curl-test";

  nativeBuildInputs = [ pkgconfig ];
  buildInputs = [ curl_asan ];

  src = builtins.fetchGit ./.;

  NIX_CFLAGS_COMPILE = extraCFLAGS;

  dontStrip = true;

  installPhase = ''
    install -D -t $out/bin test
  '';
}
