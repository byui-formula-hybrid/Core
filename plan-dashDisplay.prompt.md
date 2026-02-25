## Plan: Dash Display Module — CAN-Driven LVGL Dashboard

**TL;DR** — We will build the dash module inside `lib/dash/` by copying the structure used in the inverter code. That means: CAN message structs (using `uint64_t` bitfields), a `DashController` that receives CAN frames, applies fault/timeout logic, and stores a clean data model. Then we add the LVGL UI (4 pages) and ESP32 hardware setup. Hardware-specific code is kept behind `#ifndef ARDUINO_ARCH_NATIVE` so native tests still work. CAN IDs are fixed. RTD buzzer is a placeholder. SD telemetry comes later.

---

### Hardware Overview (Two Displays)

| Display | Role | Board | Resolution | Driver IC | Touch |
|---------|------|-------|------------|-----------|-------|
| **Small** | Speed + basic stats (driver-facing secondary gauge) | Waveshare ESP32-S3-LCD-2.8C (SKU 30254) | 480×480 round | GC9A01 (round panel, SPI) | Optional CST816S capacitive |
| **Large** | Main dash — 4-page LVGL UI | TBD (specs to be provided) | TBD | TBD | Likely GT911 or FT5x06 |

> Both boards are ESP32-S3 based. They talk to the car over TWAI (CAN) and to each other over UART.

---

### Steps

---

**Phase 0 — Environment & Hardware Bringup (do this BEFORE the display arrives)**

> Goal: make sure the toolchain works and LVGL can run, even before the display is here.

**Step 0.1 — Verify PlatformIO + ESP-IDF toolchain**

- Build the firmware: run `.scripts\powershell\build.ps1` (this runs `pio run -e esp32dev`).
- Run native tests: run `.scripts\powershell\test.ps1` (this runs `pio test -e native`).
- If either script fails because tools are missing, run `.scripts\powershell\install.ps1` first.

**Step 0.2 — Add LVGL to `platformio.ini` (ESP32 env only)**

In the `[env:esp32dev]` section, add:
```ini
lib_deps =
    lvgl/lvgl@^8.3.10
build_flags =
    -std=c++11
    -DLV_CONF_INCLUDE_SIMPLE
```
Create `include/lv_conf.h` using LVGL's template. Set it for the small display (480×480, 16-bit color). Required settings:
```c
#define LV_HOR_RES_MAX   480
#define LV_VER_RES_MAX   480
#define LV_COLOR_DEPTH   16
#define LV_TICK_CUSTOM   1        // use esp_timer_get_time()
#define LV_USE_DEMO_WIDGETS 1    // enable built-in widgets demo
```

**Step 0.3 — Download Waveshare demo and study the driver layer**

- Demo zip: `https://files.waveshare.com/wiki/ESP32-S3-LCD-2.8/ESP32-S3-LCD-2.8-Demo.zip`
- Open the ESP-IDF demo project: `ESP32-S3-LCD-2.8-Test`.
- Read these functions (do not copy yet):
    - `Driver_Init()` — how GC9A01 is initialized over SPI
    - `LCD_init()` — SPI pin assignments (MOSI, MISO, CLK, CS, DC, RST, BL)
    - `tp_init()` — touch controller init (CST816S over I2C, INT pin)
    - LVGL display flush callback (`lv_disp_drv_t.flush_cb`)
    - LVGL tick source (`lv_tick_inc()` driven by `esp_timer`)
    - LVGL input device callback for touch

> **Note (2.8C "C" variant):** The round 480×480 panel uses the **GC9A01** display driver IC (not ST7789T3). SPI wiring is similar, but the init sequence is different. Confirm this in the demo source.

[X] **Step 0.4 — Port a stub LVGL "Hello World" into the Core project (builds without display)**

Create `lib/dash/ui/lvgl_stub.h` (ESP32 only, guarded by `#ifndef ARDUINO_ARCH_NATIVE`). It should:
- Provide a `flush_cb` that does nothing (so it compiles without a real display).
- Initialize LVGL with a dummy display driver.
- Create one `lv_label` that says `"DASH INIT OK"`.

Update `src/main.cpp` to call the stub init. Goal: `pio run -e esp32dev` builds with LVGL linked. Flash to any ESP32-S3 dev board and verify it boots without crashing.

[ ] **Step 0.5 — Run the LVGL `lv_demo_widgets()` as a smoke test**

