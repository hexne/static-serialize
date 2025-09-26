# static-serialize
# 编译器
本项目暂时使用clang-p2996仓库，仓库地址为`https://github.com/bloomberg/clang-p2996/tree/p2996`

# 使用
修改`build.h`中的编译器路径然后`base build.sh`

# 类型支持支持情况  
[x] 基础类型  
[x] std::string  

# 注意事项
指针不被支持，需自行实现serialize函数
