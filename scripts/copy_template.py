# PlatformIO extra script to copy template files into libdeps
# Should be run as pre:extra script in platformio.ini (libs are installed before build)

import os
import shutil
import glob
from SCons.Script import Import

Import("env")

project_dir = env.subst('${PROJECT_DIR}')
pioenv = env.get('PIOENV', os.environ.get('PIOENV'))

# Files in this project's "template files/" dir that we will copy
TPL_DIR = os.path.join(project_dir, 'template files')
FILES_TO_COPY = [
    # Copy User_Setup.h -> TFT_eSPI lib folder (so TFT_eSPI picks up the project setup)
    { 'name': 'User_Setup.h', 'dest': 'lib', 'lib': 'TFT_eSPI' },
    # Copy lv_conf.h -> env root next to the lvgl folder (lv_conf.h must be available to the lvgl build)
    { 'name': 'lv_conf.h', 'dest': 'env_root', 'lib': 'lvgl' },
]

# Library name to target
LIB_NAME = 'TFT_eSPI'

# Optionally support per-environment template subdir
env_tpl_dir = os.path.join(TPL_DIR, pioenv) if pioenv else None

def find_lib_dir(project_dir, env_name, lib_name):
    # Prefer exact path
    base = os.path.join(project_dir, '.pio', 'libdeps', env_name)
    if not os.path.isdir(base):
        return None
    # Find library path by name prefix (works across versions)
    libs = glob.glob(os.path.join(base, lib_name + '*'))
    if not libs:
        return None
    # Prefer directory exactly named LIB_NAME
    for p in libs:
        if os.path.basename(p) == lib_name:
            return p
    # Otherwise return first match
    return libs[0]


def copy_file_to_env_root(project_dir, env_name, filename, lib_name=None):
    base = os.path.join(project_dir, '.pio', 'libdeps', env_name)
    if not os.path.isdir(base):
        return False

    # If a lib_name is given, ensure lib exists in this environment
    if lib_name:
        lib_exists = bool(glob.glob(os.path.join(base, lib_name + '*')))
        if not lib_exists:
            return False

    src = os.path.join(env_tpl_dir if env_tpl_dir and os.path.isdir(env_tpl_dir) else TPL_DIR, filename)
    if not os.path.isfile(src):
        return False

    dst = os.path.join(base, filename)
    try:
        # Overwrite the env-root target with template. No backup is kept by default.
        shutil.copy2(src, dst)
        print('[copy_template] Copied env root file: %s -> %s' % (src, dst))
        return True
    except Exception as e:
        print('[copy_template] Error copying %s -> %s: %s' % (src, dst, str(e)))
        return False


def copy_templates():
    if not pioenv:
        print('[copy_template] PIOENV not set. Skipping template copy.')
        return

    lib_dir = find_lib_dir(project_dir, pioenv, LIB_NAME)
    if lib_dir is None:
        print('[copy_template] Library %s not found in .pio/libdeps/%s' % (LIB_NAME, pioenv))
        return

    print('[copy_template] Found library folder: %s' % lib_dir)

    # Prefer per-env templates if present
    source_dir = env_tpl_dir if env_tpl_dir and os.path.isdir(env_tpl_dir) else TPL_DIR
    if not os.path.isdir(source_dir):
        print('[copy_template] Template directory not found: %s' % source_dir)
        return

    for f in FILES_TO_COPY:
        filename = f['name']
        dest_type = f.get('dest', 'lib')
        lib_name = f.get('lib')

        if dest_type == 'lib':
            src = os.path.join(source_dir, filename)
            if not os.path.isfile(src):
                print('[copy_template] Template file not found in %s: %s' % (source_dir, filename))
                continue
            dst = os.path.join(lib_dir, filename)
            try:
                # Overwrite the TFT_eSPI library file with the template.
                # No backup is created by default to avoid cluttering the project.

                shutil.copy2(src, dst)
                print('[copy_template] Copied: %s -> %s' % (src, dst))
            except Exception as e:
                print('[copy_template] Error copying %s -> %s: %s' % (src, dst, str(e)))
        elif dest_type == 'env_root':
            # Put file in .pio/libdeps/<env>/ to be visible to lvgl as described
            copied = copy_file_to_env_root(project_dir, pioenv, filename, lib_name)
            if not copied:
                print('[copy_template] env_root copy skipped or failed for %s (lib: %s)\n' % (filename, lib_name))

# Run the copier
copy_templates()
