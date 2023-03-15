#include "image_util.hpp"
/***
 *
 * @param image  输入的原始图像
 * @param out_image 添加边框后的输出图像
 * @param new_shape 添加边框后期望的图像形状
 * @param stride 确定添加边框后的形状是否应该是固定的还是可变的
 * @param color
 * @param fixed_shape
 * @param scale_up 是否允许缩放到比原始图像更大的形状
 * @return 添加边框的比例因子
 */
float Letterbox(const cv::Mat &image,
                cv::Mat &out_image,
                const cv::Size &new_shape,
                int stride,
                const cv::Scalar &color,
                bool fixed_shape,
                bool scale_up) {
    cv::Size shape = image.size();
    // 计算添加边框的比例因子
    float r = std::min(
            (float) new_shape.height / (float) shape.height, (float) new_shape.width / (float) shape.width);
    // 如果不允许缩放到比原始图像更大的形状，则将比例因子限制在1.0以下
    if (!scale_up) {
        r = std::min(r, 1.0f);
    }

    // 计算添加边框后的图像的宽度和高度
    int new_unpad[2]{
            (int) std::round((float) shape.width * r), (int) std::round((float) shape.height * r)};

    cv::Mat tmp;
    if (shape.width != new_unpad[0] || shape.height != new_unpad[1]) {
        cv::resize(image, tmp, cv::Size(new_unpad[0], new_unpad[1]));
    } else {
        tmp = image.clone();
    }

    // 计算添加边框后的图像与期望的形状之间的差距，并将差距平均分配到每个边界
    float dw = new_shape.width - new_unpad[0];
    float dh = new_shape.height - new_unpad[1];

    if (!fixed_shape) {
        // 如果不固定添加边框后的形状，则将差距除以stride并取整
        dw = (float) ((int) dw % stride);
        dh = (float) ((int) dh % stride);
    }

    dw /= 2.0f;
    dh /= 2.0f;

    int top = int(std::round(dh - 0.1f));
    int bottom = int(std::round(dh + 0.1f));
    int left = int(std::round(dw - 0.1f));
    int right = int(std::round(dw + 0.1f));
    cv::copyMakeBorder(tmp, out_image, top, bottom, left, right, cv::BORDER_CONSTANT, color);
    // 该比例表示输入图像的尺寸与输出图像的尺寸之比
    return 1.0f / r;
}

template<typename T>
T clip(const T &n, const T &lower, const T &upper) {
    return std::max(lower, std::min(n, upper));
}

void ScaleCoords(const cv::Size &img_shape, cv::Rect &coords, const cv::Size &img_origin_shape) {
    float gain = std::min((float) img_shape.height / (float) img_origin_shape.height,
                          (float) img_shape.width / (float) img_origin_shape.width);

    int pad[2] = {(int) (((float) img_shape.width - (float) img_origin_shape.width * gain) / 2.0f),
                  (int) (((float) img_shape.height - (float) img_origin_shape.height * gain) / 2.0f)};

    coords.x = (int) std::round(((float) (coords.x - pad[0]) / gain));
    coords.y = (int) std::round(((float) (coords.y - pad[1]) / gain));

    coords.width = (int) std::round(((float) coords.width / gain));
    coords.height = (int) std::round(((float) coords.height / gain));

    coords.x = clip(coords.x, 0, img_origin_shape.width);
    coords.y = clip(coords.y, 0, img_origin_shape.height);
    coords.width = clip(coords.width, 0, img_origin_shape.width);
    coords.height = clip(coords.height, 0, img_origin_shape.height);
}

