with import <nixpkgs> {};

stdenv.mkDerivation rec {
   name = "primal";
   src = ./.;
   buildInputs = [ boost163 autoconf autoconf-archive automake ];

   preConfigure = ''
      ./bootstrap
   '';

   configureFlags = ''
      --with-boost-libdir=${boost163}/lib
   '';
}
