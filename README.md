# GDMenuAnimation
This mod allows you to have any animation instead of the normal bg in your game menu.

## Usage
First, get any image sequence you want. If a gif, it should be split into frames, cocos doesnt have a gif support. Next, all frames should be placed into a folder `bganim` in the game root folder. They should also be named as `X.png` starting with 0 (0.png, 1.png, 12.png, etc).

On the first launch the config file will be generated.
```ini
[General]
AnimationSpeed = 15 // FPS of the animation basically (bigger => faster)
UseAntiAlisaing = 1 // Use anti aliasing (smoothing the texture instead of a pixelart-like look). 0 or 1.
```
You can edit it freely to any values you'd like.

Also a small tip, if you want a pixelart-like bg, you dont have to upscale it to your display resolution, you can just keep its resolution as low as possible but set `UseAntiAliasing` in the config to 0. The mod will scale it automatically without anti-aliasing (smoothing) the texture.

## Contacts

If you have any questions, message me on Discord: Jaan#2897.