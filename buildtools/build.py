#!/usr/bin/env python3
import sys
import subprocess
from pathlib import Path


def run_step(description: str, command: list[str], cwd: Path | None = None) -> None:
    """Run a command and print nice status messages"""
    print(f"\n→ {description}")
    print("   " + str(command))
    
    try:
        result = subprocess.run(
            command,
            check=True,
            cwd=cwd,
            text=True,
            capture_output=True
        )
        if result.stdout.strip():
            print("   Output:", result.stdout.strip())
    except subprocess.CalledProcessError as e:
        print(f"\nERROR: Command failed with exit code {e.returncode}")
        if e.stdout:
            print("STDOUT:", e.stdout.strip())
        if e.stderr:
            print("STDERR:", e.stderr.strip())
        sys.exit(1)


def build_examples(root: str, preset: str, install_prefix: str):
    examples_dir = root / "examples"
    example_dirs = [
        p.relative_to(root) for p in examples_dir.iterdir()
        if p.is_dir() and (p / "CMakeLists.txt").exists()
    ]

    if not example_dirs:
        print("Warning: No example subdirectories with CMakeLists.txt found in examples/")

    steps = []

    vcpkg_prefix = root / "vcpkg_installed/x64-linux"
    cmake_prefix_path = f"{install_prefix}"
    if vcpkg_prefix.exists():
        cmake_prefix_path += f";{vcpkg_prefix}"

    for rel_example_src in example_dirs:
        example_src = root / rel_example_src
        example_build = example_src / "builds" / preset
        example_build.parent.mkdir(parents=True, exist_ok=True)

        configure_example_cmd = [
            "cmake",
            "-S", str(example_src),
            "-B", str(example_build),
            "--preset", preset,
            f"-DCMAKE_PREFIX_PATH={cmake_prefix_path}",
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        ]

        steps.append((f"Configuring example: {rel_example_src}", configure_example_cmd))

        build_example_cmd = ["cmake", "--build", str(example_build), "--parallel"]
        steps.append((f"Building example: {rel_example_src}", build_example_cmd))

    if example_dirs:
        print(f"   Examples found: {', '.join(str(d) for d in example_dirs)}")

        print("\nExample executables (assuming default target name matches folder):")
        for rel_example_src in example_dirs:
            folder_name = rel_example_src.name
            exe_path = root / rel_example_src / "builds" / preset / folder_name
            rel_exe = exe_path.relative_to(root)
            print(f"   {folder_name}: ./{rel_exe}")
        print("\n(Adjust if your CMake target names differ .)\n")
    print("="*60 + "\n")

    return steps

def build_main(root: str,  preset: str, install_prefix: str):
    steps = []
    main_build_dir = root / "builds" / preset

    main_build_dir.mkdir(parents=True, exist_ok=True)

    configure_main_cmd = [
        "cmake",
        "-S", str(root),
        "-B", str(main_build_dir),
        "--preset", preset,
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        f"-DCMAKE_INSTALL_PREFIX={install_prefix}",
    ]


    steps.append(("Configuring main project", configure_main_cmd))

    build_main_cmd = ["cmake", "--build", str(main_build_dir), "--parallel"]
    steps.append(("Building main project", build_main_cmd))

    install_cmd = [
        "cmake",
        "--install", str(main_build_dir),
        "--prefix", str(install_prefix),
    ]

    steps.append(("Installing main project", install_cmd))

    print(f"\n=== Generating project with preset: {preset} ===")
    print(f"   Root:           {root}")
    print(f"   Install prefix: {install_prefix}")
    print(f"   Main build dir: {main_build_dir}")

    return steps

def main():
    if len(sys.argv) != 2:
        print("Usage: build.py <preset-name>")
        print("Examples:")
        print("   ./build.py debug")
        sys.exit(1)

    preset = sys.argv[1].strip()
    root = Path(__file__).parent.parent.resolve()


    install_prefix = root / "install"


    steps = build_main(root, preset, str(install_prefix))
    steps += build_examples(root, preset, str(install_prefix))

    for description, command in steps:
        run_step(description, command)


    print("\n" + "="*60)
    print()

    print("SUCCESS! All projects built and installed.")
    print(f"Main install location: {install_prefix}")


if __name__ == "__main__":
    main()
