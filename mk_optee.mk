FILEPATH=$(readlink -f "$0")
DIRPATH=$(dirname "$FILEPATH")

optee_dir="/home/rlk/code/optee_3.16"
dest_dir=$(dirname $(readlink -f $0))/testcase/optee

echo $DIRPATH

export PATH="$optee_dir/toolchains/aarch64/bin:$PATH"

export CROSS_COMPILE_HOST=aarch64-linux-gnu-
export CROSS_COMPILE_TA=aarch64-linux-gnu-
export CROSS_COMPILE_CA=aarch64-linux-gnu-
export TA_DEV_KIT_DIR=$DIRPATH/sdk/optee_sdk_3.12/export-ta_arm64
export ROOT_BUILD_DIR=$DIRPATH

export ARCH=arm64

make -C $DIRPATH -f optee.mak V=0 clean
make -C $DIRPATH -f optee.mak V=0 all

demo_name="parameters"

for name in $demo_name
do
	echo "Copy "$name" binaries(TA & CA) to $optee_dir/out-br/-------------------"
	cp ./out/ta/$name/*.ta $optee_dir/out-br/target/lib/optee_armtz
	cp ./out/ca/$name/${name}_demo $optee_dir/out-br/target/usr/bin
done
