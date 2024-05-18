Update the fork repo now and then...
https://github.com/oltronix/qmk_firmware/

docker build . -t kimiko
docker run -it --rm -v /Users/androlau651/code/_hob/kimiko/v4:/qmk/v4 kimiko:latest

docker run -it --rm --entrypoint /bin/bash -v /Users/androlau651/code/_hob/kimiko/v4:/qmk/v4 kimiko:latest

cp -r /qmk/v4/pub/. v4/
qmk compile -km v4 -c
qmk config user.keyboard=keycapsss/kimiko
qmk compile -km v4 -kb keycapsss/kimiko/rev1 -c


