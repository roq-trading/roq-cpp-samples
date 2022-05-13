# Change Log

All notable changes will be documented in this file.

## Head

## 0.8.4 &ndash; 2022-05-14

## 0.8.3 &ndash; 2022-03-22

### Changed

* Rename headers to .hpp (#195)
* Conda packaging should pin versions to match x.x.x (#189)
* The `--timer_freq` flag was incorrectly defaulting to zero (#188)

## 0.8.2 &ndash; 2022-02-18

### Changed                                                                                                                
                                                                                                                           
* Always log packaging and build information (#159)

## 0.8.1 &ndash; 2022-01-16

### Changed                                                                                                                
                                                                                                                           
* Upgrade to C++20 (#158)

## 0.8.0 &ndash; 2022-01-12

### Removed                                                                                                             

* `client::DepthBuilder` (#152)  

## 0.7.9 &ndash; 2021-12-08

### Changed

* Use `cache::MarketByPrice` instead of `client::DepthBuilder`
* `MarketByPriceUpdate` and `MarketByOrderUpdate` now include price/quantity decimals (#119)

## 0.7.8 &ndash; 2021-11-02

### Added

* Add `max_trade_vol` and `trade_vol_step_size` to ReferenceData (#100)
* Demonstrate how to conservatively measure queue position of limit order (#21)

### Changed

* Remove custom literals (#110)
* ReferenceData currencies should follow FX conventions (#99)
* Replace `snapshot` (bool) with `update_type` (UpdateType) (#97)
* Align `PositionUpdate` with the FIX protocol (#89)

## 0.7.7 &ndash; 2021-09-20

### Changed

* Demonstrate how `GatewaySettings` should be applied to `DepthBuilder` (#56)

## 0.7.6 &ndash; 2021-09-02

### Added

* example-7 to demonstrate how to use collector for custom metrics (#8)
* event-log to demonstrate how to read event-logs (#10)
* example-6 to demonstrate and test event-log multiplexing (#7)

### Changed

* New Simulation interface requiring a specification of factory methods and latencies (#7)

## 0.7.5 &ndash; 2021-08-08

## 0.7.4 &ndash; 2021-07-20

## 0.7.3 &ndash; 2021-07-06

## 0.7.2 &ndash; 2021-06-20

## 0.7.1 &ndash; 2021-05-30

## 0.7.0 &ndash; 2021-04-15

### Added

* Example demonstrating the use of a secondary thread and `CustomMessage`

### Changed

* Using new Logging interface

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
