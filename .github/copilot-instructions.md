# Copilot Instructions for CYD LVGL Template

## Project Architecture

- Boards: `JC2432W328R` (resistive), `JC2432W328C` (capacitive), `JC4827W543R` (resistive)
- LVGL-only UI (no Squareline). Display via `TFT_eSPI`; touch via `XPT2046_Touchscreen` (R) or `bb_captouch` (C)
- Per-board PlatformIO environments in `platformio.ini` set pins, rotation, and model macros
- Templates in `template files/` are auto-copied by `scripts/copy_template.py` before build; no manual copying

## Environments (platformio.ini)

- `env:jc2432w328r`: `-DMODEL_JC2432W328R -DTFT_ROTATION=1`, XPT2046 SPI, `TOUCH_CS=33`, `TOUCH_IRQ=36`
- `env:jc2432w328c`: `-DMODEL_JC2432W328C -DTFT_ROTATION=1`, CST820 I2C, `I2C_SDA=33`, `I2C_SCL=32`, `CST820_INT=21`, `CST820_RST=25`
- `env:jc4827w543r`: `board=esp32-s3-devkitc-1`, `-DMODEL_JC4827W543R -DTFT_ROTATION=1`, NV3041A over QSPI with Arduino_GFX (`cs=45 sck=47 d0=21 d1=48 d2=40 d3=39`), XPT2046 SoftSPI (`TOUCH_CS=38`, `TOUCH_IRQ=3`, `TOUCH_MOSI=11`, `TOUCH_MISO=13`, `TOUCH_CLK=12`)

## Display Geometry

- CYD (2432): panel 240×320; LVGL size derives from `TFT_ROTATION`
- JC4827: panel 480×272 (ESP32-S3, NV3041A via Arduino_GFX); LVGL size derives from `TFT_ROTATION`

## Workflows

- Build: `pio run -e <env>` (default env is `jc2432w328c`)
- Upload: `pio run -e <env> --target upload`
- UI: Implement screens/events directly in `src/main.cpp` (and other `src/*.cpp`)
- Templates auto-apply: `template files/lv_conf.h` and `template files/<env>/User_Setup.h` are copied under `.pio/libdeps/<env>/`

## Project Patterns

- Pins are defined in both `platformio.ini` (build flags) and `template files/<env>/User_Setup.h`. Keep them consistent
- JC4827 uses Arduino_GFX NV3041A over QSPI and XPT2046 on SoftSPI; do not assume shared TFT SPI pins
- Resistive touch needs calibration ranges (TOUCH_X/Y_MIN/MAX) if points are offset; set via build flags
- Minimal loop for UI responsiveness; no scheduler/sensor manager by default

## Key Files

- `platformio.ini`: environments, build flags, libraries
- `scripts/copy_template.py`: pre-build copier for LVGL/TFT configs
- `template files/<env>/User_Setup.h`: TFT_eSPI per-board config
- `src/main.cpp`: app entry and LVGL logic

## References

- See `README.md` for pinouts, build steps, and board links

---

If anything is ambiguous, consult `platformio.ini` and `README.md`, or ask for clarification.
