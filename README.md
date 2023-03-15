# Human-posture-detection-with-Yolo-HRNet(Run on CPU)
本项目是在学习KuiperInfer之后自行构建的基于CPU的深度学习推理系统，实现对于YOLO与HRNet模型的推理。为纯C++和部分指令实现深度学习框架。
*2023-3-10* 实现了dilated convolution，preule，F.unsamplenearest等算子。

# 使用的技术和开发环境
-开发语言：C++ 17
-数学库：Armadillo + OpenBlas
-加速库：OpenMP
-单元测试：Google Test
-性能测试：Google Benchmark

# 安装过程
安装必要环境
 <apt install cmake, openblas-devel, lapack-devel, arpack-devel, SuperLU-devel>
下载并编译armadillo https://arma.sourceforge.net/download.html
编译安装glog\google test\google benchmark

# 致谢

github上最好的推理框架 https://github.com/zjhellofss/KuiperInfer/
