---
title: Development
description: Building, testing, and contributing to Periapsis
---

# Development

This guide covers building Periapsis from source and contributing to the project.

## Requirements

| Tool | Version | Purpose |
|------|---------|---------|
| Python | 3.14+ | Build tooling via CPPython |
| CMake | 4.0+ | Build system |

## Quick Start

=== "Windows"

    ```powershell
    # Clone the repository
    git clone https://github.com/synodic/periapsis.git
    cd periapsis

    # Install Python dependencies
    pdm install

    # Optional: install the project through Porringer
    porr install
    ```

=== "Linux/macOS"

    ```bash
    # Clone the repository
    git clone https://github.com/synodic/periapsis.git
    cd periapsis

    # Install Python dependencies
    pdm install

    # Optional: install the project through Porringer
    porr install
    ```

Installation only synchronizes Python and project tooling. It does not apply
the shared Copier configuration.

## Building with CMake

*TODO*

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

Periapsis uses `synodic/configuration` as a versioned Copier overlay. Updating
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
