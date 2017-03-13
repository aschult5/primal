FROM nixos/nix:1.11
EXPOSE 1734

RUN nix-channel --add https://nixos.org/channels/nixpkgs-unstable nixpkgs
RUN nix-channel --update

RUN mkdir /app
COPY . /app

RUN nix-env -f /app/default.nix -i primal

ENTRYPOINT [ "primal" ]
CMD ["--help"]
