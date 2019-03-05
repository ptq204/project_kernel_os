# project_kernel_os
Modfiy any code in rgenerator.c  
currently working on "Major and Minor file number"  

## Usage:

### Clone repository
```sh
sudo apt-get install git
git clone https://github.com/ptq204/project_kernel_os.git
cd project_kernel_os
```

### Run with script
```sh
sudo chmod +x test_script.sh \
./test_script.sh
```

### Run manually

#### Make and load module
```sh
make all \
sudo insmod rgenerator.ko
```
#### Test module
```sh
sudo cat /dev/random_generator
```

#### Unload module
```sh 
sudo rmmod rgenerator \
dmesg | tail -10
```
