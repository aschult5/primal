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
      ${gcc}/bin/g++ -std=c++14 -Iinclude/ src/primal.cxx -o primal -lboost_program_options
   '';

   installPhase = ''
      mkdir -p $out/bin
      rsync -ar primal $out/bin
   '';
}
