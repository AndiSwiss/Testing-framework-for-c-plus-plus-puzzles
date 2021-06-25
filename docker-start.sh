# delete old container if exists, ignore stdout/stderr
docker container rm -f testing_cplusplus &>/dev/null
# -it attaches std streams to the container (type exit to exit the container)
# -v maps the host working directory /home/testing_cplusplus in the container, such that all files/folder are synced between host/container
# -w changes the working directory of the container to /home/testing_cplusplus upon start
# --rm deletes the container after exit
docker run -it -v `pwd`:/home/testing_cplusplus -w /home/testing_cplusplus --name testing_cplusplus --rm testing_cplusplus bash