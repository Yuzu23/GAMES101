//
// Created by LEI XU on 4/27/19.
//

#include "Texture.hpp"


Eigen::Vector3f Texture::getColorBilinear(float u, float v){
    // auto u_img = u * width;
    // auto v_img = (1 - v) * height;

    // float u_min = std::floor(u_img);
    // float u_max = std::min((float)width, std::ceil(u_img));
    // float v_min = std::floor(v_img);
    // float v_max = std::min((float)height, std::ceil(v_img));

	// auto Q11 = image_data.at<cv::Vec3b>(v_max, u_min);
	// auto Q12 = image_data.at<cv::Vec3b>(v_max, u_max);

	// auto Q21 = image_data.at<cv::Vec3b>(v_min, u_min);
	// auto Q22 = image_data.at<cv::Vec3b>(v_min, u_max);
    
    // float rs = (u_img - u_min) / (u_max - u_min);
	// float rt = (v_img - v_max) / (v_min - v_max);
	// auto cBot = (1 - rs) * Q11 + rs * Q12;
	// auto cTop = (1 - rs) * Q21 + rs * Q22;
	// auto P = (1 - rt) * cBot + rt * cTop;
    
    // return Eigen::Vector3f(P[0], P[1], P[2]);
    
    auto u_img = u * width;
    auto v_img = (1 - v) * height;
    // auto color = image_data.at<cv::Vec3b>(v_img, u_img);
    cv::Mat patch;
    // std::clog << "v_img=" << v_img << " | u_img=" << u_img << std::endl;
    // std::clog << "height=" << height << " | width=" << width << std::endl;        
    cv::getRectSubPix(image_data, cv::Size(1, 1), cv::Point2f(u_img, v_img), patch);
    auto color = patch.at<cv::Vec3b>(0, 0);
    return Eigen::Vector3f(color[0], color[1], color[2]);
}