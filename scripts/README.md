PlatformIO copy templates helper

This folder contains a simple SCons/PlatformIO extra script which copies files from

    template files/

into your project's .pio/libdeps/<env> library directories. It is run automatically as a pre-build step through the `extra_scripts` setting in `platformio.ini`.

Per-environment overrides

If you have files specific to an environment (for example, `jc2432w328c`), place them under

    template files/jc2432w328c/

The script will prefer files from that folder for the env with the same name.

Files copied by default

- `User_Setup.h` -> copied into the `TFT_eSPI` library folder under `.pio/libdeps/<env>/TFT_eSPI/` when found in `template files/` or per-env folder.
- `lv_conf.h` -> copied to the `.pio/libdeps/<env>/` root (next to the `lvgl` library folder) — this mirrors the LVGL configuration requirement.

Behavior

- Existing files are overwritten by the templates (no `.bak` is created).
- The script logs what it copies and whether anything is skipped.

If you want to add more templates, update `scripts/copy_template.py` FILES_TO_COPY mapping.
