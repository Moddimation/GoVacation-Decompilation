#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
import os
import glob
from pathlib import Path
from typing import Any, Dict, List, Tuple

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "SGVEAF_D",  # USA Debug Proto
    "SGVEAF",  # USA Release
    "SGVPAF",  # Europe Release
    "SGVJAF",  # Japan Release
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "latest"
config.dtk_tag = "v1.5.1"
config.objdiff_tag = "v3.0.0-beta.8"
config.sjiswrap_tag = "v1.2.1"
config.wibo_tag = "0.6.16"

# Project

## Helper (Guess who generated this)
def cflags_paths_expand(cflags):
    seen = set()
    converted = []

    for flag in cflags:
        if flag.startswith("-i "):
            path = flag[3:]
            if any(c in path for c in "*?[]"):
                matches = glob.glob(path, recursive=True)
                for p in matches:
                    norm_path = os.path.normpath(p)
                    flag_str = f"-i {norm_path}"
                    if os.path.isdir(p) and flag_str not in seen:
                        converted.append(flag_str)
                        seen.add(flag_str)
            else:
                if flag not in seen:
                    converted.append(flag)
                    seen.add(flag)
        else:
            if flag not in seen:
                converted.append(flag)
                seen.add(flag)

    cflags[:] = converted

config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I {config.build_dir}/{config.version}/include",
    f"-I {config.build_dir}/{config.version}/bin",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# WIP Feature to specify separating by libraries
config.nolib = [
    "*", # Leaving libraries unused for now, needs decomp to be finished
]

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-W all",
    "-O4,p",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-i decomp/Prog/project",
    "-i decomp/CodeWarrior/**/Include",
    "-i decomp/CodeWarrior/**/INCLUDE",
    f"-i {config.build_dir}/{config.version}/include",
    f"-DVERSION_{config.version}",
]
cflags_gc = [
    *cflags_base,
    "-multibyte",
]
cflags_wii = [
    *cflags_base,
    "-enc SJIS",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_cw = [
    *cflags_gc,
    "-Cpp_exceptions off",
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
    "-func_align 4",
]

# REL flags
cflags_rel = [
    *cflags_wii,
    "-sdata 0",
    "-sdata2 0",
]

# fill in * , because MWCC not supporting that
cflags_paths_expand(cflags_base)
cflags_paths_expand(cflags_cw)

config.linker_version = "Wii/1.5"

# Helper function for SDK libraries
def SDKLib(lib_name: str, files: List[Tuple[bool, str]], lib_name_override: str="", conf: Dict[str,str]={"":""}) -> Dict[str, Any]:
    objects = []
    if lib_name_override == "":
        lib_name_override = str

    dirname = f"SDK/src/{lib_name}"
    for matching, filename in files:
        filepath = f"{dirname}/{filename}"
        objects.append(Object(matching, filepath))

    __cflags = cflags_sdk + [f"-i decomp/{dirname}"]

    return {
        "lib": lib_name_override,
        "cflags": __cflags,
        "progress_category": "sdk",
        "src_dir": "decomp",
        "objects": objects,
        **conf
    }

# Helper function for Dolphin libraries
def RevolutionLib(lib_name: str, files: List[Tuple[bool, str]], conf:Dict[str,str]={"":""}) -> Dict[str, Any]:
    return SDKLib(f"dolphin/{lib_name}", files, f"{lib_name}")

# Helper function for CodeWarrior runtime libraries
def CWLib(lib_name: str, sub_path: str, files: List[Tuple[bool, str]], conf: Dict[str, str]={"":""}) -> Dict[str, Any]:
    objects = []
    dirpath = f"CodeWarrior/PowerPC_EABI_Support/{sub_path}"
    for matching, filename in files:
        filepath = f"{dirpath}/{filename}"
        objects.append(Object(matching, filepath))

    __cflags = cflags_cw + [f"-i decomp/{dirpath}"]

    return {
        "lib": lib_name,
        "mw_version": "GC/3.0a5",
        "cflags": __cflags,
        "progress_category": "cw",
        "src_dir": f"decomp",
        "objects": objects,
        **conf
    }

# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "cflags": cflags_rel,
        "progress_category": "game",
        "src_dir": f"decomp",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False

config.libs = [

    # CodeWarrior

    CWLib("Runtime.PPCEABI.H", "Runtime/Src", [
        (NonMatching, "__init_cpp_exceptions.cpp"),
        (Matching, "__mem.c"),
        (Matching, "global_destructor_chain.c"),
    ]),
    
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
#def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
#    # Don't modify the link order for matching builds
#    if not config.non_matching:
#        return objects
#    if module_id == 0:  # DOL
#        return objects + ["dummy.c"]
#    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Main Game"),
    ProgressCategory("lib", "Libraries"),
    ProgressCategory("sdk", "Revolution SDK"),
    ProgressCategory("cw", "CodeWarrior Runtime"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
