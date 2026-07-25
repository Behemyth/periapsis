---
title: Native test bootstrap plan
description: Plan for preparing Periapsis native tests through Porringer and CPPython
---

# Native test bootstrap plan

## Outcome

Make `porr install` leave Periapsis ready for VS Code's native CMake/CTest Test
Explorer. After a one-time editable CPPython installation, the install flow
will install native dependencies, activate the host toolchain, configure the
selected generator, build the test target, and verify that tests are
discoverable without running them.

The recommended route is to use PDM's built-in `post_install` project script
to call a new idempotent `cppython bootstrap` command. This avoids a PDM plugin
bootstrap cycle and requires no Porringer code change.

## CPPython lifecycle

1. Add a provider-neutral `[tool.cppython.bootstrap]` model under
   `CPPythonLocalConfiguration` with `configure`, `build`, `test`, `targets`,
   `require-tests`, and `environment` fields. Lifecycle and environment values
   may be scalars or host maps keyed by normalized `windows`, `linux`, and
   `macos`. Resolve the current host centrally, while allowing `.cppython.toml`
   to override checked-in project values.
2. Add `Project.bootstrap(...)` and the `cppython bootstrap` CLI command. Keep
   existing standalone `build` and `test` commands and their configuration
   fallback behavior.
3. Preserve the existing third-party `Generator` protocol. Add a separate
   structural `BootstrapGenerator` capability with `configure(configuration)`,
   `build_targets(configuration, targets)`, and
   `discover_tests(configuration) -> list[str]`.
4. Run bootstrap in this order: install provider tooling and dependencies,
   synchronize generated files, configure the generator, build requested
   targets, and validate test discovery.
5. Raise a clear `NotSupportedError` when bootstrap is requested from a
   generator without the `BootstrapGenerator` capability. Built-in CMake and
   Meson generators must implement the capability.
6. Make `require-tests=true` fail when discovery returns zero tests. Bootstrap
   must never execute the tests.

## Generator and host tooling

### CMake and Meson

1. Separate the CPPython-managed CMake preset fragment (`preset_file`) from
   the preset graph used for commands (`presets_file`). Default the latter to
   the project-root `CMakePresets.json`.
2. Run CMake configure, build, and CTest from the project root with an
   absolute `--presets-file` argument. Forward requested targets to
   `cmake --build`.
3. Require CMake 4.4 for this lifecycle and parse
   `ctest --show-only=json-v1` for test discovery. This fixes the current
   failure mode where commands run beside a common preset fragment that cannot
   see the visible platform presets.
4. Expose the existing Meson setup path as configure, pass requested targets
   to `meson compile`, and parse `meson introspect --tests` output.
5. Keep Conan and vcpkg provider APIs unchanged. They continue producing
   generator-specific synchronization data.

### Windows environment

1. Add a reusable scoped command-environment service in CPPython.
2. For an `msvc` environment, accept an already usable developer environment
   or locate Visual Studio with `vswhere`, invoke `VsDevCmd.bat` for x64, and
   capture its environment.
3. Activate the mapping through a scoped context for each native lifecycle
   entry point. Make `run_subprocess` pass the mapping explicitly to child
   processes.
4. Move Conan default-profile detection out of `ConanProvider.__init__` into
   install/update. Temporarily overlay `os.environ` around Conan's in-process
   API calls so profile detection and package builds see the same MSVC
   environment, then restore it.
5. Do not persist a machine-specific environment snapshot. Fail early when
   MSVC cannot be activated. Non-Windows profiles default to `inherit`.

### Tool prerequisites

Set CPPython's `cmake` extra to include `cmake>=4.4` and `ninja>=1.13`.
Resolve CMake, CTest, and Ninja from CPPython's running pipx environment before
configured paths and ambient `PATH` entries. Fail before dependency
installation when a selected profile's required generator or compiler is
unavailable.

Install the sibling CPPython checkout once from PowerShell:

```powershell
pipx install --editable --force --include-deps "D:\projects\synodic\cppython[cmake,conan,git]"
```

The `--include-deps` option exposes the required application shims, including
CMake, CTest, Ninja, Conan, and Git where those extras provide them.

## Periapsis adoption

1. Raise the CMake baseline to 4.4 in `CMakeLists.txt` and the root preset
   metadata. Upgrade preset schemas only where a newer field requires it.
