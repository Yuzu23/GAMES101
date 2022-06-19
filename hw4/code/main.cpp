#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // TODO: Implement de Casteljau's algorithm
    if (control_points.size() == 1)
    {
        return control_points[0];
    }

    std::vector<cv::Point2f> lerp_points;
    for (size_t i = 1; i< control_points.size(); i++)
    {
        cv::Point2f lerp_v2f = (1-t) * control_points[i - 1] + t * control_points[i];
        lerp_points.push_back(lerp_v2f);
    }

    return recursive_bezier(lerp_points, t);
    
}

float distance(cv::Point2f point, cv::Point2f pixiv) {
    return sqrt(pow((point.x - pixiv.x), 2) + pow((point.y - pixiv.y), 2));
}

float draw_collor(float distance) 
{
    if (distance <= 0.2) {
        return 255;
    }
    return 255 * (1 - distance/0.7);
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = recursive_bezier(control_points, t);

        float x_min = floor(point.x);
        float x_max = ceil(point.x);
        float y_min = floor(point.y);
        float y_max = ceil(point.y);

        auto p1 = cv::Point2f(x_min, y_min);
        auto p2 = cv::Point2f(x_min, y_max);
        auto p3 = cv::Point2f(x_max, y_min);
        auto p4 = cv::Point2f(x_max, y_max);

        float d1 = distance(point, p1);
        float d2 = distance(point, p2);
        float d3 = distance(point, p3);
        float d4 = distance(point, p4);

        window.at<cv::Vec3b>(p1)[1] = draw_collor(d1);
        window.at<cv::Vec3b>(p2)[1] = draw_collor(d2);
        window.at<cv::Vec3b>(p3)[1] = draw_collor(d3);
        window.at<cv::Vec3b>(p4)[1] = draw_collor(d4);

    }

}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
            // naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
