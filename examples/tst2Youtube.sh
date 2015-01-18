#!/bin/bash
gst-launch-1.0 videotestsrc pattern=0 is-live=true !\
timeoverlay halignment=left valignment=bottom text="Stream time:" shaded-background=true ! \
'video/x-raw, format=(string)BGRA, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive, width=320, height=240, framerate=30/1' ! \
queue ! videoconvert ! x264enc bitrate=4000 key-int-max=50 bframes=0 byte-stream=false aud=true tune=zerolatency ! \
h264parse ! 'video/x-h264,level=(string)'4.1',profile='main ! queue ! mux. audiotestsrc is-live=true ! \
'audio/x-raw, format=(string)S16LE, endianness=(int)1234, signed=(boolean)true, width=(int)16, depth=(int)16, rate=(int)44100, channels=(int)2' ! \
queue ! voaacenc bitrate=128000 ! aacparse ! audio/mpeg,mpegversion=4,stream-format=raw ! queue ! \
flvmux streamable=true name=mux ! \
rtmpsink location="rtmp://a.rtmp.youtube.com/live2/<replace with stream key> live=true flashver=FMLE/3.0(compatible;FMSc/1.0)"
