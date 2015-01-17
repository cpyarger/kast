

gst-launch-0.10  videomixer name=mix ! ffmpegcolorspace ! autovideosink \
v4l2src device=/dev/video1  ! videoscale ! video/x-raw-rgb, framerate=30/1, width=350, height=250 ! ffmpegcolorspace ! tee name=videoTee1 \
 v4l2src device=/dev/video0  ! videoscale ! video/x-raw-rgb, framerate=30/1, width=350, height=250 ! ffmpegcolorspace ! tee name=videoTee \
 videoTee.  ! videobox border-alpha=0 top=-200 left=-50  ! ffmpegcolorspace ! queue ! mix. \
 videoTee.  ! videobox border-alpha=0 top=-200 left=-450 ! ffmpegcolorspace ! queue ! mix. \
 videoTee1. ! videobox border-alpha=0 top=-450 left=-50  ! ffmpegcolorspace ! queue ! mix. \
 videoTee1. ! videobox border-alpha=0 top=-450 left=-450 ! ffmpegcolorspace ! queue ! mix.