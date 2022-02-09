qmk json2c v2.json -o build/keymap.c
src=$(pwd)
dest=~/_code/qmk/qmk_firmware/keyboards/keycapsss/kimiko/keymaps/v2
rm -r pub
break=$(grep -Fn "const uint16" build/keymap.c | cut -f1 -d:)
break=$[break-1]

cat build/keymap.c | tail -n +$break > build/keys.c
cp -R src/. pub
cat custom.c build/keys.c > pub/keymap.c

cp -R pub/. $dest
cd $dest
qmk compile -km v2 -c
cd $src
mkdir map
ls ~/_code/qmk/qmk_firmware -d ~/_code/qmk/qmk_firmware/* | grep hex | xargs cp -t ./map/
