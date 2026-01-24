FROM qmkfm/qmk_cli
WORKDIR /qmk

# Setup QMK firmware (repository can be overridden via build arg)
ARG QMK_REPO=qmk/qmk_firmware
RUN qmk setup ${QMK_REPO} --yes -H /qmk/qmk_firmware/ \
    && pip install -r /qmk/qmk_firmware/requirements.txt

# Copy source files
COPY . /qmk/keymap/

WORKDIR /qmk/keymap
ENV QMK_FIRMWARE=/qmk/qmk_firmware
ENTRYPOINT ["/bin/bash", "./build.sh"]
