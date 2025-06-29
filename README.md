# GoldFish Engine

## Community

 - [Discord](https://discord.gg/yHWZVwu2Ta)
 - IRC: #goldfish at irc.nishi.boats
 - XMPP: goldfish at conference.nishi.boats

## Stack
 - OpenGL 1.1 (minimal requirement)
 - [Open Dynamics Engine](https://ode.org)
 - [zlib](https://zlib.net)
 - [Lua](https://lua.org)
 - [stb](https://github.com/nothings/stb)
 - [dr\_libs](https://github.com/mackron/dr_libs)
 - [jar](https://github.com/kd7tck/jar)
 - [Premake5](https://premake.github.io)
 - [PCRE](https://pcre.org)
 - [minisocket](https://github.com/pyrite-dev/minisocket)
 - [X25519](https://github.com/DavyLandman/compact25519)/[AES256](https://github.com/kokke/tiny-AES-c)

## OpenGL stack
 - [SDL2](https://wiki.libsdl.org/SDL2/FrontPage) (default)

These things are not default; optional:
 - GDI
 - X11
 - [GLFW](https://www.glfw.org/)

## Sound stack
 - [miniaudio](https://github.com/mackron/miniaudio) (default)

These things are not default; optional:
 - [ALSA](https://www.alsa-project.org)
 - DSound/WinMM
 - [SDL2](https://wiki.libsdl.org/SDL2)
 - [OSS](http://www.4front-tech.com/oss.html)
 - Sun/audio(4)

   NOTE: Currently only supports audio(4) on NetBSD since 9.0
