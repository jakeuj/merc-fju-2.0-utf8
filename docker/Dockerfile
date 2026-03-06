FROM ubuntu:24.04

LABEL maintainer="Sango Merc-FJU maintainers"

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=zh_TW.UTF-8 \
    LC_ALL=zh_TW.UTF-8 \
    TERM=xterm-256color

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        csh \
        git \
        libncurses-dev \
        locales \
        python3 \
        python3-venv \
        telnet \
    && locale-gen zh_TW.UTF-8 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN chmod +x /app/scripts/clean-runtime.sh \
    && chmod +x /app/scripts/render-merc-ini.sh \
    && /app/scripts/clean-runtime.sh /app \
    && /app/scripts/render-merc-ini.sh /app /app \
    && rm -rf /app/.git

RUN useradd -m -r -s /bin/bash mud \
    && chown -R mud:mud /app \
    && chmod +x /app/docker/entrypoint.sh \
    && chmod +x /app/scripts/bootstrap.sh

RUN cd /app/src \
    && make clean \
    && make

USER mud

VOLUME ["/app/player", "/app/mail", "/app/board", "/app/vote", "/app/log", "/app/debug", "/app/etc"]

ENTRYPOINT ["/app/docker/entrypoint.sh"]
