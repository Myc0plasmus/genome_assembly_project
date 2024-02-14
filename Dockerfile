FROM nixos/nix

WORKDIR /application

COPY . .

RUN ["chmod", "+x", "/application/build.sh"]



ENTRYPOINT [ "nix-shell" ]