with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [ cmake ];
  buildInputs = [ glog gflags ];
  shellHook = ''
    NIX_ENFORCE_PURITY=0 cmake -B build
    NIX_ENFORCE_PURITY=0 cmake --build build
    /application/build/main
  '';
}
