qmk json2c restructure.json -o keymap.c
src=$(pwd)
cat keymap.c | head -n 8 > kmhead.c
cat keymap.c | tail -n 8 > kmtail.c
cd  ~/_code/qmk/qmk_firmware/keyboards/keycapsss/kimiko/keymaps/mymap/
cat "$src/kmhead.c" combos.mc "$src/kmtail.c" > keymap.c

qmk compile
mv ~/_code/qmk/qmk_firmware/keycapsss_kimiko_rev1_mymap.hex $src
