FROM ubuntu:24.04 AS builder

RUN apt update && apt install -y make g++ libjsoncpp-dev libcivetweb-dev

WORKDIR /arta
COPY . .

RUN make clean arta

FROM ubuntu:24.04
WORKDIR /arta
COPY --from=builder /arta/bin/arta /usr/local/bin/arta
COPY --from=builder /arta/engine.json /arta/
COPY --from=builder /arta/config.json /arta/

CMD ["arta"]