After the stub builds:
- Replace the stub label with `lv_demo_widgets()` (requires `LV_USE_DEMO_WIDGETS 1` in `lv_conf.h`).
- Flash to the Waveshare board when it arrives.
- Expected result: the LVGL widget gallery shows up on the 480×480 round screen.
- If colors/orientation look wrong, adjust `LV_COLOR_16_SWAP`, `LV_DISP_ROT_180`, or SPI speed in `lv_conf.h` or the driver init.

**Step 0.6 — Learn LVGL basics (do while waiting for display)**

Read the LVGL docs and try these topics in order:
1. `lv_scr_act()` — the active screen object
2. `lv_label_create()` / `lv_label_set_text()` — basic text
3. `lv_style_t` — colors, fonts, padding
4. `lv_obj_set_style_bg_color()` — set background/indicator colors
5. `lv_arc_t`, `lv_meter_t` — gauge-style widgets (useful for speed/RPM)
6. `lv_tabview_t` — tab-based page navigation (replaces custom navigation code)
7. `lv_event_cb_t` — touch/click event callbacks
8. `lv_timer_create()` — periodic data refresh (alternative to update loop)

> **Recommended exercise:** Build a small LVGL program that shows a static number (e.g., "RPM: 1234") and changes color on a button press. This proves touch and styling work before real CAN data.

**Step 0.7 — UART data bridge between two ESP32s (simulate CAN data flow early)**

Use UART to feed fake data to the display board before CAN is ready. This lets you build and test the UI without the rest of the car.

**Sender ESP32** (any dev board): runs a FreeRTOS task that sends a fixed-size frame every 250 ms over UART2:
```
// Example framing (binary, 16 bytes)
struct MockFrame {
    uint8_t  start;       // 0xAA
    uint16_t pack_voltage_raw;  // e.g., 4200 = 420.0 V
    uint16_t motor_rpm;
    uint8_t  throttle_pct;
    uint8_t  ams_fault;
    uint8_t  imd_fault;
    uint8_t  can_lost;
    uint8_t  error_code;
    uint8_t  end;         // 0xBB
    // pad to 16 bytes
};
```
Wire it like this: Sender TX → Receiver RX, and connect GND on both boards. Use UART2 on both boards (GPIO 17/16 by default on ESP32-S3, but check the board pinout).

**Receiver ESP32** (the Waveshare display board): runs a FreeRTOS task that reads UART2, parses `MockFrame`, updates `DataModel`, and the LVGL labels refresh on the next tick.

> This is separate from CAN on purpose. Once CAN is ready, the UI uses the same `DataModel`, only the input source changes.

**Step 0.8 — Validate `lv_tabview` page switching via touch**

Before building the real pages, make a simple navigation test:
- Create 4 empty `lv_tabview` tabs: Drive, Diagnostics, Faults, Charging
- Put a single label on each tab naming the page
- Verify touch input switches tabs correctly on the round display
- Verify the round display clip (the corners of the 480×480 square are outside the circle — ensure nothing important is in the corners)

---

**Phase 1 — CAN Message Definitions**

1. **Create `lib/dash/messages.h`** — Define `Dash::` structs using the same `uint64_t` bitfield pattern as `lib/inverter/DTIX50/messages.h`. One struct per inbound CAN frame:
   - `AMSVoltCurrent` (0x101) — `pack_voltage: 16`, `pack_current: 16`, `reserved: 32`
   - `AMSTemperature` (0x102) — `max_cell_temp: 16`, `reserved: 48`
   - `AMSFault` (0x103) — `fault_flags: 8`, `reserved: 56`
   - `IMDFault` (0x104) — `fault_flags: 8`, `reserved: 56`
   - `MotorStatus` (0x201) — `rpm: 16`, `reserved: 48`
   - `VCUStatus` (0x301) — `ts_active: 8`, `reserved: 56`
   - `PedalData` (0x401) — `throttle_pct: 8`, `brake_pressed: 8`, `adc1: 16`, `adc2: 16`, `error_code: 8`, `sync_fault: 8` (full 8-byte frame)
   - `Heartbeat` (0x3FF) — `alive: 8`, `reserved: 56`
   - Add a `namespace Dash::CANID` with `constexpr uint32_t` for every ID, keeping them in one place for easy future changes.

2. **Create `test/test_dash/test_dash_messages.cpp`** — Add encode/decode roundtrip tests for each struct. Follow the same pattern as `test/test_inverter/DTIX50/test_inverter_message.cpp`. Make sure the byte layout matches the CAN signal mapping table.

---

**Phase 2 — Data Model & Fault Logic (hardware-independent, natively testable)**

