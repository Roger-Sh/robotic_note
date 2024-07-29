#include <opencv2/opencv.hpp>

class Draw
{
public:
    Draw(int W, int H, std::string window_name, cv::MouseCallback mouse_callback = NULL, cv::Scalar color = cv::Scalar(255, 255, 255)) :
        W(W),
        H(H),
        window_name(window_name),
        color(color)
    {
        cv::namedWindow(window_name);
        if (mouse_callback != NULL)
            cv::setMouseCallback(window_name, mouse_callback);
    }

    void clear()
    {
        canvas = cv::Mat::zeros(H, W, CV_8UC3);
        canvas = canvas + cv::Scalar(color[0], color[1], color[2]);
    }

    void draw(std::vector<cv::Point>& points, cv::Scalar color = cv::Scalar(255, 0, 0), int thickness = 2)
    {
        for (unsigned i = 0; i < points.size() - 1; i++)
        {
            cv::line(canvas, points[i], points[i + 1], color, thickness);
        }
        cv::line(canvas, points[0], points[points.size() - 1], color, thickness);
    }

    void draw_pt(cv::Point point, cv::Scalar color = cv::Scalar(0, 255, 0), int thickness = 2, bool dotted = false)
    {
        cv::circle(canvas, point, 5, color, thickness);
    }

    void draw_path(std::vector<cv::Point>& points, cv::Scalar color = cv::Scalar(255, 0, 0), int thickness = 2, bool dotted = false)
    {
        for (unsigned i = 0; i < points.size() - 1; i++)
        {
            if (dotted)
            {
                if (i % 2 == 0)
                {
                    cv::circle(canvas, points[i], 2, color, thickness);
                }
            }
            else
            {
                cv::line(canvas, points[i], points[i + 1], color, thickness);
                cv::circle(canvas, points[i], 3, cv::Scalar(0, 0, 0), 1);
            }
        }
        if (!dotted)
        {
            cv::circle(canvas, points[points.size() - 1], 3, cv::Scalar(0, 0, 0), 1);
        }
    }

    void add_text(
        std::string text,
        cv::Scalar color = cv::Scalar(255, 0, 0),
        int thickness    = 2,
        double fontScale = 1,
        cv::Point org    = cv::Point(100, 50))
    {
        cv::putText(canvas, text, org, cv::FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, cv::LINE_AA);
    }

    int show()
    {
        cv::imshow(window_name, canvas);
        int k = cv::waitKey(30);
        return k;
    }

    cv::Mat canvas;
    std::string window_name;

private:
    int W, H;
    cv::Scalar color;
};