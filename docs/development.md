---
title: Development
description: Building, testing, and contributing to Periapsis
---

# Development

This guide covers building Periapsis from source and contributing to the project.

## Requirements

| Tool | Version | Purpose |
|------|---------|---------|
| [Porringer](https://github.com/synodic/porringer) | fork changes | Installs declared tools and prepares the project |
| Python | 3.15+ | Build tooling via CPPython |
| CMake | 4.5.0-rc1+ | Build system |
| Ninja | latest | Build tool |
| C++ compiler | Matches the chosen identity | Compiles the project |

## Quick Start

`porr install` installs the declared tools, synchronizes the Python project, and
runs the project's `pdm run prepare` step to prepare native test dependencies.
Preview the commands with `porr preview`.

This fork pins CPPython to an unreleased commit in `Behemyth/cppython`. It also
requires the corresponding unreleased Porringer fork changes; the published
Porringer release does not provide this setup. With pipx using Python 3.15 or
newer, install the tested fork commit before running `porr install`:

```shell
pipx install "git+https://github.com/Behemyth/porringer.git@812b0cb5f2268a845af246273771b876f2cc3b35"
```

Use pipx's `--python` option if its default interpreter is older than 3.15.

For automation, `porr install --json --yes` approves the installation and
returns one versioned JSON result on stdout, with human-readable logs on
stderr. Preparation reports the selected identity, build-system backend,
dependency provider, and authoritative output directory. Successful preparation
does not verify a native build or test run; run the CMake workflow separately.

The current Python lock selects CMake 4.4.4, which is too old for this project.
Until a compatible CMake is available in the Python package and the lock is
updated, install [CMake 4.5](https://cmake.org/download/) separately and point
`CMAKE_BINARY` at its executable before running setup. This manual prerequisite
means setup is not yet fully one-click.

=== "Windows"

    ```powershell
    # Select the installed CMake 4.5 executable for CPPython
    $env:CMAKE_BINARY = 'C:\Program Files\CMake\bin\cmake.exe'
    & $env:CMAKE_BINARY --version

    # Clone the repository
    git clone https://github.com/Behemyth/periapsis.git
    cd periapsis

    # Install the toolchain and Python dependencies
    porr install
    ```

=== "Linux/macOS"

    ```bash
    # Select the installed CMake 4.5 executable for CPPython
    export CMAKE_BINARY=/absolute/path/to/cmake
    "$CMAKE_BINARY" --version

    # Clone the repository
    git clone https://github.com/Behemyth/periapsis.git
    cd periapsis

    # Install the toolchain and Python dependencies
    porr install
    ```

The toolchain is declared in `tool/porringer/porringer.json`.

Installation does not apply the shared Copier configuration.

For a persistent, machine-local CPPython override, create an ignored
`.cppython.toml` at the project root instead of setting `CMAKE_BINARY`:

```toml
[generators.cmake]
cmake_binary = "C:/Program Files/CMake/bin/cmake.exe"
```

Use the path to your own executable. If both overrides are set, they must
identify the same executable. Once the Python lock provides a compatible CMake,
remove the override to use that packaged version.

## Building with CMake

After preparation, `pdm run build` and `pdm run test` use the host's default
Development preset unless a local default is configured.

For direct CMake use, select a compatible executable in your terminal or IDE.
`CMAKE_BINARY` selects the executable for CPPython; it does not change the
executable selected by an IDE or by a plain `cmake` command. For example, in
PowerShell:

```powershell
& $env:CMAKE_BINARY --list-presets=all
& $env:CMAKE_BINARY --workflow --preset windows-x64-msvc
```

The identity workflow builds and tests Debug, Development, and Release.
Single-config presets append the configuration, such as
`windows-x64-msvc-development`, and work with Visual Studio Open Folder and
VS Code CMake Tools. Visual Studio or the terminal must have a matching C++
compiler environment on `PATH`. Re-run `pdm run prepare` after changing the
identity or native dependency declarations.

On macOS with Homebrew LLVM, CMake cannot find libc++'s `import std` manifest
automatically. Before configuring, set `SYNODIC_CXX_STDLIB_MODULES_JSON` to the
manifest path:

```bash
export SYNODIC_CXX_STDLIB_MODULES_JSON="$(brew --prefix llvm)/lib/c++/libc++.modules.json"
```

Leave the variable unset on other hosts so CMake detects the manifest itself.

## CI and optional testing

The full native checks run automatically for pull requests and merge queues,
not for standalone pushes or on a schedule. Pushing to an open pull request
still updates its normal CI checks. Fast checks and identity preflight run
concurrently; native lanes start once preflight succeeds. The required result
still needs every stage and lane to pass.

Each lane builds and tests its multi-config tree, then configures every
single-config preset without building a duplicate tree. The CI adapter executes
the committed workflow steps, collects independent build/test failures, and
continues later configurations when possible. A CI-only CMake presets file
keeps tests quiet and writes JUnit results without changing local presets.

The lane artifact `native-<identity>-<attempt>` contains the stage logs,
`result.json`, a concise reproduction summary, JUnit XML, and SARIF source
diagnostics. Clang and MSVC errors also appear as check annotations. SARIF is
generated from compiler text output; check jobs do not upload to code scanning
or require write permissions. A report never overrides a failed required job.

Hosted Windows lanes use a temporary ReFS Dev Drive for the copied checkout,
Conan home, and build trees. This setup affects only the hosted runner, never
the developer's local drive.

Users and agents can request an additional single-identity run through GitHub
Actions' **Run workflow** option or the GitHub CLI:

```shell
gh workflow run native-check.yml --repo Behemyth/periapsis --ref YOUR_BRANCH -f identity=windows-x64-msvc
```

Choose a branch containing the manual workflow and an identity declared by that
workflow. The same command works for `Behemyth/cppython` with its declared
identities. Manual runs use a separate, non-required result and do not replace
the full pull-request checks.

Pull-request and merge-queue runs restore the shared Conan package cache but
never save it. Once this workflow is on `development`, warm the cache for an
identity by running the same command with `--ref development`; only a
successful manual run on the default branch saves.

## Documentation

Documentation is built with [Zensical](https://github.com/synodic/zensical) :

```bash
# Serve locally
zensical serve
```

```bash
# Build static site
zensical build
```

## Shared configuration

This fork uses `Behemyth/configuration` as a versioned Copier overlay. Updating
that overlay is an explicit maintenance operation and must be reviewed like a
normal code change.

Check for a newer configuration release without changing files:

    pdm run config-check

Before applying an update, commit or stash any unrelated work so the
destination tree is clean. Then run:

    pdm run config-update

Review the generated diff, including `.copier-answers.yml`. Resolve any inline
conflict markers before committing. A Copier update may also produce `.rej`
files when rejection mode is used; those files must be reviewed and removed.
Run the relevant build, test, and documentation checks, then commit the
configuration update separately from unrelated changes.

The `pdm install`, `pdm update`, and `porr install` commands never run Copier.
If the template has no published Git tags yet, `config-check` cannot compare
stable releases; publish the configuration baseline and release tags before
enabling routine updates.