3. **Create `lib/dash/data_model.h`** — A plain struct `Dash::DataModel` holding the latest processed values in real units:
   - `float pack_voltage_V`, `pack_current_A`, `max_cell_temp_C`
   - `uint16_t motor_rpm`
   - `float speed_kph` (computed from RPM + gear ratio — placeholder formula)
   - `uint8_t throttle_pct`, `bool brake_pressed`
   - `uint16_t pedal_adc1`, `pedal_adc2`, `uint8_t pedal_error_code`, `bool pedal_sync_fault`
   - `bool ts_active`, `bool glv_on`, `bool ready_to_drive`
   - `bool ams_fault` (latched), `bool imd_fault` (latched)
   - `bool can_comm_lost`
   - `uint32_t last_heartbeat_ms` (for timeout calc)
   - `enum class SystemPage { DRIVE, DIAGNOSTICS, FAULTS, CHARGING }`
   - Latched fault reset methods: `reset_ams_fault()`, `reset_imd_fault()`

4. **Create `lib/dash/controller.h` / `lib/dash/controller.cpp`** — `Dash::Controller` class:
    - **Constructor**: takes `shared_ptr<CAN::Provider>`, `unique_ptr<Core::iLockStrategy>`, `unique_ptr<Core::iThreadStrategy>`.
    - **`start()` / `stop()`**: starts/stops a CAN receive loop thread (same pattern as `Inverter::DTIX50::Heartbeat` in `lib/inverter/DTIX50/heartbeat.cpp`).
    - **CAN receive loop** (static `void poll(void*)`): calls `provider->receive(frame, 50ms)`, switches on `frame.identifier`, decodes the matching struct from Phase 1, applies scale factors (example: `raw * 0.1` for voltage), and updates `DataModel` under a lock.
    - **Timeout detection**: after each receive (or timeout), if `current_time - last_heartbeat_ms > 500`, set `can_comm_lost = true`. Reset to false on a valid heartbeat.
    - **Fault latching**: `ams_fault` and `imd_fault` become `true` when a fault frame arrives, and only clear with `reset_ams_fault()` / `reset_imd_fault()`.
    - **Pedal fault persistence**: if `error_code > 0` or `sync_fault` is true, set the field; when error returns to 0, keep it for at least 2 seconds before clearing.
    - **`const DataModel& data() const`**: read accessor (caller locks externally or uses a snapshot).
    - **`DataModel snapshot()`**: returns a lock-guarded copy for the UI thread.

5. **Create `test/test_dash/test_dash_controller.cpp`** — Unit tests using `MockCanService` + `NativeLockStrategy` + `NativeThreadStrategy`:
    - Inject a mock `on_receive` that returns pre-built frames.
    - Test voltage/current decode to real units.
    - Test AMS fault latching (fault frame sets true; clear frame does not clear).
    - Test IMD fault latching (same pattern).
    - Test CAN timeout (no heartbeat for 600 ms sets `can_comm_lost = true`).
    - Test CAN recovery (heartbeat clears `can_comm_lost`).
    - Test pedal error code decode and 2-second persistence.
    - Test pedal sync fault.
    - Test `snapshot()` returns a consistent copy.

---

**Phase 3 — Pedal Error Code Decoder (testable utility)**

6. **Create `lib/dash/pedal_errors.h`** — `Dash::PedalErrors` namespace:
    - `const char* decode(uint8_t code)` — map codes 1–15 to readable strings ("Pot 1 Open Circuit", "Slew Rate Critical", "Pedal Implausible", etc.). Return `"Unknown"` for unmapped codes, and `nullptr` or `""` for code 0.
    - Use a `constexpr` or `static const` array to keep it embedded-friendly.

7. **Create `test/test_dash/test_pedal_errors.cpp`** — Test every code 0–15 maps to the expected string.

---

**Phase 4 — Test Entrypoint Update**

8. **Update `test/test_dash/test_main.cpp`** — Replace the placeholder with a real entrypoint that calls `run_dash_message_tests()`, `run_dash_controller_tests()`, `run_dash_pedal_error_tests()`.

9. **Create `test/test_dash/test_main.h`** — Forward-declare the three runner functions (same pattern as `test/test_inverter/test_main.h`).

---

**Phase 5 — LVGL UI Layer (hardware-guarded)**

> All files in this phase are **guarded with `#ifndef ARDUINO_ARCH_NATIVE`** (or excluded from native builds) so `pio test -e native` keeps working.

