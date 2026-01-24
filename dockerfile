# base
FROM qmkfm/qmk_cli AS base
WORKDIR /qmk

RUN python3 -m pip install qmk
RUN qmk setup oltronix/qmk_firmware --yes -H /qmk/qmk_firmware/

FROM base AS build
COPY ./build.sh .
ENTRYPOINT ["/bin/bash", "./build.sh"]


