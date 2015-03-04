FROM fedora:latest
RUN yum install -y wget
RUN wget -P /etc/yum.repos.d http://download.gluster.org/pub/gluster/glusterfs/LATEST/Fedora/glusterfs-fedora.repo
RUN yum -y install glusterfs glusterfs-fuse
ADD mymount.so /mymount.so
RUN echo "/mymount.so" >> /etc/ld.so.preload
CMD sleep 100000000