#include <gtest/gtest.h>
#include <glog/logging.h>

// g++ test_main.cpp -o test_main -lgtest -lgtest_main -lglog
// -lgtest、-lgtest_main和-lglog表示链接Google Test和Google Logging
// add_executable(test_DLRF test_main.cpp ${DIR_TEST_DATA} ${DIR_TEST_LAYER} ${DIR_TEST_NET} ${DIR_TEST_RUNTIME})
// 使用./test_main 运行

int main(int argc, char *argv[]) {
  // 初始化Google Test框架
  testing::InitGoogleTest(&argc, argv);
  // 初始化Google Logging库，并传入一个字符串"DLRF_CPU"作为日志文件名
  google::InitGoogleLogging("DLRF_CPU");
  // 检查日志目录是否存在，如果不存在则创建该目录
  std::filesystem::create_directory("./log/");
  //设置日志输出目录
  FLAGS_log_dir = "./log/";
  // 同时输出到标准输出和日志文件中
  FLAGS_alsologtostderr = true;
  // 输出一条日志信息
  LOG(INFO) << "Start test...\n";
  return RUN_ALL_TESTS();
}