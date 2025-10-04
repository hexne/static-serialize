# static-serialize
# 编译器
本项目暂时使用clang-p2996仓库，仓库地址为`https://github.com/bloomberg/clang-p2996/tree/p2996`

# 使用
修改`build.h`中的编译器路径然后`base build.sh`

# 支持情况  
- [ ] 版本控制
- [x] 基础类型  
- [x] std::string  
- [x] std::array
- [x] std::vector
- [x] 检测指针，存在指针时需用户提供序列化函数(包括shared_ptr和unique_ptr)

# 注意事项
指针不被支持，需自行实现serialize函数
