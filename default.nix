with import <nixpkgs> {};

stdenv.mkDerivation rec {
   name = "primal";
   src = ./.;
   buildInputs = [ rsync gcc boost163 ];

   unpackPhase = ''
      rsync -a ${src}/. ./
      chmod ug+w -R .
   '';

   buildPhase = ''
      ${gcc}/bin/g++ -std=c++14 -I${boost163}/include primal.cxx -o primal
   '';

   installPhase = ''
      mkdir -p $out/bin
      rsync -ar primal $out/bin
   '';
}
