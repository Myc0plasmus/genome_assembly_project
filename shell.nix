with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [ cmake ];
  buildInputs = [ glog gflags ];
}
