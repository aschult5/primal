with import <nixpkgs> {};

stdenv.mkDerivation rec {
   name = "primal";
   src = ./.;
   buildInputs = [ rsync boost163 ];

   unpackPhase = ''
      rsync -a ${src}/. ./
      chmod ug+w -R .
   '';

   buildPhase = ''
      ${gcc}/bin/g++ -Wall -Werror -std=c++14 -Iinclude/ src/primal.cxx src/client.cxx src/server.cxx -o primal -lboost_program_options -lboost_system
   '';

   installPhase = ''
      mkdir -p $out/bin
      rsync -a primal $out/bin
   '';
}
