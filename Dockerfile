# Specify the image from which you are working
FROM atlas/analysisbase:21.2.85-centos7

# Put the current repo (the one in which this Dockerfile resides) in the directory specified here
# Note that this directory is created on the fly and does not need to reside in the repo already
ADD . /Bootcamp

# Go into the directory specified here (again, it will create the directory if it doesn't already exist)
WORKDIR /Bootcamp/build

# Create a run directory
RUN sudo mkdir /Bootcamp/run

# Source the ATLAS analysis environment
# Make sure the directory containing your analysis code (and the code inside it) is owned by atlas user
# Build your source code using cmake
RUN source ~/release_setup.sh &&  \
    sudo chown -R atlas /Bootcamp && \
    cmake ../source && \
    make
