## Motivation

This project is inspired by [factoriotools/factorio-docker](https://github.com/factoriotools/factorio-docker) and aims to make minimalistic docker image for running [Factorio server](https://factorio.com/download).

## Usage

### Running

#### Quick start

```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    factorio-server-dockerized
```
This will load the latest save located at `server-data/saves` directory if a `.zip` file exists there. Otherwise it will create a new save file based on map settings (`./at-host/configs/map-gen-settings.json` and `./at-host/configs/map-settings.json` in the example above) and load it. If map settings are missing they are being copied from corresponding examples (`factorio/data/map-gen-settings.example.json` and `factorio/data/map-settings.example.json`).

#### Creating a new save

```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    factorio-server-dockerized init
```
This will create a new save and load it.

The save is being created based on map settings (`./at-host/configs/map-gen-settings.json` and `./at-host/configs/map-settings.json` in the example above). If map settings are missing they are being copied from examples (`factorio/data/map-gen-settings.example.json` and `factorio/data/map-settings.example.json`).

Default run actually fallbacks to this run when there is no zip file in `server-data/saves` directory.


#### Loading a specific save

```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    factorio-server-dockerized start-save save42.zip 
```
This will load a save file named `save42.zip` from `server-data/saves` directory.

Save file name can be also provided via `SAVE_FILE_NAME` environment variable, for instance like this:
```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    --env SAVE_FILE_NAME="save42.zip" \
    factorio-server-dockerized start-save
```

Command `start-save` can be shortened to `save`.

#### Loading latest save

```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    factorio-server-dockerized start-latest
```
This will load a save with the latest modification date in `server-data/saves` directory.

This is default behaviour and default run actually does this when there is a zip file in `server-data/saves` directory.

Command `start-latest` can be shortened to `latest`.


#### Starting scenario

```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    factorio-server-dockerized start-scenario base/wave-defense
```
This will load `base/wave-defense` scenario. 

Scenario identfier format is `[MOD/]NAME`:
> The scenario is looked for inside the given mod. If no mod is given, it is looked for in the top-level scenarios directory.

Therefore in case scenario name does not contain mod name the scenario will be searched at `/opt/factorio/scenarios` directory within a container.

Like loading a specific save, scenario can be specified via `SCENARIO` environment variable:
```bash
docker run \
    --detach \
    -p 34197:34197/udp \
    -p 27015:27015/tcp \
    --mount type=bind,source=./at-host/data,target=/opt/factorio/server-data \
    --mount type=bind,source=./at-host/configs,target=/opt/factorio/configs \
    --mount type=bind,source=./at-host/mods,target=/opt/factorio/mods \
    --name factorio-server \
    --env SCENARIO="base/wave-defense" \
    factorio-server-dockerized start-scenario
```

Command `start-scenario` can be shortened to `scenario`.

### Volumes

This image uses 3 volumes:
- **data** - should be mounted to `/opt/factorio/server-data`, stores all data (such as `player-data.json`) and `saves` directory
- **configs** - should be mounted to `/opt/factorio/configs`, stores all server and map settings
- **mods** - should be mounted to `/opt/factorio/mods`, stores all mods and `mod-list.json` file


### Configuring

Most of the Factorio configuration can be made via configurations files:
- `configs/server-settings.json`
- `configs/map-settings.json`
- `configs/map-gen-settings.json`

Also enabled and disabled mods can be configured via file `mods/mod-list.json`.


#### Space Age DLC

To switch Space Age, you can modify `mod-list.json` file within `mods` directory or use `DISABLE_SPACE_AGE_DLC` environment variable.

By default, the DLC is enabled, it can be also turned off by setting environment variable `DISABLE_SPACE_AGE_DLC` to `true`.

Using `DISABLE_SPACE_AGE_DLC=true` turns components of the DLC (such as `quality`) off. These components/mods can be enabled individually with disabled Space Age DLC by using `ENABLE_SPACE_AGE_<COMPONENT_NAME>` environment variables. So, `ENABLE_SPACE_AGE_QUALITY=true` turns on quality feature.

**Examples**:

- All components of Space Age and the DLC are disabled, except `elevated-rails`:
```bash
DISABLE_SPACE_AGE_DLC=true
ENABLE_SPACE_AGE_ELEVATED_RAILS=true
```

- All components of Space Age and the DLC are disabled:
```bash
DISABLE_SPACE_AGE_DLC=true
```

- Everything is enabled:
```bash
# Default behaviour and can be skipped
DISABLE_SPACE_AGE_DLC=false
```

#### Mods

By default all mods discovered in `mods` directory are enabled. They can be manually reordered or disabled via `mod-list.json` file in the directory. They can be diabled also by environment variables: `DISABLE_<MOD_NAME>_MOD=true`. For instance, to disable `test-mod-name` mod provide `DISABLE_TEST_MOD_NAME=true` environment variable.

Mods settings are stored in `mod-settings.dat` file also at `mods` directory. The only option to edit these settings is to put pre-configured file (from full game) in the directory.