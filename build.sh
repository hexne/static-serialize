compile_dir=~/clang-latest
compile="${compile_dir}/bin/clang++"
compile_op="-std=c++26 -stdlib=libc++ -fparameter-reflection -freflection-latest -Wno-reserved-module-identifier"
export LD_LIBRARY_PATH=${compile_dir}/lib/x86_64-unknown-linux-gnu:$LD_LIBRARY_PATH

check_error() {
    if [ $? -ne 0 ]; then
        echo "=========================================================================================="
        exit 1
    fi
}

clear
mkdir -p build && cd build

echo "=========================================================================================="
if [ ! -f std.pcm ]; then
    echo "compile std.pcm..."
    ${compile} ${compile_op} --precompile -o std.pcm ${compile_dir}/share/libc++/v1/std.cppm
    check_error
fi

echo "compile serialize.pcm..."
${compile} ${compile_op} -fmodule-file=std=std.pcm --precompile ../serialize.cppm
check_error

echo "compile a.out..."
${compile} ${compile_op} -fmodule-file=std=std.pcm -fmodule-file=serialize=serialize.pcm ../main.cpp
check_error
echo "=========================================================================================="

./a.out
