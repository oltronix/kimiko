qmk json2c specials.json -o keymap.c
src=$(pwd)
cd  ~/_code/qmk/qmk_firmware/keyboards/keycapsss/kimiko/keymaps/mymap/
cat "$src/keymap.c" combos.mc > keymap.c

qmk compile
mv ~/_code/qmk/qmk_firmware/keycapsss_kimiko_rev1_mymap.hex $src