2. Remove `cppython[cmake,conan,git]` from `[build-system].requires` in
   `pyproject.toml`. `pdm.backend` does not import CPPython, and PEP 517
   isolation cannot provide the durable editable CLI or IDE build tree.
3. Add portable bootstrap host maps in `pyproject.toml`:
   - Windows: configure `windows-ninja-msvc`, build/test
     `windows-ninja-msvc-debug`, environment `msvc`.
   - Linux: configure `linux-ninja-gcc`, build/test `linux-ninja-gcc-debug`,
     environment `inherit`.
   - macOS: configure `macos-ninja-clang`, build/test
     `macos-ninja-clang-debug`, environment `inherit`.
4. Set target `tests`, `require-tests=true`, and the CMake
   `presets-file = "CMakePresets.json"`. Keep Conan as Periapsis's selected
   provider.
5. Preserve `gtest_discover_tests(tests)` with POST_BUILD discovery. Building
   the explicit `tests` target remains necessary; CMake 4.4's `test_prep/all`
   is not a replacement.
6. Add `.cppython.toml` to `.gitignore` for machine-local overrides.
7. Add `[tool.pdm.scripts].post_install = "cppython bootstrap"`. PDM runs this
   built-in lifecycle hook after dependency synchronization and editable
   self-install, so Porringer's existing plain `pdm install` becomes the
   automatic trigger.
8. Document `pdm install --skip=post_install` as the explicit opt-out for
   dependency-only installs and previews. PDM's `--dry-run` still executes
   lifecycle scripts unless they are skipped.
9. Document the one-time pipx installation, CMake 4.4, Ninja, and Visual
   Studio C++ workload prerequisites, the state materialized by `porr install`,
   and the one unavoidable VS Code action: select the host's configure preset
   once on first open. CMake Tools can then reuse the persisted selection and
   discover the already-built CTest tree.

## Tests and rollout

1. Add CPPython unit coverage for host-map resolution and local overrides, CLI
   wiring, bootstrap ordering and failure propagation, required and optional
   zero-test behavior, CMake command construction, absolute preset-file
   handling, target forwarding, CTest JSON parsing, Meson behavior, and
   Windows `VsDevCmd` environment propagation.
2. Update mock generators and protocol tests for the optional
   `BootstrapGenerator` capability.
3. Add an integration fixture using CMake and a lightweight provider or mock.
   A clean bootstrap must configure a tree, build the requested test target,
   discover one or more tests, and remain idempotent on a second run.
4. Gate a real MSVC/Ninja integration case on Windows CI. Retain Linux and
   macOS generator cases where those toolchains are available.
5. Validate CPPython independently with lint, type checking, and its full test
   suite. Install it editable with pipx and confirm `cppython --help` resolves
   to the sibling checkout before applying the Periapsis hook.
6. Perform end-to-end acceptance from a clean ignored state:
   - Remove Periapsis's `build/` directory.
   - Run `porr install` from an ordinary PowerShell or VS Code terminal.
   - Verify the Conan toolchain, selected preset cache, Debug `tests`
     executable, GoogleTest metadata, and non-empty CTest JSON.
   - Run `porr install` again to prove idempotency.
   - Select the Windows configure preset once in VS Code, refresh CMake Tests,
     and run/debug an individual GoogleTest case in Test Explorer.

## Decisions and boundaries

- Use PDM's project-defined `post_install` script, not a CPPython PDM plugin.
  A plugin would need to be installed before it could observe the same install,
  creating a bootstrap cycle.
- Porringer needs no code change. Its preview and execution parity remain
  intact because the project command remains `pdm install`.
- Keep Periapsis on `pdm.backend`. CPPython's PEP 517 backend remains a
  packaging path, not the durable IDE-workspace bootstrap.
- Use native CMake Tools/CTest integration rather than a pytest bridge.
- Require CMake 4.4 and use `--presets-file`; do not rely on the command's
  working directory to select a preset graph.
- Commit portable host defaults, but do not commit an absolute sibling path or
  a captured MSVC environment.
- `porr install` prepares and validates tests but does not run them.
- Automatic selection of a VS Code configure preset in a brand-new workspace,
  replacing CMake Tools, changing Periapsis to `cppython.build`, running tests
  during install, changing Porringer's lifecycle, and redesigning the preset
  families are out of scope.
