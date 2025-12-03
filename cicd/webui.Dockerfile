FROM ubuntu:24.04 AS builder

RUN apt update && apt install -y make g++ libjsoncpp-dev libcivetweb-dev

WORKDIR /arta
COPY . .

RUN make clean webui

FROM ubuntu:24.04
RUN apt update && apt install -y libcivetweb-dev libjsoncpp-dev
WORKDIR /arta
COPY --from=builder /arta/bin/webui /usr/local/bin/
COPY --from=builder /arta/config.json /arta/
COPY --from=builder /arta/www /arta/www

CMD ["webui"]
