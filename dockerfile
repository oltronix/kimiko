FROM qmkfm/qmk_cli
WORKDIR /qmk

# Setup QMK firmware (repository and branch can be overridden via build args)
ARG QMK_REPO=oltronix/qmk_firmware
ARG QMK_BRANCH=master
ARG QMK_CACHE_BUST=1
RUN qmk setup ${QMK_REPO} -b ${QMK_BRANCH} --yes -H /qmk/qmk_firmware/ \
    && pip install -r /qmk/qmk_firmware/requirements.txt

# Keymap sources are mounted at runtime to /qmk/keymap
WORKDIR /qmk/keymap
ENV QMK_FIRMWARE=/qmk/qmk_firmware
ENV QMK_BRANCH=${QMK_BRANCH}
ENTRYPOINT ["/bin/bash", "./build.sh"]
