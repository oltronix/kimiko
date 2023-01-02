# OLED_ENABLE = yes
# OLED_DRIVER = SSD1306   # Enables the use of OLED displays
# ENCODER_ENABLE = yes       # ENables the use of one or more encoders
RGBLIGHT_ENABLE = no      # Enable keyboard RGB underglow

# LTO: Link Time Optimizations.
# Reduce compiled size, but will automatically disable the legacy TMK Macros and Functions features.
# This does not affect QMK Macros and Layers
LTO_ENABLE = yes

KEY_OVERRIDE_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes

SRC += oneshot.c
SRC += tapdance.c
SRC += achordion.c

