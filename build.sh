clear

compile_dir=~/clang-latest
compile="${compile_dir}/bin/clang++"
compile_op="-std=c++26 -stdlib=libc++ -fparameter-reflection -freflection-latest"

mkdir -p build && cd build

${compile} ${compile_op} --precompile -o std.pcm ${compile_dir}/share/libc++/v1/std.cppm
${compile} ${compile_op} -fmodule-file=std=std.pcm std.pcm --precompile ../serialize.cppm
${compile} ${compile_op} -fmodule-file=std=std.pcm std.pcm -fmodule-file=serialize=serialize.pcm ./serialize.pcm ../main.cpp

export LD_LIBRARY_PATH=${compile_dir}/lib/x86_64-unknown-linux-gnu:$LD_LIBRARY_PATH && ./a.out
