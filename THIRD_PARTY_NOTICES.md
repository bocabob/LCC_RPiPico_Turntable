# Third-party notices

The node's firmware is built from three kinds of code:

- **This project's own files**, under the BSD 2-Clause License in [LICENSE](LICENSE).
- **Third-party code kept in this repository** (vendored), each part under its own license.
- **Third-party code linked in at build time** from the Arduino-Pico core and the Arduino
  libraries in the README's Dependencies section, which are not in this repository.

Every firmware image (`.uf2`, `.bin` or `.elf`, such as the ones in `build/`) holds all three.
Their licenses ask for these notices to go with it, so this file and the [licenses/](licenses/)
folder belong with every firmware image, and their text in the documentation of any board sold
with the firmware on it.

Taken from the linker map of the latest build (`build/rp2040.rp2040.rpipico2/*.map`), which
also names the Arduino-Pico version that build used. Check this file again when a library is
added or changed.

> **The firmware image as a whole is AGPL-3.0.** NeoPixelConnect is AGPL-3.0, and the local AccelStepper copy is GPL-3.0. This project's own
> files stay BSD-2-Clause, which allows that combination, but anyone who distributes the
> image must do so under the AGPL-3.0's terms: with the complete source that builds it.

## Summary

| Component | Where | License |
|---|---|---|
| OpenLcbCLib | `src/openlcb/`, `src/drivers/canbus/`, `src/utilities/`; adapted from its templates and BasicNode example: `openlcb_user_config.*`, `can_user_config.h`, `src/pico/` | BSD-2-Clause |
| BBCapTouch | `src/application_drivers/my_bb_captouch.*` | Apache-2.0 |
| AccelStepper (a modified local copy) | `src/application_drivers/AccelStepper.*` | GPL-3.0 |
| Noto Sans Bold, as a smooth font array | `NotoSansBold36Font.h` | OFL-1.1 |
| Arduino-Pico core, with ArduinoCore-API, the SPI, Wire, LittleFS and PicoOTA libraries, and its OTA boot loader | linked | LGPL-2.1 |
| Raspberry Pi Pico SDK | linked | BSD-3-Clause |
| TinyUSB (in the Pico SDK) | linked | MIT |
| littlefs (in the core's LittleFS library and OTA boot loader) | linked | BSD-3-Clause |
| uzlib (in the OTA boot loader) | linked | zlib |
| TFT_eSPI, or its fork TFT_eSPI_RA8876, as the build selects | Arduino library | FreeBSD, BSD and MIT notices |
| RA8876_RP2040 (only with `DISPLAY_DRIVER_RA8876_NATIVE`) | Arduino library | MIT |
| NeoPixelConnect | Arduino library | **AGPL-3.0** |
| NeoPixelBus by Makuna | Arduino library | LGPL-3.0 |
| ACAN2517 (the CAN controller) | Arduino library | MIT |
| I2C_EEPROM | Arduino library | MIT |
| LibPrintf | Arduino library | MIT |
| newlib, libgcc, libstdc++ | the compiler toolchain | see the last section |

## What a firmware release has to carry

1. **These notices:** this file and [licenses/](licenses/), next to the image, or their text in
   the product's manual.
2. **The LGPL's source offer.** The Arduino-Pico core (LGPL-2.1) and NeoPixelBus (LGPL-3.0) are
   linked into the image, so whoever receives it must be able to build it again with a changed
   library. This repository's complete source covers the application; the libraries' source,
   for the exact versions used, has to be offered from the same place as the image (on GitHub,
   attach their source archives to the release, or at the least link their exact tags).
3. **The AGPL-3.0's terms for the whole image:** the same source offer, covering every
   library in the image, and the image passed on under the AGPL-3.0.

## OpenLcbCLib

<https://github.com/JimKueneman/OpenLcbCLib>: Copyright (c) 2024, 2025, 2026,
Jim Kueneman. Distributed under this license:

```
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```

## BBCapTouch

BitBank Capacitive Touch Sensor Library, written by Larry Bank,
<https://github.com/bitbank2/bb_captouch>. Copyright 2023 BitBank Software, Inc. Modified by
Bob Gamble to start it on a chosen I2C port (`my_bb_captouch.*`, whose headers say so).

```
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

The full text is [licenses/Apache-2.0.txt](licenses/Apache-2.0.txt).

## AccelStepper

<https://www.airspayce.com/mikem/arduino/AccelStepper/>: Copyright (C) 2009-2021
Mike McCauley. Offered under the GNU General Public License version 3 (or a commercial license
from its author). The copy in `src/application_drivers/` is modified for this node, as the
README's Dependencies section says. The full text is [licenses/GPL-3.0.txt](licenses/GPL-3.0.txt).

## Noto Sans

The fast clock's font, `NotoSansBold36Font.h`, is Noto Sans Bold
(<https://notofonts.github.io/>, Google and the Noto Project Authors) converted to a smooth
font array. It is under the SIL Open Font License 1.1, full text in
[licenses/OFL-1.1.txt](licenses/OFL-1.1.txt).

## Arduino-Pico core

Arduino-Pico, <https://github.com/earlephilhower/arduino-pico>: Copyright (c) Earle F.
Philhower, III and contributors, with parts from the ESP8266 Arduino core and from Arduino's
ArduinoCore-API. The parts linked into this firmware (the core itself, ArduinoCore-API, the SPI,
Wire, LittleFS and PicoOTA libraries, and the OTA boot loader that applies a firmware update
at boot) are under the GNU Lesser General Public License, version 2.1 or later. The full text is
[licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt).

## Raspberry Pi Pico SDK

<https://github.com/raspberrypi/pico-sdk>

```
Copyright 2020 (c) 2020 Raspberry Pi (Trading) Ltd.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

## littlefs

<https://github.com/littlefs-project/littlefs>, as included in the core's LittleFS library.

```
Copyright (c) 2022, The littlefs authors.
Copyright (c) 2017, Arm Limited. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

-  Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
-  Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.
-  Neither the name of ARM nor the names of its contributors may be used to
   endorse or promote products derived from this software without specific prior
   written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

## uzlib

<https://github.com/pfalcon/uzlib>, its decompressor, as included in the core's OTA boot loader.

```
uzlib - Deflate/Zlib-compatible LZ77 compression/decompression library

Copyright (c) 2003 Joergen Ibsen
Copyright (c) 1997-2014 Simon Tatham
Copyright (c) 2014-2020 Paul Sokolovsky

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
   not claim that you wrote the original software. If you use this
   software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must
   not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
   distribution.
```

## TFT_eSPI

<https://github.com/Bodmer/TFT_eSPI>, by Bodmer, or its fork TFT_eSPI_RA8876
(<https://github.com/bocabob/TFT_eSPI_RA8876>), which carries the same notices. They grew from
Adafruit's ILI9341 and GFX libraries. Bodmer's original code:

```
Software License Agreement (FreeBSD License)

Copyright (c) 2023 Bodmer (https://github.com/Bodmer)

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
```

The Adafruit_GFX functions:

```
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```

The Adafruit_ILI9341 library it started from (MIT):

```
  This is our library for the Adafruit  ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
```

## RA8876_RP2040

<https://github.com/bocabob/RA8876_RP2040>, the RP2040 port of the RA8876 library by
Warren Watson, mjs513, KurtE and MorganS (Ra8876LiteTeensy). Its notice:

```
Copyright (c) 2017-2024 Warren Watson. Modified by mjs513, KurtE and MorganS.

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```

## NeoPixelConnect

<https://github.com/MrYsLab/NeoPixelConnect>: Copyright (c) 2020-2025 Alan Yorinks.
Under the GNU Affero General Public License version 3, full text in
[licenses/AGPL-3.0.txt](licenses/AGPL-3.0.txt).

## NeoPixelBus

<https://github.com/Makuna/NeoPixelBus>, written by Michael C. Miller. Under the GNU
Lesser General Public License version 3, which adds its permissions to the GNU General Public
License version 3: both texts are here, [licenses/LGPL-3.0.txt](licenses/LGPL-3.0.txt) and
[licenses/GPL-3.0.txt](licenses/GPL-3.0.txt).

## MIT-licensed components

- **TinyUSB**, <https://github.com/hathach/tinyusb>, as included in the Pico SDK:
  Copyright (c) 2018, hathach (tinyusb.org).
- **ACAN2517**, <https://github.com/pierremolinaro/acan2517>: Copyright (c) 2018 Pierre Molinaro.
- **I2C_EEPROM**, <https://github.com/RobTillaart/I2C_EEPROM>: Copyright (c) 2011-2025 Rob Tillaart.
- **LibPrintf**, <https://github.com/embeddedartistry/arduino-printf>: Copyright (c) 2019 Embedded Artistry.

Each is distributed under this license:

```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## The C library and compiler runtime

The compiler toolchain (pqt-gcc, from Arduino-Pico) links in:

- **newlib**, the C library: Copyright Red Hat, Inc. and others, under the BSD-style and similar
  permissive licenses collected in newlib's `COPYING.NEWLIB`
  (<https://sourceware.org/git/?p=newlib-cygwin.git;a=blob;f=COPYING.NEWLIB>).
- **libgcc and libstdc++**: GPL-3.0 with the GCC Runtime Library Exception, which puts no
  conditions on a program compiled with GCC.
