# Last Attempt Percentage

A Geode mod for Geometry Dash that displays your last attempt percentage on screen during gameplay.

<img src="logo.png" width="150" alt="the mod's logo" />

## Features
- **Persistent Display**: Shows your last death percentage throughout your attempts
- **Smart Detection**: Only records actual deaths, not checkpoint respawns
- **Customizable Position**: Choose from 7 different screen positions
- **Decimal Precision**: Optional display with 2 decimal places
- **Clean Reset**: Percentage resets when entering a new level

## Installation
1. Install [Geode](https://geode-sdk.org/install) for Geometry Dash
2. Download the latest release from the [Releases](https://github.com/YourUsername/Last_Attempt_Percentage/releases) page
3. Place the .geode file in your Geode mods folder

## Configuration
Access settings through Geode's mod menu:
- **Show Decimal Percentages**: Toggle decimal display (47% vs 47.32%)
- **Display Position**: Choose where the percentage appears on screen

## Build Instructions
```sh
# Assuming you have the Geode CLI set up already
geode build
```

## Development
Built with [Geode SDK](https://geode-sdk.org/) v4.8.0

## License
This project is licensed under the MIT License.
