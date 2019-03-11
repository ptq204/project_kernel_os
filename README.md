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
This is the kernel which has a set of core components and links in additional services via modules. It has some advantages like:
- add new driver directly to kernel without recompiling the kernel (linking services dynamically).  
- Reduce kernel size: only necessary kernel is inserted.  
The Linux kernel module is a file with extension **.ko**

## Build linux kernel module
- Use library <linux/*> which provides functions and macro in kernel mode.
- <linux/module>  
  **module_init**: define which functions will execute when insert module into kernel  
  **module_exit**: define which functions will execute when remove module from kernel
- Use **printk** print message in C but is used for Linux kernel => easy to debug
- Compile linux kernel using **make** command.
- **sudo insmod "module_name"** to insert module into kernel
- **sudo rmmod "module_name"** to remove module from kernel
- **lsmod**: check if module is loaded successfully. The information contains two columns:  
  - the amount of memory used by module (usually see 16384)
  - number of instances of module are being used.  
- **dmesg**: view the kernel log.
## Driver
- Driver is a controller which operates or controls the devices under its right.
- Driver is important because it provides a hardware user interface. It can communicate with the device through **computer bus** (Device specific) or communicate with the OS (OS specific).
## Character device driver
**Basic characteristics**
- byte oriented device
- transfer data to/from user by reading or writing byte data in character-by-character stream.
- to use this we need to access device files which connect with driver through VFS (virtual file system):
  - Applications do file operations on device files.
  - VFS (virtual file system ) translates the functions of the driver defined in device files.
  - Those functions are executed in kernel mode.
  - The order: Application -> Device files -> Character device driver -> Character device.  

### Initialization
**Major and minor file number**
- Kernel uses Major number to "map" the device with an assoociated driver instead of file's name.
- Before using the driver we need to dynamically allocate a free major number and register the device file number by using the following function:
```c
alloc_chrdev_region(dev_t *first, unsigned int firstminor, unsigned int cnt, char *name);
```
where **firstminor** is a first of the requested range of minor numbers, **cnt** is a number of minor numbers. The function above will return a major number which is chosen dynamically along with the first minor number. We can see the returned number in *dev

**Create device class and device file**
- Create a virtual device class using:
```c
class_create(THIS_MODULE, DEVICE_CLASS);
```
- Afterwards the name in device class will appear in /sys/class/
- Create device file:
```c
device_create(struct class *class, struct device *parent, dev_t devt, void *drvdata, const char *device_name);
```
- Afterwards there will be a file in /dev/ with the name provided by device_name
- Then we add it to **cdev**. Here, the **struct cdev** is the kernel's internal structure that represents char devices. It contains file operations structure and information of major/minor number the of driver. Therefore, we need to register a structure of **cdev** to register a character device in kernel.

### Termination
**Exit module**


### Functionality
**Implementing file operations**
- To have file operations functionality, first we must include the file linux/fs.h
- Afterwards we map the operations we want to the prototype function declarations we provided:
```c
struct file_operations fops = {
       read: device_read,
       open: device_open,
};
```
- Here we only really need the read function so we implement it as follow:
```c
static ssize_t device_read(struct file *file, char *c, size_t size, loff_t *loff_t){

        int i;
        char buf[60];
        if(*loff_t > 0){
                return 0;
        }
        get_random_bytes(&i, sizeof(int));
        printk("Random number: %d\n", i);

        sprintf(buf, "%d", i);
        if(copy_to_user(c, buf, strlen(buf))){
                return -EFAULT;
        }
        *loff_t+=strlen(buf);
        return strlen(buf);
}
```
- The main idea is to first generate a random integer and save it in "i".
- We use printk to check number generated in the kernel log
- We use sprintf to put the numbers as a string into the buffer "buf"
- Finally the string buffer gets copied to the user space.
- "*loff_t" is used to check the offset. We want to return a char sequence represents a random integer number so we have to read 4 times (int = 4 bytes and each time we read 1 byte to buffer). Therefore, we check if *loff_t > 3 (we had read 4 times) then return 0 (end of file).
- **buf** has a length 60 to ensure it can completely contain byte stream from an integer.
