"""
Simple, beginner‑friendly PlatformIO extra script

What this does (and why):
1) Copies files from your project's "template files/" folder into the active
   PlatformIO environment's libdeps directory .pio/libdeps/<env>/.
   - Keeping generated/config files inside libdeps keeps them per‑environment
     and out of source control (beginner‑friendly and reproducible).

2) Two common cases are handled out of the box:
   - User_Setup.h -> into the TFT_eSPI library folder so the library picks up
     your display setup. TFT_eSPI reads this from its own folder.
   - lv_conf.h   -> into the environment root (.pio/libdeps/<env>/) so LVGL
     can find your config during its build. LVGL searches next to its sources
     or in the compiler include path; placing it at env root makes it visible.

3) Optional per‑environment overrides:
   If you create a folder named "template files/<env_name>/", those files will
   be used instead of the default "template files/" versions for that env.

How to enable:
Add this to your platformio.ini environment section:
  extra_scripts = pre:scripts/copy_template.py
Using "pre:" ensures copies happen before compilation.
"""

import os
import shutil
import glob
from SCons.Script import Import

# PlatformIO provides a construction environment we can query
Import("env")

# Resolve key paths and the active env name in a robust, readable way
PROJECT_DIR = os.path.abspath(env.subst("$PROJECT_DIR"))  # project root
ENV_NAME = env.get("PIOENV") or os.environ.get("PIOENV")  # e.g. jc2432w328c
LIBDEPS_ROOT = (
    os.path.join(PROJECT_DIR, ".pio", "libdeps", ENV_NAME) if ENV_NAME else None
)

# Where your templates live inside the project (space in name is OK on all OSes)
TPL_DIR = os.path.join(PROJECT_DIR, "template files")
# Per‑environment override directory (optional)
ENV_TPL_DIR = (
    os.path.join(TPL_DIR, ENV_NAME) if ENV_NAME else None
)

# Declare what to copy in a very simple, readable structure.
# target: "libdir"  -> copy into the specific library folder (found by name/prefix)
#         "envroot" -> copy into .pio/libdeps/<env>/ so other libs can see it
FILES = [
    {"name": "User_Setup.h", "target": "libdir", "lib": "TFT_eSPI"},
    # Always use the top-level template for lv_conf.h (no per-env override)
    {"name": "lv_conf.h",   "target": "envroot", "per_env": False},
]


def get_src_for_item(item) -> str:
    """Resolve the source directory for a given file.

    Most files can use per‑env overrides if present, but some (like lv_conf.h)
    should always come from the top‑level template folder.
    """
    use_per_env = item.get("per_env", True)
    if use_per_env and ENV_TPL_DIR and os.path.isdir(ENV_TPL_DIR):
        return ENV_TPL_DIR
    return TPL_DIR


def find_library_dir(lib_name):
    """Find a library folder inside .pio/libdeps/<env>/ by prefix‑matching.

    Why prefix: PlatformIO often appends versions (e.g., "TFT_eSPI_ID1559").
    We prefer an exact name match and fall back to the first prefix match.
    """
    if not LIBDEPS_ROOT or not os.path.isdir(LIBDEPS_ROOT):
        return None
    candidates = glob.glob(os.path.join(LIBDEPS_ROOT, lib_name + "*"))
    if not candidates:
        return None
    for p in candidates:
        if os.path.basename(p) == lib_name:
            return p
    return candidates[0]


def safe_copy(src: str, dst: str) -> bool:
    """Copy a file, creating the destination directory if needed.

    Why copy2: preserves timestamps/metadata which helps repeatable builds.
    """
    try:
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)
        print(f"[copy_template] Copied: {src} -> {dst}")
        return True
    except Exception as e:
        print(f"[copy_template] ERROR copying {src} -> {dst}: {e}")
        return False


def run():
    # Basic sanity checks with helpful messages for beginners
    if not ENV_NAME:
        print("[copy_template] PIOENV not set. Is this running under PlatformIO? Skipping.")
        return

    if not os.path.isdir(TPL_DIR):
        print(f"[copy_template] Template directory not found: {TPL_DIR}. Nothing to do.")
        return

    # Ensure the env root exists so we can drop envroot files even on first build
    if LIBDEPS_ROOT and not os.path.isdir(LIBDEPS_ROOT):
        # Why create: sometimes the first run hasn't created libdeps yet.
        os.makedirs(LIBDEPS_ROOT, exist_ok=True)

    for item in FILES:
        name = item["name"]
        target = item["target"]
        src_dir = get_src_for_item(item)
        src = os.path.join(src_dir, name)

        if not os.path.isfile(src):
            print(f"[copy_template] Skipping (not found): {src}")
            continue

        if target == "libdir":
            lib_name = item.get("lib")
            lib_dir = find_library_dir(lib_name) if lib_name else None
            if not lib_dir:
                print(
                    f"[copy_template] Library '{lib_name}' not found in {LIBDEPS_ROOT}. "
                    "It will be copied on the next build once dependencies are installed."
                )
                continue
            dst = os.path.join(lib_dir, name)
            safe_copy(src, dst)

        elif target == "envroot":
            if not LIBDEPS_ROOT:
                print("[copy_template] No libdeps root available. Skipping envroot copy.")
                continue
            dst = os.path.join(LIBDEPS_ROOT, name)
            safe_copy(src, dst)

        else:
            print(f"[copy_template] Unknown target '{target}' for {name}. Skipping.")


# Execute when PlatformIO runs the extra script
run()
