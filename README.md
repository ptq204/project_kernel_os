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
  
# Report

## Module
- **Loadable kernel modules**:  
The kernel has a set of core components and links in additional services via modules. add new driver directly to kernel without recompiling the kernel (linking services dynamically).  
Reduce kernel size: only necessary kernel is inserted  
file .ko

## Build linux kernel module
- Use library <linux/*> which provides functions and macro in kernel mode.
- <linux/module>  
  **module_init**: define which functions will execute when insert module into kernel  
  **module_exit**: define which functions will execute when remove module from kernel
- Use **printk** print message in C but is used for Linux kernel => easy to debug
- Compile linux kernel: ***Fill this***
- **sudo insmod "module_name"** to insert module into kernel
- **sudo rmmod "module_name"** to remove module from kernel
- **lsmod**: check if module is loaded successfully. The information contains two columns:  
  - the amount of memory used by module (usually see 16384)
  - number of instances of module are being used.  
- **dmesg**: view the kernel log.
## Driver
## Character device driver
**Basic characteristics**
- byte oriented device
- transfer data to/from user by reading or writing byte data in character-by-character stream.
- to use this we need to access device files which connect with driver through VFS (virtual file system):
  - Applications do file operations on device files.
  - VFS (virtual file system ) translates the functions of the driver defined in device files.
  - Those functions are executed in kernel mode.
  - The order: Application -> Device files -> Character device driver -> Character device.  

**Major and minor file number**
- Kernel uses Major number to "map" the device with an assoociated driver instead of file's name.
- Before using the driver we need to dynamically allocate a free major number and register the device file number by using the following function:
  - *alloc_chrdev_region*(dev_t *first, unsigned int firstminor, unsigned int cnt, char *name);

**Create device class and device file**
- Create a virtual device class using:
  - *class_create*(THIS_MODULE, DEVICE_CLASS);
- Afterwards the name in device class will appear in /sys/class/
- Create device file:
  - *device_create*(struct class *class, struct device *parent, dev_t devt, void *drvdata, const char *device_name);
- Afterwards there will be a file in /dev/ with the name provided by device_name

