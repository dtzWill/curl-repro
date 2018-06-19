{ nixpkgs ? <nixpkgs> }:

with import nixpkgs {};

stdenv.mkDerivation {
  name = "curl-test";

  nativeBuildInputs = [ pkgconfig ];
  buildInputs = [ curl ];

  src = builtins.fetchGit ./.;

  installPhase = ''
    install -D -t $out/bin test
  '';
}