10. **Create `lib/dash/ui/ui.h` / `lib/dash/ui/ui.cpp`** — `Dash::UI` class:
    - **Constructor**: takes a pointer/reference to `Dash::DataModel` snapshot for rendering.
    - **`init()`**: initializes LVGL display, touch input, and screen objects.
    - **`update(const DataModel& snapshot)`**: called periodically; updates LVGL widgets.
    - **`lv_timer_handler()` wrapper**: called every ~5 ms from a FreeRTOS task or the main loop.

11. **Create `lib/dash/ui/page_drive.h` / `lib/dash/ui/page_drive.cpp`** — Drive Mode page:
    - Top bar: `TS: ON/OFF` | `GLV: ON/OFF` | `RTD: YES/NO`
    - Middle indicators: AMS (red/green), IMD (red/green), Pedal Fault (yellow/green), CAN Status (green/"COMM LOST" red banner)
    - Bottom data row: Voltage, Current, Speed, RPM, Throttle %, Brake ON/OFF
    - Color logic: green = normal, yellow = warning, red = critical/fault. Use LVGL styles.

12. **Create `lib/dash/ui/page_diagnostics.h` / `lib/dash/ui/page_diagnostics.cpp`** — Diagnostics page:
    - Cell voltage min/max (from `DataModel`)
    - Cell temperature distribution (bar or gauge)
    - Pedal ADC1 & ADC2 raw values
    - Pedal error code decoded via `PedalErrors::decode()`
    - CAN message status indicators (per-ID last-received timestamp)

13. **Create `lib/dash/ui/page_faults.h` / `lib/dash/ui/page_faults.cpp`** — Fault History page:
    - Scrollable list of fault entries (timestamp, source string, CAN ID)
    - Touch to expand details
    - Requires a small ring buffer or `std::vector` of `FaultEntry` structs in `DataModel` (or a separate `FaultLog` class): `{ uint32_t timestamp_ms, const char* source, uint32_t can_id, uint8_t code }`.

14. **Create `lib/dash/ui/page_charging.h` / `lib/dash/ui/page_charging.cpp`** — Charging page:
    - Pack SOC (if available from AMS — may need an additional CAN field)
    - Charging voltage / current
    - AMS / IMD active status
    - Charging fault messages

15. **Create `lib/dash/ui/navigation.h` / `lib/dash/ui/navigation.cpp`** — Tab bar or swipe-based page navigation:
    - 4 tabs: Drive | Diagnostics | Faults | Charging
    - Touch handler switches active `lv_obj_t*` screen
    - Defaults to Drive page on boot

---

**Phase 6 — Hardware Setup & Main Integration**

16. **Create `lib/dash/hardware.h` / `lib/dash/hardware.cpp`** — `Dash::Hardware` (guarded `#ifndef ARDUINO_ARCH_NATIVE`):
    - Pin definitions for CAN TX/RX, SPI/I2C pins for TFT, and touch CS/IRQ. Use placeholders with `// TODO: set to actual pins`.
    - `init_can()`: create `ESP32S3CanService`, wrap in `CAN::Provider`, call `begin()`.
    - `init_display()`: configure the LVGL display driver and flush callback (placeholder for the real driver init, e.g. ILI9488/ST7796).
    - `init_touch()`: configure the LVGL input device (placeholder for FT5x06/GT911).
    - RTD buzzer placeholder: `buzz_rtd()` drives a GPIO/PWM pin for 1–3 seconds at 2000 Hz. Mark `// TODO: confirm if dash owns this`.

17. **Update `src/main.cpp`** — Wire everything together (guarded for ESP32 build only):
    ```
    // Pseudocode — not actual code
    Dash::Hardware hw;
    hw.init_can();
    hw.init_display();
    hw.init_touch();

    auto controller = Dash::Controller(hw.provider(), lock, thread);
    controller.start();

    Dash::UI ui;
    ui.init();

    // Main loop
    while(true) {
        auto snap = controller.snapshot();
        ui.update(snap);
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    ```

---

**Phase 7 — Documentation**

18. **Fill in `docs/modules/DASH.md`** — Complete module documentation:
    - Architecture overview, class diagram (text), CAN ID table, fault behavior table, page descriptions, pin assignments placeholder, dependencies.

19. **Update `docs/HARDWARE.md`** — Fill in DCM section with display module hardware details.

---

**Phase 8 — Telemetry (deferred, later step)**

20. **Create `lib/dash/telemetry.h` / `lib/dash/telemetry.cpp`** — `Dash::Telemetry` class (future):
    - SD card init + FAT filesystem
    - Log `DataModel` snapshots + fault entries to CSV/binary at configurable interval
    - Guarded behind `#ifndef ARDUINO_ARCH_NATIVE`
    - Unit-testable logger interface with file-system abstraction

