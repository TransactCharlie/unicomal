FROM alpine:latest

RUN wget https://js-dos.com/6.22/current/js-dos.js && \
    wget https://js-dos.com/6.22/current/wdosbox.js && \
    wget https://js-dos.com/6.22/current/wdosbox.wasm.js

RUN apk add darkhttpd

COPY index.html .
COPY comal.zip .

ENTRYPOINT ["/usr/bin/darkhttpd", "/", "--port", "8000"]