{ doCheck ? false }:

with import <nixpkgs> {};

stdenv.mkDerivation rec {
   inherit doCheck;

   name = "primal";
   src = ./.;
   buildInputs = [ boost163 autoconf autoconf-archive automake gtest ];

   preConfigure = ''
      ./bootstrap
   '';

   configureFlags = ''
      --with-boost-libdir=${boost163}/lib
   '';
}
