## Setup OPTEE SDK for CA and TA demo App


#### 1、Download code from github

`git clone git@github.com:m8939605/test_ca_ta.git`

#### 2、Check optee SDK folder

In SDK path, there is already a optee SDK, you could use what you want optee version 

```c
test@test-21:~/workspace/code/application/test_ca_ta$ ls sdk/
optee_sdk_3.12
test@test-21:~/workspace/code/application/test_ca_ta$ 
```

**Setup optee toolchains**

By default, this code doesn't include toolchains, you shold download or complie by yourself:<br>
- gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu.tar.xz<br>
- gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf.tar.xz<br> 

Then chenge toolchains path in mk_optee.mk:
```c
(vim mk_optee.mk)
optee_dir="/home/test/workspace/code/optee"
export PATH="$optee_dir/toolchains/aarch64/bin:$PATH"
```

#### 3、Compile TA/CA
In this code, it include a skeleton code `hello`, just run following command to compile ca/ta:<br>

- ./mk_optee.mk<br>

#### 4、Clone TA/CA

Run `./clone hello new_name` one pair CA/TA, then run `./mk_optee.mk` again to buid new_name CA/TA

Note:<br>
- Compiled CA is in out/ca path<br>
- Compiled TA is in out/ta path<br>
