FROM fedora:latest
RUN yum install -y wget attr
RUN wget -P /etc/yum.repos.d http://download.gluster.org/pub/gluster/glusterfs/LATEST/Fedora/glusterfs-fedora.repo
RUN yum -y install glusterfs glusterfs-fuse
ADD mymount.so /mymount.so
ADD sleep.sh /sleep.sh
RUN echo "/mymount.so" >> /etc/ld.so.preload
CMD /sleep.sh