# Bluetooth Dongle for Ting
This is an attempt to give bluetooth HID ability to the OiD reading pens. I named this repo as "dongle", however the device is quite big, as you won't call it dongle. Well, who cares!
![Product with OiD-printed card](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0408.jpg)

## Generating OiD
I used the OiD generator called (Tingeltangel.)[https://github.com/Martin-Dames/Tingeltangel]
Java binary can be also found (here)[https://www.ting-el-tangel.de/doku.php?id=download] Please be aware that most of the information is avaiable only on German. However the app is straight forward, probably you can use it without much knowledge about German.

## System
As shown here, a letter is modulated to simple sound pulse like morse signal based on ascii code binary bits. The processor (ATTiny45) samples the sound burst and encodes the sampled bits based on simple packet format with header and footer.
![Very rough algorithm visualsation](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0235.jpg)


The sound files are stored to the Ting device as wav or MP3. When user reads a OiD printed surface with the pen, Ting plays the corresponding sound file.
![Process Diagram](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0236.jpg)

The daata is extracted using footer and header. No parity bit is used. Just simple (and primitive) bit alignment.
![Signal Processing](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/signal.jpg)

## Circuit
The circit is extremely simple, using only one OP amp and an ATTiny controller.
![Circuit Diagram](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/circuit.jpg)
![Milling PCB](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0402.jpg)
![Mounting Components](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0403.jpg)
![Mounting Modules](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0404.jpg)
![Whole assembly](https://raw.githubusercontent.com/yuskegoto/BluetoothDongleForTing/master/pics/IMG_0407.jpg)

## Reference
There is also a thread about Ting reverse engineering in mikrocontroller.net. The thread is also in German too.
(https://www.mikrocontroller.net/topic/214479)[https://www.mikrocontroller.net/topic/214479]

