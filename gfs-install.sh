#!/bin/sh
if mountpoint -q /target; then
	echo "Installing gluster to /target"
	for i in "/usr/bin/fusermount-glusterfs" "/usr/lib64/glusterfs/" "/usr/sbin/mount.glusterfs" "/usr/sbin/glusterfs" "/usr/sbin/glusterfsd" "/lib64/libglusterfs.so.0" "/lib64/libgfrpc.so.0" "/lib64/libgfxdr.so.0" "/lib64/libglusterfs.so.0.0.1" "/lib64/libgfrpc.so.0.0.1" "/lib64/libgfxdr.so.0.0.1"
    do 
        echo  $i
        cp -fr $i /target/$i
    done

    mkdir /var/log/glusterfs/
    touch /var/log/glusterfs/mnt-gluster.log

else
	echo "/target is not a mountpoint, please run with "
	echo "-v /:/target"
fi