---

### File Tree Summary

```
include/
└── lv_conf.h               ← LVGL config (Phase 0.2)

lib/dash/
├── messages.h              ← CAN structs (Phase 1)
├── data_model.h            ← Plain data struct (Phase 2)
├── controller.h/.cpp       ← CAN→DataModel logic (Phase 2)
├── pedal_errors.h          ← Error code strings (Phase 3)
├── hardware.h/.cpp         ← Pin config, HW init (Phase 6)
├── telemetry.h/.cpp        ← SD logging (Phase 8, deferred)
└── ui/
    ├── lvgl_stub.h         ← Build-only stub flush driver (Phase 0.4)
    ├── ui.h/.cpp           ← LVGL init + update loop (Phase 5)
    ├── navigation.h/.cpp   ← Tab/page switching (Phase 5)
    ├── page_drive.h/.cpp
    ├── page_diagnostics.h/.cpp
    ├── page_faults.h/.cpp
    └── page_charging.h/.cpp

test/test_dash/
├── test_main.cpp           ← Entrypoint (Phase 4)
├── test_main.h             ← Forward decls (Phase 4)
├── test_dash_messages.cpp  ← Encode/decode (Phase 1)
├── test_dash_controller.cpp← Fault/timeout logic (Phase 2)
└── test_pedal_errors.cpp   ← String mapping (Phase 3)
```

---

### Verification

- **Phase 0 gate**: `pio run -e esp32dev` builds with LVGL linked. `lv_demo_widgets()` renders on the Waveshare board without crashes. UART mock data updates an LVGL label.
- **Native tests**: `pio test -e native` (or `.scripts\powershell\test.ps1`) — Phases 1–4 must pass. LVGL/hardware files are excluded with `#ifndef ARDUINO_ARCH_NATIVE`.
- **ESP32 build**: `pio run -e esp32dev` (or `.scripts\powershell\build.ps1`) — full build including UI and hardware code. LVGL is in `[env:esp32dev]` `lib_deps`.
- **Manual on-hardware**: Flash to dash ESP32, connect CAN transceiver + display. Verify pages render, faults latch (red), CAN timeout shows within 500 ms, touch navigation works, pedal error text is readable.
- **Round display note**: Do not place critical UI elements in the four corners of the 480×480 frame (they are clipped by the circular bezel).
- **Rule compliance check**: EV 9.1.5 (TSSI visible), EV 9.4.x (IMD red + labeled), EV 9.6.x (AMS red + labeled), EV 8.1.6 (indicators in sunlight).

### Decisions

- **CAN IDs finalized**: Using 0x101–0x401 and 0x3FF as specified. All IDs live in `Dash::CANID` constants.
- **LVGL version**: v8.3.10 (matches Waveshare demo). Do not use v9.x yet.
- **Small display driver**: Waveshare ESP32-S3-LCD-2.8C uses **GC9A01** over SPI (round panel, 480×480). Confirm in the demo source before writing the init sequence.
- **LVGL in Core repo**: Full display code lives here. Hardware code is guarded with `#ifndef ARDUINO_ARCH_NATIVE` so native tests work.
- **Early development via UART**: Use the UART mock data bridge (Step 0.7) to build the UI before CAN is ready.
- **RTD buzzer**: Placeholder in `Dash::Hardware`, ownership TBD.
- **Telemetry**: Phase 8, deferred.
- **PlatformIO dep**: `lvgl/lvgl@^8.3.10` in `[env:esp32dev]` `lib_deps` only. Display SPI driver uses ESP-IDF SPI master API (no TFT_eSPI).
- **Fault latching**: AMS/IMD faults latch and only clear via explicit `reset_*()` calls (touch button on Faults page). Pedal faults persist for at least 2 seconds then auto-clear.

### TODO

- [ ] RTD buzzer: confirm which module owns this (dash vs VCU)
- [ ] Confirm GC9A01 is the correct driver IC for the 2.8C round panel (check demo zip source)
- [ ] Get pin assignments for the 2.8C: SPI (MOSI/MISO/CLK/CS/DC/RST/BL) and touch I2C (SDA/SCL/INT)
- [ ] Decide on large display hardware (specs pending)
- [ ] Confirm UART GPIO pins for the inter-board data bridge (check both board schematics)
- [ ] Confirm if the 2.8C "C" variant has touch populated or is it optional (SKU: 30254)
