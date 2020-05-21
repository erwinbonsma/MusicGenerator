Music tracker for Gamebuino
===========================

This repository contains a simple tracker that is able to generate multi-track music. It
is heavily inspired by the tracker in [PICO-8][]. It supports the same instruments and
effects, but is it a bit more primitive, as it is intended for the [Gamebuino][] console.
This device only supports 10-bit audio and its CPU power is limited. Therefore, all
calculations are integer-based and only 8-bit multiplications are used.

The actual integration of the tracker into the Gamebuino library is currently available in
the [music-gen branch of my fork][]. It may end up part of the official library someday,
but that is not the case yet.

[Gamebuino]: https://gamebuino.com
[PICO-8]: https://www.lexaloffle.com/pico-8.php
[music-gen branch of my fork]: https://github.com/erwinbonsma/Gamebuino-META/tree/music-gen