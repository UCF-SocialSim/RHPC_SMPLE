FROM ubuntu:16.04

#Get basics
RUN \
    apt-get update && \
    apt-get install -y build-essential make g++ mpich wget cmake libgtest-dev curl libnetcdf-dev netcdf-bin && \ 
    mkdir -p /opt/sfw &&  cd /usr/src/gtest && \
    cmake CmakeLists.txt && make

# install boost 1.76
RUN mkdir -p /app && cd /app && \
    wget -q https://sourceforge.net/projects/boost/files/boost/1.76.0/boost_1_76_0.tar.gz && \
    tar xzf boost_1_76_0.tar.gz && cd boost_1_76_0 && \
    ./bootstrap.sh --prefix=/opt/sfw/Boost/Boost_1.76 --with-libraries=date_time,serialization,system,filesystem,mpi && \
    echo "using mpi : mpic++ ;" >> project-config.jam && \
    ./b2 -j4 --layout=tagged install threading=multi link=shared,static

# Install Repast HPC  
RUN \
    mkdir -p /app && cd /app && \
    wget -q https://github.com/Repast/repast.hpc/releases/download/v2.3.0/repast_hpc-2.3.0.tgz && \
    tar zxf repast_hpc-2.3.0.tgz && cd repast_hpc-2.3.0 && \
    cd /app/repast_hpc-2.3.0/MANUAL_INSTALL && ./install.sh netcdf  && \
    cd /app/repast_hpc-2.3.0/MANUAL_INSTALL && ./install.sh curl  && \
    cd /app/repast_hpc-2.3.0/MANUAL_INSTALL && ./install.sh boost  && \
    cp -rf /root/sfw/* /opt/sfw/ && cd /app/repast_hpc-2.3.0 && \
    ./configure --prefix=/opt/sfw/repast_hpc-2.3.0 --with-boost-include=/opt/sfw/Boost/Boost_1.61/include \
        --with-boost-lib-dir=/opt/sfw/Boost/Boost_1.61/lib \
        --with-netcdf=/root/sfw/NetCDF/ --with-netcdf-cxx=/root/sfw/NetCDF-cxx/ && \
    make && \
    make install


    
