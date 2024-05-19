# Keymap

## Super cool power tabbing
I have placed a hold layer on my enter button, if I activate that layer I and press one ATAB_DOWN or ATAB_UP it acts as pressing alt tab, the FRST buttons then act as navigation in the alt-tab window until the layer is released.
If I instead press the CTAB_LEFT or CTAB_RIGHT key it will tab through tabs in the current application until the layer is released.
Finally I have added a hacky special case that sends the MAC command for switching window for the current app on ATAB_LEFT or ATAB_RIGHT if the alt tab window isn't already open.

## Window management
I have tried adding enough keys for both win and mac to be able to manage windows without the mouse, but some options have no default shortcut and needs to be added.
### Mac
 Maximize window: Window->Zoom : CTL-CMD-Z
 Move Left: Move Window to Left Side of Screen : CTL-CMD-ALT-Left 
 Move Right: Move Window to Right Side of Screen : ⌃⌥⌘R
 

# Build
I'm using a janky build script that copies a qmk configurator JSON and all the C code into a qmk docker image.

## Green path
Build the dockerfile from the v4 folder, crating a image with the tag kimiko:
`docker build . -t kimiko`
This will take some time since the qmk_cli image is humogeous.
Then run the latest build of the image:
    -v:  with the v4 folder mounted as qmk/v4
    -it: print the console output into the terminal
    --rm: remove the container after the run is finished
`docker run -it --rm -v /Users/androlau651/code/_hob/kimiko/v4:/qmk/v4 kimiko:latest`
The build.sh script will build a final keymap from the mounted files.
It compiles the keymap.json file into a C keymap which I extract only the keycode matrix from.
That key matrix is appended to the custom.c which is the final keymap.
The keymap along with all dependencies in the /src folder will then be copied into the qmk_firmware folder as a kimiko keymap that we can compile.
After the compile is done the resulting firmware will be copied to the host system to a /map folder.

## Debug
This can and has broken at several points in time so I have included some commands for debugging the build.
Shell in the kimiko container:
`docker run -it --rm --entrypoint /bin/bash -v /Users/androlau651/code/_hob/kimiko/v4:/qmk/v4 kimiko:latest`

### It is good to
Update the fork repo now and then...
https://github.com/oltronix/qmk_firmware/

The image is built with a personal fork of the qmk_firmware, I don't know why I have a fork since I have everything custom in this repo but that is where I'm at right now, so not updating the firmware does mean I miss out on new features and bug fixes and lovely breaking changes.

This is the line in the dockerfile that sets up the fork.
`RUN qmk setup oltronix/qmk_firmware --yes -H /qmk/qmk_firmware/`

### Manually try to compile the firmware
This assumes you are in the kimiko keymaps folder in qmk_firmware
```
cp -r /qmk/v4/pub/. v4/
qmk compile -km v4 -c
qmk config user.keyboard=keycapsss/kimiko
qmk compile -km v4 -kb keycapsss/kimiko/rev1 -c
```

