V="v4"
kb="keycapsss/kimiko/rev1"
cd v4
qmk json2c keymap.json -o build/keymap.c
src=$(pwd)
dest=../qmk_firmware/keyboards/keycapsss/kimiko/keymaps/$V
rm -rf pub
break=$(grep -Fn "const uint16_t PROGMEM keymaps" build/keymap.c | cut -f1 -d:)
break=$[break-1]

cat build/keymap.c | tail -n +$break > build/keys.c
cp -R src/. pub
cat custom.c build/keys.c > pub/keymap.c

cp -R pub/. $dest
cd $dest
qmk compile -e CONVERT_TO=helios -km $V -c -kb $kb
cd $src
mkdir map
ls ../qmk_firmware -d ../qmk_firmware/* | grep uf2 | xargs cp -t ./map/
