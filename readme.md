# Kimiko Keyboard Configuration

Custom QMK keymap for the [Keycapsss Kimiko](https://keycapsss.com) split keyboard with Helios controllers.

## Build Instructions

### Prerequisites
- Docker installed and running

### Building the Firmware

1. **Build the Docker image** (first time or after QMK updates):
   ```bash
   docker build -t kimiko-build .
   ```

2. **Compile the firmware**:
   ```bash
   docker run -v $(pwd):/qmk/keymap kimiko-build
   ```

3. **Flash**: Copy `output/keycapsss_kimiko_rev1_kimiko_helios.uf2` to your keyboard in bootloader mode.

### Refreshing QMK Firmware

To pick up upstream QMK changes, rebuild with cache busting:
```bash
docker build --build-arg QMK_CACHE_BUST=$(date +%s) -t kimiko-build .
```

### Clean Build

If you encounter build issues, clear the build directories:
```bash
rm -rf build/ output/
docker run -v $(pwd):/qmk/keymap kimiko-build
```

### Configuration

The build can be customized via environment variables:
```bash
docker run -v $(pwd):/qmk/keymap \
  -e KEYBOARD=keycapsss/kimiko/rev1 \
  -e KEYMAP=kimiko \
  -e CONVERT_TO=helios \
  kimiko-build
```

### Debugging

Shell into the container for debugging:
```bash
docker run -it --rm --entrypoint /bin/bash -v $(pwd):/qmk/keymap kimiko-build
```

## Features

### OS Toggle (Mac/Windows)
Toggle between Mac and Windows key mappings with `MC_TOGGLE`. The current OS is displayed on the secondary OLED. Key combinations like copy/paste automatically use the correct modifier (Cmd on Mac, Ctrl on Windows).

### Power Tabbing
Hold the layer key on enter to activate navigation mode:
- `ATAB_DOWN`/`ATAB_UP`: Alt-Tab application switching
- Arrow keys navigate within the Alt-Tab window
- `CTAB_LEFT`/`CTAB_RIGHT`: Tab switching within current application
- `ATAB_LEFT`/`ATAB_RIGHT`: Mac window switching (when Alt-Tab not active)

### Window Management
Keys for managing windows on both Mac and Windows:
- Maximize, minimize, left/right snap
- Lock screen, screenshot

**Mac shortcuts that need manual setup:**
- Maximize: Window → Zoom: `Ctrl+Cmd+Z`
- Move Left: `Ctrl+Cmd+Alt+L`
- Move Right: `Ctrl+Cmd+Alt+R`

### Layers
- `_COLEMK`: Default Colemak-DH layout
- `_QWERTY`: QWERTY layout
- `_WINNAV`: Window navigation and management
- `_VINAV`: Vi-style navigation
- `_NUM`: Number pad
- `_SYM`: Symbols
- `_MOUSE`: Mouse keys
- `_CONF`: Configuration layer
- `_SELECT`: Selection mode

## Project Structure

```
├── build.sh        # Build orchestration script
├── dockerfile      # Docker build environment
├── keymap.json     # QMK Configurator keymap export
├── custom.c        # Custom keycode handlers and OLED code
└── src/
    ├── config.h    # QMK configuration
    ├── rules.mk    # Build rules and features
    ├── oneshot.c/h # One-shot modifier implementation
    ├── swapper.c/h # Window/app switching
    ├── tapdance.c/h# Tap dance handlers
    └── achordion.c/h # Home row mod chord detection
```
