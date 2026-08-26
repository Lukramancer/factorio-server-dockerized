FROM debian:13-slim AS factorio-download

ARG FACTORIO_VERSION=stable

RUN apt-get update && apt-get install -y curl xz-utils

RUN rm -f /etc/apt/apt.conf.d/docker-clean && \
    echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache

RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        curl \
        xz-utils && \
    rm -rf /var/cache/apt/archives/partial/*

RUN mkdir -p opt && \
    curl -L "https://factorio.com/get-download/${FACTORIO_VERSION}/headless/linux64" | \
    tar --extract --file - -J --directory opt && \
    mkdir opt/factorio/configs opt/factorio/saves opt/factorio/mods

RUN sed --in-place 's|__PATH__executable__/../../config|opt/factorio/configs|' opt/factorio/config-path.cfg


FROM gcc:trixie AS starter-build

COPY starter /starter/src
WORKDIR /starter

RUN cc src/*.c src/utils/*.c -o start


FROM gcr.io/distroless/cc-debian13

COPY --from=factorio-download opt /opt/

COPY --from=starter-build /starter/start /opt/factorio

WORKDIR /opt/factorio

VOLUME [ \
    "/opt/factorio/configs", \
    "/opt/factorio/saves", \
    "/opt/factorio/mods" \
]

ENTRYPOINT ["/opt/factorio/start"]