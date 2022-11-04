# Changelog

Tag documentation for any major or minor features, fixes, additions to Xcore.

---

## Version 0.0.1-alpha

This is an experimental release.

### Features

- Keylogger

## Version 0.0.2-alpha

This is an experimental release.

### Features

- added chained argument parsing (`bspc` styled)

  ```zsh
  ./xcore -G p::0x280000A@border::0x280000A@k::36   \
          -L g::desktops::+@p::0x2600006            \
          -S a::0x2600006::hello::any               \
          -S center                                 \
          -G class::0x2600006@t::0x280000A          \
          -M help@-h                                \
          -G --help                                 \
          -M log-keyboard                           \
          -G class::0x2600006
  ```

- divided operations into categories like get/fetch, list, set/apply, monitor/log
- added `fetch.[active_window|window_class|window_geometry|window_title]`
- added `fetch.[total_desktops|pid|border_width|keyname_from_keycode]`
- added `list.[window_geometry(shell_format)|desktop_names|window_protocols]`
- added `apply.[window_focus|window_activate|center_pointer|window_state]`
- added `monitor.[key_logger]`
- added all `--help|-h|h|help`, `--version|-V` and argument error messages

### Features

- Added monitor option `led`.
- Moved source files to `src` directory.
