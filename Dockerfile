FROM alpine:latest

RUN wget https://js-dos.com/6.22/current/js-dos.js && \
    wget https://js-dos.com/6.22/current/wdosbox.js && \
    wget https://js-dos.com/6.22/current/wdosbox.wasm.js

RUN apk add darkhttpd zip

COPY index.html .
RUN mkdir COMAL
COPY comal/* COMAL/
COPY code/* COMAL/
RUN zip -r comal.zip COMAL

ENTRYPOINT ["/usr/bin/darkhttpd", "/", "--port", "8000"]