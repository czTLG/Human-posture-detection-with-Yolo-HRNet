# Human-posture-detection-with-Yolo-HRNet(Run on CPU)
## 简介
本项目是在学习[KuiperInfer](https://github.com/zjhellofss)深度学习推理框架之后，自行构建的基于CPU的深度学习推理系统，实现对于YOLO与HRNet模型的人体姿态识别推理。

*2023-3-10* 实现了dilated_convolution，preule，F.unsamplenearest等算子。

## 使用的技术和开发环境
* 开发语言：C++ 17
* 数学库：Armadillo + OpenBlas
* 加速库：OpenMP
* 单元测试：Google Test
* 性能测试：Google Benchmark

## 安装过程
1. 安装必要环境
```shell
 apt install cmake, openblas-devel, lapack-devel, arpack-devel, SuperLU-devel>
```
2. 下载并编译armadillo https://arma.sourceforge.net/download.html
3. 编译安装glog\google test\google benchmark

## 致谢

GitHub上最好的深度学习推理框架 https://github.com/zjhellofss/KuiperInfer/

感谢：[zjhellofss](https://github.com/zjhellofss), [liuxubit](https://github.com/liuxubit), [Azusachan](https://github.com/Azusachan), [wfs2010](https://github.com/wfs2010), [mlmz](https://github.com/minleminzui)
