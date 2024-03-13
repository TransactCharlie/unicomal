FROM alpine:latest

ARG GAME_URL
ARG GAME_ARGS

WORKDIR site
RUN wget https://js-dos.com/6.22/current/js-dos.js && \
    wget https://js-dos.com/6.22/current/wdosbox.js && \
    wget https://js-dos.com/6.22/current/wdosbox.wasm.js && \
    wget -O game.zip "$GAME_URL"

RUN apk add darkhttpd

COPY index.html .
RUN sed -i s/GAME_ARGS/$GAME_ARGS/ index.html

ENTRYPOINT ["/usr/bin/darkhttpd", "/site", "--port", "8000"]