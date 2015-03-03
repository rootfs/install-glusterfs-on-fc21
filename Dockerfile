FROM fedora:latest
RUN yum install -y wget
RUN wget -P /etc/yum.repos.d http://download.gluster.org/pub/gluster/glusterfs/LATEST/Fedora/glusterfs-fedora.repo
RUN yum -y install glusterfs glusterfs-fuse
ADD gfs-install.sh /gfs-install.sh
CMD /gfs-install.sh