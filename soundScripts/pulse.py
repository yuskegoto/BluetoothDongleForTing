import math
import pyaudio
import wave

PyAudio = pyaudio.PyAudio

bitrate = 44100 #number of frames per second/frameset.

frequency = 5000 #Hz, waves per second, 261.63=C4-note.
pulseLength = 0.008 #with of pulse
signalCycle = 0.016 #cycle time for pulsing
repeats = 500
outputFile = False
WAVE_OUTPUT_FILENAME = "c.wav"
sendCharacter = 'C'

pauseLength = pulseLength - signalCycle

pulseFrames = int(bitrate * pulseLength)
pauseFrames = int(bitrate * pauseLength)
blancFrames = int(bitrate * signalCycle)


waveData = ''   #sond data container
pulseData = ''  #single pulse
for x in xrange(pulseFrames):
    pulseData = pulseData+chr(int(math.sin(x/((bitrate/frequency)/math.pi))*127+128))
for x in xrange(pulseFrames % bitrate):
    pulseData = pulseData+chr(128)
for x in xrange(pauseFrames):
    pulseData = pulseData+chr(128)

blancData = ''
for x in xrange(blancFrames):
    blancData = blancData+chr(128)


print('send character: ' + sendCharacter)
asciiVal = ord(sendCharacter)

asciiInArray = [0,0,0,0,0,0,0,0]
i = 0
j = asciiVal

for i in range(8):
    k = 2**(7-i)
    asciiInArray[i] = j // k
    j = j % k

#first pulse
for i in range(3):
    waveData = waveData + pulseData
    print(1)

#ascii code to audio pulse
for i in asciiInArray:
    if i == 1:
        waveData = waveData + pulseData
        print(1)
    else:
        waveData = waveData + blancData
        print(0)

#end code
waveData = waveData + blancData
print(0)

for i in range(2):
    waveData = waveData + pulseData
    print(1)

for i in range(2):
    waveData = waveData + blancData
    print(0)

frames = []

p = PyAudio()
stream = p.open(format = p.get_format_from_width(1),
                channels = 1,
                rate = bitrate,
                output = True)

for i in range(repeats):
    stream.write(waveData)
#    data = stream.read(1024)
    frames.append(waveData)

stream.stop_stream()
stream.close()
p.terminate()

if outputFile:
    FORMAT = p.get_format_from_width(1)

    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(1)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(bitrate)
    wf.writeframes(b''.join(frames))
    wf.close()
