{ nixpkgs ? builtins.fetchTarball channel:nixos-unstable }:

with import nixpkgs {};

let

  extraCFLAGS = [
    "-g"
    "-O1"
    "-fsanitize=address"
  ];

  curl_asan = curl.overrideDerivation (o: {
    NIX_CFLAGS_COMPILE = (o.NIX_CFLAGS_COMPILE or []) ++ extraCFLAGS;
    dontStrip = true;

    configureFlags = (o.configureFlags or []) ++ [
      "--enable-debug"
      "--enable-curldebug"
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
