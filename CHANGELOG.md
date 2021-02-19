# Change Log

All notable changes will be documented in this file.

## Head

## 0.6.1 &ndash; 2021-02-19

### Added

* Example demonstrating how to import third-party data sources using
  the Flatbuffers encoding format

## 0.6.0 &ndash; 2021-02-02

### Changed

* Now using `absl::flags` (instead of gflags)

## 0.5.0 &ndash; 2020-12-04

### Changed

* `PositionUpdate` can include `Side::UNDEFINED` to report net positions.

## 0.4.5 &ndash; 2020-11-09

### Changed

* Refactored `example-3` for readability
* Replaced `Application` with `Service`

## 0.4.4 &ndash; 2020-09-20

### Added

* `example-4` as a simple subscriber.

## 0.4.3 &ndash; 2020-09-02

### Changed

* `example-3` using new simulator interface.

## 0.4.2 &ndash; 2020-07-27

### Removed

* Automake support

## 0.4.1 &ndash; 2020-07-17

### Changed

* CMake is now the default build system for all Roq solutions.

## 0.4.0 &ndash; 2020-06-30

### Changed

* New signature for all `Event` interfaces
* Combined `{Daily|Session}Statistics` into `StatisticsUpdate`.

## 0.3.9 &ndash; 2020-06-09

### Added

* Linking to `roq-logging`.

### Removed

* Linking to `roq-api` (now header-only).

## 0.3.8 &ndash; 2020-06-06

## 0.3.7 &ndash; 2020-05-27

## 0.3.6 &ndash; 2020-05-02

## 0.3.5 &ndash; 2020-04-22

## 0.3.4 &ndash; 2020-04-08

## 0.3.3 &ndash; 2020-03-04
